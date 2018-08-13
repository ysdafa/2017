#include "stdafx.h"

extern bool change_disk(int disk);
extern bool read_disk(int sector, char* data);
extern bool write_disk(int sector, char* data);

static int MOD = 1 << 16;

typedef struct _fNode {
	int disk_id;
	int sector_id;
	int start;
	int sector_size;
	bool toBeDel;
	_fNode* next;
	_fNode() {
		disk_id = -1; sector_id = -1; start = -1; sector_size = -1; toBeDel = false; next = NULL;
	};
} FNode;

typedef struct _file {
	int file_hash;
	int file_size;
	char file_name[16];
	FNode *fn;
} File;
File FS[100];

typedef struct _splitNode {
	FNode* preNode;
	FNode* aftNode;
	_splitNode() { preNode = NULL; aftNode = NULL; }
}SplitNode;

bool Book[16 * 1024] = { false };

void getUnUsed(int &disk, int &sector)
{
	int i = 0;
	for (i = 0; i < 16 * 1024; i++) {
		if (Book[i] == false)
			break;
	}
	Book[i] = true;
	disk = i / 1024;
	sector = i % 1024;
}

static unsigned long myhash(const char *str, int str_len)
{
	unsigned long hash = 5381;

	for (int i = 0; i < str_len; i++)
		hash = (((hash << 5) + hash) + str[i]) % MOD;

	return hash % MOD;
}

bool MyStrCmp(char *src, char* dest, int size){
	for (int i = 0; i < size; i++) {
		if (src[i] != dest[i]) {
			return false;
		}
	}
	return true;
}

void MyStrCpy(char *src, char* dest, int offset, int size) {
	int i;
	for (i = 0; i < size; i++) {
		dest[i] = src[offset + i];
	}
	for (; i < 1024; i++)
	{
		dest[i] = '\0';
	}
}

int FindFile(char* filename, bool &bFound)
{
	int i, j;
	int file_hash = myhash(filename, 16);
	for (i = 0; i < 100; i++) {
		if (FS[i].file_hash != -1) {
			if (FS[i].file_hash == file_hash && MyStrCmp(FS[i].file_name, filename, 16)) {
				bFound = true;
				return i;
			}
		}
		else // new file
		{
			FS[i].file_hash = file_hash;
			for (j = 0; j < 16; j++) FS[i].file_name[j] = filename[j];
			FS[i].file_size = 0;
			bFound = false;
			return i;
		}
	}
	return - 1;
}

void MyWriteDisk(int disk, int sector, char* data)
{
	change_disk(disk);
	write_disk(sector, data);
}

void MyReadDisk(int disk, int sector, char* data)
{
	change_disk(disk);
	read_disk(sector, data);
}

void merge_file(int fileIndex)
{
	int temp = 0;
}

void split_fs(int fileIndex, int offset, int offset2, SplitNode& splitNode) // in the certain sector
{
	// find the offset block
	char temp_char[1024];
	FNode* iter = FS[fileIndex].fn;
	FNode* temp = NULL;
	FNode* aftNode = NULL;
	FNode* preNode = NULL;
	int curSize = 0;
	while (iter) {
		curSize += iter->sector_size;
		if (curSize >= offset) break;
		iter = iter->next;
	}

	preNode = iter;
	preNode->sector_size = preNode->sector_size - (curSize - offset);

	//int aftNode_sector_size = curSize - offset2;
	//if (aftNode_sector_size > 0) {
	aftNode = new FNode;
	char temp_charWrite[1024];

	MyReadDisk(preNode->disk_id, preNode->sector_id, temp_char);
	int disk, sector;
	getUnUsed(disk, sector);
	MyStrCpy(temp_char, temp_charWrite, preNode->sector_size, curSize - offset2);
	MyWriteDisk(disk, sector, temp_charWrite);

	aftNode->disk_id = disk; aftNode->sector_id = sector; aftNode->start = 0;
	aftNode->sector_size = curSize - offset2;
	aftNode->next = preNode->next;
	preNode->next = NULL;
	//}

	splitNode.preNode = preNode;
	splitNode.aftNode = aftNode;
}

void delete_fs(int fileIndex)
{
	FNode* iter = FS[fileIndex].fn;
	FNode* temp = NULL;
	FNode* headNode = NULL;
	FNode* tailNode = NULL;

	bool bDelStart = false;
	bool bDelEnd = false;
	FNode* delNodes[300];
	int k = 0;
	while (iter->next) {
		if (iter->next->toBeDel == true && bDelStart == false)
		{
			headNode = iter;
			bDelStart = true;
			iter = iter->next;
			delNodes[k] = iter; k++;
			continue;
		}

		if (bDelStart && iter->next->toBeDel == false) {
			iter = iter->next;
			delNodes[k] = iter; k++;
			continue;
		}
		
		if (iter->next->toBeDel == true && bDelStart == true && bDelEnd == false)
		{
			tailNode = iter->next->next;
			iter = iter->next;
			delNodes[k] = iter; k++;
			bDelEnd = true;
			break;
		}

		iter = iter->next;
	}
	headNode->next = tailNode;
	for (int i = 0; i < k; i++) {
		temp = delNodes[i];
		Book[temp->disk_id * 1024 + temp->sector_id] = false;
		delete temp;
	}
}

void init(void)
{
	//init FS
	int i = 0; int j = 0;
	for (i = 0; i < 100; i++) {
		FS[i].file_hash = -1;
		FS[i].file_size = -1;
		for (j = 0; j < 100; j++) {
			FS[i].file_name[j] = '\0';
		}
		FNode* iter = FS[i].fn;
		FNode* temp = NULL;
		while (iter)
		{
			temp = iter;
			iter = iter->next;
			if (temp) {
				delete temp; temp = NULL;
			}
		}
	}
}

void read_file(char* filename, int offset, char* data, int size)
{
	
}

void new_insert_file(int fileIndex, int offset, char* data, int size)
{
	int block_size = 1024;
	char temp_char[1024];
	int disk;
	int sector;
	int myoffset = 0;
	int total_size = size;
	FS[fileIndex].file_size += total_size;

	FNode* iter = FS[fileIndex].fn;
	FNode* temp = NULL;
	
	while (total_size > block_size) {
		getUnUsed(disk, sector);
		// write the actual data
		MyStrCpy(data, temp_char, 0, 1024);
		MyWriteDisk(disk, sector, temp_char);
		
		// make the file node
		iter = FS[fileIndex].fn;
		temp = new FNode();
		temp->disk_id = disk; temp->sector_id = sector; temp->start = 0; temp->sector_size = block_size; temp->next = NULL;
		if (iter == NULL) {
			FS[fileIndex].fn = temp;
		}
		else
		{
			while (iter->next) { iter = iter->next; }
			iter->next = temp;
		}

		total_size -= block_size;
		myoffset += block_size;
	}

	if (total_size > 0) {
		getUnUsed(disk, sector);
		MyStrCpy(data, temp_char, myoffset, total_size);
		MyWriteDisk(disk, sector, temp_char);

		// make the file node
		iter = FS[fileIndex].fn;
		temp = new FNode();
		temp->disk_id = disk; temp->sector_id = sector; temp->start = 0; temp->sector_size = total_size; temp->next = NULL;
		if (iter == NULL) {
			FS[fileIndex].fn = temp;
		}
		else
		{
			while (iter->next) { iter = iter->next; }
			iter->next = temp;
		}
	}
}

void exist_insert_file(int fileIndex, int offset, char* data, int size)
{
	int block_size = 1024;
	char temp_char[1024];
	char temp_charWrite[1024];
	int disk;
	int sector;
	int myoffset = 0;
	int total_size = size;
	FS[fileIndex].file_size += total_size;

	FNode* iter = FS[fileIndex].fn;
	FNode* temp = NULL;

	// make function!!
	SplitNode splitNode;
	split_fs(fileIndex, offset, offset, splitNode);

	while (total_size > block_size) {
		getUnUsed(disk, sector);
		// write the actual data
		MyStrCpy(data, temp_char, 0, 640);
		MyWriteDisk(disk, sector, temp_char);

		// make the file node
		temp = new FNode();
		temp->disk_id = disk; temp->sector_id = sector; temp->start = 0; temp->sector_size = block_size; temp->next = NULL;

		iter = splitNode.preNode;
		while (iter->next) { iter = iter->next; }
		iter->next = temp;

		total_size -= block_size;
		myoffset += block_size;
	}

	if (total_size > 0) {
		getUnUsed(disk, sector);
		MyStrCpy(data, temp_char, myoffset, total_size);
		MyWriteDisk(disk, sector, temp_char);

		// make the file node
		temp = new FNode();
		temp->disk_id = disk; temp->sector_id = sector; temp->start = 0; temp->sector_size = total_size; temp->next = NULL;
		iter = splitNode.preNode;
		while (iter->next) { iter = iter->next; }
		iter->next = temp;
	}

	iter = iter->next;
	iter->next = splitNode.aftNode;

	// later do merge
	merge_file(fileIndex);
}

void insert_file(char* filename, int offset, char* data, int size)
{
	bool bFound = false;
	int fileIndex = -1;
	fileIndex = FindFile(filename, bFound);

	if (!bFound) { // insert a new file
		new_insert_file(fileIndex, offset, data, size);
	}
	else { // insert to the exist file
		exist_insert_file(fileIndex, offset, data, size);
	}
}


void delete_file(char* filename, int offset, int size)
{
	bool bFound = false;
	int fileIndex = -1;
	fileIndex = FindFile(filename, bFound);

	if (bFound) { // delete an exist file
		FS[fileIndex].file_size -= size;
		// find the offset block
		FNode* iter = FS[fileIndex].fn;
		FNode* temp = NULL;
		int curSizeStart = 0;
		int curSizeEnd = 0;
		while (iter) {
			curSizeStart += iter->sector_size;
			if (curSizeStart >= offset) break;
			iter = iter->next;
		}

		FNode* start_node = iter;
		FNode* end_node = NULL;
		curSizeEnd = curSizeStart;
		if (curSizeEnd >= (offset + size)) {
			end_node = iter;
		}
		else {
			iter = iter->next;
			while (iter) {
				curSizeEnd += iter->sector_size;
				if (curSizeEnd >= (offset + size)) break;
				iter = iter->next;
			}
			end_node = iter;
		}

		if (start_node == end_node) { // split the current node

			SplitNode splitNode;
			split_fs(fileIndex, offset, offset+size, splitNode);
			if (splitNode.aftNode) splitNode.preNode->next = splitNode.aftNode;
		}
		else {
			SplitNode splitNodeStart;
			split_fs(fileIndex, offset, offset, splitNodeStart);
			if (splitNodeStart.aftNode) splitNodeStart.preNode->next = splitNodeStart.aftNode;
			splitNodeStart.aftNode->toBeDel = true;

			SplitNode splitNodeEnd;
			split_fs(fileIndex, offset+size, offset+size, splitNodeEnd);
			if (splitNodeEnd.aftNode) splitNodeEnd.preNode->next = splitNodeEnd.aftNode;
			splitNodeEnd.preNode->toBeDel = true;

			delete_fs(fileIndex);

		}
	}
	else { // insert to the exist file
		//exist_insert_file(fileIndex, offset, data, size);
		int a = 0;
	}

	// later do merge
	merge_file(fileIndex);
}