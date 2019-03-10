#ifndef DRIVERS_H
#define DRIVERS_H
#include "system.h"
#include "screen.h"
/////////////////////   DRIVERS   ///////////////////////
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
/////////////////////   CMOS   ///////////////////////
Date_and_Time Get_Date_and_Time();
/////////////////////   ATA   ///////////////////////
void ata_pio_read(size_t lba, void *buffer, size_t count);
/////////////////////   TIMER   ///////////////////////
void timer_wait(int ticks);
/////////////////////   MOUSE   ///////////////////////

#endif