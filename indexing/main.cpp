#include <stdio.h>

#include "structure.h"
#include "process2.h"

char *news[2] = {
	"APW",
	"NYT"
};

int main() {
	FILE *wordfile = fopen("./word.dat", "wt");	//word info file
	FILE *indexfile = fopen("./index.dat", "w");	//index.dat file
	FILE *TFpath[101];
	clock_t start, end;
	int word_id = 0;
	float* docsum;	//document당 weight를 구하기 위해 분모 미리 저장
	int preDF = 0;

	start = clock();
	//*************************************process1 -> make TF and WordDat Structure**********************************
	//init TFpaths
	for (int i = 1; i <= 100; i++) {
		char path[BUF];
		sprintf(path, "./TFdat/TF%d.txt", i);
		TFpath[i] = fopen(path, "w+");
	}

	//start file reading
	for (int dir = 0; dir < 2; dir++) {
		for (int year = 1998; year <= 2000; year++) {
			for (int month = 1; month <= 12; month++) {
				for (int day = 1; day <= 31; day++) {
					char path[BUF];
					if (dir == 0) {
						sprintf(path, "../../ir_corpus/stemming/%s/%d/%d%02d%02d_%s_ENG.txt", news[dir], year, year, month, day, news[dir]);
					}
					else {
						sprintf(path, "../../ir_corpus/stemming/%s/%d/%d%02d%02d_%s.txt", news[dir], year, year, month, day, news[dir]);
					}

					extractTF(path,TFpath);	//extract TF for each document
				}
			}
		}
	}
	docsum = (float*)malloc(sizeof(float)*(totalN + 1));	//docsum배열 초기화
	for (int i = 0; i <= totalN; i++) {
		docsum[i] = 0;
	}

	end = clock();
	printf("Indexing end : %lf sec over...!\n", (double)(end - start) / 1000);
	printf("make word.dat!!! total N : %d\n",totalN);
	printf("Press Any Key to make word.dat file\n");
	system("pause");
	
	//write word.dat
	for (WMap::iterator iter = wordinfo.begin(); iter != wordinfo.end(); ++iter) {
		fprintf(wordfile, "%d\t%s\t%d\t%d\t%d\n", ++word_id, (iter->first).c_str(), (iter->second).TFvec.size(), (iter->second).CF,preDF);
		preDF += (iter->second).TFvec.size();
	}
	fclose(wordfile);
	printf("word id : %d\n", word_id);
	system("pause");

	//init file pointer for TFpath
	for (int i = 1; i <= 100; i++) {
		fseek(TFpath[i], 0, SEEK_SET);
	}
	start = clock();
	//**********************process2 ->  read TF.txt and save weight data**************************
	for (int i = 1; i <= 100; i++) {
		readTF(TFpath, i, docsum);	//여기서 TFdat 파일 close
	}

	//***********************process3 -> wordinfo 돌면서 inverse indexing***************************
	printf("Inverse Indexing.....!\n");
	word_id = 1;	//init wordid
	for (WMap::iterator iter = wordinfo.begin(); iter != wordinfo.end(); ++iter) {
		//각 key의 벡터 돌면서 indexing
		for (int i = 0; i < iter->second.TFvec.size(); i++) {
			float index = (log10f((float)(iter->second).TFvec[i].second)+1.0)*log10f((float)totalN / (float)iter->second.TFvec.size());
			index = index / sqrtf(docsum[iter->second.TFvec[i].first]);
			fprintf(indexfile, "%6d %6d %6d %6.5f\n", word_id, iter->second.TFvec[i].first, iter->second.TFvec[i].second ,index);
		}
		++word_id;
	}
	fclose(indexfile);
	end = clock();
	printf("word id : %d\n", word_id-1);
	printf("InverseIndexing end : %lf sec over...!\n", (double)(end - start) / 1000);
	system("pause");

	return 0;
}