#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>
#include <memory.h>

static char dummy1[5][1024][1024];
static char disk[16][1024][1024];
static int disk_id;
static int disk_total;
static char file_name[100][16];
static int file_size[100];
static char dummy2[6][1024][1024];

int tocou=0;
int maxff;
int maxf[100];
int maxfd;
int maxd[100];
static int SCORE = 0;
static int seed = 3;
static int MOD = 1 << 16;
static int PANELTY = 1000000;

extern void init();
extern void read_file(char* filename, int offset, char* data, int size);
extern void insert_file(char* filename, int offset, char* data, int size);
extern void delete_file(char* filename, int offset, int size);

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

static unsigned long hash(const char *str, int str_len)
{
	unsigned long hash = 5381;

	for (int i = 0; i < str_len; i++)
		hash = (((hash << 5) + hash) + str[i]) % MOD;

	return hash % MOD;
}

bool change_disk(int disk)
{
	if (disk >= 16 || disk < 0) return false;

	disk_id = disk;

	return true;
}


bool read_disk(int sector, char* data)
{
	if (sector >= 1024 || sector < 0) return false;

	memcpy(data, &(disk[disk_id][sector][0]), 1024);

	return true;
}


bool write_disk(int sector, char* data)
{
	if (sector >= 1024 || sector < 0) return false;

	memcpy(&(disk[disk_id][sector][0]), data, 1024);

	return true;
}

static void _build(void)
{
	init();

	for (int c = 0; c < 100; c++)
	{
		for (int l = 0; l < 16; l++)
			file_name[c][l] = 'A' + (pseudo_rand() % 26);

		file_size[c] = 0;
	}
}


static void _insert(void)
{tocou++;
	char data[4096];
	char file_name_arg[17];

	int file = pseudo_rand() % 100;
    maxf[file]++;
    if(maxf[file]>maxff)
        maxff=maxf[file];
	int offset = (pseudo_rand() * pseudo_rand()) % (file_size[file] + 1);
	int data_size = pseudo_rand() % 4096 + 1;

	for (register int i = 0; i < data_size; i++)
		data[i] = pseudo_rand() % 255 + 1;

	for (register int i = 0; i < 16; i++)
		file_name_arg[i] = file_name[file][i];

	insert_file(file_name_arg, offset, data, data_size);
	file_size[file] += data_size;
	disk_total += data_size;
}


static void _delete(void)
{
	char file_name_arg[17];
	int file = pseudo_rand() % 100;
	int offset = (pseudo_rand() * pseudo_rand()) % (file_size[file] + 1);
	int data_size = pseudo_rand() % 4096 + 1;

	if (file_size[file] - offset < data_size)
		data_size = file_size[file] - offset;

	if (data_size == 0)
		return;

	for (register int i = 0; i < 16; i++)
		file_name_arg[i] = file_name[file][i];

	delete_file(file_name_arg, offset, data_size);

	file_size[file] -= data_size;
	disk_total -= data_size;
}


static void _verify(int file)
{
	char file_name_arg[17];
	char data[4096];
	int correct_ans = 0;
	int offset = (pseudo_rand() * pseudo_rand()) % (file_size[file]);
	int data_size = pseudo_rand() % 4096 + 1;

	if (file_size[file] - offset < data_size)
		data_size = file_size[file] - offset;


	for (register int i = 0; i < 16; i++)
		file_name_arg[i] = file_name[file][i];

	read_file(file_name_arg, offset, data, data_size);
	int user_ans = hash(data, data_size);
	scanf("%d", &correct_ans);

	if (correct_ans != user_ans)
		SCORE += PANELTY;
}

int main(void)
{
	freopen("sample_input.txt", "r", stdin);
	setbuf(stdout, NULL);
	printf("---------------\n");
	clock_t START = clock();
	for (int T = 0; T < 10; T++)
	{tocou=0;
    maxff=0;
    maxfd=0;
        for(int i=0;i<100;i++)
        {maxf[i]=0;
        maxd[i]=0;
        }
		_build();
		disk_total = 0;

		while (disk_total < 10 * 1024 * 1024)
		{
			switch (pseudo_rand() % 10)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5: _insert(); break;
			case 6:
			case 7:
			case 8:
			case 9: _delete(); break;
			}
		}
        printf("tocout  %d  \n",tocou);
		for (int i = 0; i < 100; i++)
			_verify(i);
        printf("maxinsert %d\n", maxff);
        int min=160;
        for(int i=0;i<100;i++)
        {
        //if(maxf[i]>150)
            //printf("i:  %d  %d\n", i, maxf[i]);
        }
        
	}

	SCORE += (clock() - START) / (CLOCKS_PER_SEC / 1000);
	printf("SCORE: %d\n", SCORE);
    while(1);
}



/////////////user code

extern bool change_disk(int disk);
extern bool read_disk(int sector, char* data);
extern bool write_disk(int sector, char* data);

bool allFileShow;

int diskBUse[200];
int diskTrueUse[16][1000][3];  //0 start 1 end  2  All size util this block
int nowUseSector = 0;
short nowUseDisk;
int fileIdx;

struct fileName
{
    char name[17];
    int nextHash;
};
fileName mfn[101];

struct fileBlock
{
    int start;
    int end;
    int Disk;
    int Sector;
    int size;
    fileBlock* next;
};
fileBlock  mfb[101];
int fbnum[101];
int hashTable[101];

int mhash(const char *str)
{
	int hash = 31;

	for (int i = 0; i < 16; i++)
		hash = (((hash << 5) + hash) + str[i]) % 100;

	return hash % 100;
}

bool mcmp(const char *str1,const char *str2)
{

	for (int i = 0; i < 16; i++)
    {
		if(str1[i] != str2[i])
        {
            return false;
        }

    }
    return true;

}



int getID(char* filename)
{
    int fileID;
    int hash=mhash(filename);
    bool find=false;

    int next=hashTable[hash];
    if(next==0)
    { 
           return 0;
    }
    else{
        while(next)
        {
            if( mcmp(filename,mfn[next].name) )
            {
                find=true;
                fileID=next;
                return fileID;
                break;
            }

            next=mfn[next].nextHash;

        }
        if(!find)
        {   
            return 0;
        }
    }

}

void outdisk(fileBlock * fb, char* data)
{
    nowUseDisk=fb->Disk;
    nowUseSector=fb->Sector;
    change_disk(nowUseDisk);
    read_disk(nowUseSector,data);
}

void indisk(fileBlock * fb, char* data,int size)
{
    if(nowUseSector>1023)
    {    
        nowUseSector=0;
        nowUseDisk++;
    }
    fb->Disk=nowUseDisk;
    fb->Sector=nowUseSector;
    fb->size=size;
    change_disk(nowUseDisk);
    write_disk(nowUseSector,data);
}

fileBlock* getNewRight(fileBlock* ori,int rsize)
{
    fileBlock *newright=new fileBlock();
    char tmp[1024];
    outdisk(ori,tmp);
    return newright;
}

void init(void)
{
    nowUseSector=0;
    nowUseDisk=0;
    allFileShow=false;
    fileIdx=1;
    for(int i=0; i<101; i++)
    {
        hashTable[i] = 0;
        mfn[i].nextHash=0;
        fbnum[i]=0;
        mfb[i].size= 0;
        mfb[i].next=0;
    }
}


void read_file(char* filename, int offset, char* data, int size)
{
    int fileID=getID(filename);
    fileBlock* blocktmp= &mfb[fileID];
    fileBlock* pre=0;
    int nsum=0;
    while(offset<blocktmp->size+nsum )
    {
        nsum+=blocktmp->size;
        pre=blocktmp;
        blocktmp = blocktmp->next;       
    }
}


void insert_file(char* filename, int offset, char* data, int size)
{
    int fileID;
    int hash=mhash(filename);
    bool find=false;

    int next=hashTable[hash];
    if(next==0)
    { 
        hashTable[hash]=fileIdx;
        for(int i=0;i<16;i++)
            mfn[fileIdx].name[i]=filename[i];
        fileID=fileIdx;
        fileIdx++;
    }
    else{
        while(next)
        {
            if( mcmp(filename,mfn[next].name) )
            {
                find=true;
                fileID=next;
                break;
            }
            next=mfn[next].nextHash;
        }
        if(!find)
        {   
            int tmp=hashTable[hash];
            hashTable[hash]=fileIdx;
            for(int i=0;i<16;i++)
                mfn[fileIdx].name[i]=filename[i];
                
            mfn[fileIdx].nextHash=tmp;
            fileID=fileIdx;
            fileIdx++;
        }
    }
    

    int nsum=0;
    char tmp[1024];
    char rightP[1024];
    fileBlock* blocktmp= &mfb[fileID];
    fileBlock* pre=0;
    while(offset<blocktmp->size+nsum )
    {
        nsum+=blocktmp->size;
        pre=blocktmp;
        blocktmp = blocktmp->next;       
    }
    int left=offset-nsum;
    int right=blocktmp->size-left;
    change_disk(blocktmp->Disk);
    read_disk(blocktmp->Sector,tmp);
    fileBlock* rightPart=new fileBlock();
    int need=size/1024;
    if(size % 1024)
        need++;
    if(left) 
        pre=blocktmp;
    for(int i=0;i<need;i++)
    {
        size= size- (need*1024);
        if(size>1023)
            size=1024;

        if(pre)
        {
            fileBlock* newfb=new fileBlock();
            pre->next=newfb;
        }
        else
        {
            indisk(&mfb[fileID],data+(1024*i),size);
            pre=&mfb[fileID];
        }

    }

}


void delete_file(char* filename, int offset, int size)
{
    int fileID=getID(filename);
    


}

