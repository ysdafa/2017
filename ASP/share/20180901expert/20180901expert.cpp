// 20180901expert.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>

static char document[1024 * 1024 + 2];
static int DS;
static int SCORE = 0;
static int PANELTY = 0;
static int seed = 3;

static int pseudo_rand(void);
static void verify(char* document);

extern void create(void);
extern void put_char(char c);
extern void put_backspace(void);
extern void put_enter(void);
extern void cursor_up(int n);
extern void cursor_down(int n);
extern void cursor_left(int n);
extern void cursor_right(int n);
extern void close(char* document);

int main(void)
{
	freopen("sample_input.txt", "r", stdin);
	setbuf(stdout, NULL);
	printf("---------------\n");

	clock_t START = clock();

	for (int T = 0; T < 100; T++)
	{
		create();
		DS = 0;

		while (DS < 1024 * 1024 - 1)
		{
			if (pseudo_rand() % 100 == 99)
			{
				put_enter(); DS++; // The enter key is single char
			}
			if (pseudo_rand() % 100 == 99)
			{
				switch (pseudo_rand() % 4)
				{
				case 0: cursor_up(pseudo_rand() % 100);   break; // Move the cursor up n lines
				case 1: cursor_down(pseudo_rand() % 100); break; // Move the cursor down in lines
				case 2: cursor_left(pseudo_rand() % 100); break; // The cursor should always be in the current line
				case 3: cursor_right(pseudo_rand() % 100);   break; // The cursor should always be in the current line
				}
			}
			if (pseudo_rand() % 100 == 99)
			{
				if (DS != 0)
				{
					put_backspace(); DS--; // Delete single char of current cursor position
				}
			}
			put_char('A' + (pseudo_rand() % 26)); DS++; // Insert single char of current cursor position
		}

		close(document); // The document must be verified
		verify(document);
	}


	SCORE += (clock() - START) / (CLOCKS_PER_SEC / 1000);
	printf("SCORE: %d\n", SCORE + PANELTY);
	//while (1);
}

static int pseudo_rand(void)
{
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

static void verify(char *document)
{
	unsigned long hash = 5381;

	for (int i = 0; i < 1024 * 1024 - 1; i++)
	{
		hash = (((hash << 5) + hash) + document[i]) % 2531011;
	}

	int ans_val;
	scanf("%d", &ans_val);
	if (hash != ans_val)
		PANELTY += 1000000;
}
