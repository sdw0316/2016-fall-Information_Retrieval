#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>

#define BUF 128
#define FALSE 0
#define TRUE 1

using namespace std;

char *comp[3] = {
	"<DOCNO>",
	"<HEADLINE>",
	"<TEXT>"
};

char *comp2[3] = {
	"</DOCNO>",
	"</HEADLINE>",
	"</TEXT>"
};

char *news[2] = {
	"APW",
	"NYT",
};

char *tok = " \t`~!@#$%^&*()-=_+{}[]\'\",;:";
vector <char> token;

char* tolowers(char *str) {
	for (int i = 0; i < strlen(str); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}

void fprintStr(FILE* fp, char* str) {
	tolowers(str);
	for (int i = 0; i < strlen(str); i++) {
		fprintf(fp, "%c", str[i]);
		if (str[i] >= 48 && str[i] <= 57)
			fprintf(fp," ");
	}
	if (str[strlen(str) - 1] != '\n') {
		fprintf(fp, " ");
		return;
	}
}

void removeTok(char* str) {
	int len = strlen(str);
	int flag = FALSE;
	/*단어 끝이 \n일시 무시*/
	if (str[len - 1] == '\n') {
		len--;
		flag = TRUE;
	}

	/*단어 마지막의 's 삭제*/
	if (str[len - 2] == '\'' && str[len - 1] == 's') {
		if (flag) {
			str[len - 2] = '\n';
			str[len - 1] = NULL;
		}
		else {
			str[len - 2] = NULL;
			str[len - 1] = NULL;
		}
	}

	/*그 외의 token 삭제*/
	for (; *str != NULL; str++){
		//-나 _는 공백으로
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

void parsing(char* path1, char* path2) {
	FILE *fp;	//file open
	FILE *fp2;	//save result

	fp = fopen(path1, "rt");

	/*만약 읽을 파일이 없다면 return*/
	if (fp == NULL) {
		printf("%s does not exist.\n", path1);
		return;
	}
	else {
		fp2 = fopen(path2, "wt");
	}
	char line[BUF];	//read line
	char *str;	//read line per str
	char flag = FALSE;

	/*file에서 line 단위로 open*/
	while (fgets(line,BUF,fp)) {
		str = strtok(line, tok);

		if (!flag) {
			for (int mode = 0; mode < 3; mode++) {
				int len = strlen(comp[mode]);
				if (!strncmp(str, comp[mode],len)) {
					flag = TRUE;
					if (mode == 0) {
						//printf("[DOCNO] : ");
						fprintf(fp2, "[DOCNO]\n");
						flag++;
					}
					else if (mode == 1) {
						//printf("[HEADLINE] : ");
						fprintf(fp2, "[HEADLINE]\n");
					}
					else {
						//printf("[TEXT]\n");
						fprintf(fp2, "[TEXT]\n");
					}
					break;
				}
			}
		}

		if (flag) {
			/*line 시작이 단어일시*/
			if (strncmp(str, "<", 1)) {
				//printf("%s ", str);
				if (flag != TRUE + 1) {
					removeTok(str);
				}
				//fprintf(fp2, "%s", tolowers(str));
				fprintStr(fp2, str);
			}
			/*breaking tag일시*/
			else if (!strncmp(str, comp2[0], 8) || !strncmp(str, comp2[1], 11) || !strncmp(str, comp2[2], 7)) {
				flag = FALSE;
				continue;
			}

			/*line에서 단어 단위로 read*/
			while (str = strtok(NULL, tok)) {
				//만약 breaking 태그라면
				if (!strncmp(str, comp2[0],8) || !strncmp(str, comp2[1],11) || !strncmp(str, comp2[2],7)) {
					flag = FALSE;
					fprintf(fp2, "\n");
					break;
				}
				//printf(" %s", str);
				if (flag != TRUE + 1) {
					removeTok(str);
					//fprintf(fp2, " %s", tolowers(str));
					fprintStr(fp2, str);
				}
				else
					fprintf(fp2, "%s ", str);
			}
		}
	}

	printf("%s was processed!\n", path1);
	fclose(fp);
	fclose(fp2);
	
	return;
}

int main() {
	FILE *fp3 = fopen("../src/token.txt", "rt");

	char line[BUF];	//read line

	if (fp3 ==NULL) {
		return -1;
	}

	/*token */
	while (fgets(line, BUF, fp3)) {
		token.push_back(line[0]);
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
						sprintf(path, "../../ir_corpus/%s/%d/%d%02d%02d_%s_ENG", news[dir], year, year, month, day, news[dir]);
						sprintf(path2, "../../ir_corpus/parsing/%s/%d/%d%02d%02d_%s_ENG.txt", news[dir], year, year, month, day, news[dir]);
					}
					else {
						sprintf(path, "../../ir_corpus/%s/%d/%d%02d%02d_%s", news[dir], year, year, month, day, news[dir]);
						sprintf(path2, "../../ir_corpus/parsing/%s/%d/%d%02d%02d_%s.txt", news[dir], year, year, month, day, news[dir]);
					}
					parsing(path, path2);
				}
			}
		}
	}
	

	//parsing("../../ir_corpus/APW/1998/19980605_APW_ENG", "../../ir_corpus/parsing//APW/1998/19980605_APW_ENG.txt");

	return 0;
}