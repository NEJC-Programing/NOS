#include "../include/drivers.h"
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