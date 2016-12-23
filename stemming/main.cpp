#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "porter2_stemmer.h"

#define BUF 128
#define FALSE 0
#define TRUE 1

using namespace std;

int totalN = 0;
FILE *docfile = fopen("./doc.dat", "wt");

char *news[2] = {
	"APW",
	"NYT",
};

void removeEnter(char *str) {
	int len = strlen(str) -1;
	if (str[len] == '\n')
		str[len] = NULL;
	return;
}

void stemFile(char* path1, char* path2) {
	FILE *fp = fopen(path1, "rt");	//file open
	FILE *fp2;	//save result

	char line[BUF];	//read line
	char *str;	//read line per str
	string stem;

	/*만약 읽을 파일이 없다면 return*/
	if (fp == NULL) {
		printf("%s does not exist.\n", path1);
		return;
	}
	else {
		fp2 = fopen(path2, "wt");
	}

	if (fp2 == NULL) {
		printf("%s open error\n", path2);
		exit(1);
	}
	if (docfile == NULL) {
		printf("File open error.\n");
		exit(1);
	}

	char flag = FALSE;

	string docno;	//docno for each document
	int doclen;	//document length

	/*file에서 line 단위로 open*/
	while (fgets(line, BUF, fp)) {
		/*DOCNO에선 stemming 안함*/
		if (!strncmp(line, "[DOCNO]", 7)) {
			fprintf(fp2, "%s", line);
			if (docno.length() != 0) {
				fprintf(docfile, "%d\t%s\t%d\n", totalN, docno.c_str(), doclen);
			}
			totalN++;
			doclen = 0;	//해당 문서의 길이 0으로 초기화

			//그 다음라인(DOCNO)을 문서 제목에 저장
			fgets(line, BUF, fp);
			fprintf(fp2, "%s", line);
			line[strlen(line) - 1] = NULL;
			docno = line;
			continue;
		}
		else {
			str = strtok(line, " \t");

			if (str[0] == '\n') {
				flag = TRUE;
				continue;
			}
			else
				flag = FALSE;

			/*line이 stopword 체크하는 태그일시*/
			if (!strncmp(str, "[HEADLINE]", 10)) {
				fprintf(fp2, "%s", str);
				continue;
			}
			else if (!strncmp(str, "[TEXT]", 6)) {
				fprintf(fp2, "%s", str);
				continue;
			}
			/*그외엔 stopword 검사*/
			else {
				if(str[0] != '\n' && str[0] != '\t' && str[0] != ' ')
					doclen++;
				removeEnter(str);
				stem = str;
				//if (!isFalse(stem))
				Porter2Stemmer::stem(stem);
				//if(stem.size() <= 15)
					fprintf(fp2, "%s ", stem.c_str());
			}
			/*line에서 단어 단위로 read*/
			while (str = strtok(NULL, " ")) {
				if (str[0] != '\n' && str[0] != '\t' && str[0] != ' ')
					doclen++;
				removeEnter(str);
				stem = str;
				//if (!isFalse(stem))
					Porter2Stemmer::stem(stem);
				//if (stem.size() <= 15)
					fprintf(fp2, "%s ", stem.c_str());
			}
		}
		if (!flag) {
			fprintf(fp2, "\n");
		}
	}

	if (docno.length() != 0) {
		fprintf(docfile, "%6d\t%s\t%4d\n", totalN, docno.c_str(), doclen);
	}

	printf("%s was processed!\n", path1);
	fclose(fp);
	fclose(fp2);
	return;
}

int main() {
	//FILE *fp3 = fopen("../src/proper_noun.txt", "rt");	//false postive txt

	char line[BUF];	//read line
	char *str;	//read line per str
	string stem;

	char flag = FALSE;

	/*
	if (fp3 ==NULL) {
		printf("파일 오픈 실패\n");
		return -1;
	}

	//false positive array에 save
	while (fgets(line, BUF, fp3)) {
		int len = strlen(line) - 1;
		line[len] = NULL;
		string temp = line;
		proper_noun.push_back(temp);
	}
	fclose(fp3);
	*/

	/*file read start*/
	for (int dir = 0; dir < 2; dir++) {

		for (int year = 1998; year <= 2000; year++) {
			for (int month = 1; month <= 12; month++) {
				for (int day = 1; day <= 31; day++) {
					char path[BUF];
					char path2[BUF];
					if (dir == 0) {
						sprintf(path, "../../ir_corpus/stopword/%s/%d/%d%02d%02d_%s_ENG.txt", news[dir], year, year, month, day, news[dir]);
						sprintf(path2, "../../ir_corpus/stemming/%s/%d/%d%02d%02d_%s_ENG.txt", news[dir], year, year, month, day, news[dir]);
					}
					else {
						sprintf(path, "../../ir_corpus/stopword/%s/%d/%d%02d%02d_%s.txt", news[dir], year, year, month, day, news[dir]);
						sprintf(path2, "../../ir_corpus/stemming/%s/%d/%d%02d%02d_%s.txt", news[dir], year, year, month, day, news[dir]);
					}
					stemFile(path, path2);
				}
			}
		}
	}

	fclose(docfile);
	return 0;
}