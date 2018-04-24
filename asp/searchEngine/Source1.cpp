#include "stdafx.h"


//////////////////////////////////////////////////////////////////////////////////
#include<stdio.h>
#include<string.h>


#pragma GCC optimize("-Ofast")
#define MAXPAGES 50000
#define MAXWORDS 10000
#define MAXBLOCK 10000
#define HASHSIZE 0x7FFF


int NPAGE;
struct page_node {
	int id;
	int num;
	char word[4][11];
	int block;
	bool flag;
};

struct hash_node {
	int hash_table[HASHSIZE];
	int next[MAXPAGES];
};
struct hash_node page_hash[4];
struct page_node page_record[MAXPAGES];

struct keyword
{
	int hash_value;
	char word[11];
};
struct keyword keyword_hash[HASHSIZE];

unsigned BKDRHash(char *str)
{
	unsigned int seed = 131;
	unsigned int hash = 0;
	while (*str)
	{
		hash = hash * seed + (*str++);
	}
	return (hash & HASHSIZE);
}
int mstrcpy(char *des, char *source)
{
	int i = 0;
	do {
		des[i] = source[i];
	} while (source[i++] != '\0');

	return i - 1;
}
int mstrcmp(char *des, char *source)
{
	int i = 0;
	while (des[i] == source[i] && des[i] && source[i])
	{
		i++;
	}
	return (int)(des[i]) - (int)(source[i]);
}

void init(int n) {

	NPAGE = n;
	for (int i = 0; i < HASHSIZE; i++)
	{
		page_hash[0].hash_table[i] = -1;
		page_hash[1].hash_table[i] = -1;
		page_hash[2].hash_table[i] = -1;
		page_hash[3].hash_table[i] = -1;
		keyword_hash[i].hash_value = -1;

	}
	for (int i = 0; i < NPAGE; i++)
	{
		page_hash[0].next[i] = -1;
		page_hash[1].next[i] = -1;
		page_hash[2].next[i] = -1;
		page_hash[3].next[i] = -1;
	}
}

void insert_hash(char *key, int id, int hash_index)
{
	int pos = BKDRHash(key);
	page_hash[hash_index].next[id] = page_hash[hash_index].hash_table[pos];
	page_hash[hash_index].hash_table[pos] = id;
}
void addPage(int mId, int m, char word[][11]) {
	//printf("addPage mId:%d , pageWordNum:%d\n", mId, m);
	int pos;
	int flag = 0;
	int k;
	page_record[mId].id = mId;
	for (int i = 0; i < m; i++)
	{
		mstrcpy(page_record[mId].word[i], word[i]);
		insert_hash(word[i], mId, i);

		pos = BKDRHash(word[i]);
		k = pos;
		while (keyword_hash[k].hash_value != -1)
		{
			if (keyword_hash[k].hash_value == pos && (!mstrcmp(keyword_hash[k].word, word[i])))
			{
				flag = flag + (1 << i);
				break;
			}
			k++;
			k = k & HASHSIZE;
		}
	}

	page_record[mId].block = flag;

	page_record[mId].flag = 1;
	page_record[mId].num = m;
}

void removePage(int mId) {

	page_record[mId].flag = 0;
}

void blockWord(char word[]) {

	int pos = BKDRHash(word);
	int head = 0;

	for (int i = 0; i < 4; i++)
	{
		head = page_hash[i].hash_table[pos];
		while (head != -1)
		{
			if (!mstrcmp(word, page_record[head].word[i]))
			{
				page_record[head].block = page_record[head].block + (1 << i);
			}
			head = page_hash[i].next[head];
		}
	}

	int i = pos;
	while (keyword_hash[i].hash_value != -1)
	{
		i++;
		i = i & HASHSIZE;
	}
	keyword_hash[i].hash_value = pos;
	mstrcpy(keyword_hash[i].word, word);
}

void recoverWord(char word[]) {

	int pos = BKDRHash(word);
	int head = 0;

	for (int i = 0; i < 4; i++)
	{
		head = page_hash[i].hash_table[pos];

		while (head != -1)
		{
			if (!mstrcmp(word, page_record[head].word[i]))
			{
				page_record[head].block = page_record[head].block - (1 << i);
			}
			head = page_hash[i].next[head];
		}
	}

	int i = pos;
	while (keyword_hash[i].hash_value != -1)
	{
		if (keyword_hash[i].hash_value == pos && (!mstrcmp(keyword_hash[i].word, word)))
		{
			keyword_hash[i].hash_value = -1;
			keyword_hash[i].word[0] = '\0';
			break;
		}
		i++;
		i = i & HASHSIZE;
	}

}
int get_count(char word[11])
{
	int pos = BKDRHash(word);
	int head;
	int ans_count = 0;

	for (int i = 0; i < 4; i++)
	{
		head = page_hash[i].hash_table[pos];

		while (head != -1)
		{
			if (page_record[head].flag == 1 && page_record[head].block == 0)
			{
				if (!mstrcmp(word, page_record[head].word[i]))
				{
					ans_count++;
				}
			}
			head = page_hash[i].next[head];
		}
	}
	return ans_count;
}
int get_sum_count(char word1[11], char word2[11])
{
	int pos = BKDRHash(word1);
	int head;
	int ans_count = 0;
	for (int i = 0; i < 4; i++)
	{
		head = page_hash[i].hash_table[pos];
		while (head != -1)
		{
			if (page_record[head].flag == 1 && page_record[head].block == 0)
			{
				if (!mstrcmp(word1, page_record[head].word[i]))
				{
					for (int j = 0; j < page_record[head].num; j++)
					{
						if (j == i)
							continue;
						if (!mstrcmp(word2, page_record[head].word[j]))
						{
							ans_count++;
						}
					}
				}
			}
			head = page_hash[i].next[head];
		}
	}
	return ans_count;
}

int search(char word[][11], int mode) {

	int ans_count = 0;
	int temp_count1 = 0;
	int temp_count2 = 0;
	int temp_count3 = 0;

	if (mode == 0)
	{
		ans_count = get_count(word[0]);
		//printf("mode:%d  word:%s\n", mode, word[0]);

	}
	else if (mode == 1)//and
	{
		ans_count = get_sum_count(word[0], word[1]);
		//printf("mode:%d  word:%s  %s\n", mode, word[0], word[1]);

	}
	else if (mode == 2)//or
	{
		temp_count1 = get_count(word[0]);
		temp_count2 = get_count(word[1]);
		temp_count3 = get_sum_count(word[0], word[1]);

		ans_count = temp_count1 + temp_count2 - temp_count3;
		//printf("mode:%d  word:%s  %s\n", mode, word[0], word[1]);
	}

	//printf("---------------------------------------------search:%d\n", ans_count);
	return ans_count;
}
