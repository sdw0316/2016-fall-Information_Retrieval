#include "dat_structure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF 128
WordTable table;
DocVal doc_list[DOC_MAX];
double aval = 0;

void loadWord() {
	FILE *fp = fopen("./src/word.dat", "r");
	char line[BUF];

	if (fp == NULL)
		exit(1);

	while (fgets(line, BUF, fp)) {
		TableVal temp;
		string key;
		char *str;
		int count = 1;

		str = strtok(line, "\t");
		temp.id = atoi(str);

		while (str = strtok(NULL, "\t")) {
			switch (count) {
			case 1:
				key = str;
				break;
			case 2:
				temp.DF = atoi(str);
				break;
			case 3:
				break;
			case 4:
				temp.offset = atoi(str);
				break;
			}
			count++;
		}
		table[key] = temp;
	}

	fclose(fp);
}

void loadDocs() {
	FILE *fp = fopen("./src/doc.dat", "r");
	char line[BUF];
	int num = 0;

	if (fp == NULL)
		exit(1);

	while (fgets(line, BUF, fp)) {
		DocVal temp;
		char *str;
		int count = 1;

		str = strtok(line, " \t");

		while (str = strtok(NULL, " \t")) {
			switch (count) {
			case 1:
				temp.doc_no = str;
				break;
			case 2:
				temp.length = atoi(str);
				aval += (double)temp.length/(double)DOC_MAX;
				break;
			default:
				break;
			}
			count++;
		}
		doc_list[num++] = temp;
	}

	fclose(fp);
}