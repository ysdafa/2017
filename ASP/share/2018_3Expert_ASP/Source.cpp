#include "stdafx.h"

#define ABSSub(x, y) (x>y?(x-y):(y-x))

extern void move_trash(int y, int x, int d);
struct note {
	int x;
	int y;
	int s;
	int f;
};
static struct note que[1000001];

static int book[1000][1000];

static int ori_trash_map[1000][1000], trash_can[4];

typedef struct tDis {
	int x; int y;
	int mind;
	int canId;
	int d[3];
} TDis;
TDis TD[3][5000];

int move_map[4][2] = { -1, 0,
1, 0,
0, -1,
0, 1 };


void move_trash_local(int y, int x, int d)
{
	//if (result >= RESULT) return;

	//result++;

	int oy = y;
	int ox = x;

	switch (d)
	{
	case 0: y--; break;
	case 1: y++; break;
	case 2: x--; break;
	case 3: x++; break;
	}
	if ((y<0) || (y >= 1000) || (oy<0) || (oy >= 1000)) return;
	if ((x<0) || (x >= 1000) || (ox<0) || (ox >= 1000)) return;

	if (ori_trash_map[y][x] == -1 || ori_trash_map[oy][ox] != -1) return;

	if (ori_trash_map[y][x] == 0)
	{
		ori_trash_map[y][x] = -1;
		ori_trash_map[oy][ox] = 0;
	}
	else
	{
		int i = ori_trash_map[y][x] - 1;
		if (trash_can[i] == 3500)
			return;

		ori_trash_map[oy][ox] = 0;
		trash_can[i]++;
	}
}

void bfs(int y, int x, int offset)
{
	int head, tail;
	int tx, ty;
	int k, l;

	head = 1;
	tail = 1;
	que[tail].x = x;
	que[tail].y = y;

	que[tail].f = 0;
	que[tail].s = 0;
	tail++;

	int count = 0;

	while (head < tail)
	{
		if (count == 3500 && offset!=2) return;
		int i = ori_trash_map[y][x] - 1;

		for (k = 0; k < 4; k++)
		{
			tx = que[head].x + move_map[k][1];
			ty = que[head].y + move_map[k][0];

			if (tx < 0 || tx >= 1000 || ty < 0 || ty >= 1000)
				continue;

			if (book[ty][tx] == 0)
			{
				book[ty][tx] = 1;
				que[tail].x = tx;
				que[tail].y = ty;
				que[tail].f = head;
				que[tail].s = que[head].s + 1;
				// go back
				int current = tail;
				tail++;
				int deltax, deltay;

				if (ori_trash_map[que[current].y][que[current].x] == -1)
				{
					while (que[current].f >= 0 && current > 0)
					{
						deltax = que[que[current].f].x - que[current].x;
						deltay = que[que[current].f].y - que[current].y;
						for (l = 0; l < 4; l++)
						{
							if (move_map[l][0] == deltay && move_map[l][1] == deltax)
							{
								move_trash(que[current].y, que[current].x, l);
								move_trash_local(que[current].y, que[current].x, l);
								break;
							}
						}
						current = que[current].f;
					}
					count++;
				}
			}
		}

		head++;
	}
}

void quickSort(int left, int right, TDis td[])
{
	if (left > right) return;

	int i = left; int j = right;

	TDis temp = td[left];
	TDis t;
	while (i!=j) {
		if (td[j].mind >= temp.mind && i<j) j--;
		if (td[i].mind <= temp.mind && i<j) i++;

		if (i < j)
		{
			t = td[i];
			td[i] = td[j];
			td[j] = t;
		}
	}
	td[left] = td[i];
	td[i] = temp;

	quickSort(left, i - 1, td);
	quickSort(i+1, right, td);
}

void test(int trash_map[1000][1000]) {
	int x, y;// , k, dir;
	int trash_canx[3];
	int trash_cany[3];
	int can_index = 0;

	trash_can[0] = trash_can[1] = trash_can[2] = 0;
	for (y = 0; y < 1000; y++)
	{
		for (x = 0; x < 1000; x++)
		{
			ori_trash_map[y][x] = trash_map[y][x];
			if (trash_map[y][x] > 0)
			{
				trash_canx[can_index] = x;
				trash_cany[can_index] = y;
				can_index++;
				//printf("trash_canx is %d, trash_cany is %d \n", x, y);
			}
		}
	}

	int K3[3] = { 0 };
	int canNum[3] = {0};
	int d[3] = {0};
	int total = 0;
	for (y = 0; y < 1000; y++)
	{
		for (x = 0; x < 1000; x++)
		{
			if (ori_trash_map[y][x] == -1) {
				//TD[k].x = x; TD[k].y = y;
				d[0] = ABSSub(x, trash_canx[0]) + ABSSub(y, trash_cany[0]);
				d[1] = ABSSub(x, trash_canx[1]) + ABSSub(y, trash_cany[1]);
				d[2] = ABSSub(x, trash_canx[2]) + ABSSub(y, trash_cany[2]);
				if (d[0] <= d[1] && d[0] <= d[2]) {
					TD[0][K3[0]].mind = d[0]; TD[0][K3[0]].canId = 0;
					TD[0][K3[0]].d[0] = d[0]; TD[0][K3[0]].d[1] = d[1]; TD[0][K3[0]].d[2] = d[2];
					TD[0][K3[0]].x = x; TD[0][K3[0]].y = y;
					canNum[0]++; K3[0]++;

					total += TD[0][K3[0]].mind;
				}
				if (d[1] < d[2] && d[1] < d[0]) {
					TD[1][K3[1]].mind = d[1]; TD[1][K3[1]].canId = 1;
					TD[1][K3[1]].d[0] = d[0]; TD[1][K3[1]].d[1] = d[1]; TD[1][K3[1]].d[2] = d[2];
					TD[1][K3[1]].x = x; TD[1][K3[1]].y = y;
					canNum[1]++; K3[1]++;
					total += TD[1][K3[1]].mind;
				}
				if (d[2] < d[1] && d[2] < d[0]) {
					TD[2][K3[2]].mind = d[2]; TD[2][K3[2]].canId = 2;
					TD[2][K3[2]].d[0] = d[0]; TD[2][K3[2]].d[1] = d[1]; TD[2][K3[2]].d[2] = d[2];
					TD[2][K3[2]].x = x; TD[2][K3[2]].y = y;
					canNum[2]++; K3[2]++;
					total += TD[2][K3[2]].mind;
				}
			}
		}
	}

	for (int i = 0; i < 3; i++) {
		if (canNum[i] >= 3500) {
			quickSort(0, canNum[i]-1, TD[i]);
		}
	}
	


	/*for (int i = 0; i < 3; i++)
	{
		for (y = 0; y < 1000; y++)
		{
			for (x = 0; x < 1000; x++)
			{
				book[y][x] = 0;
			}
		}
		book[trash_cany[0]][trash_canx[0]] = 1;
		book[trash_cany[1]][trash_canx[1]] = 1;
		book[trash_cany[2]][trash_canx[2]] = 1;
		bfs(trash_cany[i], trash_canx[i], i);
	}*/
}