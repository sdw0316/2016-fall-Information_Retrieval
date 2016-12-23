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

		//첫 라인이 DONCO tag라면 새로운 문서 발견, 그 다음줄이 document no
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
			/*line 끝까지 계속 읽으며 TF추출*/
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
	//해당 doc Map에서 단어 출현 안했으면 id를 부여하고 TF는 1
	if (docinfo.find(word) == docinfo.end()) {
		TFVal temp = { totalN, 1 };
		docinfo[word] = temp;
	}
	//만약 해당 문서의 word map에 단어 존재시엔 그냥 CF값만 증가
	else {
		docinfo[word].TF++;
	}

}

void clearDocInfo(FILE* TFpath[]) {
	for (TFMap::iterator iter = docinfo.begin(); iter != docinfo.end(); ++iter) {
		//word ID mod 100 = p라면 TFp 파일에 저장
		int index;
		TFpair temp(iter->second.docid, iter->second.TF);

		//clear하며 전체 워드맵에 저장
		//만약 해당 wordmap에 key가 없다면
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

