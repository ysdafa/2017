// 2018_5expert.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <memory.h>
#include <time.h>
#include <stdlib.h>

#define N4096 4096
#define N256  256
#define D4096 16777216
#define N16   16
#define N4064 4064
#define N1024 1024
#define N255  255


static unsigned char mydummy1[N256];
static unsigned char tempmap[4096][4096];

enum Direct
{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM
};

typedef struct _Block_Edge {
	int X; // block x
	int Y; // block y
	int toX;
	int toY;
	int book;
	int nEdgeSum[4];
	int nextRect[4][2];
	int edgeNum[4][256];
	int rectNumRow[256];
	int rectNumCollum[256];
} Block_Edge;

typedef struct _Block_Index {
	int X;
	int Y;
	int book;
} Block_Index;

Block_Index BI[16][16];
Block_Edge BE[16][16];

int EdgeOffset[8][4] = { 0, 1, 0, 0, //top 0
						1, 0, 0, 0, //right 1
						1, 0, 0, 255, //left 2
						0, 1, 255, 0, //bottom 3
};

int MyIntCmp(int dest[], int src[], int N)
{
	int i = 0;
	while (i<N) { 
		if (dest[i] != src[i]) 
			return -1;
		i++;
	}
	return 0;
}

void MyUnsignedCharCpy(unsigned char dest[], unsigned char src[], int N)
{
	int i = 0;
	while (i<N) { dest[i] = src[i]; i++; }
	//dest[i] = '\0';
}

void compare(int rectNumA[], int rectNumB[], float& likeRateRect)
{
	float deltaRectNum = 0;
	float totalRectNum = 0;
	for (int i = 0; i < 256; i++)
	{
		if (rectNumA[i] != 0) totalRectNum++;
		if (rectNumB[i] != 0) totalRectNum++;
		if (rectNumA[i] != 0 && rectNumA[i] == rectNumB[i]) deltaRectNum+=2;
	}

	if (totalRectNum == 0) likeRateRect = 0;
	else likeRateRect = deltaRectNum / totalRectNum * 100;
	//return result;
}


void findRight(int i, int j, Block_Edge startBe, int depth)
{
	if (depth >= 15) return;
	int m, n;
	if (j>15) return;

	float likeRate = 0;
	Block_Edge likeBE;
	int likem;
	int liken;

	for (m = 0; m < 16; m++) {
		for (n = 0; n < 16; n++) {
			if (m == i && n == j) continue;
			if (BE[m][n].book == 0) {
				if ((startBe.nEdgeSum[2] == BE[m][n].nEdgeSum[1]) && (MyIntCmp(startBe.edgeNum[2], BE[m][n].edgeNum[1], 256) == 0)) {									
					if (BI[startBe.toX][startBe.toY + 1].book == 0) {
						BE[m][n].toX = startBe.toX;
						BE[m][n].toY = startBe.toY + 1;
						BE[m][n].book = 1;
						BI[BE[m][n].toX][BE[m][n].toY].book = 1;
						BI[BE[m][n].toX][BE[m][n].toY].X = m;
						BI[BE[m][n].toX][BE[m][n].toY].Y = n;
						findRight(BE[m][n].X, BE[m][n].Y, BE[m][n], ++depth);
					}

					return;
				}
					
				float likeRect;
				compare(startBe.rectNumRow, BE[m][n].rectNumRow, likeRect);

				if(likeRect > likeRate) {
					likeRate = likeRect;              
					likeBE = BE[m][n];
					likem = m;
					liken = n;
				}
			}
		}
	}

	if (likeBE.nEdgeSum[0] > 0 || likeBE.nEdgeSum[1] > 0 || likeBE.nEdgeSum[2] > 0 || likeBE.nEdgeSum[3] > 0) {
		if (likeBE.book == 0) {		
			if (BI[startBe.toX][startBe.toY + 1].book == 0) {
				likeBE.toX = startBe.toX;
				likeBE.toY = startBe.toY + 1;
				likeBE.book = 1;

				BE[likem][liken].book = 1;
				BE[likem][liken].toX = likeBE.toX;
				BE[likem][liken].toY = likeBE.toY;
				BI[likeBE.toX][likeBE.toY].book = 1;
				BI[likeBE.toX][likeBE.toY].X = likem;
				BI[likeBE.toX][likeBE.toY].Y = liken;
				findRight(likeBE.X, likeBE.Y, likeBE, ++depth);
			}
		}
	}
}

void findDown(int i, int j, Block_Edge startBe, int depth)
{
	if (depth >= 15) return;
	int m, n;
	if (j>15) return;

	float likeRate = 0;
	Block_Edge likeBE;
	int likem;
	int liken;

	for (m = 0; m < 16; m++) {
		for (n = 0; n < 16; n++) {
			if (m == i && n == j) continue;
			if (BE[m][n].book == 0) {
				if ((startBe.nEdgeSum[3] == BE[m][n].nEdgeSum[0]) && (MyIntCmp(startBe.edgeNum[3], BE[m][n].edgeNum[0], 256) == 0)
					|| (startBe.nEdgeSum[3] == BE[m][n].nEdgeSum[4]) && (MyIntCmp(startBe.edgeNum[3], BE[m][n].edgeNum[4], 256) == 0)
					|| (startBe.nEdgeSum[7] == BE[m][n].nEdgeSum[0]) && (MyIntCmp(startBe.edgeNum[7], BE[m][n].edgeNum[0], 256) == 0)
					|| (startBe.nEdgeSum[7] == BE[m][n].nEdgeSum[4]) && (MyIntCmp(startBe.edgeNum[7], BE[m][n].edgeNum[4], 256) == 0)) {
					
					if (BI[startBe.toX + 1][startBe.toY].book == 0) {
						BE[m][n].toX = startBe.toX + 1;
						BE[m][n].toY = startBe.toY;
						BE[m][n].book = 1;
						BI[BE[m][n].toX][BE[m][n].toY].book = 1;
						BI[BE[m][n].toX][BE[m][n].toY].X = m;
						BI[BE[m][n].toX][BE[m][n].toY].Y = n;
						findDown(BE[m][n].X, BE[m][n].Y, BE[m][n], ++depth);
					}

					return;
				}

				float likeRect;
				compare(startBe.rectNumCollum, BE[m][n].rectNumCollum, likeRect);

				if (likeRect > likeRate) {
					likeRate = likeRect;
					likeBE = BE[m][n];
					likem = m;
					liken = n;
				}
			}
		}
	}

	if (likeBE.nEdgeSum[0] > 0 || likeBE.nEdgeSum[1] > 0 || likeBE.nEdgeSum[2] > 0 || likeBE.nEdgeSum[3] > 0) {
		if (likeBE.book == 0) {			
			if (BI[startBe.toX + 1][startBe.toY].book == 0) {
				likeBE.toX = startBe.toX + 1;
				likeBE.toY = startBe.toY;
				likeBE.book = 1;

				BE[likem][liken].book = 1;
				BE[likem][liken].toX = likeBE.toX;
				BE[likem][liken].toY = likeBE.toY;

				BI[likeBE.toX][likeBE.toY].book = 1;
				BI[likeBE.toX][likeBE.toY].X = likem;
				BI[likeBE.toX][likeBE.toY].Y = liken;
				findDown(likeBE.X, likeBE.Y, likeBE, ++depth);
			}		
		}
	}
}

void test(unsigned char map[N4096][N4096])
{
	int i, j, k, m;
	//get the reverse map

	//memset(BI, 0, 16 * 16 * sizeof(Block_Index));

	//memset(BE, 0, 16 * 16 * sizeof(Block_Edge));

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			BI[i][j].book = 0;
			BI[i][j].X = 0;
			BI[i][j].Y = 0;

			BE[i][j].X = 0; // block x
			BE[i][j].Y = 0; // block y
			BE[i][j].toX = 0;
			BE[i][j].toY = 0;
			BE[i][j].book = 0;
			for (k = 0; k < 4; k++) {
				BE[i][j].nEdgeSum[k] = 0;
				BE[i][j].nextRect[k][0] = BE[i][j].nextRect[k][1] = 0;
				for (m = 0; m < 256; m++) {
					BE[i][j].edgeNum[k][m] = 0;
				}
			}
			for (m = 0; m < 256; m++) {
				BE[i][j].rectNumRow[m] = 0;
				BE[i][j].rectNumCollum[m] = 0;
			}
		}
	}

	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			BE[i][j].X = i;
			BE[i][j].Y = j;

			for (k = 0; k < 4; k++) {
				for (m = 0; m < 256; m++) {
					int ii = i * 256 + EdgeOffset[k][0] * m + EdgeOffset[k][2];
					int jj = j * 256 + EdgeOffset[k][1] * m + EdgeOffset[k][3];

					BE[i][j].nEdgeSum[k] += map[ii][jj];
					BE[i][j].edgeNum[k][m] = map[ii][jj];
				}
			}
			for (k = 0; k < 256; k++) {
				int tempRect[256] = { 0 };
				for (m = 0; m < 256; m++) {
					int ii = i * 256 + k;
					int jj = j * 256 + m;
					if (map[ii][jj] > 0)
						tempRect[map[ii][jj]]++;
				}
				int tempM = 0;
				int tempCount = 0;
				for (m = 0; m < 256; m++) {
					if (tempRect[m] > tempCount) {
						tempCount = tempRect[m];
						tempM = m;
					}
				}
				if (tempRect[tempM] > 16) {
					BE[i][j].rectNumRow[k] = tempM;
				}
			}

			for (k = 0; k < 256; k++) {
				int tempRect[256] = { 0 };
				for (m = 0; m < 256; m++) {
					int ii = i * 256 + m;
					int jj = j * 256 + k;
					if (map[ii][jj] > 0)
						tempRect[map[ii][jj]]++;
				}
				int tempM = 0;
				int tempCount = 0;
				for (m = 0; m < 256; m++) {
					if (tempRect[m] > tempCount) {
						tempCount = tempRect[m];
						tempM = m;
					}
				}
				if (tempRect[tempM] > 16) {
					BE[i][j].rectNumCollum[k] = tempM;
				}
			}

			BE[i][j].book = 0;
		}
	}

	//jude the angle and the edge of total map
	//int Angle[4][2] = { 0 }; // top-left, top-right, bottom-left, bottom-right
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			if (BE[i][j].nEdgeSum[0] == 0) {
				if (BE[i][j].nEdgeSum[1] == 0) {
					BE[i][j].toX = 0; BE[i][j].toY = 0; BI[0][0].X = i; BI[0][0].Y = j; BI[0][0].book = 1;
					BE[i][j].book = 1;
				} // top-left
				if (BE[i][j].nEdgeSum[2] == 0) {
					BE[i][j].toX = 0; BE[i][j].toY = 15; BI[0][15].X = i; BI[0][15].Y = j; BI[0][15].book = 1;
					BE[i][j].book = 1;
				} // top-right
				BE[i][j].toX = 0;
			}
			if (BE[i][j].nEdgeSum[3] == 0) {
				if (BE[i][j].nEdgeSum[1] == 0) {
					BE[i][j].toX = 15; BE[i][j].toY = 0; BI[15][0].X = i; BI[15][0].Y = j; BI[15][0].book = 1;
					BE[i][j].book = 1;
				}  // bottom-left
				if (BE[i][j].nEdgeSum[2] == 0) {
					BE[i][j].toX = 15; BE[i][j].toY = 15; BI[15][15].X = i; BI[15][15].Y = j; BI[15][15].book = 1;
					BE[i][j].book = 1;
				} // bottom-right
				BE[i][j].toX = 15;
			}
			if (BE[i][j].nEdgeSum[1] == 0) {
				BE[i][j].toY = 0;
			}
			if (BE[i][j].nEdgeSum[2] == 0) {
				BE[i][j].toY = 15;
			}
		}
	}

	Block_Edge be = BE[BI[0][0].X][BI[0][0].Y];

	findRight(BI[0][0].X, BI[0][0].Y, be, 0);
	findDown(BI[0][0].X, BI[0][0].Y, be, 0);

	for (i = 1; i < 16; i++)
	{
		be = BE[BI[i][0].X][BI[i][0].Y];
		findRight(BI[i][0].X, BI[i][0].Y, be, 0);
	}

	//unsigned char *tempmap = (unsigned char *)malloc(4096 * 4096 * sizeof(unsigned char));
	for (i = 0; i < 16; i++) {
		for (j = 0; j < 16; j++) {
			int y2 = i;
			int x2 = j;
			int y1 = BI[i][j].X;
			int x1 = BI[i][j].Y;

			for (int cy = 0; cy < N256; cy++)
			{
				MyUnsignedCharCpy(&tempmap[(y2*N256 + cy)][x2*N256], &map[y1*N256 + cy][x1*N256], N256);
				//memcpy(mydummy1, &map[y1*N256 + cy][x1*N256], N256);
				//memcpy(&map[y1*N256 + cy][x1*N256], &map[y2*N256 + cy][x2*N256], N256);
				//memcpy(&map[y2*N256 + cy][x2*N256], mydummy1, N256);
			}
		}
	}
	for (i = 0; i < 4096; i++) {
		MyUnsignedCharCpy(map[i], tempmap[i], 4096);
	}
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

	// orgmap
	//int i, j, k, m, n;

	//for (i = 0; i < 16; i++) {
	//	for (j = 0; j < 16; j++) {
	//		ORGBE[i][j].X = i;
	//		ORGBE[i][j].Y = j;

	//		for (k = 0; k < 4; k++) {
	//			for (m = 0; m < 256; m++) {
	//				int ii = i * 256 + EdgeOffset[k][0] * m + EdgeOffset[k][2];
	//				int jj = j * 256 + EdgeOffset[k][1] * m + EdgeOffset[k][3];

	//				ORGBE[i][j].nEdgeSum[k] += orgmap[ii][jj];
	//				if (orgmap[ii][jj]>0)
	//					ORGBE[i][j].rectNum[k][orgmap[ii][jj]]++;
	//			}
	//		}
	//	}
	//}
	/*printf("\norgmap\n");
	for (i = 0; i < 4096; i++) {
		for (j = 0; j < 4096; j++) {
			printf("%4d", orgmap[i][j]);
		}
		printf("\n");
	}*/

	/*for (i = 0; i < 1; i++) {
		for (j = 0; j < 16; j++) {
			for (m = 0; m < 256; m++) {
				for (n = 0; n < 256; n++) {
					printf("%4d", orgmap[i*256+m][j*256+n]);
				}
				printf("\n");
			}
			printf("\n");
		}
		printf("\n");
	}*/
	//for (i = 0; i < 1; i++) {
	//	for (j = 0; j < 16; j++) {
	//		for (m = 1; m < 3; m++) {
	//			for (n = 0; n < 256; n++) {
	//				printf("%4d", ORGBE[i][j].rectNum[m][n]);
	//			}
	//			printf("\n");
	//		}
	//		printf("\n");
	//	}
	//}
	//printf("\n");

	//printf("\norgmap nEdgeSum\n");
	//for (i = 0; i < 16; i++) {
	//	for (j = 0; j < 16; j++) {
	//		printf("%8d %8d, %8d %8d\t", ORGBE[i][j].nEdgeSum[0], ORGBE[i][j].nEdgeSum[1], ORGBE[i][j].nEdgeSum[2], ORGBE[i][j].nEdgeSum[3]);
	//	}
	//	printf("\n");
	//}

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

		//int diff = 0;
		//for (int i = 0; i < 4096; i++) {
		//	for (int j = 0; j < 4096; j++) {
		//		if (newmap[i][j] != orgmap[i][j])
		//			diff++;
		//	}
		//}


		printf("%2d : %6d\n", T + 1, result);

	}

	return 0;

}
