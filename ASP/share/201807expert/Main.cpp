#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MEMORYSIZE		65536

static unsigned char mem[MEMORYSIZE];

#define CMD_PUT		100
#define CMD_DEL		200
#define CMD_GET 	300
#define CMD_GETKEY	400

#define MAXLEN		12

extern bool init(int N);
extern void put(unsigned char key_in[MAXLEN + 1], unsigned char value_in[MAXLEN + 1]);
extern void del(unsigned char key_in[MAXLEN + 1]);
extern void get(unsigned char key_in[MAXLEN + 1], unsigned char value_out[MAXLEN + 1]);
extern void getkey(unsigned char value_in[MAXLEN + 1], unsigned char key_out[MAXLEN + 1]);

bool memread(unsigned char dest[], int pos, int len) {
	if (pos < 0 || len <= 0 || pos + len > MEMORYSIZE)
		return false;
	for (register int i = 0; i < len; ++i)
		dest[i] = mem[pos + i];
	return true;

	
}

bool memwrite(unsigned char src[], int pos, int len) {
	if (pos < 0 || len <= 0 || pos + len > MEMORYSIZE)
		return false;

	for (register int i = 0; i < len; ++i)
		mem[pos + i] = src[i];

	return true;
}

static bool ztrcmp(unsigned char *a, unsigned char *b) {
	while (*a && *a == *b)
		++a, ++b;

	return *a == *b;
}

static bool run() {
	int Q, N;
	unsigned char key[MAXLEN + 1], value[MAXLEN + 1];
	unsigned char key_u[MAXLEN + 1], value_u[MAXLEN + 1];

	scanf("%d %d", &Q, &N);

	bool okay = init(N);
	
	for (int q = 0; q < Q; ++q) {
		int cmd;
		scanf("%d", &cmd);

		switch (cmd) {
		case CMD_PUT:
			scanf("%s %s", key, value);
			if (okay) put(key, value);
			break;
		case CMD_DEL:
			scanf("%s", key);
			if (okay) del(key);
			break;
		case CMD_GET:
			scanf("%s %s", key, value);
			if (okay) {
				get(key, value_u);
				if (!ztrcmp(value, value_u))
					okay = false;
			}
			break;
		case CMD_GETKEY:
			scanf("%s %s", value, key);
			if (okay) {
				getkey(value, key_u);
				if (!ztrcmp(key, key_u))
					okay = false;
			}
			break;
		default:
			break;
		}
	}

	return okay;
}

int main() {
	int TC;
	
	freopen("sample_input.txt", "r", stdin);

	setbuf(stdout, NULL);
	scanf("%d", &TC);

	int totalscore = 0;
	for (int testcase = 1; testcase <= TC; ++testcase) {
		int score = run() ? 100 : 0;
		printf("#%d %d\n", testcase, score);
		totalscore += score;
	}
	printf("total score = %d\n", totalscore / TC);
	return 0;
}