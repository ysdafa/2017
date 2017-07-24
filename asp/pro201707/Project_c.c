#define MAX_SCREEN_HEIGHT 1000
#define MAX_SCREEN_WIDTH  1000

#define BORDER            '+'
#define SPACE             ' '

enum TYPE {
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
		struct { char* strPtr; } text;
		struct { int height, width; char* imagePtr; } image;
	};
} Element;

void init() {

}

int create(Element* elemet) {
	return -1;  // return id
}

void add(int parentId, int childId) {

}

void show(int elementId, char screen[MAX_SCREEN_HEIGHT][MAX_SCREEN_WIDTH]) {

}