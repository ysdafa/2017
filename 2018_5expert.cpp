// 2018_5expert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <memory.h>
#include <time.h>

#define N4096 4096
#define N256  256
#define D4096 16777216
#define N16   16
#define N4064 4064
#define N1024 1024
#define N255  255

unsigned char mymap[4096][4096];

void test(unsigned char map[N4096][N4096])
{
	int i, j, k;
	int Sum_Block[16][16][4] = { 0 };

	for (i = 0; i < 16; i++)
	{
		for (j = 0; j < 16; j++)
		{
			for (k = 0; k < 256; k++)
			{
				Sum_Block[i][j][0] += map[i * 256][j * 256 + k]; // top
				Sum_Block[i][j][3] += map[i * 256 + 255][j * 256 + k]; // bottom

				Sum_Block[i][j][1] += map[i * 256 + k][j * 256]; // right
				Sum_Block[i][j][2] += map[i * 256 + k][j * 256 + 255]; // left
			}
		}
	}

	int a = 0;
}

static int seed = 3;
static unsigned char dummy1[N256];
static unsigned char dummy2[N256];
static unsigned char dummy3[N256];
static unsigned char orgmap[N4096][N4096];
static unsigned char newmap[N4096][N4096];
static unsigned char xoverlap[N4096][N4096];
static unsigned char yoverlap[N4096][N4096];


bool check_map(unsigned char map[N4096][N4096])
{
	if (memcmp(orgmap, map, D4096) == 0)
		return true;

	return false;
}

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

static bool line_check(int x1, int x2, int y1, int y2)
{
	if (x2 - x1 < N16)return false;
	if (y2 - y1 < N16)return false;
	if (memcmp(&xoverlap[y1][x1], dummy3, x2 - x1 + 1) != 0)return false;
	if (memcmp(&xoverlap[y2][x1], dummy3, x2 - x1 + 1) != 0)return false;
	if (memcmp(&yoverlap[x1][y1], dummy3, y2 - y1 + 1) != 0)return false;
	if (memcmp(&xoverlap[x2][y1], dummy3, y2 - y1 + 1) != 0)return false;

	return true;
}

static void draw_line(int x1, int x2, int y1, int y2)
{
	memset(&xoverlap[y1][x1], 1, x2 - x1 + 1);
	memset(&xoverlap[y2][x1], 1, x2 - x1 + 1);
	memset(&yoverlap[x1][y1], 1, y2 - y1 + 1);
	memset(&yoverlap[x2][y1], 1, y2 - y1 + 1);
}

static void draw_rect(void)
{
	int x1, x2, y1, y2, t;

	while (true)
	{
		x1 = N16 + (pseudo_rand() % N4064);
		y1 = N16 + (pseudo_rand() % N4064);
		x2 = N16 + (pseudo_rand() % N4064);
		y2 = N16 + (pseudo_rand() % N4064);

		if (x1 > x2)
		{
			t = x1;
			x1 = x2;
			x2 = t;
		}

		if (y1 > y2)
		{
			t = y1;
			y1 = y2;
			y2 = t;
		}

		if (line_check(x1, x2, y1, y2))
		{
			draw_line(x1, x2, y1, y2);
			break;
		}
	}

	int c = 1 + pseudo_rand() % N255;
	for (int x = x1; x <= x2; x++)orgmap[y1][x] = c;
	for (int x = x1; x <= x2; x++)orgmap[y2][x] = c;
	for (int y = y1; y <= y2; y++)orgmap[y][x1] = c;
	for (int y = y1; y <= y2; y++)orgmap[y][x2] = c;

}

static void build_map(void)
{
	memset(orgmap, 0, D4096);
	memset(xoverlap, 0, D4096);
	memset(yoverlap, 0, D4096);

	for (int c = 0; c < N1024; c++)
	{
		draw_rect();
	}
	memcpy(newmap, orgmap, D4096);

	for (int c = 0; c < N256; c++)
	{
		int x1 = pseudo_rand() % N16;
		int y1 = pseudo_rand() % N16;
		int x2 = pseudo_rand() % N16;
		int y2 = pseudo_rand() % N16;

		for (int cy = 0; cy < N256; cy++)
		{
			memcpy(dummy1, &newmap[y1*N256 + cy][x1*N256], N256);
			memcpy(&newmap[y1*N256 + cy][x1*N256], &newmap[y2*N256 + cy][x2*N256], N256);
			memcpy(&newmap[y2*N256 + cy][x2*N256], dummy1, N256);
		}

	}
}

int main(void)
{
	setbuf(stdout, NULL);
	printf("RESULT\n");

	int result = 1000000;
	memset(dummy3, 0, 256);

	for (register int T = 0; T < 10; T++)
	{
		build_map();
		time_t START = clock();
		test(newmap);
		result += (int)((clock() - START) / (CLOCKS_PER_SEC / 1000));
		if (check_map(newmap))
			result -= 100000;

		printf("%2d : %6d\n", T + 1, result);

	}

	return 0;

}
