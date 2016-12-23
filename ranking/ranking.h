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

void buildTable(wordHash query_word);	//�� query object q�� ��ȸ�ϸ� score table ����

void vectorScoring(vector<DocScore> *score, Query q);	//�ش� query�� ���� ���� ������ ���� list�� Vector scoring�� vector�� �����ֱ�
void BMScoring(vector<DocScore> *score, Query q);	//�ش� query�� ���� ���� table�� BM25 ���ھ�� �����ֱ�
void showRank(Query q, vector<DocScore> score, FILE* fp);	//score�� ����� query ��� ��ũ