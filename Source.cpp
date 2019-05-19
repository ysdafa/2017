#include "stdafx.h"

extern int check(unsigned char key[200]);

static unsigned char tempK[200] = { 0 };
static int book[200] = { 0 };
static unsigned char sKey[200] = { 0 };

void adjust(int index)
{
	//if done
	if (book[index]) return;
	
	int ret1, ret2;

	// check whether the number in the index is right
	ret1 = check(tempK);
	int t = tempK[index];
	tempK[index] = 0;
	ret2 = check(tempK);

	
	if (ret1/256 > ret2/256)
	{
		book[index] = 1;
		tempK[index] = t;
		return;
	}

	//reset back
	tempK[index] = t;

	// for loop to switch the none match number to index
	int i, j, k;
	for (k = index+1; k < 200; k++)
	{
		if (book[k]) continue;
		
		//ret1 maybe changed for the k matched!!
		ret1 = check(tempK);
		//switch the number
		int t = tempK[index];
		tempK[index] = tempK[k];
		tempK[k] = t;
		ret2 = check(tempK);

		if (ret2/256 == ret1/256 + 1)
		{
			int bt = tempK[index];
			tempK[index] = 0;
			int ret3 = check(tempK);
			tempK[index] = bt;

			
			if (ret3/256 == ret1/256 ) // match the index num
			{
				book[index] = 1;
				tempK[index] = bt;
				break;
			}
			else // match the k num
			{
				book[k] = 1;
				tempK[index] = bt;
				continue; //shoud match the k
			}
		}

		if (ret2/256 == ret1/256 + 2) // match both
		{
			book[index] = 1;
			book[k] = 1;
			break;
		}

		//restore back
		t = tempK[index];
		tempK[index] = tempK[k];
		tempK[k] = t;
	}
}
void test(unsigned char key[200])
{
	int i, j, k;
	
	for (k = 0; k < 200; k++) book[k] = 0;

	j = 0;
	for (k = 1; k < 256; k++)
	{
		for (i = 0; i < 200; i++) sKey[i] = k;
		int ret = check(sKey);
		ret = ret / 256;
		
		if (ret >= 1)
		{
			tempK[j] = k;
			j++;
		}
	}

	int ret1 = check(tempK);

	for (k = 0; k < 200; k++)
	{
		adjust(k);
	}

	int ret2 = check(tempK);
}
