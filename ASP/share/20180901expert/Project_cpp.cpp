
#pragma region main

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

	for (register int T = 0; T < 100; T++)
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
	while (1);
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

#pragma endregion



// user code
#define BLEN 100

struct head
{
	int blockInd;
	int nextHead;
	int preHead;
};
struct block
{
	int len;
	char content[BLEN];
	int nextBlock;
	int preBlock;
};

char global[1048577][BLEN];
bool used[1048577];

head Allhead[1048577];
block AllBlock[1048577];
block tmpBlock;

int lines;
int nowline;
int sumline;
int leftIndex;
int nowblock;
int blockInd;
int headInd;
int cursorPos;

void mystrcpy(char* from, char* to, int lencpy)
{
	int len = 0;
	while (len != lencpy)
	{
		to = from;
		++from;
		++to;
		++len;
	}
	
}

void create(void)
{
	lines = 0;
	nowblock = 1;
	nowline = 1;
	headInd = 1;
	blockInd = 1;
	sumline = 1;

	Allhead[1].blockInd = 1;
	Allhead[1].nextHead = 0;
	Allhead[1].preHead = 0;
	AllBlock[1].len = 0;
	AllBlock[1].nextBlock = 0;
	AllBlock[1].preBlock = 0;
	cursorPos = 0;
	leftIndex = 0;
}

void put_char(char c)
{
	int thislen = AllBlock[nowblock].len;
	leftIndex++;
	if (cursorPos == thislen)
	{
		if (cursorPos < BLEN)
		{
			cursorPos++;

			AllBlock[nowblock].content[thislen] = c;
			AllBlock[nowblock].len++;
		}
		else
		{
			tmpBlock.len = 1;
			tmpBlock.preBlock = nowblock;
			tmpBlock.content[0] = c;
			tmpBlock.nextBlock = AllBlock[nowblock].nextBlock;

			blockInd++;
			AllBlock[nowblock].nextBlock = blockInd;

			AllBlock[blockInd] = tmpBlock;
			nowblock = blockInd;
			cursorPos = 1;

		}
	}
	else
	{
		tmpBlock.len = AllBlock[nowblock].len - cursorPos;
		tmpBlock.preBlock = blockInd + 2;
		tmpBlock.nextBlock = AllBlock[nowblock].nextBlock;


		AllBlock[blockInd] = tmpBlock;

		block tmp2;
		tmp2.len = 1;
		tmp2.preBlock = nowblock;
		tmp2.nextBlock = blockInd + 1;
		tmp2.content[0] = c;

		mystrcpy(AllBlock[nowblock].content + cursorPos, tmpBlock.content, tmpBlock.len);

		
		AllBlock[nowblock].nextBlock = blockInd + 2;

		AllBlock[blockInd + 2] = tmp2;
		AllBlock[blockInd + 1] = tmpBlock;
		blockInd += 2;

	}

}

void put_backspace(void)
{
	int thislen = AllBlock[nowblock].len;
	if (cursorPos == 0)
	{
		if (Allhead[nowline].blockInd == nowblock)
		{
			if (Allhead[nowline].preHead)
			{
				Allhead[Allhead[nowline].preHead].nextHead = Allhead[nowline].nextHead;
				nowline = Allhead[nowline].preHead;
				sumline--;
				int thisblock = Allhead[nowline].blockInd;
				int sum = 0;
				while (thisblock)
				{
					sum += AllBlock[thisblock].len;
					if (!AllBlock[thisblock].nextBlock)
					{
						AllBlock[thisblock].len--;
						AllBlock[thisblock].nextBlock = nowblock;
						AllBlock[nowblock].preBlock = thisblock;
						leftIndex = sum - 1;
					}
					thisblock = AllBlock[thisblock].nextBlock;
				}
			}
			else
			{
				//donothing
			}
		}
		else
		{
			AllBlock[AllBlock[nowblock].preBlock].nextBlock = AllBlock[nowblock].nextBlock;
			nowblock = AllBlock[nowblock].preBlock;
			
			AllBlock[nowblock].len--;
			cursorPos = AllBlock[nowblock].len;
			leftIndex--;
		}

	}
	else
	{
		cursorPos--;
		AllBlock[nowblock].len--;
		leftIndex--;
	}

}

void put_enter(void)
{
	headInd++;
	Allhead[nowline].nextHead = headInd;
	Allhead[headInd].preHead = nowline;
	Allhead[headInd].nextHead = Allhead[nowline].nextHead;
	sumline++;
	nowline = headInd;
	if (cursorPos == AllBlock[nowblock].len && cursorPos < BLEN)
	{
		int thislen = AllBlock[nowblock].len;
		AllBlock[nowblock].len++;
		AllBlock[nowblock].content[thislen] = '\n';
		if (AllBlock[nowblock].nextBlock)
		{
			Allhead[headInd].blockInd = AllBlock[nowblock].nextBlock;
			AllBlock[nowblock].nextBlock = 0;
		}
		else
		{
			tmpBlock.len = 0;
			blockInd++;
			tmpBlock.nextBlock = 0;
			tmpBlock.preBlock = 0;
			AllBlock[blockInd] = tmpBlock;
			Allhead[headInd].blockInd = blockInd;
		}

	}
	else
	{
		int thisLen = AllBlock[nowblock].len;

		blockInd++;
		tmpBlock.len = thisLen - cursorPos + 1;
		mystrcpy(AllBlock[nowblock].content + cursorPos, tmpBlock.content, tmpBlock.len);
		tmpBlock.preBlock = 0;
		tmpBlock.nextBlock = AllBlock[nowblock].nextBlock;
		AllBlock[blockInd] = tmpBlock;
		Allhead[headInd].blockInd = blockInd;
		AllBlock[nowblock].content[cursorPos] = '\n';
		AllBlock[nowblock].len = cursorPos + 1;
	}
	leftIndex = 0;
	cursorPos = 0;
}

void cursor_up(int n)
{
	bool flag = false;
	while (n--)
	{
		if (Allhead[nowline].preHead)
		{
			flag = true;
			nowline = Allhead[nowline].preHead;
		}
		else
		{
			break;
		}
	}
	if (flag)
	{
		int thisBlock = Allhead[nowline].blockInd;
		int sum = 0;
		while (thisBlock)
		{
			if ((leftIndex - sum) > AllBlock[thisBlock].len)
			{
				cursorPos = AllBlock[thisBlock].len - 1;
				if (AllBlock[thisBlock].nextBlock)
				{
					sum += AllBlock[thisBlock].len;
					thisBlock = AllBlock[thisBlock].nextBlock;
				}
				else
				{
					leftIndex = sum + AllBlock[thisBlock].len;
					cursorPos = AllBlock[thisBlock].len;
					break;
				}
			}
			else
			{
				cursorPos = leftIndex - sum;
				break;
			}

		}

	}
}

void cursor_down(int n)
{
	bool flag = false;
	while (n--)
	{
		if (Allhead[nowline].nextHead)
		{
			flag = true;
			nowline = Allhead[nowline].nextHead;
		}
		else
		{
			break;
		}
	}
	if (flag)
	{
		int thisBlock = Allhead[nowline].blockInd;
		int sum = 0;
		while (thisBlock)
		{
			if ((leftIndex - sum) > AllBlock[thisBlock].len)
			{
				cursorPos = AllBlock[thisBlock].len - 1;
				if (AllBlock[thisBlock].nextBlock)
				{
					sum += AllBlock[thisBlock].len;
					thisBlock = AllBlock[thisBlock].nextBlock;
				}
				else
				{
					leftIndex = sum + AllBlock[thisBlock].len;
					cursorPos = AllBlock[thisBlock].len;
					break;
				}
			}
			else
			{
				cursorPos = leftIndex - sum;
				break;
			}

		}
		
	}
}

void cursor_left(int n)
{
	while (n--)
	{
		if (cursorPos > 0)
		{
			cursorPos--;
			leftIndex--;
		}
		else if (AllBlock[nowblock].preBlock)
		{
			leftIndex--;
			nowblock = AllBlock[nowblock].preBlock;
			cursorPos = AllBlock[nowblock].len - 1;
		}
		else
		{
			break;
		}

	}
}

void cursor_right(int n)
{
	while (n--)
	{
		if (cursorPos < AllBlock[nowblock].len)
		{
			cursorPos++;
			leftIndex++;
		}
		else if (AllBlock[nowblock].nextBlock)
		{
			cursorPos = 0;
			leftIndex++;
			nowblock = AllBlock[nowblock].nextBlock;
		}		
		else
		{
			break;
		}

	}

}

void close(char* document)
{
	int i = 0;
	int j = 0;
	int headf = 1;
	while (headf)
	{
		int thisblock = Allhead[headf].blockInd;
		while (thisblock)
		{
			mystrcpy(AllBlock[thisblock].content, document + j, AllBlock[thisblock].len);
			j += AllBlock[thisblock].len;
			thisblock = AllBlock[thisblock].nextBlock;
		}
		
		headf = Allhead[headf].nextHead;
	}

}

