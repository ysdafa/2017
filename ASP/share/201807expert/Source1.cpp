#include "pch.h"

#define NULL 0

typedef struct _node {
	int len;
	int index;
	node* pre;
	node* nxt;
	char data[100];
} node;

typedef struct _line {
	_line* pre;
	_line* nxt;
	int len;
	int index;
	node front;
	node tail;
} line;

line l[100000];
node n[100000];

int tail_n;
int tail_l;

int global_line, global_cursor;
int global_left, global_right;

void create()
{
	tail_n = 0; tail_l = 0;
	global_line = 0; global_cursor = 0;
	global_right = 0; global_left = 0;
	
	l[0].len = 0; l[0].index = 0; l[0].pre = NULL; l[0].nxt = NULL;
	tail_l++;

	n[0].len = 0; n[0].index = 0; n[0].pre = NULL; n[0].nxt = NULL;
	tail_n++;

	l[0].front.nxt = &n[0]; l[0].tail.pre = &n[0];
	n[0].pre = &l[0].front; n[0].nxt = &l[0].tail;
}

void put(char c)
{
	int index = n[global_cursor].len;
	if (index == 99) { // add new node

	}
	n[global_cursor].data[index] = c;
	n[global_cursor].len++;
}

void backspace()
{}

void enter()
{}

void move_up(int n)
{
}

void move_down(int n)
{}

void move_left(int n)
{}

void move_right(int n)
{}

