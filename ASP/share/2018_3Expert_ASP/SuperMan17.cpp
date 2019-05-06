#include "stdafx.h"

extern void move_trash(int x, int y, int d);

typedef struct {
	int x, y;
} point;

point trash[10000], can[3];

typedef struct {
	unsigned short int dis, ind;
} distance;

distance d[3][10000];
bool valid[10000];

int absol(int a, int b) {
	if (a > b) return a - b;
	else return b - a;
}

int diff(point a, point b) {
	return absol(a.x, b.x) + absol(a.y, b.y);
}

int diff(int c, int t) {
	return absol(trash[t].x, can[c].x) + absol(trash[t].y, can[c].y);
}

void merge(unsigned short int low, unsigned short int mid, unsigned short int high, unsigned short int can) {
	unsigned short int i, j, k;
	unsigned short int n1 = mid - low + 1;
	unsigned short int n2 = high - mid;

	distance L[5000], R[5000];

	for (i = 0; i < n1; i++)
		L[i] = d[can][low + i];
	for (j = 0; j < n2; j++)
		R[j] = d[can][mid + 1 + j];

	i = 0;
	j = 0;
	k = low;
	while (i < n1 && j < n2)
	{
		if (L[i].dis <= R[j].dis)
		{
			d[can][k] = L[i];
			i++;
		}
		else
		{
			d[can][k] = R[j];
			j++;
		}
		k++;
	}

	while (i < n1)
	{
		d[can][k] = L[i];
		i++;
		k++;
	}

	while (j < n2)
	{
		d[can][k] = R[j];
		j++;
		k++;
	}
}

void sort(unsigned short int low, unsigned short int high, unsigned short int can) {
	if (low < high)
	{
		unsigned short int mid = (low + high) / 2;
		sort(low, mid, can);
		sort(mid + 1, high, can);
		merge(low, mid, high, can);
	}
}

void test(int trash_map[1000][1000]) {
	int pointInd = 0;
	for (unsigned short int j = 0; j < 1000; j++) {
		for (unsigned short int i = 0; i < 1000; i++) {
			switch (trash_map[j][i]) {
			case 0: break;
			case -1: trash[pointInd].x = i;
				trash[pointInd].y = j;
				valid[pointInd++] = false;
				break;
			default: can[trash_map[j][i] - 1].x = i;
				can[trash_map[j][i] - 1].y = j;
			}
		}
	}

	for (unsigned short int i = 0; i < 3; i++) {
		for (unsigned short int j = 0; j < 10000; j++) {
			d[i][j].ind = j;
			d[i][j].dis = diff(i, j);
		}
	}

	sort(0, 9999, 0);
	sort(0, 9999, 1);
	sort(0, 9999, 2);

	unsigned canInd[3] = { 0, 0, 0 }, count = 10000;

	while (count) {
		for (unsigned short int i = 0; i < 3; i++) {
			unsigned short int index = canInd[i];
			while (index < 10000 && valid[d[i][index].ind]) index++;
			if (index >= 10000) continue;
			unsigned short int x = trash[d[i][index].ind].x, y = trash[d[i][index].ind].y, dx = can[i].x, dy = can[i].y;
			while (y > dy) move_trash(y--, x, 0);
			while (y < dy) move_trash(y++, x, 1);
			while (x > dx) move_trash(y, x--, 2);
			while (x < dx) move_trash(y, x++, 3);
			valid[d[i][index].ind] = true;
			canInd[i]++;
			count--;
		}
	}
}
