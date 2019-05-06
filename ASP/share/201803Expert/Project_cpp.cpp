#include <iostream>
#include <ctime>

using namespace std;

void test(int trash_map[1000][1000]);

static int seed = 3;  // the seed will be changed
static int result = 0;
static int dummy0[222];
static int trash_map[1000][1000];
static int dummy1[333];
static int ori_trash_map[1000][1000];
static int dummy2[444];
static int trash_can[3];

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

		for (int y = 0; y < 1000; y++)
			for (int x = 0; x < 1000; x++)
				if (ori_trash_map[y][x] == -1) result += 10000;
	}
	
	cout << "RESULT : " << result << endl;
	int T; cin >> T;
	return 0;
}

////////////////
///  user  /////
////////////////


void move_trash(int y, int x, int d);

void test(int trash_map[1000][1000])
{
	int i, j, k,dir;
	for (i = 0; i < 1000; i++)
	{
		for (j = 0; j < 1000; j++)
		{
			move_trash(i, j, 3);
		}
	}

	
}
