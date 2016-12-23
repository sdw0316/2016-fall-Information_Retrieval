#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#define BUF 128
#define FALSE 0
#define TRUE 1

using namespace std;

vector<char*> stopword;

char *news[2] = {
	"APW",
	"NYT",
};

int checkStop(char *word) {
	int len = strlen(word);
	if (word[len - 1] == '\n') {
		word[len - 1] = NULL;
	}
	for (int i = 0; i < stopword.size(); i++) {
		if (!strcmp(word, stopword[i])|| !strcmp(word,"\t")) {
			return TRUE;
		}
	}
	return FALSE;
}

void removeStop(char* path1, char* path2) {/*file���� line ������ open*/
	FILE *fp = fopen(path1, "rt");	//file open
	FILE *fp2;

	char line[BUF];	//read line
	char *str;	//read line per str

	char flag = FALSE;

	/*���� ���� ������ ���ٸ� return*/
	if (fp == NULL) {
		printf("%s does not exist.\n", path1);
		return;
	}
	else {
		fp2 = fopen(path2, "wt");
	}

	while (fgets(line, BUF, fp)) {
		int num = 0;//line�� �����ϴ� �ܾ� ����;

		/*DOCNO���� stopword ���� ����*/
		if (!strncmp(line, "[DOCNO]", 7)) {
			fprintf(fp2, "%s", line);
			continue;
		}
		else {
			str = strtok(line, " \t");

			/*line�� stopword üũ�ϴ� �±��Ͻ�*/
			if (!strncmp(str, "[HEADLINE]", 10)) {
				fprintf(fp2, "%s", str);
			}
			else if (!strncmp(str, "[TEXT]", 6)) {
				fprintf(fp2, "%s", str);
				continue;
			}
			/*�׿ܿ� stopword �˻�*/
			else {
				if (!checkStop(str)) {
					num++;
					fprintf(fp2, "%s ", str);
				}
			}
			/*line���� �ܾ� ������ read*/
			while (str = strtok(NULL, " \t")) {
				if (!checkStop(str)) {
					num++;
					fprintf(fp2, "%s ", str);
				}
			}
		}
		if (num)
			fprintf(fp2, "\n");
	}

	printf("%s was processed!\n", path1);
	fclose(fp);
	fclose(fp2);
	return;
}

int main() {
	FILE *fp3 = fopen("../src/stopword.txt", "rt");
	char line[BUF];	//read line

	if (fp3==NULL) {
		printf("���� ���� ����\n");
		return -1;
	}

	//stopword array�� stopword save
	while (fgets(line, BUF, fp3)) {
		int len = strlen(line) - 1;
		line[len] = NULL;
		char *temp = (char*)malloc(sizeof(char)*len);
		strcpy(temp, line);
		stopword.push_back(temp);
	}
	fclose(fp3);

	/*file read start*/
	for (int dir = 0; dir < 2; dir++) {

		for (int year = 1998; year <= 2000; year++) {
			for (int month = 1; month <= 12; month++) {
				for (int day = 1; day <= 31; day++) {
					char path[BUF];
					char path2[BUF];
					if (dir == 0) {
						sprintf(path, "../../ir_corpus/parsing/%s/%d/%d%02d%02d_%s_ENG.txt", news[dir], year, year, month, day, news[dir]);
						sprintf(path2, "../../ir_corpus/stopword/%s/%d/%d%02d%02d_%s_ENG.txt", news[dir], year, year, month, day, news[dir]);
					}
					else {
						sprintf(path, "../../ir_corpus/parsing/%s/%d/%d%02d%02d_%s.txt", news[dir], year, year, month, day, news[dir]);
						sprintf(path2, "../../ir_corpus/stopword/%s/%d/%d%02d%02d_%s.txt", news[dir], year, year, month, day, news[dir]);
					}
					removeStop(path, path2);
				}
			}
		}
	}


	return 0;
}