// 201803Expert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <ctime>

using namespace std;

void test(int trash_map[1000][1000]);

static int seed = 1000;  // the seed will be changed
static int result = 0;
static int dummy0[222];
static int trash_map[1000][1000];
static int dummy1[333];
static int ori_trash_map[1000][1000];
static int dummy2[444];
static int trash_can[3];
static int book[1000][1000];

struct note {
	int x;
	int y;
	int s;
	int f;
};
static struct note que[1000001];

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

static void build_map(void)
{
	for (int y = 0; y < 1000; y++)
		for (int x = 0; x < 1000; x++)
			trash_map[y][x] = 0;

	for (int c = 0; c < 10000;)
	{
		int x = pseudo_rand() % 1000;
		int y = pseudo_rand() % 1000;

		if (trash_map[y][x] == 0)
		{
			trash_map[y][x] = -1; // trash
			c++;
		}
	}

	for (int c = 1; c <= 3;)
	{
		int x = pseudo_rand() % 1000;
		int y = pseudo_rand() % 1000;

		if (trash_map[y][x] == 0)
		{
			trash_map[y][x] = c; // trash_can
			c++;
		}
	}

	trash_can[0] = trash_can[1] = trash_can[2] = 0;

	for (int y = 0; y < 1000; y++)
		for (int x = 0; x < 1000; x++)
			ori_trash_map[y][x] = trash_map[y][x];
}

void move_trash(int y, int x, int d)
{
	if (result == 1000000000) return;

	result++;

	int ox = x;
	int oy = y;

	switch (d)
	{
	case 0: y--; break;
	case 1: y++; break;
	case 2: x--; break;
	case 3: x++; break;
	}

	if ((x < 0) || (x >= 1000) || (ox < 0) || (ox >= 1000)) return;
	if ((y < 0) || (y >= 1000) || (oy < 0) || (oy >= 1000)) return;
	
	if (ori_trash_map[y][x] == -1 || ori_trash_map[oy][ox] != -1) return;

	if (ori_trash_map[y][x] == 0)
	{
		ori_trash_map[oy][ox] = 0;
		ori_trash_map[y][x] = -1;
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

int main(void)
{
	cout << "---------------" << endl;

	for (register int T = 0; T < 10; T++)
	{
		build_map();

		time_t START = clock();
		test(trash_map);
		//result += ((clock() - START) / (CLOCKS_PER_SEC / 1000));
		
		cout << "RESULT time : " << result << endl;
		for (int y = 0; y < 1000; y++)
			for (int x = 0; x < 1000; x++)
				if (ori_trash_map[y][x] == -1) result += 1;
	}

	cout << "RESULT : " << result << endl;
	int T; cin >> T;
	return 0;
}

////////////////
///  user  /////
////////////////


void move_trash(int y, int x, int d);

int move_map[4][2] = { -1, 0,
					   1, 0,
					   0, -1,
					   0, 1 };


void bfs(int y, int x)
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
	
	while (head < tail)
	{
		int i = ori_trash_map[y][x] - 1;

		if (trash_can[i] == 3500)
		{
			return;
		}

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
								break;
							}
						}
						current = que[current].f;
					}
				}
			}
		}

		head++;
	}
}


void test(int trash_map[1000][1000])
{
	int x, y;// , k, dir;
	int trash_canx[3];
	int trash_cany[3];
	int can_index = 0;
	for (y = 0; y < 1000; y++)
	{
		for (x = 0; x < 1000; x++)
		{
			if (trash_map[y][x] > 0)
			{
				trash_canx[can_index] = x;
				trash_cany[can_index] = y;
				can_index++;
				//printf("trash_canx is %d, trash_cany is %d \n", x, y);
			}
		}
	}

	for (int i = 0; i < 3; i++)
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
		bfs(trash_cany[i], trash_canx[i]);
	}
	
	int resultemp=0;
	for (int y = 0; y < 1000; y++)
	for (int x = 0; x < 1000; x++)
	if (ori_trash_map[y][x] == -1) resultemp += 1;

	cout << "RESULT  resultemp: " << resultemp << endl;

}
