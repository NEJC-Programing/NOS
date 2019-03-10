#include "system.h"
#ifndef IQR_H
#define IQR_H
void irq_uninstall_handler(int irq);
void irq_install_handler(int irq, void (*handler)(struct regs *r));
#endif