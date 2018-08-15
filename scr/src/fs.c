#include "../include/fs.h"

/* 	Read superblock from device dev, and check the magic flag.
	Return NULL if not a valid EXT2 partition */
superblock* ext2_superblock() {
	superblock* sb = buffer_read(EXT2_SUPER);
	if (sb->magic != EXT2_MAGIC)
		return NULL;
	return sb;
}

block_group_descriptor* ext2_blockdesc() {
	return buffer_read(EXT2_SUPER + 1);
}

inode* ext2_inode(int dev, int i) {
	superblock* s = ext2_superblock();
	block_group_descriptor* bgd = ext2_blockdesc();

	int block_group = (i - 1) / s->inodes_per_group; // block group #
	int index 		= (i - 1) % s->inodes_per_group; // index into block group
	int block 		= (index * INODE_SIZE) / BLOCK_SIZE; 
	bgd += block_group;

	// Not using the inode table was the issue...
	uint32_t* data = buffer_read(bgd->inode_table+block);
	inode* in = (inode*)((uint32_t) data + (index % (BLOCK_SIZE/INODE_SIZE))*INODE_SIZE);
	return in;
}

uint32_t ext2_read_indirect(uint32_t indirect, size_t block_num) {
	char* data = buffer_read(indirect);
	return *(uint32_t*) ((uint32_t) data + block_num*4);
}

void* ext2_read_file(inode* in) {
	assert(in);
	if(!in)
		return NULL;

	int num_blocks = in->blocks / (BLOCK_SIZE/SECTOR_SIZE);	

	assert(num_blocks != 0);
	if (!num_blocks) 
		return NULL;


	size_t sz = BLOCK_SIZE*num_blocks;
	void* buf = malloc(sz);
	assert(buf != NULL);

	int indirect = 0;

	/* Singly-indirect block pointer */
	if (num_blocks > 12) {
		indirect = in->block[12];
	}

	int blocknum = 0;
	char* data;
	for (int i = 0; i < num_blocks; i++) {
		if (i < 12) {
			blocknum = in->block[i];
			char* data = buffer_read(blocknum);
			memcpy((uint32_t) buf + (i * BLOCK_SIZE), data, BLOCK_SIZE);
		}
		if (i > 12) {
			blocknum = ext2_read_indirect(indirect, i-13);
			char* data = buffer_read(blocknum);
			memcpy((uint32_t) buf + ((i-1) * BLOCK_SIZE), data, BLOCK_SIZE);
		}

	}
	return buf;
}


void* ext2_file_seek(inode* in, size_t n, size_t offset) {
	int nblocks 	= ((n-1 + BLOCK_SIZE & ~(BLOCK_SIZE-1)) / BLOCK_SIZE);
	int off_block 	= (offset / BLOCK_SIZE);	// which block
	int off 		= offset % BLOCK_SIZE;		// offset in block

	void* buf = malloc(nblocks*BLOCK_SIZE);		// round up to whole block size

	assert(nblocks <= in->blocks/2);
	assert(off_block <= in->blocks/2);
	for (int i = 0; i < nblocks; i++) {
		buffer* b = buffer_read(in->block[off_block+i]);
		memcpy(buf + (i*BLOCK_SIZE), b->data + off, BLOCK_SIZE);
		//printf("Read @ block %d (%d)\n",in->block[off_block+i], off_block);
		off = 0;	// Eliminate offset after first block
	}
	return buf;

}

/* Finds an inode by name in dir_inode */
int ext2_find_child(const char* name, int dir_inode) {
	if (!dir_inode)
		return -1;
	inode* i = ext2_inode(1, dir_inode);			// Root directory

	char* buf = malloc(BLOCK_SIZE*i->blocks/2);
	memset(buf, 0, BLOCK_SIZE*i->blocks/2);

	for (int q = 0; q < i->blocks / 2; q++) {
		char* data = buffer_read(i->block[q]);
		memcpy((uint32_t)buf+(q * BLOCK_SIZE), data, BLOCK_SIZE);
	}

	dirent* d = (dirent*) buf;
	
	int sum = 0;
	int calc = 0;
	do {
		// Calculate the 4byte aligned size of each entry
		calc = (sizeof(dirent) + d->name_len + 4) & ~0x3;
		sum += d->rec_len;
		//printf("%2d  %10s\t%2d %3d\n", (int)d->inode, d->name, d->name_len, d->rec_len);
		if (strncmp(d->name, name, d->name_len)== 0) {
			
			free(buf);
			return d->inode;
		}
		d = (dirent*)((uint32_t) d + d->rec_len);

	} while(sum < (1024 * i->blocks/2));
	free(buf);
	return -1;
}


void lsroot() {
//	print("lsroot");
	inode* i = ext2_inode(1, 2);			// Root directory

	char* buf = malloc(BLOCK_SIZE*i->blocks/2);
//	printx("Inode@: ", i);

	for (int q = 0; q < i->blocks / 2; q++) {
//		printx("Block: ", i->block[q]);
		char* data = buffer_read(i->block[q]);
		memcpy((uint32_t)buf+(q * BLOCK_SIZE), data, BLOCK_SIZE);
	}

	dirent* d = (dirent*) buf;

	int sum = 0;
	int calc = 0;
	print("Root directory:\n");
	do {
		
		// Calculate the 4byte aligned size of each entry
		calc = (sizeof(dirent) + d->name_len + 4) & ~0x3;
		sum += d->rec_len;
		print("/");
		print(d->name);
		printch('\n');
		if (d->rec_len != calc && sum == 1024) {
			/* if the calculated value doesn't match the given value,
			then we've reached the final entry on the block */
			//sum -= d->rec_len;
			d->rec_len = calc; 		// Resize this entry to it's real size
		//	d = (dirent*)((uint32_t) d + d->rec_len);
		}

		d = (dirent*)((uint32_t) d + d->rec_len);


	} while(sum < 1024);
}

