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

void preProcessingEnd();	//할당된 메모리 제거
void queryInit(Query *q);	//Query object initialize
void insertWord(string str, Query *q,int flag);	//Query object에 새 word 추가
void preProcessing();	//initialize token&stopword list
char* tolowers(char *str);
void tokenizing(char *str);	//해당 단어 tokenizing
int isStop(char *str);	//해당 단어 stopword인지 검사
void queryProcessing(FILE *path, vector<Query> *query);	//path에서 topic들 읽어 query벡터에 각각 저장