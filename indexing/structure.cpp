#include "structure.h"

WMap wordinfo;
TFMap docinfo;
int totalN = 0;

void extractTF(char* path,FILE* TFpath[]) {
	FILE *fp = fopen(path, "rt");	//Document file open
	char line[BUF];	//read line
	char *str;	//read line per str

	clock_t start, end;
	if (fp == NULL) {
		return;
	}
	else {
		start = clock();
		printf("%s is processing....\n", path);
	}

	//read file
	while (fgets(line, BUF, fp)) {

		//ù ������ DONCO tag��� ���ο� ���� �߰�, �� �������� document no
		if (!strncmp(line, "[DOCNO]", 7)) {
			if (!docinfo.empty()) {
				clearDocInfo(TFpath);
			}
			totalN++;

			fgets(line, BUF, fp);
			continue;
		}
		else if (!strncmp(line, "[HEADLINE]", 10) || !strncmp(line, "[TEXT]", 6)) {
			continue;
		}
		else {
			str = strtok(line, " ");

			if (str[0] != '\n' && str[0] != '\t') {
				checkTF(str);
			}
			/*line ������ ��� ������ TF����*/
			while (str = strtok(NULL, " ")) {
				if (str[0] != '\n' && str[0] != '\t') {
					checkTF(str);
				}
			}
		}
	}
	
	clearDocInfo(TFpath);

	end = clock();
	printf("\r%lf second over...\n", (double)(end - start) / 1000);
	fclose(fp);
	return;
}

void checkTF(char* word) {
	//�ش� doc Map���� �ܾ� ���� �������� id�� �ο��ϰ� TF�� 1
	if (docinfo.find(word) == docinfo.end()) {
		TFVal temp = { totalN, 1 };
		docinfo[word] = temp;
	}
	//���� �ش� ������ word map�� �ܾ� ����ÿ� �׳� CF���� ����
	else {
		docinfo[word].TF++;
	}

}

void clearDocInfo(FILE* TFpath[]) {
	for (TFMap::iterator iter = docinfo.begin(); iter != docinfo.end(); ++iter) {
		//word ID mod 100 = p��� TFp ���Ͽ� ����
		int index;
		TFpair temp(iter->second.docid, iter->second.TF);

		//clear�ϸ� ��ü ����ʿ� ����
		//���� �ش� wordmap�� key�� ���ٸ�
		if (wordinfo.find(iter->first) == wordinfo.end()) {
			wordinfo[iter->first].TFvec.push_back(temp);
			wordinfo[iter->first].CF = iter->second.TF;
		}
		else {
			wordinfo[iter->first].TFvec.push_back(temp);
			wordinfo[iter->first].CF += iter->second.TF;
		}

		index = ((iter->second).docid) % 100;

		if (TFpath[index + 1] == NULL) {
			printf("File open error.\n");
			exit(1);
		}

		fprintf(TFpath[index+1], "%d\t%s\t%d\n", totalN, (iter->first).c_str(), (iter->second).TF);
	}

	docinfo.clear();
}

