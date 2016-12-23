#include "process1.h"
#include "porter2_stemmer.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <vector>

#define BUF 128

#define TRUE 1
#define FALSE 0

#define QUERY_FLAG 0
#define	NUM_FLAG 1
#define TITLE_FLAG 2
#define DESC_FLAG 3
#define NARR_FLAG 4

vector<char> token;
vector<char*> stopword;
char *tokens = "`~!@#$%^&*()-_+=\n\t ";

void preProcessingEnd() {
	token.clear();
	/*
	for (vector<char*>::iterator iter = stopword.begin(); iter != stopword.end();) {
		vector<char*>::iterator current = iter;
		stopword.erase(current);
	}
	*/
	stopword.clear();
}
void queryInit(Query *q) {
	q->id = 0;
	q->title.clear();
	q->desc.clear();
	q->narr.clear();
	q->word.clear();
	return;
}

void insertWord(string str, Query *q, int flag) {
	if (str.size() == 0)
		return;
	/*title과 desc만 query term으로...먼저 query term에 넣고*/
	if (flag == TITLE_FLAG || flag == DESC_FLAG) {
		if (q->word.find(str) == q->word.end()) {
			q->word[str] = 1;
		}
		else {
			q->word[str]++;
		}
	}
	switch (flag) {
	case NUM_FLAG:
		q->id = atoi(str.c_str());
		break;
	case TITLE_FLAG:
		if (q->title.find(str) == q->title.end()) {
			q->title[str] = 1;
		}
		else {
			q->title[str]++;
		}
		q->word[str] += 9;
		break;
	case DESC_FLAG:
		if (q->desc.find(str) == q->desc.end()) {
			q->desc[str] = 1;
		}
		else {
			q->desc[str]++;
		}
		q->word[str] += 0;
		break;
	case NARR_FLAG:
		if (q->narr.find(str) == q->narr.end()) {
			q->narr[str] = 1;
		}
		else {
			q->narr[str]++;
		}
		break;
	default:
		break;
	}
}

void preProcessing() {
	FILE *tokenfp = fopen("./src/token.txt", "r");
	FILE *stopfp = fopen("./src/stopword.txt", "r");
	char line[BUF];	//read per line

	if (tokenfp == NULL || stopfp == NULL) {
		exit(1);
	}

	//store all tokens in token list
	while (fgets(line, BUF, tokenfp)) {
		token.push_back(line[0]);
	}
	fclose(tokenfp);

	//store all stopwords in stopword list
	while (fgets(line, BUF, stopfp)) {
		int len = strlen(line) - 1;
		line[len] = NULL;
		char *temp = (char*)malloc(sizeof(char)*len);
		strcpy(temp, line);
		stopword.push_back(temp);
	}
	fclose(stopfp);

	return;
}

char* tolowers(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

void tokenizing(char *str) {
	int len = strlen(str);
	/*단어 끝이 \n일시 무시*/
	/*
	if (str[len - 1] == '\n') {
		str[len - 1] = NULL;
		len--;
	}*/

	/*단어 마지막의 's 삭제*/
	if (str[len - 2] == '\'' && str[len - 1] == 's') {
		str[len - 2] = NULL;
		str[len - 1] = NULL;
	}

	/*그 외의 token 삭제*/
	for (; *str != NULL; str++) {
		if (*str == '-' || *str == '_') {
			*str = ' ';
			continue;
		}
		else {
			for (int i = 0; i < token.size(); i++) {
				if (*str == token[i]) {
					strcpy(str, str + 1);
					str--;
					break;
				}
			}
		}
	}
}

int isStop(char *str) {
	int len = strlen(str);
	for (int i = 0; i < stopword.size(); i++) {
		if (!strcmp(str, stopword[i]) || !strcmp(str, "\t")) {
			return TRUE;
		}
	}
	return FALSE;
}

void queryProcessing(FILE *path, vector<Query> *query) {
	char line[BUF];	//read per line
	char *str;	//read per word
	int flag = QUERY_FLAG;
	Query temp;

	while (fgets(line, BUF, path)) {

		if (!strncmp(line, "\n", 1)) {
			continue;
		}
		if (!strncmp(line, "<top>", 5)) {
			//만약 line의 시작이 top이면 query object 초기화
			if (flag == QUERY_FLAG) {
				queryInit(&temp);
			}
		}
		else if (!strncmp(line, "</top>", 6)) {
			//</top>에서는 현재까지 만들어놓은 query object를 query vector에 저장하고 다시 플래그 변환
			query->push_back(temp);
			flag = QUERY_FLAG;
		}
		else {
			str = strtok(line, tokens);

			if (str == NULL)
				continue;

			//line의 첫 단어 check후 tag면 flag로 변환
			if (!strncmp(str, "<num>", 5)) {
				flag = NUM_FLAG;
				str = strtok(NULL, tokens);
			}
			else if (!strncmp(str, "<title>", 7)) {
				flag = TITLE_FLAG;
			}
			else if (!strncmp(str, "<desc>", 6)) {
				flag = DESC_FLAG;
				str = strtok(NULL, tokens);
			}
			else if (!strncmp(str, "<narr>", 6)) {
				flag = NARR_FLAG;
				str = strtok(NULL, tokens);
			}
			//tag가 아니라 word면 processing
			else {
				if (flag != NUM_FLAG)
					tokenizing(str);
				if (!isStop(tolowers(str))) {
					string stem = str;
					Porter2Stemmer::stem(stem);
					insertWord(stem, &temp, flag);
				}
			}
			while (str = strtok(NULL, tokens)) {
				if (flag != NUM_FLAG)
					tokenizing(str);
				if (!isStop(tolowers(str))) {
					string stem = str;
					Porter2Stemmer::stem(stem);
					
					insertWord(stem, &temp, flag);
				}
			}
		}
	}

	return;
}