// 201801expert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>

static const char alpha[10][8][8] = {
	{ // 0
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ // 1
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 }
	},
	{ // 2
		{ 0, 0, 0, 0, 8, 8, 8, 8 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 8, 8, 8 }
	},
	{ // 3
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 0, 0, 0 },
{ 0, 0, 0, 0, 8, 8, 8, 8 }
	},
	{ // 4
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ // 5
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 }
	},
	{ // 6
		{ 8, 8, 8, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 8, 8, 8, 8, 0, 0, 0, 0 }
	},
	{ // 7
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 0, 0, 0, 8, 0, 0, 0, 0 },
{ 8, 8, 8, 8, 0, 0, 0, 0 }
	},
	{ // 8
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 8, 0, 0, 8, 0, 0 },
{ 0, 0, 8, 0, 0, 8, 0, 0 },
{ 0, 0, 8, 0, 0, 8, 0, 0 },
{ 0, 0, 8, 8, 8, 8, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 }
	},
	{ // 9
		{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 8, 8, 8, 8, 0, 0 },
{ 0, 0, 8, 0, 0, 8, 0, 0 },
{ 0, 0, 8, 0, 0, 8, 0, 0 },
{ 0, 0, 8, 0, 0, 8, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0 }
	}
};


static int dummy1[12];

static char photo[16][64];

static int dummy2[34];

static int seed;

static int dummy3[56];

static void bitblt(int bx, int by, int n)
{
	for (int y = 0; y < 8; y++)
		for (int x = 0; x < 8; x++)
			photo[by + y][bx + x] = alpha[n][y][x];
}

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

extern int recog(char photo[16][64]);

static void shot(int n)
{
	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 64; x++)
			photo[y][x] = 0;

	int cursor = 0;
	int pos = 1000;

	for (int c = 0; c < 4; c++)
	{
		cursor += pseudo_rand() % 9;
		//printf("cursor draw on %d\n", cursor);  //DEBUG
		int x = cursor;
		int y = pseudo_rand() % 9;

		bitblt(x, y, (n / pos) % 10);

		cursor += 8;
		pos /= 10;
	}

	for (int y = 0; y < 16; y++)
		for (int x = 0; x < 64; x++)
			if (pseudo_rand() % 10 == 0)
				photo[y][x] = (photo[y][x] == 0) ? 8 : 0;
}

int main(void)
{
	seed = 3;  // the seed will be changed

	printf("---------------\n");
	int RESULT = 0;
	for (int c = 0; c < 2000; c++)
	{
		int number = pseudo_rand() % 10000;
		//printf("num is -- %d\n", number);  //debug
		shot(number);

		/*for (int y = 0; y < 16; y++){
			for (int x = 0; x < 64; x++)
			{
				if (photo[y][x]==8 )printf("1 ");
				else
					printf("0 ");
			}	
			printf("\n");
		}*/ // DEBUG
		if (recog(photo) == number)
			RESULT++;
		else
		{
			printf("num is -- %d\n", number);  //debug
			printf("ans is -- %d\n\n", recog(photo));  //debug
		}
	}
	printf("RESULT : %d\n", RESULT);
	scanf_s("%d", &seed);  //DEBUG to stop console
	return 0;
}
int find[4];
int compare(char photo[16][64], int x, int y, int n);
int recog(char photo[16][64]) {
	int cursor = 0;
	int result = 0;
	for (int i = 0; i < 4; i++)
	{
		int x = cursor, y = 0;
		int bx = cursor, b_cover = 0, bn = 0;
		for (int iy = 0; iy <= 8; iy++)
			for (int ix = 0; ix <= 8; ix++)
				for (int in = 0; in<10; in++) {
					int cover = compare(photo, x + ix, iy, in);
					if (cover>b_cover) {
						bx = x + ix;
						b_cover = cover;
						bn = in;
					}
				}

		cursor = bx + 8;
		result = result * 10 + bn;
	}
	return result;
}

int compare(char photo[16][64], int x, int y, int n) {
	int cover = 0;
	for (int xi = 0; xi<8; xi++)
		for (int yi = 0; yi<8; yi++) {
			if (alpha[n][yi][xi] == 8)
				if (photo[y + yi][x + xi] == alpha[n][yi][xi])
					cover++;
				else
					cover--;
			/*if(al[n][yi][xi]==1)
			if(photo[y+yi][x+xi]==0)
			cover++;
			else
			cover--;*/
		}

	return cover;
}
