#pragma once
#include "process1.h"
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

typedef struct _score_element{
	string term;	//word
	int qtf;	//query tf of word
	int dtf;	//document tf of word
	double weight;	//tf idf of word in doc
}STableVal;

typedef pair<double, int > DocScore;	// score, doc_id
typedef unordered_map<int, vector<STableVal>> ScoreTable;	//doc id => (qtf, dtf, weight);

void buildTable(wordHash query_word);	//한 query object q를 순회하며 score table 형성

void vectorScoring(vector<DocScore> *score, Query q);	//해당 query와 관련 문서 저장해 놓은 list를 Vector scoring해 vector에 때려넣기
void BMScoring(vector<DocScore> *score, Query q);	//해당 query와 관련 문서 table을 BM25 스코어링해 때려넣기
void showRank(Query q, vector<DocScore> score, FILE* fp);	//score에 저장된 query 결과 랭크