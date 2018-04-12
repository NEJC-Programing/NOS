#ifndef FS_H
#define FS_H
#include "../include/screen.h"
#include "../include/system.h"
#include "../include/util.h"
typedef void (*fat32ReadBlock(uint32 lba, uint8* buff));

typedef unsigned int size_t;

typedef uint32 fat32Error;
#define FAT32_NO_ERROR				0x00000000
#define FAT32_DISK_INSANE			0x00000008
#define FAT32_PART_BAD_TYPE			0x00000100
#define FAT32_PART_BAD_SECTSIZE			0x00000200
#define FAT32_PART_BAD_FATCOUNT			0x00000400
#define FAT32_PART_INSANE			0x00000800

typedef struct {
	fat32ReadBlock*	readBlock;
	uint8		typeCode;
	uint32	lbaBegin;
	uint32	numSectors;
	uint16	bytesPerSector;		//Always 512 Bytes
	uint8		sectorsPerCluster;	//1,2,4,8,16,32,64,128
	uint16	numReservedSectors;	//Usually 0x20
	uint8		numFATs;		//Always 2
	uint32	sectorsPerFAT;		//Depends on disk size
	uint32	rootDirectoryCluster;	//Usually 0x00000002
	uint16	sanity;			//Always 0xAA55
	uint64	lbaFAT;
	uint64	lbaCluster;
} fat32Partition;

typedef struct {
	fat32ReadBlock*	readBlock;
	uint8		partitions[4][16];
	uint16	sanity;			//Should be 0xAA55
} fat32Disk;

typedef struct {
	fat32Partition*	partition;
	//Context Info
} fat32FileContext;

typedef struct {
	fat32Partition*	partition;
	//Context Info
} fat32BrowseContext;

fat32Error fat32OpenDisk(fat32Disk* disk,fat32ReadBlock readBlock);
fat32Error fat32OpenPartition(
	fat32Partition* partition, fat32Disk* disk, size_t index
);


#define BPB_BytsPerSec_Offset	0x0B
#define BPB_SecPerClus_Offset	0x0D
#define BPB_RsvdSecCnt_Offset	0x0E
#define BPB_NumFATs_Offset	0x10
#define BPB_FATSz32_Offset	0x24
#define BPB_RootClus_Offset	0x2C
#define BPB_Sanity_Offset	0x1FE

void byteCopy(void* dst, void* src, size_t num);
void extricateShort(uint16* dst, uint8* src);
void extricateWord(uint32* dst, uint8* src);

#endif