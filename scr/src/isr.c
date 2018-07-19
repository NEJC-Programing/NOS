#include "../include/isr.h"
#include "../include/idt.h"
#include "../include/screen.h"
#include "../include/util.h"

void isr_install() {
    set_idt_gate(0, (uint32)isr0);
    set_idt_gate(1, (uint32)isr1);
    set_idt_gate(2, (uint32)isr2);
    set_idt_gate(3, (uint32)isr3);
    set_idt_gate(4, (uint32)isr4);
    set_idt_gate(5, (uint32)isr5);
    set_idt_gate(6, (uint32)isr6);
    set_idt_gate(7, (uint32)isr7);
    set_idt_gate(8, (uint32)isr8);
    set_idt_gate(9, (uint32)isr9);
    set_idt_gate(10, (uint32)isr10);
    set_idt_gate(11, (uint32)isr11);
    set_idt_gate(12, (uint32)isr12);
    set_idt_gate(13, (uint32)isr13);
    set_idt_gate(14, (uint32)isr14);
    set_idt_gate(15, (uint32)isr15);
    set_idt_gate(16, (uint32)isr16);
    set_idt_gate(17, (uint32)isr17);
    set_idt_gate(18, (uint32)isr18);
    set_idt_gate(19, (uint32)isr19);
    set_idt_gate(20, (uint32)isr20);
    set_idt_gate(21, (uint32)isr21);
    set_idt_gate(22, (uint32)isr22);
    set_idt_gate(23, (uint32)isr23);
    set_idt_gate(24, (uint32)isr24);
    set_idt_gate(25, (uint32)isr25);
    set_idt_gate(26, (uint32)isr26);
    set_idt_gate(27, (uint32)isr27);
    set_idt_gate(28, (uint32)isr28);
    set_idt_gate(29, (uint32)isr29);
    set_idt_gate(30, (uint32)isr30);
    set_idt_gate(31, (uint32)isr31);

    set_idt(); // Load with ASM
}

/*Handlers*/
void isr0()
{
    print_colored("Kernel {Div By 0} ",4,0);
    return;
    panic(exception_messages[0],1);
   
}
void isr1()
{
    panic(exception_messages[1],1);    

}
void isr2()
{
    panic(exception_messages[2],1);    

}
void isr3()
{
    panic(exception_messages[3],1);    

}
void isr4()
{
    panic(exception_messages[4],1);    

}
void isr5()
{
    panic(exception_messages[5],1);    

}
void isr6()
{
    panic(exception_messages[6],1);    

}
void isr7()
{
    panic(exception_messages[7],1);    

}
void isr8()
{
    panic(exception_messages[8],1);    

}
void isr9()
{
    panic(exception_messages[9],1);    

}
void isr10()
{
    panic(exception_messages[10],1);    

}
void isr11()
{
    panic(exception_messages[11],1);    

}
void isr12()
{
    panic(exception_messages[12],1);    

}
void isr13()
{
    panic(exception_messages[13],1);    

}
void isr14()
{
    panic(exception_messages[14],1);    

}
void isr15()
{
    panic(exception_messages[15],1);    

}
void isr16()
{
    panic(exception_messages[16],1);    

}
void isr17()
{
    panic(exception_messages[17],1);    

}
void isr18()
{
    panic(exception_messages[18],1);    

}
void isr19()
{
    panic(exception_messages[19],1);    

}
void isr20()
{
    panic(exception_messages[20],1);    

}
void isr21()
{
    panic(exception_messages[21],1);    

}
void isr22()
{
    panic(exception_messages[22],1);    

}
void isr23()
{
    panic(exception_messages[23],1);    

}
void isr24()
{
    panic(exception_messages[24],1);    

}
void isr25()
{
    panic(exception_messages[25],1);    

}
void isr26()
{
    panic(exception_messages[26],1);    

}
void isr27()
{
    panic(exception_messages[27],1);    

}
void isr28()
{
    panic(exception_messages[28],1);    

}
void isr29()
{
    panic(exception_messages[29],1);    

}
void isr30()
{
    panic(exception_messages[30],1);    

}
void isr31()
{
    panic(exception_messages[31],1);    

}


/*End Handlers*/



/* To print the message which defines every exception */
string exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};
