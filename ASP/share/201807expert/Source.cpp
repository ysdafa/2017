#include "pch.h"

const int blocksize = 1024;
const int blocknum = 1024 * 16;
//char s[101][1024 * 1024];

typedef struct _node {
	int id;
	int len, nxt;
} node;
node a[100][1024];

bool book[100][1024];
int que[16 * 1024+10];
int tailblock = 0;

static int MOD = 1 << 16;

#define mymin(x,y) ( (x)<(y)?(x):(y))

typedef struct _file {
	char name[17];
	int hash;
} file;
file f[100];

extern bool change_disk(int disk);
extern bool read_disk(int sector, char* data);
extern bool write_disk(int sector, char* data);

unsigned long myhash(const char *str, int str_len) {
	unsigned long hash = 5381;
	for (int i = 0; i < str_len; i++)
		hash = (((hash << 5) + hash) + str[i]) % MOD;
	return hash % MOD;
}

void mymemcpy(char* dest, char *src, int n) {
	for (int i = 0; i < n;i++) {
		dest[i] = src[i];
	}
}

bool mystrcmp(char* dest, char *src, int n) {
	for (int i = 0; i < n; i++) {
		if (dest[i] != src[i]) return false;
	}
	return true;
}

int getPosition(int k)
{
	int i = 0;
	while (book[k][i] != false) i++;

	return i;
}

int newnode(int k)
{
	int id = que[tailblock];
	tailblock--;
	
	int i = getPosition(k);
	a[k][i].id = id;
	book[k][i] = true;

	return i;
}

void delnode(int k, int id, int t)
{
	tailblock++;
	que[tailblock] = id;

	book[k][t] = false;
}

void find(int k, int &now, int &pos)
{
	for (now = 0; now != -1 && a[k][now].len < pos; now = a[k][now].nxt) 
		pos -= a[k][now].len;
	//pos >0; a[now].len<pos;
}

void _write_disk(int k, int now, char* data)
{
	int disk = a[k][now].id / 1024;
	int sector = a[k][now].id % 1024;
	change_disk(disk);
	write_disk(sector, data);
}

void _read_disk(int k, int now, char* data)
{
	int disk = a[k][now].id / 1024;
	int sector = a[k][now].id % 1024;
	change_disk(disk);
	read_disk(sector, data);
}

void fillnode(int k, int now, int n, char *data, int nxt)
{
	a[k][now].nxt = nxt; 
	a[k][now].len = n;
	char tdata[blocksize] = {0};
	mymemcpy(tdata, data, n);

	_write_disk(k, now, tdata);
}

void divide(int k, int now, int pos)
{
	if (pos == a[k][now].len) return;
	int t = newnode(k);
	char data[blocksize] = { 0 };
	_read_disk(k, now, data);

	fillnode(k, t, a[k][now].len - pos, data + pos, a[k][now].nxt);
	a[k][now].nxt = t; a[k][now].len = pos;
}

void maintain(int k, int now)
{
	for (; now != -1; now = a[k][now].nxt)
	{
		int t = a[k][now].nxt;

		if (t == -1 || a[k][now].len + a[k][t].len > blocksize) continue; // if no maintain, just skip

		char data[blocksize] = { 0 };
		_read_disk(k, now, data);

		bool flag = false;
		for (; t != -1 && a[k][now].len + a[k][t].len <= blocksize; t = a[k][t].nxt) {
			char tdata[blocksize] = { 0 };
			_read_disk(k, t, tdata);

			mymemcpy(data + a[k][now].len, tdata, a[k][t].len);
			a[k][now].len += a[k][t].len;
			a[k][now].nxt = a[k][t].nxt;
			delnode(k, a[k][t].id, t);
			flag = true;
		}
		// no change, don't write
		if (flag) {
			_write_disk(k, now, data);
		}
		
	}
}

void insert(int k, int pos, char *s, int n)
{
	int now, i = 0;
	find(k, now, pos);
	divide(k, now, pos);
	for (i = 0; i + blocksize <= n; i += blocksize)
	{
		int t = newnode(k);
		fillnode(k, t, blocksize, s + i, a[k][now].nxt);
		if (now != t) {
			a[k][now].nxt = t;
			now = t;
		}
	}

	if (i < n)
	{
		int t = newnode(k);
		fillnode(k, t, n - i, s + i, a[k][now].nxt);
		
		if (now != t) {
			a[k][now].nxt = t;
		}
	}
	maintain(k, now);
}

void del(int k, int pos, int n)
{
	int now, i, t;
	int oripos = pos+n;
	find(k, now, pos);
	divide(k, now, pos);
	/*for (i = a[k][now].nxt; i != -1 && a[k][i].len < n; i = a[k][i].nxt)
		n -= a[k][i].len;
	divide(k, i, n);
	*/
	
	find(k, i, oripos);
	divide(k, i, oripos);

	i = a[k][i].nxt;

	for (t = a[k][now].nxt; t != i; t = a[k][now].nxt)
	{
		a[k][now].nxt = a[k][t].nxt;
		delnode(k, a[k][t].id, t);
	}
	maintain(k, now);
}

void get(int k, int pos, char* s, int n)
{
	int now, i, t;
	find(k, now, pos);
	i = mymin(n, a[k][now].len - pos);

	char data[blocksize] = { 0 };
	_read_disk(k, now, data);

	mymemcpy(s, data + pos, i);
	for (t = a[k][now].nxt; t != -1 && i + a[k][t].len <= n; t = a[k][t].nxt)
	{
		_read_disk(k, t, data);

		mymemcpy(s + i, data, a[k][t].len);
		i += a[k][t].len;
	}

	if (i < n)
	{
		_read_disk(k, t, data);
		mymemcpy(s + i, data, n - i);
	}
}

int getfileK(char* filename) {
	int filehash = myhash(filename, 16);
	int n = -1;
	for (int i = 0; i < 100; i++)
	{
		if (f[i].hash == -1) {
			n = i; 
			break;
		}
		if (f[i].hash == filehash && mystrcmp(filename, f[i].name, 16))
			return i; // found the file k
	}

	f[n].hash = filehash;
	mymemcpy(f[n].name, filename, 16);
	return n;
}

void init()
{
	tailblock = 0;
	for (int i = blocknum-1; i >= 0; i--) {
		tailblock++;
		que[tailblock] = i;
	}

	for (int k = 0; k < 100; k++) {
		a[k][0].len = 0;
		a[k][0].nxt = -1;
		f[k].hash = -1;
		for (int i = 0; i < 16; i++) f[k].name[i] = 0;
	}

	for (int k = 0; k < 100; k++) {
		for (int i = 0; i < blocknum/10; i++) {
			book[k][i] = false;
		}
	}
}

void read_file(char* filename, int offset, char* data, int size)
{
	int k = getfileK(filename);
	get(k, offset, data, size);
}

void insert_file(char* filename, int offset, char* data, int size)
{
	int k = getfileK(filename);
	insert(k, offset, data, size);
}

void delete_file(char* filename, int offset, int size) 
{
	int k = getfileK(filename);
	del(k, offset, size);
}