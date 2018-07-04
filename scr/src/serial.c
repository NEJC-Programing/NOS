#include "../include/serial.h"

int serial_received() {
   return inb(0x3f8 + 5) & 1;
}

char read_serial() {
   while (serial_received() == 0);
   return inb(0x3f8);
}

// Send

int is_transmit_empty() {
   return inb(0x3f8 + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);
   outb(0x3f8,a);
}

void serial_init() {
   outb(0x3f8 + 1, 0x00);
   outb(0x3f8 + 3, 0x80);
   outb(0x3f8 + 0, 0x03);
   outb(0x3f8 + 1, 0x00);
   outb(0x3f8 + 3, 0x03);
   outb(0x3f8 + 2, 0xC7);
   outb(0x3f8 + 4, 0x0B);
}