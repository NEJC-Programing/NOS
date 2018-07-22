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
///////////////////////////////////           TIMER            ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/* This will keep track of how many ticks that the system
*  has been running for */
unsigned int timer_ticks = 0;

// allow kernel drivers to request callbacks when a timer
// tick executes.
#define NUM_TICK_HANDLERS 255

typedef void (*tick_handler_func)();
tick_handler_func tick_handlers[NUM_TICK_HANDLERS];
unsigned int next_tick_handler_id;

void timer_install_tick_handler(void (*tick_handler)()) {
	tick_handlers[next_tick_handler_id++] = tick_handler;
	print("new tick handler registered\n");
}


/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. */
void timer_handler(struct regs *r) {
	unsigned int i;

	timer_ticks++;

	// invoke everyone who has registered a timer tick handler
	for (i = 0; i < next_tick_handler_id; i++)
		tick_handlers[i]();

	//if (timer_ticks % 18 == 0) {
	//	kprintf("One second has passed. %i tick handlers installed.\n", next_tick_handler_id);
	//}
}

/* This will continuously loop until the given time has been reached */
void timer_wait(int ticks) {
	unsigned long eticks;

	eticks = timer_ticks + ticks;
	while(timer_ticks < eticks);
}

void timer_install() {
	/* Installs 'timer_handler' to IRQ0 */
	irq_install_handler(0, timer_handler);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////            ATA             ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool primary_detected = false;
unsigned short disk_info[256];
bool pio_48_supported = false;
unsigned int sector_count = 0;

void ata_pio_install() {
	char detect;
	unsigned short tmp;
	unsigned int i, tmp2;

	print("initialising disk\n");
	//timer_install_tick_handler(ata_pio_poller); // register our callback.
	//To use the IDENTIFY command, select a target drive by sending 0xA0 for the master drive, or 0xB0 for the slave, to the "drive select" IO port.
	//On the Primary bus, this would be port 0x1F6.
	outb(0x1F6, 0xA0);

	//Then set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0 (port 0x1F2 to 0x1F5).
	outb(0x1F2, 0);
	outb(0x1F3, 0);
	outb(0x1F4, 0);
	outb(0x1F5, 0);

	//Then send the IDENTIFY command (0xEC) to the Command IO port (0x1F7).
	outb(0x1F7, 0xEC);

	//Then read the Status port (0x1F7) again. If the value read is 0, the drive does not exist.
	detect = inb(0x1F7);

	if (detect != 0) {
		primary_detected = true;
		print("primary drive detected\n");
	} else {
		print("drive does not exist\n");
		return;
	}

	//For any other value: poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears.
	while ((inb(0x1F7) & 0x80) == 0x80) ; //wait until BUS BUSY bit to be cleared
	
	//Because of some ATAPI drives that do not follow spec, at this point you need to check the LBAmid and LBAhi ports (0x1F4 and 0x1F5)
	//to see if they are non-zero. If so, the drive is not ATA, and you should stop polling.

	detect = inb(0x1F4);
	detect += inb(0x1F5);
    
	if (detect > 0) {
		print("primary disk detected is ATAPI (CD-ROM style) not PATA\n");
		return;
	}

	//Otherwise, continue polling one of the
	//Status ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
	//do {
	//   detect = inb(0x1F7);
	//} while (detect && 0x04);
	//while ((inb(0x1F7) & 0x40) == 0x40) ; // wait for DRIVE READY bit to be set
    
	//At that point, if ERR is clear, the data is ready to read from the Data port (0x1F0). Read 256 words, and store them.
	for (i = 0; i < 256; i++) {
		disk_info[i] = inw(0x1F0);
	}

	//bit 10 of word 83 determines if the disk supports read48 mode.
	if ((disk_info[83] & 0x10) == 0x10) {
		pio_48_supported = true;
		print("pio 48 read supported\n");
	} else {
		print("pio 48 read NOT supported\n");
		tmp2 = disk_info[83];
		printf("word 83 = 0x%x\n", tmp2);
	}

	tmp2 = disk_info[60]; //word size
	tmp2 = tmp2 << 16;
	tmp2 += disk_info[61];
	printf("disk supports 0x%x total lba addressable sectors in pio 28 mode\n");
	sector_count = tmp2;

}

void ata_pio_read(size_t lba, void *buffer, size_t count) {
	size_t bytes_read;
	char *buff = (char*)buffer;
	unsigned short word;
	char chr;
	int tmp;
	//primary bus
	unsigned short port = 0x1F0;
	unsigned short slavebit = 0;

	printf("ata_pio_read: 0x%x, 0x%x, %d\n", lba, buff, count);

	if (count == 0) { //nothing to read.
		printf("nothing to read..\n");
		return;
	}

	//Send 0xE0 for the "master" or 0xF0 for the "slave", ORed with the highest 4 bits of the LBA to port 0x1F6:
	outb(0x1F6, 0xE0 | (slavebit << 4) | ((lba >> 24) & 0x0F));
	//outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));

	//Send a NULL byte to port 0x1F1, if you like (it is ignored and wastes lots of CPU time):
	outb(0x1F1, 0x00);

	outb(0x1F2, 0x01); // Read one sector

	//Send the low 8 bits of the LBA to port 0x1F3:
	outb(0x1F3, (unsigned char) lba);

	//Send the next 8 bits of the LBA to port 0x1F4:
	outb(0x1F4, (unsigned char)(lba >> 8));

	//Send the next 8 bits of the LBA to port 0x1F5:
	outb(0x1F5, (unsigned char)(lba >> 16));

	//outb(0x1F6, 0xE0 | (1 << 4) | ((lba >> 24) & 0x0F));

	//Send the "READ SECTORS" command (0x20) to port 0x1F7:
	outb(0x1F7, 0x20);

	//Wait for an IRQ or poll.
	for(tmp = 0; tmp < 3000; tmp++)
	{}

	while ((inb(0x1F7) & 0x80) != 0x0) {} //wait until BUS BUSY bit to be cleared
	while ((inb(0x1F7) & 0x40) != 0x40) {} // wait for DRIVE READY bit to be set

	//Transfer 256 words, a word at a time, into your buffer from I/O port 0x1F0. (In assembler, REP INSW works well for this.)
	//(word = 2 bytes to 256 word = 512 bytes)

	bytes_read = 0;

	while (bytes_read < 256 ) {

		word = 0;
		word = inw(0x1F0);

		buff[bytes_read * 2] = word & 0xFF;
		buff[(bytes_read * 2) + 1] = word >> 8;

		bytes_read++;
	}

	printf("%d bytes read!\n", bytes_read);

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