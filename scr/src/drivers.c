#include "../include/drivers.h"
uint32_t get_physical(uint32_t virtual);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////          Driver API         ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
driver_t * drivers=0;
int last_driverid=0;
void driver_init()
{
	print("Starting Driver API\n");
	
}

int driver_add(driver_t driver)
{
	printf("Adding %s to Driver API id#%d\n",driver.driver_name,last_driverid);
	driver.driver_id=last_driverid;
	last_driverid++;
	drivers[driver.driver_id] = driver;
	print("running init\n");
	drivers[driver.driver_id].driver_init();
	return driver.driver_id;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////            ATA             ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
    uint8_t     status;
    uint8_t     first_chs[3];
    uint8_t     type;
    uint8_t     last_chs[3];
    uint32_t    first_lba;
    uint32_t    num_sectors;
} __attribute((packed)) partition_entry_t;

#define NUM_PARTITIONS_MAX  4
typedef struct {
    uint8_t             bootstrap[436];
    uint8_t             unique_disk[10];
    partition_entry_t   partition[NUM_PARTITIONS_MAX];
    uint16_t            signature;
} __attribute__((packed)) mbr_t;

// in-memory copy of a partition entry
typedef struct {
    bool        valid;
    bool        bootable;
    uint32_t    starting_sec_lba;
    uint32_t    num_secs;
} mpe_t;

static mpe_t _partition_entry[NUM_PARTITIONS_MAX];
static bool _bootable_partition_found;

// check whether all the bytes that make up the given partition entry are zero
static bool _is_all_zero(const partition_entry_t *pe)
{
    uint8_t *p;
    size_t i;

    for (i = 0, p = (uint8_t *) pe; i < sizeof(*pe); i++, p++)
        if (*p)
            return false;

    return true;
}

static inline
bool _is_bootable(const partition_entry_t *pe)
{
    if (0x80 == pe->status)
        return true;

    return false;
}

static
int _set_partition_info(unsigned int pnum, const partition_entry_t *pe)
{
    // if an entry of the partition table is unused, 
    // then it should be set to all 0
    if (_is_all_zero(pe)) {
        _partition_entry[pnum].valid = false;
        return 0;
    }

    _partition_entry[pnum].starting_sec_lba = pe->first_lba;
    _partition_entry[pnum].num_secs = pe->num_sectors;

    // set whether partition is bootable
    if (_is_bootable(pe)) {
        // any one of the partitions may be bootable,
        // but at most one partition should be bootable
        if (_bootable_partition_found)
            return 1;
        _bootable_partition_found = true;
        _partition_entry[pnum].bootable = true;
    } else
        _partition_entry[pnum].bootable = false;
    
    _partition_entry[pnum].valid = true;
    return 0;
}

int mbr_init(void)
{
    // TODO move this out from the stack
    mbr_t mbr;
    partition_entry_t *pe;
    size_t i;

    for (i = 0; i < NUM_PARTITIONS_MAX; i++)
        _partition_entry[i].valid = false;
    
    if (sizeof(mbr_t) != sizeof(ata_sector_t))
        return 1;
    if (ata_read_sector_lba(0, (ata_sector_t *) &mbr))
        return 1;

    _bootable_partition_found = false;
    for (i = 0, pe = mbr.partition; i < NUM_PARTITIONS_MAX; i++, pe++)
        _set_partition_info(i, pe);

    return 0;
}

int mbr_get_partition_info(unsigned int pnum,
                           uint32_t *start_sec, uint32_t *num_secs)
{
    if (!_partition_entry[pnum].valid)
        return 1;   // unused entry

    if (start_sec)
        *start_sec = _partition_entry[pnum].starting_sec_lba;
    
    if (num_secs)
        *num_secs = _partition_entry[pnum].num_secs;

    return 0;
}

static void _display_partition_entry(const mpe_t *pe)
{
    if (!pe->valid) {
        printf("unused\n");
        return;
    }

    if (pe->bootable)
        printf(" * \n");
    printf("starting sector lba:   %d\n", pe->starting_sec_lba);
    printf("size in sectors:       %d\n", pe->num_secs);
}

void mbr_display(void)
{
    size_t i;

    for (i = 0; i < NUM_PARTITIONS_MAX; i++) {
        printf("partition %d\n", i);
        _display_partition_entry(&_partition_entry[i]);
    }
}
/////////////////////////////
static inline
bool _within_partition(unsigned int pnum, uint32_t sec_num_lba)
{
    uint32_t num_secs;

    if (mbr_get_partition_info(pnum, 0, &num_secs))
        return false;   // unused partition

    if (sec_num_lba >= num_secs)    // out of partition bounds
        return false;

    return true;
}


static inline
int _rel2abs_sector(unsigned int pnum, uint32_t rsec_num, uint32_t *asec_num)
{
    uint32_t start_asec;
    
    if (!_within_partition(pnum, rsec_num))
        return 1;

    if (mbr_get_partition_info(pnum, &start_asec, 0))
        return 1;

    *asec_num = start_asec + rsec_num;
    return 0;
}

int hdd_read_sector(unsigned int pnum, uint32_t rsec_num, ata_sector_t *buf)
{ 
    uint32_t asec_num;

    if (_rel2abs_sector(pnum, rsec_num, &asec_num))
        return 1;       

    return ata_read_sector_lba(asec_num, buf);
}

void hdd_display_mbr(void)
{
    mbr_display();
}
///////////////////////////
#define DATA        0x1f0 // [r/w]  data register
#define ERR         0x1f1 // [r]    error register
#define FEATURES    0x1f1 // [w]    features register
#define SEC_CNT     0x1f2 // [r/w]  how many sectors to read/write
#define SEC_NUM     0x1f3 // [r/w]  sector number   / LBAlo
#define LBA_LO      SEC_NUM
#define CYL_LO      0x1f4 // [r/w]  cylinder low    / LBAmid
#define LBA_MID     CYL_LO
#define CYL_HI      0x1f5 // [r/w]  cylinder high   / LBAhi
#define LBA_HI      CYL_HI
#define DRV_HD      0x1f6 // [r/w]  drive/head
#define CMD         0x1f7 // [w]    command port
#define STATUS      0x1f7 // [r]    status register
#define CONTROL     0x3f6 // [w]    control register
#define ASTATUS     0x3f6 // [r]    alternate status register


typedef struct {
    // 0-3  head select bits
    unsigned int head_sel   : 4;

    // 4    drive select
    //          0 -> drive select 0
    //          1 -> drive select 1
    unsigned int drive_sel  : 1;
   
    // 5    set to one
    unsigned int one0       : 1;
   
    // 6    set to zero
    unsigned int zero       : 1;
   
    // 7     set to one
    unsigned int one1       : 1;
} __attribute__((packed)) ata_drv_hd_t;


typedef struct {
    // 0    ERR: previous command ended in an error
    unsigned int  err   : 1;
    
    //  1 IDX
    unsigned int idx    : 1;

    // 2    CORR
    unsigned int corr   : 1; 

    // 3    DRQ: set when the drive has PIO data to transfer
    //           or is ready to accept PIO data
    unsigned int drq    : 1; 

    // 4    SRV: overlapped mode service request
    unsigned int srv    : 1;

    // 5    DF: Drive Fault Error (does not set ERR)
    unsigned int df     : 1;

    // 6    RDY: drive is ready
    unsigned int rdy    : 1;
 
    // 7    BSY: controller executing a command 
    //           wait for it to clear, in case of hang => sw reset
    unsigned int bsy    : 1; 
} __attribute__((packed)) ata_status_t;


typedef struct {
    unsigned int        : 1;
    // 1    nIEN enable bit for the drive interrupt to the host
    //              0 -> INTRQ enabled
    //              1 -> INTRQ disabled
    unsigned int n_ien   : 1;

    // 2    host software reset bit
    //      when set to 1 the drive is reset
    unsigned int srst   : 1;
    unsigned int        : 4;
    unsigned int hob    : 1;
} __attribute__((packed)) ata_control_t;



// commands:
#define CMD_FT          0x50    // format track
#define CMD_RSWR        0x20    // read sectors with retry
#define CMD_RSWOR       0x21    // read sectors without retry
#define CMD_RLWR        0x22    // read long with retry
#define CMD_RLWOR       0x23    // read long without retry
#define CMD_WSWR        0x30    // write sectors with retry
#define CMD_WSWOR       0x31    // write sectors without retry
#define CMD_WLWR        0x32    // write long with retry
#define CMD_WLWOR       0x32    // write long without retry
#define CMD_IDENTIFY    0xec

static void _set_control(ata_control_t control)
{
    uint8_t reg;

    memcpy(&reg, &control, sizeof(control));
    outb(CONTROL, reg);
}

static inline
ata_status_t _read_status(void)
{
    ata_status_t status;
    uint8_t reg;

    reg = inb(STATUS);
    memcpy(&status, &reg, sizeof(reg));

    return status;   
}

static inline
bool _is_busy(void)
{
    ata_status_t status;

    status = _read_status();
    return status.bsy;
}

static inline
bool _is_drq(void)
{
    ata_status_t status;

    status = _read_status();
    return status.drq;    
}

// check BSY & DRQ before trying to send a command

static void _reset(void)
{
    ata_control_t control;

    memset(&control, 0, sizeof(control)); 

    control.srst = 1;
    _set_control(control);

    // TODO introduce a delay??

    // the reset bit has to be cleared manually
    control.srst = 0;
    _set_control(control);

    
    while (_is_busy());
}

static ata_drv_hd_t _read_drv_hd(void)
{
    ata_drv_hd_t drv_hd;
    uint8_t reg;

    reg = inb(DRV_HD);
    memcpy(&drv_hd, &reg, sizeof(reg));

    return drv_hd;
}

static void _write_drv_hd(ata_drv_hd_t drv_hd)
{
    uint8_t reg;

    memcpy(&reg, &drv_hd, sizeof(reg));
    outb(DRV_HD, reg); 
}

static uint16_t _read_data(void)
{
    ata_status_t status;
int timeout = 0;
    // TODO timeout
    do { 
        status = _read_status();
        timeout++;
            if (timeout > 999999) 
            {
                print("Timeout\n");
                goto SSS;
            }
    } while (!status.drq);
SSS:
    return inw(DATA);
}


static int _send_cmd(unsigned int cmd)
{
    ata_status_t status;

    // check DRQ bit before issuing a command
    status = _read_status();
    if (status.drq)
        return 1;

    // TODO timeout
    while (_is_busy())
        ;

    outb(CMD, cmd);

    return 0;
}

static int _select_head(unsigned int head_num)
{
    ata_drv_hd_t drv_hd;
    
    if (head_num > 15)
        return 1;

    drv_hd = _read_drv_hd();
    drv_hd.head_sel = head_num;
    _write_drv_hd(drv_hd);

    return 0;
}

// TODO disable optimizations for this function
int ata_select_drive(unsigned int drive_num)
{
    ata_drv_hd_t drv_hd;

    if (drive_num > 1)
        return 1;
    
    drv_hd = _read_drv_hd();
    drv_hd.drive_sel = drive_num;
    _write_drv_hd(drv_hd);

    // read 4 times the status register in order to achieve the 400ns delay 
    // before reading the actual value of the status register
    {
        int i;
        for (i = 0; i < 4; i++)
            (void) _read_status(); // TODO optimized away???
    }
    
    // TODO ensure drive is RDY before returning from here
    // ??
    {
        ata_status_t status;

        int timeout = 0;
        do { 
            status = _read_status();
            timeout++;
            if (timeout > 999999) 
            {
                print("Timeout\n");
                goto nessss;
            }
        } while (!status.rdy);
        nessss: 
        if (status.err)
            return 1;
    }
    
    return 0;
}

// read sector buffer data 
static void _read_buffer_data(ata_sector_t *dest)
{
    unsigned int i;
    uint16_t *data;

    for (i = 0, data = (uint16_t *) dest;
         i < sizeof(ata_sector_t)/sizeof(*data);
         i++, data++)
    {
        *data = _read_data();
    }
}

#define MODEL_NUMBER_STR_LEN    40
#define MODEL_NUMBER_STR_OFF    27

#define SERIAL_NUMBER_STR_LEN   20
#define SERIAL_NUMBER_STR_OFF   10
static struct {
    bool        valid;

    uint32_t    num_sectors;    // capacity

    uint16_t    cylinders;
    uint16_t    heads;
    uint16_t    spt;

    uint16_t    bytes_per_sector;   // unformatted

    bool        lba_supported;  // user-addressable sectors
    uint32_t    lba_max;

    bool        dma_supported;
    
    char        model_number[MODEL_NUMBER_STR_LEN+1];
    char        serial_number[SERIAL_NUMBER_STR_LEN+1];
    
} _id_data;
int strncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    for (i = 0; i++ < n && *s1; s1++, s2++) {
        if (!*s2)
            return 1;

        if (*s2 > *s1)
            return -1;

        if (*s1 > *s2)
            return 1;
    }

    return 0;
}
char *strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    for (i = 0; n; i++, n--) {
        dest[i] = src[i];
        if ('\0' == src[i])
            break;
    }

    return dest;  
}

static void _set_identify_data(const ata_sector_t *id_data)
{
    uint16_t *p;

    p = (uint16_t *) id_data;

    // TODO only valid if bit 0 of word 53 is set to 1 
    _id_data.num_sectors = (uint32_t) p[58] << 16 | p[57];

    _id_data.cylinders = p[1];
    _id_data.heads = p[3];
    _id_data.spt = p[6];


    strncpy(_id_data.model_number, (char *) (p + 27), MODEL_NUMBER_STR_LEN);
    _id_data.model_number[MODEL_NUMBER_STR_LEN] = '\0';

    _id_data.valid = true;
}

static inline
void _swap_str_bytes(char *str, size_t len)
{
    char tmp;
    size_t i;

    for (i = 0; i < len; i += 2) {
        tmp = str[i];
        str[i] = str[i+1];
        str[i+1] = tmp;
    }
}

static int _identify(unsigned int drive_num)
{
    // TODO move this out from the stack
    // TODO write a memory manager to solve this kind of problems
    ata_sector_t identify_sector;

    ata_status_t status;
    uint8_t reg;

    // so far only one and the first device is supported
    if (drive_num)
        return 1;
    
    if (ata_select_drive(drive_num))
        return 1;
    
    // set CHS to zero (LBAlo, LBAmid, LBAhi)
    outb(SEC_NUM,  0);
    outb(CYL_LO, 0);
    outb(CYL_HI,  0); 

    _send_cmd(CMD_IDENTIFY);

    status = _read_status();
    memcpy(&reg, &status, sizeof(reg));
    if (!reg)
        return 1;   // drive does not exist
    
    while (_is_busy())
        ;

    // TODO check check LBAmid, LBAhi to see if the are non-zero
    //      if so, the drive is not ATA

    while (!_is_drq())
        ;

    // read data
    _read_buffer_data(&identify_sector);
   
    // lower byte of a 16-bit word is the second character: 
    // "Generic 1234" appears as "eGenir c2143"
    // fix this issue before retrieving the string
    _swap_str_bytes((char *)
                        ((uint16_t *) &identify_sector + MODEL_NUMBER_STR_OFF),
                        MODEL_NUMBER_STR_LEN);
    _swap_str_bytes((char *)
                        ((uint16_t *) &identify_sector + SERIAL_NUMBER_STR_OFF),
                        SERIAL_NUMBER_STR_LEN);

    _set_identify_data(&identify_sector);
    
    return 0;
}

void ata_display_info(void)
{
    if (!_id_data.valid) {
        printf("no info to display\n");
        return;
    }

    printf("model: %s\n", _id_data.model_number);
    printf("capacity:  %d sectors\n", _id_data.num_sectors);
    printf("cylinders: %d\n", _id_data.cylinders);
    printf("heads:     %d\n", _id_data.heads);
    printf("spt:       %d\n", _id_data.spt);

}

int ata_init(void)
{
    print("ATA Driver Init\n");
    _reset();
    print("ATA reset ok\n");
    if (_identify(0)){
        print("There is a drive 0\n");
        ata_display_info();
        return 1;
    }
    return 0;
}

int ata_read_sector_chs(unsigned int cyl_num, unsigned int head_num,
                        unsigned int sec_num, ata_sector_t *buf)
{
    if (cyl_num > 1023)
        return 1;

    if (head_num > 15)
        return 1;

    if (sec_num < 1 || sec_num > 256)
        return 1;

    if (_select_head(head_num))
        return 1;

    // one sector to read
    outb(SEC_CNT, 1);
    outb(SEC_NUM, sec_num);
    outb(CYL_LO, cyl_num & 0xff);
    // cylinder high (2 bits)
    outb(CYL_HI, (cyl_num & 0x300) >> 8); 

    if (_send_cmd(CMD_RSWOR))
        return 1;
    
    // wait until the sector buffer requires service
    {
        ata_status_t status;

        do {
            status = _read_status();
        } while (!status.drq || status.bsy);
    
        if (status.err)
            return 1;
    }

    _read_buffer_data(buf);
    return 0;
}

static inline
int _lba2chs(const lba_t lba,
                    unsigned int *c, unsigned int *h, unsigned int *s)
{
    if (lba >= _id_data.num_sectors)
        return 1;

    *c = lba / (_id_data.heads * _id_data.spt);

    *h = (lba - *c * _id_data.heads * _id_data.spt) / _id_data.spt;

    *s = (lba - *c * _id_data.heads * _id_data.spt - *h * _id_data.spt) + 1;

    return 0;
}

// this is only software-translated LBA
int ata_read_sector_lba(lba_t lba, ata_sector_t *buf)
{
    unsigned int c, h, s;

    if (_lba2chs(lba, &c, &h, &s))
        return 1;

    return ata_read_sector_chs(c, h, s, buf);     
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////      Date And Time CMOS    ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Date_and_Time System_Date_Time;
Date_and_Time Get_Date_and_Time()
{
	read_rtc();
	return System_Date_Time;
}
#define CURRENT_YEAR        2018                            // Change this each year!
 
int century_register = 0x00;                                // Set by ACPI table parsing code if possible
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};
 
int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}
 
unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}
 
void read_rtc() {
      unsigned char century;
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;
 
      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates
 
      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      System_Date_Time.second = get_RTC_register(0x00);
      System_Date_Time.minute = get_RTC_register(0x02);
      System_Date_Time.hour = get_RTC_register(0x04);
      System_Date_Time.day = get_RTC_register(0x07);
      System_Date_Time.month = get_RTC_register(0x08);
      System_Date_Time.year = get_RTC_register(0x09);
      if(century_register != 0) {
            century = get_RTC_register(century_register);
      }
 
      do {
            last_second = System_Date_Time.second;
            last_minute = System_Date_Time.minute;
            last_hour = System_Date_Time.hour;
            last_day = System_Date_Time.day;
            last_month = System_Date_Time.month;
            last_year = System_Date_Time.year;
            last_century = century;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            System_Date_Time.second = get_RTC_register(0x00);
            System_Date_Time.minute = get_RTC_register(0x02);
            System_Date_Time.hour = get_RTC_register(0x04);
            System_Date_Time.day = get_RTC_register(0x07);
            System_Date_Time.month = get_RTC_register(0x08);
            System_Date_Time.year = get_RTC_register(0x09);
            if(century_register != 0) {
                  century = get_RTC_register(century_register);
            }
      } while( (last_second != System_Date_Time.second) || (last_minute != System_Date_Time.minute) || (last_hour != System_Date_Time.hour) ||
               (last_day != System_Date_Time.day) || (last_month != System_Date_Time.month) || (last_year != System_Date_Time.year) ||
               (last_century != century) );
 
      registerB = get_RTC_register(0x0B);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) {
            System_Date_Time.second = (System_Date_Time.second & 0x0F) + ((System_Date_Time.second / 16) * 10);
            System_Date_Time.minute = (System_Date_Time.minute & 0x0F) + ((System_Date_Time.minute / 16) * 10);
            System_Date_Time.hour = ( (System_Date_Time.hour & 0x0F) + (((System_Date_Time.hour & 0x70) / 16) * 10) ) | (System_Date_Time.hour & 0x80);
            System_Date_Time.day = (System_Date_Time.day & 0x0F) + ((System_Date_Time.day / 16) * 10);
            System_Date_Time.month = (System_Date_Time.month & 0x0F) + ((System_Date_Time.month / 16) * 10);
            System_Date_Time.year = (System_Date_Time.year & 0x0F) + ((System_Date_Time.year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }
 
      // Convert 12 hour clock to 24 hour clock if necessary
 
      if (!(registerB & 0x02) && (System_Date_Time.hour & 0x80)) {
            System_Date_Time.hour = ((System_Date_Time.hour & 0x7F) + 12) % 24;
      }
 
      // Calculate the full (4-digit) year
 
      if(century_register != 0) {
            System_Date_Time.year += century * 100;
      } else {
            System_Date_Time.year += (CURRENT_YEAR / 100) * 100;
            if(System_Date_Time.year < CURRENT_YEAR) System_Date_Time.year += 100;
      }
}