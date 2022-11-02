#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define SIBLOCKS 2048L
#define DIBLOCKS (2048L*2048L)
#define TIBLOCKS (2048L*2048L*2048L)

long computeOverheadBlocks(long diskblocks)
{
	int ohblocks = 0;
	//Each block is 8kb, therefore per kb entered, you compute the number of full blocks used, then the rest are overhead
	//if diskblocks is greater than all other blocks and the 12 direct block pointers
	//enough direct blocks?
	if(diskblocks <= 12)
	{
		return ohblocks;
	}
	diskblocks -= 12;
	ohblocks++;
	//enough single indirect pointers?
	if(diskblocks <= SIBLOCKS)
	{
		return ohblocks;
	}
	diskblocks -= SIBLOCKS;
	//enough double indirect pointers?
	ohblocks++;
	if(diskblocks <= DIBLOCKS)
	{
		//find how many groups of 2048 diskblocks we need
		ohblocks += (diskblocks/SIBLOCKS);
		diskblocks %=  SIBLOCKS; //remaining # of diskblocks
		//is there a partial group
		if(diskblocks > 0)
		{
			ohblocks++;
		}
		return ohblocks;
	}
	//going for triple indirect
	diskblocks -= DIBLOCKS;
	ohblocks += SIBLOCKS;
	if(diskblocks <= TIBLOCKS)
	{
		//CODE HERE!
		ohblocks += diskblocks/DIBLOCKS + diskblocks/SIBLOCKS;
		diskblocks %= SIBLOCKS;
		ohblocks++;

		if(diskblocks > 0)
		{
			ohblocks++;
			diskblocks += diskblocks/SIBLOCKS;
		}
		if(diskblocks > 0)
		{
			ohblocks++;
		}
		return ohblocks;
	}
	else
	{
		//file is too big
		return -1;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: diskblocks <file size in KBs>\n");
		return -1;
	}
	long filesize = atol(argv[1]);
	long diskblocks = filesize / 8;
	if (filesize % 8)
		diskblocks++;

	printf("%ld %d\n", diskblocks, computeOverheadBlocks(diskblocks));
}
