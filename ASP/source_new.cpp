const int NULL = 0;
const int maxn = 150;
typedef struct _block
{
	int id;
	int len;
	char s[maxn + 10];
	_block *prev;
	_block *next;
} block;

typedef struct _line
{
	int id;
	int len;
	block front_blk;
	block tail_blk;
	_line *prev;
	_line *next;
} line;

int g_left, g_right;
int g_ln_cursor, g_blk_cursor;

int g_ln_tail;
int g_blk_tail;

block gb[100000];
line gl[100000];

block *new_block()
{
	block *p_block = &gb[g_blk_tail];
	p_block->id = g_blk_tail;
	p_block->len = 0;
	p_block->prev = NULL;
	p_block->next = NULL;
	g_blk_tail++;
}

line *new_line()
{
	line *p_line = &gl[g_ln_tail];
	p_line->id = g_ln_tail;
	p_line->len = 0;
	p_line->prev = NULL; p_line->next = NULL;

	p_line->front_blk.id = -1; p_line->front_blk.prev = NULL; p_line->front_blk.next = NULL;
	p_line->tail_blk.id = -1; p_line->tail_blk.prev = NULL;  p_line->tail_blk.next = NULL;
	g_ln_tail++;
}

void create(void)
{
	g_ln_tail = 0; // id init to 0
	g_blk_tail = 0; 
	g_ln_cursor = 0; // current id
	g_blk_cursor = 0;
	g_left = 0; // current line left and right
	g_right = 0; 
	
	block *p_block = new_block(); // new first block and set to first line;
	line *p_line = new_line();

	p_line->front_blk.next = p_block;
	p_block->prev = &p_line->front_blk;
	p_line->tail_blk.prev = p_block;
	p_block->next = &p_line->tail_blk;
}

void put_char(char c)
{
	block *p_cur_blk = &gb[g_blk_cursor];
	if (p_cur_blk->len == maxn) // reach the maxn len, need to new block
	{
		block *p_new_blk = new_block();
		p_new_blk->prev = p_cur_blk;
		p_new_blk->next = p_cur_blk->next;
		p_cur_blk->next = p_new_blk;
		g_blk_cursor = p_new_blk->id;
	}

	gb[g_blk_cursor].s[gb[g_blk_cursor].len++] = c;
	g_left++;
	gl[g_ln_cursor].len++;
}

void put_backspace(void)
{
	if (gl[g_ln_cursor].len > 0) //has char in the line
	{
		if (g_left > 0) // has char in the left
		{
			while (gb[g_blk_cursor].len == 0) // find the block with the left char
			{
				g_blk_cursor = gb[g_blk_cursor].prev->id;
			}
			gb[g_blk_cursor].len--;
			gl[g_ln_cursor].len--;
			g_left--;
		}
		else  //empty left with right part
		{
			//will merge current line with prev line
			line *p_cur_line = &gl[g_ln_cursor];
			line *p_prev_line = p_cur_line->prev;
			line *p_next_line = p_cur_line->next;

			if(p_prev_line != NULL) 
			{
				//connect the pre line last block to cur line cur block
				block *p_cur_block = &gb[g_blk_cursor];
				block *p_prev_line_last_block = p_prev_line->tail_blk.prev;
				p_prev_line_last_block->next = p_cur_block;
				p_cur_block->prev = p_prev_line_last_block;

				//connect cur line last block to pre line tail block
				block *p_cur_line_last_block = p_cur_line->tail_blk.prev;
				p_prev_line->tail_blk.prev = p_cur_line_last_block;
				p_cur_line_last_block->next = &p_prev_line->tail_blk;

				if(p_next_line != NULL)
				{
					p_next_line->prev = p_prev_line;
					p_prev_line->next = p_next_line;
				}

				//update global variables
				g_ln_cursor = p_prev_line->id;
				g_left = p_prev_line->len;
				p_prev_line->len += g_right;
			}
		}
	}
	else //empty line
	{
		line *p_cur_line = &gl[g_ln_cursor];
		line *p_prev_line = p_cur_line->prev;
		line *p_next_line = p_cur_line->next;

		if(p_prev_line != NULL)
		{
			block *p_prev_line_last_block = p_prev_line->tail_blk.prev;
			g_blk_cursor = p_prev_line_last_block->id;
			g_ln_cursor = p_prev_line->id;
			g_left = p_prev_line->len;
			p_prev_line->len += g_right;

			if(p_next_line != NULL)
			{
				p_next_line->prev = p_prev_line;
				p_prev_line->next = p_next_line;
			}
		}
	}
}

void put_enter(void)
{
	line *p_line = new_line();

	line *p_cur_line = &gl[g_ln_cursor];
	line *p_next_line = p_cur_line->next;

	p_line->prev = p_cur_line;
	p_cur_line->next = p_line;

	if(p_next_line)
	{
		p_line->next = p_next_line;
		p_next_line->prev = p_line;
	}

	block *p_cur_block = &gb[g_blk_cursor];
	if( p_cur_block->next == &(gl[g_ln_cursor].tail_blk) )//just new block
	{
		block *p_block = new_block();
		p_line->front_blk.next = p_block;
		p_block->prev = &p_line->front_blk;
		p_line->tail_blk.prev = p_block;
		p_block->next = &p_line->tail_blk;

		g_blk_cursor = p_block->id;
		g_ln_cursor = p_line->id;
		g_left = 0;
		g_right = 0;
	}
	else // new block and link to the last right blocks
	{
		block *p_next_block = p_cur_block->next;
		block *p_last_block = p_cur_line->tail_blk.prev;

		//adjust cur line tail blk
		p_cur_block->next = &p_cur_line->tail_blk;
		p_cur_line->tail_blk.prev = p_cur_block;

		block *p_block = new_block();
		//adjust new block next blk
		p_line->front_blk.next = p_block;
		p_block->prev = &p_line->front_blk;
		p_block->next = p_next_block;
		p_next_block->prev = p_block;

		//adjust new line tail blk
		p_line->tail_blk.prev = p_last_block;
		p_last_block->prev = &p_line->tail_blk;

		g_blk_cursor = p_block->id;
		g_ln_cursor = p_line->id;
		p_line->len = g_right;
		p_cur_line->len = g_left;

		g_left = 0;
	}
}

void cursor_up(int n)
{}

void cursor_down(int n)
{}

void cursor_left(int n)
{}

void cursor_right(int n)
{}

void close(char* document)
{}