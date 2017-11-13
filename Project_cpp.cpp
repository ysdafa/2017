#include <iostream>
#include <memory.h>
#include <time.h>


using namespace std;


static char disk[65536 * 512]; // 32M HDD

static int dummy1[12];

static char filename[100][32]; // 100 Filenames
static int dummy2[34];
static int  filesize[100];
static int dummy3[56];
static char filedata[100][65536];  // 64K Files

static int seed = 0;


void disk_read(int sector, char data[512])
{
	memcpy(data, disk + sector * 512, 512);
}


void disk_write(int sector, char data[512])
{
	memcpy(disk + sector * 512, data, 512);
}


static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}


static int _file_read(int file, int pos, char data[4096], int size)
{
	if (filesize[file] < size) size = filesize[file];
	if (pos > filesize[file] - size) pos = filesize[file] - size;

	memcpy(data, filedata[file] + pos, size);

	return size;
}


static void _file_write(int file, int pos, char data[4096], int size)
{
	if (pos + size > 65536) size = 65536 - pos;
	if (pos + size > filesize[file]) filesize[file] = pos + size;

	memcpy(filedata[file] + pos, data, size);
}


extern void file_read(char filename[32], int pos, char data[4096], int size);
extern void file_write(char filename[32], int pos, char data[4096], int size);


int main(void)
{
	seed = 3;  // the seed will be changed

	cout << "---------------" << endl;

	memset(disk, 0, sizeof(disk));
	memset(filename, 0, sizeof(filename));
	memset(filedata, 0, sizeof(filedata));

	for (int c = 0; c < 100; c++)
	{
		int len = 16 + pseudo_rand() % 16;

		for (int l = 0; l < len; l++)
			filename[c][l] = 'A' + pseudo_rand() % 26;

		filename[c][len] = 0;
		filesize[c] = 0;
	}

	clock_t TIME = clock();

	int RESULT = 0;

	for (int loop = 0; loop < 25000; loop++)
	{
		int  mode = (loop < 5000) ? 10 : pseudo_rand() % 10;
		int  file = pseudo_rand() % 100;
		int  pos = (pseudo_rand() | (pseudo_rand() << 15)) % 65536;
		int  size = 1 + pseudo_rand() % 4096;

		char name[32];
		char data[4096];
		char _data[4096];

		if (mode < 9) // read
		{
			memcpy(name, filename[file], 32);
			file_read(name, pos, data, size);
			size = _file_read(file, pos, _data, size);

			if (memcmp(data, _data, size) != 0) RESULT += 100000;
		}
		else // write
		{
			for (register int c = 0; c < size; c++)
				data[c] = pseudo_rand() % 256;

			memcpy(name, filename[file], 32);
			_file_write(file, pos, data, size);
			file_write(name, pos, data, size);
		}

		for (int i = 0; i < 20; ++i)
		{
			int err = pseudo_rand() % 256;
			int pos = (pseudo_rand() | (pseudo_rand() << 15)) % (65536 * 512);
			disk[pos] = err;
		}
	}

	RESULT += (clock() - TIME) / (CLOCKS_PER_SEC / 1000);

	cout << "RESULT : " << RESULT << endl;

	return 0;
}



////////////////////////////////////
// DO NOT USE ANY STATIC MEMORY   //
// 'STACK' MEMORY CAN BE USED     //
////////////////////////////////////


extern void disk_read(int sector, char data[512]);
extern void disk_write(int sector, char data[512]);


void file_read(char filename[32], int pos, char data[4096], int size)
{
	int i, j, k;
	bool findnameb = false;
	char findName[512];
	char tmp[5200];
	int sectorNow = 0;
	int sectorFStart;
	int sectorFLast;
	int toWriteSize;
	int thisFileSize;
	int l1;
	int r1;
	int l2;
	int thisfilesize = 65536; //todo  get file size
	int index = 0;
	char datat[512];
	while (true)
	{
		disk_read(sectorNow, findName);
		for (i = 0; i < 16; i++)
		{
			for (k = 0, j = i * 32; j < i * 32 + 32; j++, k++)
			{
				if (findName[j] != filename[k])break;
			}
			if (k == 32){ findnameb = true; break; }
		}
		if (findnameb)break;
		sectorNow += 5 * 128;
		if (sectorNow > 500 * 128)break;
	}

	if ((pos + size) > thisfilesize)
	{
		pos = thisfilesize - size;
	}

	sectorFStart = pos / 512; //0 start
	sectorFLast = (pos + size) / 512;
	l1 = pos % 512;
	l2 = (512 - l1) % 512;
	r1 = (pos + size) % 512; //no use?? delete???
	for (i = sectorFStart, j = 0; i <= sectorFLast; i++, j++)  //todo need to read 4 times
	{
		disk_read(sectorNow + i, tmp + j * 512);
	}
	for (i = index, j = l1; i < size; i++, j++)
	{
		data[i] = tmp[j];
	}

	return;
}


void file_write(char filename[32], int pos, char data[4096], int size)
{
	int i, j, k, index, num, middleSec;
	bool findnameb = false;
	char findName[512];
	int sectorNow = 0;
	bool isEmpty = false;
	int isEmptyTest = 0;
	int sectorFStart;
	int sectorFLast;
	int toWriteSize;
	int thisFileSize=0;
	int l1,l2,r1;

	while (true)
	{
		disk_read(sectorNow, findName);
		for (i = 0; i < 16; i++)
		{
			isEmptyTest += findName[i];
		}
		if (isEmptyTest < ('A'*15)){ isEmpty = true; break; }
		for (i = 0; i < 16; i++)
		{
			for (k = 0, j = i * 32; j < i * 32 + 32; j++, k++)
			{
				if (findName[j] != filename[k])break;
			}
			if (k >16){ findnameb = true; break; }
		}

		if (findnameb)break;
		sectorNow += 5 * 128;
		if (sectorNow > 500 * 128)break;
	}
	if (isEmpty)
	{
		char name[512];
		for (i = 0; i < 16; i++)
		{
			for (j = 0; j < 32; j++)
				name[32 * i + j] = filename[j];
		}
		disk_write(sectorNow, name);
	}
	sectorNow += 128;
	//todo  thisFileSize=

	toWriteSize = size;
	if (pos + size > 65536) toWriteSize = 65536 - pos;
	

	l1 = pos % 512;
	l2 = (512 - l1) % 512;	
	r1 = (toWriteSize + pos) % 512;
	

	if (pos + size > thisFileSize) thisFileSize = pos + size; //todo
	sectorFStart = pos / 512;
	sectorFLast = (pos + toWriteSize) / 512;


	// todo write thisFileSize =
	for (int i = 0; i < 4; i++)  //4 record to cover err
	{
		char leftsec[512], rightsec[512], middlesec[512];
		disk_read(sectorNow + sectorFStart, leftsec);
		disk_read(sectorNow + sectorFLast, rightsec);
		if (sectorFLast > sectorFStart)
		{
			for (k = l1, index = 0; k < 512; k++, index++)
			{
				leftsec[k] = data[index];
				disk_write(sectorNow + sectorFStart, leftsec);
			}
			j = sectorFStart + 1;
			while (j < sectorFLast)
			{
				for (k = 0; k < 512; k++, index++)
				{
					middlesec[k] = data[index];
				}
				disk_write(sectorNow + j, middlesec);
				j++;
			}

			for (k = 0; k <= r1; k++, index++)
			{
				rightsec[k] = data[index];
				disk_write(sectorNow + sectorFLast, rightsec);
			}
		}
		else
		{
			for (k = l1, index = 0; k <= r1; k++, index++)
			{
				leftsec[k] = data[index];
				disk_write(sectorNow + sectorFStart, leftsec);
			}
		}
		//todo 

		sectorNow += 128;
	}
	return;
}