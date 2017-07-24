#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAX_ID            100

#define MAX_STR_LENGTH    3000
#define MAX_IMAGE_HEIGHT  50
#define MAX_IMAGE_WIDTH   50

#define MAX_SCREEN_HEIGHT 1000
#define MAX_SCREEN_WIDTH  1000

#define THRESHOLD         0x07FFFFFF

enum Command {
	INIT = 0,
	CREATE,
	ADD,
	SHOW,
};

enum Type {
	VWIDGET = 0,
	HWIDGET,
	TEXT,
	IMAGE,
};

typedef struct {
	int type;
	union {
		struct { int width; } vwidget;
		struct { int height; } hwidget;
		struct { char *strPtr; } text;
		struct { int height, width; char *imagePtr; } image;
	};
} Element;

extern void init();
extern int  create(Element* elementPtr);
extern void add(int parentId, int childId);
extern void show(int elementId, char screen[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]);

static int id[MAX_ID + 1];

static int point, expected;

static char baseStr[MAX_STR_LENGTH];
static char baseImage[MAX_IMAGE_HEIGHT][MAX_IMAGE_WIDTH];

static char bufStr[MAX_STR_LENGTH];
static char bufImage[MAX_IMAGE_HEIGHT * MAX_IMAGE_WIDTH];
static char screen[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH];

static void loadBase() {
	int N, H, W;

	scanf("%d", &N);

	int idx = 0;
	while (idx < N) {
		scanf("%c", &baseStr[idx]);
		if (baseStr[idx] != '\n' && baseStr[idx] != '\r') ++idx;
	}

	scanf("%d %d", &H, &W);
	for (int y = 0; y < H; ++y) {
		int x = 0;
		while (x < W) {
			scanf("%c", &baseImage[y][x]);
			if (baseImage[y][x] != '\n' && baseImage[y][x] != '\r') ++x;
		}
	}
}

static void readElement(Element *elementPtr) {
	int x1, y1, x2, y2, idx;

	switch (elementPtr->type) {
	case VWIDGET:
		scanf("%d", &(elementPtr->vwidget.width));
		break;
	case HWIDGET:
		scanf("%d", &(elementPtr->hwidget.height));
		break;
	case TEXT:
		scanf("%d %d", &x1, &x2);
		idx = 0;
		for (int x = x1; x <= x2; ++x)
			bufStr[idx++] = baseStr[x];
		bufStr[idx] = '\0';
		elementPtr->text.strPtr = bufStr;
		break;
	case IMAGE:
		scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
		idx = 0;
		for (int y = y1; y <= y2; ++y)
			for (int x = x1; x <= x2; ++x)
				bufImage[idx++] = baseImage[y][x];
		elementPtr->image.height = y2 - y1 + 1;
		elementPtr->image.width = x2 - x1 + 1;
		elementPtr->image.imagePtr = bufImage;
		break;
	default:
		break; // never executed
	}
}

static int calcHash(char screen[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH], int height, int width) {
	int ret = 0, count = 1;
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; ++x) {
			ret += screen[y][x] * count++;
			ret &= THRESHOLD;
		}
	return ret;
}

static void init_m() {
	for (int idx = 0; idx <= MAX_ID; ++idx)
		id[idx] = -1;
	point = 0;
}

static void run() {
	int lineN;

	scanf("%d", &lineN);
	for (int line = 0; line < lineN; ++line) {
		int command;
		int parentId, childId, elementId;
		int height, width;
		int hash, resultHash;

		Element element;

		scanf("%d", &command);
		switch (command) {
		case INIT:
			scanf("%d", &expected);
			init();
			break;
		case CREATE:
			scanf("%d %d", &elementId, &(element.type));
			readElement(&element);
			id[elementId] = create(&element);
			break;
		case ADD:
			scanf("%d %d", &parentId, &childId);
			add(id[parentId], id[childId]);
			break;
		case SHOW:
			scanf("%d %d %d %d", &elementId, &height, &width, &hash);
			show(id[elementId], screen);
			resultHash = calcHash(screen, height, width);
			if (resultHash == hash) point++;
			break;
		}
	}
}

int main() {
	int T, total_score;

	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	scanf("%d", &T);
	loadBase();

	total_score = 0;
	for (int testcase = 1; testcase <= T; ++testcase) {
		init_m();
		run();
		printf("#%d %d\n", testcase, point);
		if (point == expected) total_score++;
	}

	printf("total score = %d\n", total_score * 100 / T);
	return 0;
}