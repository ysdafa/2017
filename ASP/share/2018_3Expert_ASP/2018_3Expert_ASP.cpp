// 2018_3Expert_ASP.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include<stdio.h>
#include<iostream>
#include <time.h> 
#define RESULT 1000000000

using namespace std;
extern void test(int trash_map[1000][1000]);
static int ori_trash_map[1000][1000], trash_map[1000][1000], trash_can[4], result, seed = 3;

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}
static void build()
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
			trash_map[y][x] = -1; //trash
			c++;
		}
	}

	trash_can[0] = trash_can[1] = trash_can[2] = 0;

	for (int c = 1; c <= 3; )
	{
		int x = pseudo_rand() % 1000;
		int y = pseudo_rand() % 1000;
		if (trash_map[y][x] == 0)
		{
			trash_map[y][x] = c; //trash can
			c++;
		}
	}

	for (int y = 0; y < 1000; y++)
		for (int x = 0; x < 1000; x++)
			ori_trash_map[y][x] = trash_map[y][x];
}


void move_trash(int y, int x, int d)
{
	if (result >= RESULT) return;

	result++;

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

int main()
{
	//printf("---------------\n");
	result = 0;
	for (int t = 0; t < 10; t++)
	{
		build();
		clock_t start = clock();
		test(trash_map);
		//result += (clock() - start) / ((CLOCKS_PER_SEC) / 1000);

		for (int y = 0; y < 1000; y++)
			for (int x = 0; x < 1000; x++)
				if (ori_trash_map[y][x] == -1) result += 10000;
	}
	printf("Score: %d\n", result);
	if (result < 45000000)    // 45 Million
		printf("1\n");
	else
		printf("0\n");
}