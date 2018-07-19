#ifndef DRIVERS_H
#define DRIVERS_H
#include "system.h"
#include "screen.h"
typedef struct api_table_{
	string call_name;
	int call_id;
	uint32_t call_location;
} api_table;
typedef struct driver_t_{
	string driver_name;
	int driver_id;
	void (*driver_init)(void);
	api_table * driver_calls;
} driver_t;
Date_and_Time Get_Date_and_Time();
/////////////////////   ATA   ///////////////////////
typedef uint32_t lba_t;

#define ATA_SECTOR_SIZE     512
typedef struct {
    uint8_t data[ATA_SECTOR_SIZE];
} __attribute__((packed)) ata_sector_t;

void ata_display_info(void);
int ata_select_drive(unsigned int drive_num);
int ata_read_sector_chs(unsigned int c, unsigned int h, unsigned int s,
                        ata_sector_t *);
int ata_read_sector_lba(lba_t, ata_sector_t *);
#endif