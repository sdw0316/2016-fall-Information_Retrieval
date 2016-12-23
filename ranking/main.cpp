#include "dat_structure.h"
#include "process1.h"
#include "ranking.h"
#include <stdio.h>
#include <time.h>
#include <functional>
#include <algorithm>

int main() {
	FILE *fp, *result;
	vector<Query> query_list;	//save the information of each query
	clock_t start_all, end_all, start, end;
	
	int selector = 0;

	//*******************process 0 -> load data files(word.dat, doc.dat)************************************
	
	start = clock();
	printf("load data files......\n");
	loadWord();
	loadDocs();
	end = clock();
	printf("loading time : %lf...\n", (double)(end - start) / 1000.0);
	
	
	//*******************process 1 -> pre_query processing*************************************************
	fp = fopen("./src/topics25.txt", "r");
	if (fp == NULL) {
		exit(1);
	}

	start = clock();
	//first : read the token and stopword file
	printf("query-processing....\n");
	preProcessing();
	//second : query processing of each topics 
	queryProcessing(fp, &query_list);
	//thier : memory deallocation
	preProcessingEnd();
	
	end = clock();
	printf("query-processing time : %lf\n", (double)(end - start) / 1000.0);

	//query check
	fclose(fp);

	result = fopen("./result.txt", "w+");

	printf("\n\nSelect Model..\n\n1. Vector Space model\n2. BM25 model\n");
	while (selector != 1 && selector != 2) {
		scanf("%d", &selector);
	}
	start_all = clock();
	//*******************process 2 -> Use Model*******************************
	for (int i = 0; i < query_list.size(); i++) {
		vector<DocScore> score;
		//VSM
		start = clock();
		buildTable(query_list[i].word);	//정보 저장
		end = clock();
		printf("Score Table building time : %lf...\n", (double)(end - start) / 1000.0);
		if (selector == 1) {
			start = clock();
			vectorScoring(&score,query_list[i]);	//vector space score 계산
			sort(score.begin(), score.end());
			showRank(query_list[i], score, result);
			end = clock();
			printf("Query %d VSM result time : %lf...\n", i + 1, (double)(end - start) / 1000.0);
		}
		//BM25
		else {
			score.clear();
			start = clock();
			BMScoring(&score, query_list[i]);	//vector space score 계산
			sort(score.begin(), score.end());
			showRank(query_list[i], score, result);
			end = clock();
			printf("Query %d BM25 result time : %lf...\n", i + 1, (double)(end - start) / 1000.0);
		}
	}
	fclose(result);

	end_all = clock();
	printf("All processing time : %lf...\n", (double)(end_all - start_all) / 1000.0);
	return 0;
}