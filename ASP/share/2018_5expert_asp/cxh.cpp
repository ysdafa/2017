#include "stdafx.h"

#pragma GCC optimize ("-Ofast")
struct quve {
	int y, x, c;
	long long int left, right, up, down;
}qs[256];
unsigned char mymap[4096][4096];
int base = 1007;
bool le, up, ri, dw;

bool check_map(unsigned char map[4096][4096]);

bool checkright(int y, int x, unsigned char v, int deep) {
	if (deep == 25) return false;
	if (v == mymap[y][x + 1]) return true;
	if (mymap[y][x + 1] == 0) return false;
	return checkright(y, x + 1, v, deep + 1);
}
bool checkleft(int y, int x, unsigned char v, int deep) {
	if (deep == 25) return false;
	if (v == mymap[y][x - 1]) return true;
	if (mymap[y][x - 1] == 0) return false;
	return checkleft(y, x - 1, v, deep + 1);
}
bool checkup(int y, int x, unsigned char v, int deep) {
	if (deep == 25) return false;
	if (v == mymap[y - 1][x]) return true;
	if (mymap[y - 1][x] == 0) return false;
	return checkup(y - 1, x, v, deep + 1);
}
bool checkdown(int y, int x, unsigned char v, int deep) {
	if (deep == 25) return false;
	if (v == mymap[y + 1][x]) return true;
	if (mymap[y + 1][x] == 0) return false;
	return checkdown(y + 1, x, v, deep + 1);
}

char checkl(int y, int x) {
	int stat = 0;
	if (mymap[y][x] != 0) {
		ri = checkright(y, x, mymap[y][x], 0);
		up = checkup(y, x, mymap[y][x], 0);
		dw = checkdown(y, x, mymap[y][x], 0);
		if (up && dw) return checkl(y, x + 1);
		if (ri) stat += 1;
		if (up) stat += 1;
		if (dw) stat += 1;
		if (stat == 1)
			return mymap[y][x];
		else
			return 0;
	}
	return 0;
}
char checku(int y, int x) {
	int stat = 0;
	if (mymap[y][x] != 0) {
		ri = checkright(y, x, mymap[y][x], 0);
		le = checkleft(y, x, mymap[y][x], 0);
		dw = checkdown(y, x, mymap[y][x], 0);
		if (ri && le) return checku(y + 1, x);
		if (ri) stat += 1;
		if (le) stat += 1;
		if (dw) stat += 1;
		if (stat == 1)
			return mymap[y][x];
		else
			return 0;
	}
	return 0;
}
char checkr(int y, int x) {
	int stat = 0;
	if (mymap[y][x] != 0) {
		le = checkleft(y, x, mymap[y][x], 0);
		up = checkup(y, x, mymap[y][x], 0);
		dw = checkdown(y, x, mymap[y][x], 0);
		if (up && dw) return checkr(y, x - 1);
		if (le) stat += 1;
		if (up) stat += 1;
		if (dw) stat += 1;
		if (stat == 1)
			return mymap[y][x];
		else
			return 0;
	}
	return 0;
}
char checkd(int y, int x) {
	int stat = 0;
	if (mymap[y][x] != 0) {
		ri = checkright(y, x, mymap[y][x], 0);
		le = checkleft(y, x, mymap[y][x], 0);
		up = checkup(y, x, mymap[y][x], 0);
		if (ri && le) return checkd(y - 1, x);
		if (ri) stat += 1;
		if (le) stat += 1;
		if (up) stat += 1;
		if (stat == 1)
			return mymap[y][x];
		else
			return 0;
	}
	return 0;
}
bool ismatch[16][16];
void write(int n, int y, int x, unsigned char map[4096][4096]) {
	for (int i = 0; i<256; i++)
		for (int j = 0; j<256; j++)
			map[y + i][x + j] = mymap[qs[n].y + i][qs[n].x + j];
}
void find(int type, long long int hash, int y, int x, unsigned char map[4096][4096]) {
	for (int i = 0; i<256; i++) {
		if ((type == 2 && qs[i].left == hash) || (type == 3 && qs[i].up == hash) || (type == 0 && qs[i].right == hash) || (type == 1 && qs[i].down == hash)) {
			write(i, y * 256, x * 256, map);
			ismatch[y][x] = true;
			if (qs[i].left != 0 && !ismatch[y][x - 1]) find(0, qs[i].left, y, x - 1, map);
			if (qs[i].up != 0 && !ismatch[y - 1][x]) find(1, qs[i].up, y - 1, x, map);
			if (qs[i].right != 0 && !ismatch[y][x + 1]) find(2, qs[i].right, y, x + 1, map);
			if (qs[i].down != 0 && !ismatch[y + 1][x]) find(3, qs[i].down, y + 1, x, map);
			return;
		}
	}
}
void test(unsigned char map[4096][4096])
{
	for (int i = 0; i<4096; i++) {
		for (int j = 0; j<4096; j++) {
			mymap[i][j] = map[i][j];
		}
	}

	for (int i = 0; i<16; i++) {
		for (int j = 0; j<16; j++) {
			ismatch[i][j] = false;

			qs[i * 16 + j].y = i * 256;
			qs[i * 16 + j].x = j * 256;

			qs[i * 16 + j].left = 0;
			qs[i * 16 + j].up = 0;
			qs[i * 16 + j].right = 0;
			qs[i * 16 + j].down = 0;
			for (int k = 26; k<230; k++) {
				qs[i * 16 + j].left = qs[i * 16 + j].left * base + checkl(qs[i * 16 + j].y + k, qs[i * 16 + j].x);
				qs[i * 16 + j].up = qs[i * 16 + j].up * base + checku(qs[i * 16 + j].y, qs[i * 16 + j].x + k);
				qs[i * 16 + j].right = qs[i * 16 + j].right * base + checkr(qs[i * 16 + j].y + k, qs[i * 16 + j].x + 255);
				qs[i * 16 + j].down = qs[i * 16 + j].down * base + checkd(qs[i * 16 + j].y + 255, qs[i * 16 + j].x + k);
			}
		}
	}

	for (int i = 0; i<256; i++) {
		if (qs[i].left == 0 && qs[i].up == 0) {
			find(2, qs[i].right, 0, 1, map);
			break;
		}
	}
}
