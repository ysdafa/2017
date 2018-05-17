#include <stdio.h>
#include <string.h>

using namespace std;

struct Node{
	Node* child[26];
	char word[15];
	bool isWord;
	Node(){
		for(int i = 0; i< 26;i++)
			child[i] = NULL;
		isWord = false;	
	}
};

char noWord[] = "eh";

inline int idx(char c)
{
	return c-'a';
}

void insert(Node* node, char* key, char* value)
{
	while(*key) {
		int id = idx(*key);
		if(node->child[id]==NULL)
			node->child[id]=new Node();
		node = node->child[id];
		key++;
	}
	node->isWord = true;
	strcpy(node->word, value);
}

char* search(Node* node, char* key)
{
	while(*key)
	{
		int id = idx(*key);
		if(node->child[id] == NULL)
			return noWord;
		node = node->child[id];
		key++;
	}
	if(node->isWord)
		return node->word;
	return noWord;
}

int main()
{
	char buf[40], key[20], value[20];
	Node* root = new Node();
	while(gets(buf) && buf[0] !='\0')
	{
		sscanf(buf, "%s%s", value, key);
		insert(root, key, value);
	}
	
	printf("\n\n\n");
	while(scanf("%s", key) != EOF)
	{
		printf("%s\n", search(root, key));
	}

	return 0;
}
