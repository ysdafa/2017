// 201803Expert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <ctime>

using namespace std;

void test(int trash_map[1000][1000]);

static int seed = 5;  // the seed will be changed
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
static struct note que[10000001];

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
	//cout << "move_trash x : " <<x <<", y : " << y << endl;
	if (ori_trash_map[y][x] == -1 || ori_trash_map[oy][ox] != -1) return;

	//cout << "x : %d, y : %d " << x << y << endl;
	//cout << "move_trash x : " << x << ", y : " << y << endl;
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

	for (register int T = 0; T < 1; T++)
	{
		build_map();

		time_t START = clock();
		test(trash_map);
		//result += ((clock() - START) / (CLOCKS_PER_SEC / 1000));

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

void converge(int y, int x)
{
	int i; 
	for (i = 0; i < 4; i++)
	{
		move_trash(y + move_map[i][0], x + move_map[i][1], i);
	}
}


//void go_back(struct note nt);
//{
//	while (nt.f = !0)
//	{
//
//	}
//}

void print11x11(int y, int x)
{
	cout <<"ori_trash_map " << y << "x" << x << endl;
	for (int j = y - 5; j < y + 5; j++)
	{
		for (int i = x - 5; i < x + 5; i++)
		{
			cout << ori_trash_map[j][i];
		}
		cout << endl;
	}

	cout << "trash_map " << y << "x" << x << endl;
	for (int j = y - 5; j < y + 5; j++)
	{
		for (int i = x - 5; i < x + 5; i++)
		{
			cout << trash_map[j][i];
		}
		cout << endl;
	}

}

void bfs(int y1, int x1, int y2, int x2, int y3, int x3)
{
	
	int head, tail;
	int startx, starty, tx, ty, flag;
	int k, l;

	head = 1;
	tail = 1;
	que[tail].x = x1;
	que[tail].y = y1;
	que[tail].f = 0;
	que[tail].s = 0;
	tail++;
	que[tail].x = x2;
	que[tail].y = y2;
	que[tail].f = 0;
	que[tail].s = 0;
	tail++;
	que[tail].x = x3;
	que[tail].y = y3;
	que[tail].f = 0;
	que[tail].s = 0;
	tail++;

	book[y1][x1] = 1;
	book[y2][x2] = 1;
	book[y3][x3] = 1;

	while (head < tail)
	{

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
				int deltax, deltay;

				//cout << "tx : "<<tx << " ty :" << ty << endl;
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
								//cout << "que[current].x : " << que[current].x << " que[current].y : " << que[current].y << endl;
								//print11x11(que[current].y, que[current].x);
								move_trash(que[current].y, que[current].x, l);
								//print11x11(que[current].y, que[current].x);
								break;
							}
						}
						current = que[current].f;
					}

					//int resultemp=0;
					//int sum = 0;
					//for (int y = 0; y < 1000; y++)
					//{

					//	for (int x = 0; x < 1000; x++)
					//	{
					//		if (ori_trash_map[y][x] == -1)
					//		{
					//			resultemp += 1;
					//		}
					//		/*sum += ori_trash_map[y][x];
					//		if (ori_trash_map[y][x] != trash_map[y][x])
					//		{
					//			cout << "different y:x " << y <<":" << x << endl;
					//		}*/
					//	}
					//}

					//cout << "resultemp : " << resultemp << endl;
					////cout << "sum : " << sum << endl;
					//cout << endl;
				}

				tail++;
				if (tail > 999000)
				{
				//	cout << "tail : " << tail << ", head : " << head << endl;
				}
			}
		}

		/*int resultemp=0;
		for (int y = 0; y < 1000; y++)
			for (int x = 0; x < 1000; x++)
				if (ori_trash_map[y][x] == -1) resultemp += 1;

		cout << "RESULT : " << resultemp << endl;*/

		head++;
	}

	int resultemp=0;
	for (int y = 0; y < 1000; y++)
	for (int x = 0; x < 1000; x++)
	if (ori_trash_map[y][x] == -1) resultemp += 1;

	cout << "result : " << resultemp << endl;
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
			//move_trash(i, j, 3);
			if (trash_map[y][x] > 0)
			{
				trash_canx[can_index] = x;
				trash_cany[can_index] = y;
				can_index++;
				printf("trash_canx is %d, trash_cany is %d \n", x, y);
			}
		}
	}

	for (y = 0; y < 1000; y++)
	{
		for (x = 0; x < 1000; x++)
		{
			book[y][x] = 0;
		}
	}
	bfs(trash_cany[0], trash_canx[0], trash_cany[1], trash_canx[1], trash_cany[2], trash_canx[2]);

	
	int resultemp=0;
	for (int y = 0; y < 1000; y++)
	for (int x = 0; x < 1000; x++)
	if (ori_trash_map[y][x] == -1) resultemp += 1;

	cout << "RESULT  resultemp: " << resultemp << endl;

}
