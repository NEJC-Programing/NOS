#include "../../include/system.h"

#define IDE_BSY 		(1<<7)	// Drive is preparing to send/receive data
#define IDE_RDY 		(1<<6)	// Clear when drive is spun down, or after error
#define IDE_DF			(1<<5)	// Drive Fault error
#define IDE_ERR			(1<<0)	// Error has occured
#define BLOCK_SIZE		1024
#define SECTOR_SIZE		512
#define IDE_IO			0x1F0	// Main IO port
#define IDE_DATA		0x0 	// R/W PIO data bytes
#define IDE_FEAT		0x1 	// ATAPI devices
#define IDE_SECN		0x2 	// # of sectors to R/W
#define IDE_LOW			0x3 	// CHS/LBA28/LBA48 specific
#define IDE_MID 		0x4
#define IDE_HIGH		0x5
#define IDE_HEAD		0x6 	// Select drive/heaad
#define IDE_CMD 		0x7 	// Command/status port
#define IDE_ALT			0x3F6	// alternate status
#define LBA_LOW(c)		((uint8_t) (c & 0xFF))
#define LBA_MID(c)		((uint8_t) (c >> 8) & 0xFF)
#define LBA_HIGH(c)		((uint8_t) (c >> 16) & 0xFF)
#define LBA_LAST(c)		((uint8_t) (c >> 24) & 0xF)

#define IDE_CMD_READ  (BLOCK_SIZE/SECTOR_SIZE == 1) ? 0x20 : 0xC4
#define IDE_CMD_WRITE (BLOCK_SIZE/SECTOR_SIZE == 1) ? 0x30 : 0xC5
#define IDE_CMD_READ_MUL  0xC4
#define IDE_CMD_WRITE_MUL 0xC5

void insl(int port, void *addr, int cnt)
{
  asm volatile("cld; rep insl" :
               "=D" (addr), "=c" (cnt) :
               "d" (port), "0" (addr), "1" (cnt) :
               "memory", "cc");
}


int ide_wait(int check) {
	char r;

	// Wait while drive is busy. Once just ready is set, exit the loop
	while (((r = (char)inb(IDE_IO | IDE_CMD)) & (IDE_BSY | IDE_RDY)) != IDE_RDY);

	// Check for errors
	if (check && (r & (IDE_DF | IDE_ERR)) != 0)
		return 0xF;
	return 0;
}

static void* ide_read(void* b, uint32_t block) {

	int sector_per_block = BLOCK_SIZE / SECTOR_SIZE;	// 2
	int sector = block * sector_per_block;

	ide_wait(0);
	outb(IDE_IO | IDE_SECN, sector_per_block);	// # of sectors
	outb(IDE_IO | IDE_LOW, LBA_LOW(sector));
	outb(IDE_IO | IDE_MID, LBA_MID(sector));
	outb(IDE_IO | IDE_HIGH, LBA_HIGH(sector));
	// Slave/Master << 4 and last 4 bits
	outb(IDE_IO | IDE_HEAD, 0xE0 | (1 << 4) | LBA_LAST(sector));	
	outb(IDE_IO | IDE_CMD, IDE_CMD_READ);
	ide_wait(0);
								// Read only
	insl(IDE_IO, b, BLOCK_SIZE/4);	


	return b;
}


/* Buffer_read and write are used as glue functions for code compatibility 
with hard disk ext2 driver, which has buffer caching functions. Those will
not be included here.  */
void* buffer_read(int block) {
	return ide_read(malloc(BLOCK_SIZE), block);
}