#include "../include/drivers.h"

#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

#define ATA_PRIMARY_DCR_AS 0x3F6
#define ATA_SECONDARY_DCR_AS 0x376

#define ATA_PRIMARY_IRQ 14
#define ATA_SECONDARY_IRQ 15

uint8_t ata_pm = 0; /* Primary master exists? */
uint8_t ata_ps = 0; /* Primary Slave exists? */
uint8_t ata_sm = 0; /* Secondary master exists? */
uint8_t ata_ss = 0; /* Secondary slave exists? */

uint8_t *ide_buf = 0;
////////////////////////////////////////////////////////////////

uint16_t inportw(uint16_t portid)
{
	uint16_t ret;
	__asm__ __volatile__ ("inw %%dx, %%ax":"=a"(ret):"d"(portid));
	return ret;
}
//
#define PIC_SLAVE_CMD 0xA0
#define PIC_MASTER_CMD 0x20
#define PIC_CMD_EOI 0x20
void send_eoi(uint8_t irq)
{
	if(irq >= 8)
		outportb(PIC_SLAVE_CMD, PIC_CMD_EOI);
	outportb(PIC_MASTER_CMD, PIC_CMD_EOI);
}
//
void set_int(int i, uint32_t addr)
{
	print("Installing INT#");print(int_to_string(i));print(" to address: 0x");print(uintToHexStr(addr)); print("\n");
	set_idt_gate(i,addr);set_idt();
}
//
device_t *devices = 0;
uint8_t lastid = 0;
int device_add(device_t* dev)
{
	devices[lastid] = *dev;
	printf("Registered Device %s (%d) as Device#%d\n", dev->name, dev->unique_id, lastid);
	lastid++;
	return lastid-1;
}
//
static uint8_t task = 0;
static uint8_t task_was_on = 0;
void set_task(uint8_t i)
{
	if(!task_was_on) return;
	task = i;
}
//////////////////////////////////////////////////////////////
void ide_select_drive(uint8_t bus, uint8_t i)
{
	if(bus == ATA_PRIMARY)
		if(i == ATA_MASTER)
			outportb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xA0);
		else outportb(ATA_PRIMARY_IO + ATA_REG_HDDEVSEL, 0xB0);
	else
		if(i == ATA_MASTER)
			outportb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xA0);
		else outportb(ATA_SECONDARY_IO + ATA_REG_HDDEVSEL, 0xB0);
}

void ide_primary_irq()
{
	__asm__ __volatile__ ("add $0x1c, %esp");
    __asm__ __volatile__ ("pusha");
	send_eoi(ATA_PRIMARY_IRQ);
	__asm__ __volatile__ ("popa");
    __asm__ __volatile__ ("iret");
}

void ide_secondary_irq()
{
	__asm__ __volatile__ ("add $0x1c, %esp");
    __asm__ __volatile__ ("pusha");
	send_eoi(ATA_SECONDARY_IRQ);
	__asm__ __volatile__ ("popa");
    __asm__ __volatile__ ("iret");
}

uint8_t ide_identify(uint8_t bus, uint8_t drive)
{
	uint16_t io = 0;
	ide_select_drive(bus, drive);
	if(bus == ATA_PRIMARY) io = ATA_PRIMARY_IO;
	else io = ATA_SECONDARY_IO;
	/* ATA specs say these values must be zero before sending IDENTIFY */
	outportb(io + ATA_REG_SECCOUNT0, 0);
	outportb(io + ATA_REG_LBA0, 0);
	outportb(io + ATA_REG_LBA1, 0);
	outportb(io + ATA_REG_LBA2, 0);
	/* Now, send IDENTIFY */
	outportb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
	print("Sent IDENTIFY\n");
	/* Now, read status port */
	uint8_t status = inportb(io + ATA_REG_STATUS);
	if(status)
	{
		/* Now, poll untill BSY is clear. */
		while(inportb(io + ATA_REG_STATUS) & ATA_SR_BSY != 0) ;
pm_stat_read:		status = inportb(io + ATA_REG_STATUS);
		if(status & ATA_SR_ERR)
		{
			printf("%s%s has ERR set. Disabled.\n", bus==ATA_PRIMARY?"Primary":"Secondary", drive==ATA_PRIMARY?" master":" slave");
			return 0;
		}
		while(!(status & ATA_SR_DRQ)) goto pm_stat_read;
		printf("%s%s is online.\n", bus==ATA_PRIMARY?"Primary":"Secondary", drive==ATA_PRIMARY?" master":" slave");
		/* Now, actually read the data */
		set_task(0);
		for(int i = 0; i<256; i++)
		{
			*(uint16_t *)(ide_buf + i*2) = inportw(io + ATA_REG_DATA);
		}
		set_task(1);
	}
}

void ide_400ns_delay(uint16_t io)
{
	for(int i = 0;i < 4; i++)
		inportb(io + ATA_REG_ALTSTATUS);
}

void ide_poll(uint16_t io)
{
	
	for(int i=0; i< 4; i++)
		inportb(io + ATA_REG_ALTSTATUS);

retry:;
	uint8_t status = inportb(io + ATA_REG_STATUS);
	//printf("testing for BSY\n");
	if(status & ATA_SR_BSY) goto retry;
	//printf("BSY cleared\n");
retry2:	status = inportb(io + ATA_REG_STATUS);
	if(status & ATA_SR_ERR)
	{
		panic("ERR set, device failure!\n");
	}
	//printf("testing for DRQ\n");
	if(!(status & ATA_SR_DRQ)) goto retry2;
	//printf("DRQ set, ready to PIO!\n");
	return;
}
uint8_t ata_read_one(uint8_t *buf, uint32_t lba, device_t *dev)
{
	//lba &= 0x00FFFFFF; // ignore topmost byte
	/* We only support 28bit LBA so far */
	uint8_t drive = ((ide_private_data *)(dev->priv))->drive;
	uint16_t io = 0;
	switch(drive)
	{
		case (ATA_PRIMARY << 1 | ATA_MASTER):
			io = ATA_PRIMARY_IO;
			drive = ATA_MASTER;
			break;
		case (ATA_PRIMARY << 1 | ATA_SLAVE):
			io = ATA_PRIMARY_IO;
			drive = ATA_SLAVE;
			break;
		case (ATA_SECONDARY << 1 | ATA_MASTER):
			io = ATA_SECONDARY_IO;
			drive = ATA_MASTER;
			break;
		case (ATA_SECONDARY << 1 | ATA_SLAVE):
			io = ATA_SECONDARY_IO;
			drive = ATA_SLAVE;
			break;
		default:
			print("FATAL: unknown drive!\n");
			return 0;
	}
	//printf("io=0x%x %s\n", io, drive==ATA_MASTER?"Master":"Slave");
	uint8_t cmd = (drive==ATA_MASTER?0xE0:0xF0);
	uint8_t slavebit = (drive == ATA_MASTER?0x00:0x01);
	/*printf("LBA = 0x%x\n", lba);
	printf("LBA>>24 & 0x0f = %d\n", (lba >> 24)&0x0f);
	printf("(uint8_t)lba = %d\n", (uint8_t)lba);
	printf("(uint8_t)(lba >> 8) = %d\n", (uint8_t)(lba >> 8));
	printf("(uint8_t)(lba >> 16) = %d\n", (uint8_t)(lba >> 16));*/
	//outportb(io + ATA_REG_HDDEVSEL, cmd | ((lba >> 24)&0x0f));
	outportb(io + ATA_REG_HDDEVSEL, (cmd | (uint8_t)((lba >> 24 & 0x0F))));
	//printf("issued 0x%x to 0x%x\n", (cmd | (lba >> 24)&0x0f), io + ATA_REG_HDDEVSEL);
	//for(int k = 0; k < 10000; k++) ;
	outportb(io + 1, 0x00);
	//printf("issued 0x%x to 0x%x\n", 0x00, io + 1);
	//for(int k = 0; k < 10000; k++) ;
	outportb(io + ATA_REG_SECCOUNT0, 1);
	//printf("issued 0x%x to 0x%x\n", (uint8_t) numsects, io + ATA_REG_SECCOUNT0);
	//for(int k = 0; k < 10000; k++) ;
	outportb(io + ATA_REG_LBA0, (uint8_t)((lba)));
	//printf("issued 0x%x to 0x%x\n", (uint8_t)((lba)), io + ATA_REG_LBA0);
	//for(int k = 0; k < 10000; k++) ;
	outportb(io + ATA_REG_LBA1, (uint8_t)((lba) >> 8));
	//printf("issued 0x%x to 0x%x\n", (uint8_t)((lba) >> 8), io + ATA_REG_LBA1);
	//for(int k = 0; k < 10000; k++) ;
	outportb(io + ATA_REG_LBA2, (uint8_t)((lba) >> 16));
	//printf("issued 0x%x to 0x%x\n", (uint8_t)((lba) >> 16), io + ATA_REG_LBA2);
	//for(int k = 0; k < 10000; k++) ;
	outportb(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);
	//printf("issued 0x%x to 0x%x\n", ATA_CMD_READ_PIO, io + ATA_REG_COMMAND);

	ide_poll(io);

	set_task(0);
	for(int i = 0; i < 256; i++)
	{
		uint16_t data = inportw(io + ATA_REG_DATA);
		*(uint16_t *)(buf + i * 2) = data;
	}
	ide_400ns_delay(io);
	set_task(1);
	return 1;
}

void ata_read(uint8_t *buf, uint32_t lba, uint32_t numsects, device_t *dev)
{
	for(int i = 0; i < numsects; i++)
	{
		ata_read_one(buf, lba + i, dev);
		buf += 512;
	}
}

void ata_probe()
{
	/* First check the primary bus,
	 * and inside the master drive.
	 */
	
	if(ide_identify(ATA_PRIMARY, ATA_MASTER))
	{
		ata_pm = 1;
		device_t *dev = (device_t *)malloc(sizeof(device_t));
		ide_private_data *priv = (ide_private_data *)malloc(sizeof(ide_private_data));
		/* Now, process the IDENTIFY data */
		/* Model goes from W#27 to W#46 */
		char *str = (char *)malloc(40);
		for(int i = 0; i < 40; i += 2)
		{
			str[i] = ide_buf[ATA_IDENT_MODEL + i + 1];
			str[i + 1] = ide_buf[ATA_IDENT_MODEL + i];
		}
		dev->name = str;
		dev->unique_id = 32;
		dev->dev_type = DEVICE_BLOCK;
		priv->drive = (ATA_PRIMARY << 1) | ATA_MASTER;
		dev->priv = priv;
		dev->read = ata_read;
		device_add(dev);
		printf("Device: %s\n", dev->name);
	}
	ide_identify(ATA_PRIMARY, ATA_SLAVE);
	/*ide_identify(ATA_SECONDARY, ATA_MASTER);
	ide_identify(ATA_SECONDARY, ATA_SLAVE);*/
}

void ata_init()
{
	print("Checking for ATA drives\n");
	ide_buf = (uint16_t *)malloc(512);
	set_int(ATA_PRIMARY_IRQ, ide_primary_irq);
	set_int(ATA_SECONDARY_IRQ, ide_secondary_irq);
	ata_probe();
//	_kill();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////      Date And Time Cmos    ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
Date_and_Time System_Date_Time;
#define CURRENT_YEAR        2018                            // Change this each year!
 
int century_register = 0x00;                                // Set by ACPI table parsing code if possible
Date_and_Time Get_Date_and_Time()
{
	read_rtc();
	return System_Date_Time;
}
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