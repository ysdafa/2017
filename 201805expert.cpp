//
//  main.cpp
//  201805Expert
//
//  Created by yansu on 2018/5/19.
//  Copyright © 2018 yansu. All rights reserved.
//

//#include <iostream>


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

enum Direct
{
    TOP,
    LEFT,
    RIGHT,
    BOTTOM
};

typedef struct _Block_Edge{
    int nEdgeSum[4];
    int edgeNum[4][256];
    int rectNum[4][256];
    int X; // block x
    int Y; // block y
    int toX;
    int toY;
    int nextRect[4][2];
    int book;
} Block_Edge;

Block_Edge BE[16][16];
int EdgeOffset[4][4] = {0, 1, 0, 0, //top
                        1, 0, 0, 0, //right
                        1, 0, 0, 255, //left
                        0, 1, 255, 0 //bottom
};

void findRight(int i, int j, Block_Edge startBe)
{
    int m, n;
    if(j>15) return;
    
    for (m = 0; m < 16; m++) {
        for (n = 0; n < 16; n++) {
            if(m==i&&n==j) continue;
            if(BE[m][n].book==0) {
                if(m==12) {
                    int b = 1;
                    b = 2;
                }
                if(startBe.nEdgeSum[2]==BE[m][n].nEdgeSum[1]) {
                    BE[m][n].toX = startBe.toX;
                    BE[m][n].toY = startBe.toY+1;
                    BE[m][n].book = 1;
                    
                    findRight(BE[m][n].X, BE[m][n].Y, BE[m][n]);
                    return;
                }
            }
        }
    }
}

void findDown(int i, int j, Block_Edge startBe)
{
    int m, n;
    if(i>15) return;
    
    for (m = 0; m < 16; m++) {
        for (n = 0; n < 16; n++) {
            if(m==i&&n==j) continue;
            if(BE[m][n].book==0) {
                if(startBe.nEdgeSum[3]==BE[m][n].nEdgeSum[0]) {
                    BE[m][n].toX = startBe.toX+1;
                    BE[m][n].toY = startBe.toY;
                    BE[m][n].book = 1;
                    
                    findDown(BE[m][n].X, BE[m][n].Y, BE[m][n]);
                    return;
                }
            }
        }
    }
}

void test(unsigned char map[N4096][N4096])
{
    int i, j, k, m;
    //int Sum_Block[16][16][4] = { 0 };
    
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            BE[i][j].X = i;
            BE[i][j].Y = j;
            
            for (k = 0; k < 4; k++ ){
                for (m = 0; m < 256; m++){
                    int ii = i*256+EdgeOffset[k][0]*m+EdgeOffset[k][2];
                    int jj = j*256+EdgeOffset[k][1]*m+EdgeOffset[k][3];
                    
                    BE[i][j].nEdgeSum[k] += map[ii][jj];
                    BE[i][j].edgeNum[k][m] = map[ii][jj];
                    if(map[ii][jj]>0)
                        BE[i][j].rectNum[k][map[ii][jj]]++;
                    BE[i][j].book = 0;
                }
            }
        }
    }
    
    printf("edge sum of map is \n");
    for(i = 0; i< 16; i++) {
        for(j = 0; j<16; j++) {
            printf("\t%8d%8d%8d%8d%8d%8d\n", i, j, BE[i][j].nEdgeSum[0], BE[i][j].nEdgeSum[1], BE[i][j].nEdgeSum[2], BE[i][j].nEdgeSum[3]);
        }
        printf("\n");
    }
    
    //jude the angle and the edge of total map
    int Angle[4][2] = {0}; // top-left, top-right, bottom-left, bottom-right
    for (i = 0; i < 16; i++) {
        for (j = 0; j < 16; j++) {
            if(BE[i][j].nEdgeSum[0]==0) {
                if(BE[i][j].nEdgeSum[1]==0) {
                    BE[i][j].toX = 0; BE[i][j].toY = 0; Angle[0][0] = i; Angle[0][1] = j;
                    BE[i][j].book = 1;
                } // top-left
                if(BE[i][j].nEdgeSum[2]==0) {
                    BE[i][j].toX = 0; BE[i][j].toY = 15; Angle[1][0] = i; Angle[1][1] = j;
                    BE[i][j].book = 1;
                } // top-right
                BE[i][j].toX = 0;
            }
            if(BE[i][j].nEdgeSum[3]==0) {
                if(BE[i][j].nEdgeSum[1]==0) {
                    BE[i][j].toX = 15; BE[i][j].toY = 0; Angle[2][0] = i; Angle[2][1] = j;
                    BE[i][j].book = 1;
                }  // bottom-left
                if(BE[i][j].nEdgeSum[2]==0) {
                    BE[i][j].toX = 15; BE[i][j].toY = 15; Angle[3][0] = i; Angle[3][1] = j;
                    BE[i][j].book = 1;
                } // bottom-right
                BE[i][j].toX = 15;
            }
            if(BE[i][j].nEdgeSum[1]==0) {
                BE[i][j].toY = 0;
            }
            if(BE[i][j].nEdgeSum[2]==0) {
                BE[i][j].toY = 15;
            }
        }
    }
    
    /*printf("edge of orgmap reverse is \n");
        for(i = 0; i< 16; i++) {
            for(j = 0; j<16; j++) {
            printf("\t%8d%8d%8d%8d\n", BE[j][i].nEdgeSum[0], BE[j][i].nEdgeSum[1], BE[j][i].nEdgeSum[2], BE[j][i].nEdgeSum[3]);
        }
        printf("\n");
    }*/
    
   
    Block_Edge be = BE[Angle[0][0]][Angle[0][1]];
    
    findRight(Angle[0][0], Angle[0][1], be);
    findDown(Angle[0][0], Angle[0][1], be);
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
    
    /*for (int i = 0; i < 4096; i++) {
        printf("%6d", i);
    }
    printf("\n");
    for (int i = 0; i < 4096; i++) {
        for (int j = 0; j < 4096; j++) {
            printf("%6d", orgmap[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/
    
}

int main(void)
{
    setbuf(stdout, NULL);
    //printf("RESULT\n");
    
    int result = 1000000;
    memset(dummy3, 0, 256);
    
    for (register int T = 0; T < 1; T++)
    {
        build_map();
        time_t START = clock();
        test(newmap);
        result += (int)((clock() - START) / (CLOCKS_PER_SEC / 1000));
        if (check_map(newmap))
            result -= 100000;
        
        //printf("%2d : %6d\n", T + 1, result);
        
    }
    
    return 0;
    
}
