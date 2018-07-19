OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
ld_num_temp_pages = 4;
ld_virtual_offset = 0xC0000000;

SECTIONS
 {
   . = 0x100000;
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
   . = ALIGN(4K);
        ld_temp_pages = .;
        . += ld_num_temp_pages * 4K;
        ld_temp_pages_end = .;
  . = ALIGN(4K);
        ld_physical_end = . - ld_virtual_offset;
        ld_virtual_end = .;

 }
