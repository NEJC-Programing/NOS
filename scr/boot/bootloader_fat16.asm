;  boot16.asm
;
;  This program is both a FAT12 and FAT16 bootloader
;  Copyright (c) 2017-2018, Joshua Riek
;
;  This program is free software: you can redistribute it and/or modify
;  it under the terms of the GNU General Public License as published by
;  the Free Software Foundation, either version 3 of the License, or
;  (at your option) any later version.
;
;  This program is distributed in the hope that it will be useful,
;  but WITHOUT ANY WARRANTY; without even the implied warranty of
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;  GNU General Public License for more details.
;
;  You should have received a copy of the GNU General Public License
;  along with this program.  If not, see <http://www.gnu.org/licenses/>.
;
    
    %define BOOT_SEG   0x07c0                   ; (BOOT_SEG   << 4) + BOOT_OFF   = 0x007c00
    %define BOOT_OFF   0x0000

    %define STACK_SEG  0x0600                   ; (STACK_SEG  << 4) + STACK_OFF  = 0x007000
    %define STACK_OFF  0x1000

    %define BUFFER_SEG 0x1000                   ; (BUFFER_SEG << 4) + BUFFER_OFF = 0x010000
    %define BUFFER_OFF 0x0000

    %define LOAD_SEG   0x4000                   ; (LOAD_SEG   << 4) + LOAD_OFF   = 0x040000
    %define LOAD_OFF   0x0000
	
;---------------------------------------------------------------------
; Bootloader Memory Map
;---------------------------------------------------------------------
; Linear Address | Item
;       0x100000 | Top of memory hole
;       0x0f0000 | Video memory, MMIO, BIOS	
;       0x0a0000 | Bottom of memory hole
;       0x090000 |
;       0x080000 |
;       0x070000 |
;       0x060000 |
;       0x050000 |
;       0x040000 | Load location ~400k (starts: 0x040000)   
;       0x030000 | Disk buffer    128k (ends:   0x030000)
;       0x020000 | 
;       0x010000 | Disk buffer    128k (starts: 0x010000)
;       0x00f000 | Load Stack       4k (top:    0x010000)
;       0x00e000 |
;       0x00d000 |
;       0x00c000 |
;       0x00b000 |
;       0x00a000 |
;       0x009000 |
;       0x008000 |
; ====> 0x007000 | Boot location   512b (start: 0x007c00)
;       0x006000 | Boot stack        4k (top:   0x007000)
;       0x005000 |              
;       0x004000 |
;       0x003000 |
;       0x002000 |
;       0x001000 |
;       0x000000 | Reserved (Real Mode IVT, BDA)
;---------------------------------------------------------------------


    bits 16
 
;---------------------------------------------------
; Disk description table
;---------------------------------------------------
[ORG 0x0000]
    jmp short entryPoint                        ; Jump over OEM / BIOS param block

     OEMName               db 		"NOS"
     bytesPerSector        dw 		0x0200
     sectorsPerCluster     db 		0x08
     reservedSectors       dw 		0x0020
     fats                  db 		0x02
     rootDirEntries        dw 		0x0000
     sectors               dw 		0x0000
     mediaType       db 		0xF8
     fatSectors         dw 		0x0000
     sectorsPerTrack       dw 		0x003D
     heads        dw 		0x0002
     HiddenSectors         dd 		0x00000000
     TotalSectors     	   dd 		0x00FE3B1F		
     hugeSectors      dd 		0x00000778
     Flags                 dw 		0x0000
     FSVersion             dw 		0x0000
     RootDirectoryStart    dd 		0x00000002
     FSInfoSector          dw 		0x0001
     BackupBootSector      dw 		0x0006

     TIMES 13 DB 0 ;jumping to next offset

     DriveNumber           db 		0x00
     Signature             db 		0x29
     VolumeID              dd 		0xFFFFFFFF
     VolumeLabel           db 		"NOS        "
     SystemID              db 		"FAT32   "
;---------------------------------------------------
; Start of the main bootloader code and entry point
;---------------------------------------------------

entryPoint:
    jmp BOOT_SEG:$+5                            ; Fix the cs:ip registers
    
bootStrap:
    mov ax, BOOT_SEG                            ; Set segments to the location of the bootloader
    mov ds, ax
    mov es, ax
    
    cli
    mov ax, STACK_SEG                           ; Get the the defined stack segment address
    mov ss, ax                                  ; Set segment register to the bottom  of the stack
    mov sp, STACK_OFF                           ; Set ss:sp to the top of the 4k stack
    sti
    
    mov bp, (BOOT_SEG-STACK_SEG) << 4           ; Correct bp for the disk description table

    or dl, dl                                   ; When booting from a hard drive, some of the 
    jz loadRoot                                 ; you need to call int 13h to fix some bpb entries

    mov byte [drive], dl                        ; Save boot device number

    mov ah, 0x08                                ; Get Drive Parameters func of int 13h
    int 0x13                                    ; Call int 13h (BIOS disk I/O)
    jc loadRoot

    and cx, 0x003f                              ; Maximum sector number is the high bits 6-7 of cl
    mov word [sectorsPerTrack], cx              ; And whose low 8 bits are in ch

    movzx dx, dh                                ; Convert the maximum head number to a word
    inc dx                                      ; Head numbers start at zero, so add one
    mov word [heads], dx                        ; Save the head number
    
;---------------------------------------------------
; Load the root directory from the disk
;---------------------------------------------------

loadRoot:
    xor cx, cx

    mov ax, 32                                  ; Size of root dir = (rootDirEntries * 32) / bytesPerSector
    mul word [rootDirEntries]                   ; Multiply by the total size of the root directory
    div word [bytesPerSector]                   ; Divided by the number of bytes used per sector
    xchg cx, ax
        
    mov al, byte [fats]                         ; Location of root dir = (fats * fatSectors) + reservedSectors
    mul word [fatSectors]                       ; Multiply by the sectors used
    add ax, word [reservedSectors]              ; Increase ax by the reserved sectors

    mov word [userData], ax                     ; start of user data = startOfRoot + numberOfRoot
    add word [userData], cx                     ; Therefore, just add the size and location of the root directory
    
    mov bx, BUFFER_SEG                          ; Set the extra segment to the disk buffer
    mov es, bx

    mov bx, BUFFER_OFF                          ; Set es:bx and load the root directory into the disk buffer
    call readSectors                            ; Read the sectors

;---------------------------------------------------
; Find the file to load from the loaded root dir
;---------------------------------------------------
    
loadedRoot:
    mov di, BUFFER_OFF                          ; Set es:di to the 18k disk buffer
    
    mov cx, word [rootDirEntries]               ; Search through all of the root dir entrys for the kernel
    xor ax, ax                                  ; Clear ax for the file entry offset

  searchRoot:
    xchg cx, dx                                 ; Save current cx value to look for the filename
    mov si, filename                            ; Load the filename
    mov cx, 11                                  ; Compare first 11 bytes
    rep cmpsb                                   ; Compare si and di cx times
    je loadFat                                  ; We found the file :)

    add ax, 32                                  ; File entry offset
    mov di, BUFFER_OFF                          ; Point back to the start of the entry
    add di, ax                                  ; Add the offset to point to the next entry
    xchg dx, cx
    loop searchRoot                             ; Continue to search for the file

    mov si, fileNotFound                        ; Could not find the file
    call print
    jmp reboot
    
;---------------------------------------------------
; Load the fat from the found file   
;--------------------------------------------------

loadFat:
    mov ax, word [es:di + 15]                   ; Get the file cluster at offset 26
    mov word [cluster], ax                      ; Store the FAT cluster
    
    xor ax, ax                                  ; Size of fat = (fats * fatSectors)
    mov al, byte [fats]                         ; Move number of fats into al
    mul word [fatSectors]                       ; Move fat sectors into bx
    mov cx, ax                                  ; Store in cx
    
    mov ax, word [reservedSectors]              ; Convert the first fat on the disk

    mov bx, BUFFER_OFF                          ; Set es:bx and load the fat sectors into the disk buffer
    call readSectors                            ; Read the sectors

;---------------------------------------------------
; Calculate the total number of clusters to dertermine fat type
;---------------------------------------------------
    
fatType:
    mov bx, word [sectors]                      ; Take the total sectors subtracted
    sub bx, word [userData]                     ; by the start of the data sectors
    div word [sectorsPerCluster]                ; Now divide by the sectors per cluster
    
    mov word [totalClusters], bx                ; Save the value for later 
    
;---------------------------------------------------
; Load the clusters of the file and jump to it
;---------------------------------------------------
    
loadedFat:    
    mov ax, LOAD_SEG
    mov es, ax                                  ; Set es:bx to where the file will load (0x4000:0x0000)
    mov bx, LOAD_OFF
    
  loadFileSector:
    xor dx, dx
    xor cx, cx

    mov ax, word [cluster]                      ; Get the cluster start = (cluster - 2) * sectorsPerCluster + userData
    sub ax, 2                                   ; Subtract 2
    mov bl, byte [sectorsPerCluster]            ; Sectors per cluster is a byte value
    mul bx                                      ; Multiply (cluster - 2) * sectorsPerCluster
    add ax, word [userData]                     ; Add the userData 
    
    mov cl, byte [sectorsPerCluster]            ; Sectors to read

    mov bx, LOAD_SEG                            ; Segment address of the buffer
    mov es, bx                                  ; Point es:bx to where we will load
    mov bx, word [pointer]                      ; Increase the buffer by the pointer offset
    call readSectors                            ; Read the sectors

    mov ax, word [cluster]                      ; Current cluster number
    xor dx, dx
    
    cmp word [totalClusters], 4085              ; Calculate the next FAT12 or FAT16 sector c:
    jl calculateNextSector12
    
  calculateNextSector16:                        ; Get the next sector for FAT16 (cluster * 2)
    mov bx, 2                                   ; Multiply the cluster by two (cluster is in ax)
    mul bx

    jmp loadNextSector
    
  calculateNextSector12:                        ; Get the next sector for FAT12 (cluster + (cluster * 1.5))
    mov bx, 3                                   ; We want to multiply by 1.5 so divide by 3/2 
    mul bx                                      ; Multiply the cluster by the numerator
    mov bx, 2                                   ; Return value in ax and remainder in dx
    div bx                                      ; Divide the cluster by the denominator
   
  loadNextSector:
    push ds

    mov si, BUFFER_SEG
    mov ds, si                                  ; Tempararly set ds:si to the buffer
    mov si, BUFFER_OFF

    add si, ax                                  ; Point to the next cluster in the FAT entry
    mov ax, word [ds:si]                        ; Load ax to the next cluster in FAT
    
    pop ds

    or dx, dx                                   ; Is the cluster caluclated even?
    jz evenSector

  oddSector:
    shr ax, 4                                   ; Drop the first 4 bits of the next cluster
    jmp nextSectorCalculated

  evenSector:
    and ax, 0x0fff                              ; Drop the last 4 bits of next cluster

  nextSectorCalculated:
    mov word [cluster], ax                      ; Store the new cluster

    cmp ax, 0x0ff8                              ; Are we at the end of the file?
    jae fileJump

    add word [pointer], 512                     ; Add to the pointer offset
    jmp loadFileSector                          ; Load the next file sector

  fileJump:
    mov dl, byte [drive]                        ; We still need the boot device info
    jmp LOAD_SEG:LOAD_OFF                       ; Jump to the file loaded!

    hlt                                         ; This should never be hit 


;---------------------------------------------------
; Bootloader routines below
;---------------------------------------------------

  
;---------------------------------------------------
readSectors:
;
; Read sectors starting at a given sector by 
; the given times and load into a buffer. Please
; note that this may allocate up to 128KB of ram.
;
; @param: CX => Number of sectors to read
; @param: AX => Starting sector
; @param: ES:BX => Buffer to read to
; @return: None
;
;--------------------------------------------------
    pusha
    push es
    
  .sectorLoop:
    pusha
    
    xor dx, dx
    div word [sectorsPerTrack]                  ; Divide the lba (value in ax) by sectorsPerTrack
    mov cx, dx                                  ; Save the absolute sector value 
    inc cx

    xor dx, dx                                  ; Divide by the number of heads
    div word [heads]                            ; to get absolute head and track values
    mov dh, dl                                  ; Move the absolute head into dh
    
    mov ch, al                                  ; Low 8 bits of absolute track
    shl ah, 6                                   ; High 2 bits of absolute track
    or cl, ah                                   ; Now cx is set with respective track and sector numbers

    mov dl, byte [drive]                        ; Set correct drive for int 13h

    mov di, 5                                   ; Try five times to read the sector
    
  .attemptRead:
    mov ax, 0x0201                              ; Read Sectors func of int 13h, read one sector
    int 0x13                                    ; Call int 13h (BIOS disk I/O)
    jnc .readOk                                 ; If no carry set, the sector has been read

    xor ah, ah                                  ; Reset Drive func of int 13h
    int 0x13                                    ; Call int 13h (BIOS disk I/O)
    
    dec di                                      ; Decrease read attempt counter
    jnz .attemptRead                            ; Try to read the sector again

    mov si, diskError                           ; Error reading the disk
    call print
    jmp reboot
    
  .readOk:
    popa
    inc ax                                      ; Increase the next sector to read
    add bx, word [bytesPerSector]               ; Add to the buffer address for the next sector
    
    jnc .nextSector 

  .fixBuffer:                                   ; An error will occur if the buffer in memory
    mov dx, es                                  ; overlaps a 64k page boundry, when bx overflows
    add dh, 0x10                                ; it will trigger the carry flag, so correct
    mov es, dx                                  ; es segment by 0x1000

  .nextSector:
    loop .sectorLoop                            ; Read the next sector for cx times

    pop es
    popa
    ret

;---------------------------------------------------
reboot:
;
; What the fuck do you think this does. 
;
; @return: None
;
;---------------------------------------------------
    xor ax, ax
    int 0x16                                    ; Get a single keypress

    xor ax, ax
    int 0x19                                    ; Reboot the system


;---------------------------------------------------
print:
;
; Print out a simple string.
;
; @param: SI => String
; @return: None
;
;---------------------------------------------------
    lodsb                                       ; Load byte from si to al
    or al, al                                   ; If al is empty stop looping
    jz .done                                    ; Done looping and return
    mov ah, 0x0e                                ; Teletype output
    int 0x10                                    ; Video interupt
    jmp print                                   ; Loop untill string is null
  .done:
    ret


;---------------------------------------------------
; Bootloader varables below
;---------------------------------------------------


    filename       db "BOOT    BIN"             ; Kernel/Stage2 filename 

    fileNotFound   db "Stage2 not found!", 13, 10, 0
    diskError      db "Disk error!", 13, 10, 0
    
    userData       dw 0                         ; Start of the data sectors
    drive          db 0                         ; Boot device number
    cluster        dw 0                         ; Cluster of the file that is being loaded
    pointer        dw 0                         ; Pointer into Buffer, for loading the file
    totalClusters  dw 0                         ; Total clusters, used to determine FAT type

                   times 510 - ($ - $$) db 0x00 ; Pad remainder of boot sector with zeros
                   dw 0xaa55                    ; Boot signature