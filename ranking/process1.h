#pragma once
/**************************************
process for pre_query processing
**************************************/
#include <string>
#include <unordered_map>

using namespace std;

typedef unordered_map<string, int> wordHash;

typedef struct _query {
	int id;	//query number
	wordHash  title;	//query title
	wordHash desc;	//query description
	wordHash narr;	//query narration

	wordHash word;	// word hash ... (word, TF)
}Query;

void preProcessingEnd();	//�Ҵ�� �޸� ����
void queryInit(Query *q);	//Query object initialize
void insertWord(string str, Query *q,int flag);	//Query object�� �� word �߰�
void preProcessing();	//initialize token&stopword list
char* tolowers(char *str);
void tokenizing(char *str);	//�ش� �ܾ� tokenizing
int isStop(char *str);	//�ش� �ܾ� stopword���� �˻�
void queryProcessing(FILE *path, vector<Query> *query);	//path���� topic�� �о� query���Ϳ� ���� ����