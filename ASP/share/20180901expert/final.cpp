#include "pch.h"
#include <iostream>
//const int NULL = 0;

typedef struct _node {
	int len;
	int index;
	_node* pre;
	_node* nxt;
	char data[110];
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

static char document_debug[1024 * 1024 + 2];

void debug()
{
	return;
	FILE *fp = fopen("output.txt", "w");

	line* pl = &l[0];
	int i = 0;
	int j = 0;
	node* pn = NULL;
	int k = 0;
	while (pl != NULL) {
		pn = &pl->front;
		while (pn != &pl->tail) {
			for (j = 0; j < pn->len; j++, i++)
			{
				document_debug[i] = pn->data[j];
				k++;
				if (k == 1024 * 1024) return;
			}
			pn = pn->nxt;
		}
		document_debug[i] = '\n';
		i++; k++;
		pl = pl->nxt;
		if (k == 1024 * 1024) return;
	}
	fwrite(document_debug, sizeof(char), i, fp);
	fclose(fp);
}

void create()
{
	tail_n = 0; tail_l = 0;
	global_line = 0; global_cursor = 0;
	global_right = 0; global_left = 0;

	line *pl = &l[tail_l];
	pl->len = 0; pl->index = tail_l; pl->pre = NULL; pl->nxt = NULL;
	tail_l++;

	node *pn = &n[tail_n];
	pn->len = 0; pn->index = tail_n; pn->pre = NULL; pn->nxt = NULL;
	tail_n++;

	pl->front.index = -1; pl->tail.index = -1;
	pl->front.nxt = pn; pl->tail.pre = pn;
	pn->pre = &pl->front; pn->nxt = &pl->tail;
}

void put_char(char c)
{
	if (n[global_cursor].len == 100) { // add new node
		node *pn = &n[tail_n];
		pn->len = 0;
		pn->index = tail_n;
		pn->pre = &n[global_cursor];
		node *tmp = n[global_cursor].nxt;
		pn->nxt = tmp;
		n[global_cursor].nxt = pn;
		
		tail_n++;
		global_cursor = pn->index;
	}
	n[global_cursor].data[n[global_cursor].len] = c;
	n[global_cursor].len++;

	global_left++;
	l[global_line].len++;
}

int count = 0;
void put_backspace()
{
	debug();
	if (l[global_line].len > 0) { //
		if (global_left > 0) {
			while (n[global_cursor].len == 0) { // set the pre node
				global_cursor = n[global_cursor].pre->index;
			}
			n[global_cursor].len--;
			global_left--;
			l[global_line].len--;
		}
		else {
			//return;
			//merge the line
			line *prel = l[global_line].pre;
			line *nxtl = l[global_line].nxt;
			bool flag = false;
			if (prel != NULL) {
				prel->nxt = nxtl;
				//merge the node
				node* tmpn = &prel->front;
				while (tmpn->nxt != &prel->tail) //set the nxt line tail
				{
					tmpn = tmpn->nxt;
				}
				tmpn->nxt = n[global_cursor].nxt; // skip the current node for len = 0;
				n[global_cursor].nxt->pre = tmpn;

				node* tmpnn = n[global_cursor].nxt;
				while (tmpnn->nxt != &l[global_line].tail) //set the nxt line tail
				{
					tmpnn = tmpnn->nxt;
				}
				tmpnn->nxt = &prel->tail;
				prel->tail.pre = tmpnn;
				global_cursor = tmpn->index;
				global_line = prel->index;
				global_left = prel->len;
				prel->len = global_left + global_right;
			}
			else {
				flag = true;
				count++;
			}
			
			if (nxtl != NULL && flag != true) {
				nxtl->pre = prel;
			}
			else {
				int a = 0;
			}

			//count++;
		}
	} 
	else {
		line *prel = l[global_line].pre;
		line *nxtl = l[global_line].nxt;
		bool flag = false;
		if (prel != NULL) {
			prel->nxt = nxtl;
			//merge the node
			node* tmpn = &prel->front;
			while (tmpn->nxt != &prel->tail) //set the nxt line tail
			{
				tmpn = tmpn->nxt;
			}
			global_cursor = tmpn->index;
			global_line = prel->index;
			global_left = prel->len;
			prel->len = global_left + global_right;
		}
		else {
			flag = true;
			count++;
		}

		if (nxtl != NULL && flag != true) {
			nxtl->pre = prel;
		}
		else {
			int a = 0;
		}
		
	}
	debug();
}

void put_enter()
{
	debug();
	line *pl = &l[tail_l];
	pl->len = 0; 
	pl->index = tail_l; 
	pl->front.index = -1; pl->tail.index = -1;
	pl->pre = NULL; 
	pl->nxt = NULL;
	tail_l++;

	node *pn = &n[tail_n];
	pn->len = 0; 
	pn->index = tail_n; 
	pn->pre = NULL; 
	pn->nxt = NULL;
	tail_n++;

	pl->front.nxt = pn; pl->tail.pre = pn;
	pn->pre = &pl->front; pn->nxt = &pl->tail;

	pl->pre = &l[global_line];
	line *tmpl = l[global_line].nxt;
	l[global_line].nxt = pl;
	
	if (tmpl) {
		pl->nxt = tmpl;
		tmpl->pre = pl;
	}

	if (n[global_cursor].nxt != &l[global_line].tail) {
		node *tmpn = n[global_cursor].nxt;
		n[global_cursor].nxt = &l[global_line].tail; // set the tail of current line
		l[global_line].tail.pre = &n[global_cursor]; 
		l[global_line].len = global_left;
		
		// new node for next line
		node *pn = &n[tail_n];
		pn->index = tail_n;
		pn->len = 0; pn->pre = NULL; pn->nxt = NULL;
		tail_n++;

		pl->front.nxt = pn; // set the nxt line front
		pn->pre = &pl->front;
		pn->nxt = tmpn;
		tmpn->pre = pn;
		while (tmpn->nxt != &l[global_line].tail) //set the nxt line tail
		{
			tmpn = tmpn->nxt;
		}
		tmpn->nxt = &pl->tail;
		pl->tail.pre = tmpn;
		pl->len = global_right;
		global_cursor = pn->index;
		global_line = pl->index;
		global_left = 0;
	}
	else 
	{
		// new node for next line
		node *pn = &n[tail_n];
		pn->index = tail_n;
		pn->len = 0; pn->pre = NULL; pn->nxt = NULL;
		tail_n++;

		// set the nxt line front
		pl->front.nxt = pn; pl->tail.pre = pn;
		pn->pre = &pl->front; pn->nxt = &pl->tail;
		//ll.len = 0;
		global_cursor = pn->index;
		global_line = pl->index;
		global_left = 0;
		global_right = 0;
	}
	debug();
}

void split(line* pl, int pos)
{
	node* tmpn = &pl->front;
	int c = 0;
	while (tmpn->nxt != &pl->tail && c+ tmpn->len < pos) {
		c += tmpn->len;
		tmpn = tmpn->nxt;
	}

	if (c + tmpn->len == pos && pos != 0) { // fix the 0 error
		global_cursor = tmpn->index;
	}
	else {
		node* pn = &n[tail_n];
		pn->index = tail_n;
		pn->len = c + tmpn->len - pos; pn->pre = tmpn; pn->nxt = NULL;
		tail_n++;

		node* tp = tmpn->nxt;
		tmpn->nxt = pn;
		pn->nxt = tp;
		tp->pre = pn;
		if (pos != 0) {
			global_cursor = tmpn->index;
		}
		else {
			global_cursor = pn->index;
		}

		tmpn->len = pos - c;
		for (int i = 0; i < pn->len; i++) {
			pn->data[i] = tmpn->data[tmpn->len + i];
		}
	}
}

void cursor_up(int n)
{
	int i;
	line *p_l = &l[global_line];
	for (i = 0; i < n; i++) {
		if (p_l->pre!=NULL) {
			p_l = p_l->pre;
		}
		else break;
	}
	if(p_l != &l[global_line]) {
		if (p_l->len <= global_left) {
			global_left = p_l->len;
			global_right = 0;
			//set to the last node of the line
			node *tmpn = &p_l->front;
			while (tmpn->nxt != &p_l->tail) tmpn = tmpn->nxt;
		
			global_cursor = tmpn->index;
			global_line = p_l->index;
		}
		else {
			global_right = p_l->len - global_left;
			global_line = p_l->index;
			split(p_l, global_left);
		}
	}
}

void cursor_down(int n)
{
	int i;
	line *p_l = &l[global_line];
	for (i = 0; i < n; i++) {
		if (p_l->nxt != NULL) {
			p_l = p_l->nxt;
		}
		else break;
	}
	if (p_l != &l[global_line]) {
		if (p_l->len <= global_left) {
			global_left = p_l->len;
			global_right = 0;
			//set to the last node of the line
			node *tmpn = &p_l->front;
			while (tmpn->nxt != &p_l->tail) tmpn = tmpn->nxt;

			global_cursor = tmpn->index;
			global_line = p_l->index;
		}
		else {
			global_right = p_l->len - global_left;
			global_line = p_l->index;
			split(p_l, global_left);
		}
	}
}

void cursor_left(int n)
{
	if (global_left >= n) {
		global_left -= n;
		global_right += n;
	}
	else {
		global_right += global_left;
		global_left = 0;
	}
	split(&l[global_line], global_left);
}

void cursor_right(int n)
{
	if (global_right >= n) {
		global_left += n;
		global_right -= n;
	}
	else {
		global_left += global_right;
		global_right = 0;
	}
	split(&l[global_line], global_left);
}


void close(char* document)
{
	line* pl = &l[0];
	int i = 0;
	int j = 0;
	node* pn = NULL;
	int k = 0;
	while (pl != NULL) {
		pn = &pl->front;
		while (pn!= &pl->tail) {
			for (j = 0; j < pn->len; j++, i++)
			{
				document[i] = pn->data[j];
				k++;
				if (k == 1024 * 1024) return;
			}
			pn = pn->nxt;
		}
		document[i] = '\n';
		i++; k++;
		pl = pl->nxt;
		if (k == 1024 * 1024) return;
	}
}