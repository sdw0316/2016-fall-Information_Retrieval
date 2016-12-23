#include "ranking.h"
#include "dat_structure.h"
#include <string.h>
#include <math.h>
#include <algorithm>

#define BUF 128
#define LINE_BYTE 30
#define RESULT_NUM 1000

using namespace std;

ScoreTable score_table;

void buildTable(wordHash q) {
	FILE *fp = fopen("./src/index.dat", "r");
	if (fp == NULL) {
		exit(1);
	}
	//만약 테이블 내용물 있으면 비우고 시작
	/*
	int i = 0;
	for (ScoreTable::iterator iter = score_table.begin(); iter != score_table.end();iter++) {
		i++;
	}
	*/
	score_table.clear();

	for (wordHash::iterator iter = q.begin(); iter != q.end(); ++iter) {
		int word_id = table[iter->first].id;
		int qtf = iter->second;
		long long loc = table[iter->first].offset;

		_fseeki64(fp, loc*LINE_BYTE, SEEK_SET);

		//index.dat offset부터 DF만큼 읽기
		for (int DF = 0; DF < table[iter->first].DF; DF++) {
			char line[BUF];
			char *str;
			int count = 0;;

			int doc_id;
			int dtf;
			float weight;

			fgets(line, BUF, fp);
			str = strtok(line, " ");
			while (str = strtok(NULL, " ")) {
				count++;
				switch (count) {
				case 1:
					doc_id = atoi(str);
					break;
				case 2:
					dtf = atoi(str);
					break;
				case 3:
					weight = (float)atof(str);
					break;
				default:
					break;
				}
			}
			STableVal temp = { iter->first, qtf, dtf, weight };
			score_table[doc_id].push_back(temp);
		}
	}

	fclose(fp);
	return;
}
void vectorScoring(vector<DocScore> *score,Query q) {
	for (ScoreTable::iterator iter = score_table.begin(); iter != score_table.end(); ++iter) {
		int doc_id = iter->first;
		DocScore temp;
		
		double d_sum = 0.0;
		double dq_sum = 0.0;
		/*한 문서마다 가꼬있는 점수 scoring*/
		for (int j = 0; j < iter->second.size(); j++) {
			d_sum += (iter->second[j].weight)*(iter->second[j].weight);
			dq_sum += ((iter->second[j].weight))*(double)(iter->second[j].qtf);
		}
		temp.first = dq_sum / sqrt(d_sum);
		temp.second = doc_id;
		
		score->push_back(temp);
	}
	return;
}

void BMScoring(vector<DocScore> *score,Query q) {
	double k1 = 1.2;
	double k2 = 100.0;
	double b = 0.75;
	for (ScoreTable::iterator iter = score_table.begin(); iter != score_table.end(); ++iter) {
		double BMscore = 0.0;
		int doc_len = doc_list[iter->first].length;
		int doc_id = iter->first;
		double K = k1*((1 - b) + (b*((double)doc_len / (double)aval)));

		for (int j = 0; j < iter->second.size(); j++) {
			double co = 1.0;
			string term = iter->second[j].term;
			if (q.title.find(term) != q.title.end())
				co = 1.0;
			double df = (double)table[iter->second[j].term].DF;
			double qtf = (double)iter->second[j].qtf;
			double dtf = (double)iter->second[j].dtf;

			BMscore += co*(log10f((DOC_MAX - df + 0.5) / (df + 0.5))) * (((k1+1)*dtf)/(K+dtf)) * (((k2+1)*(qtf))/(k2+qtf));
		}
		DocScore temp = { BMscore, doc_id };
		score->push_back(temp);
	}
	return;
}

void showRank(Query q, vector<DocScore> score, FILE* fp) {
	if (fp == NULL) {
		exit(1);
	}

	fprintf(fp, "%d\n", q.id);
	int len = score.size();
	for (int i = len-1; i >= len - RESULT_NUM; i--) {
		int doc_id = score[i].second;
		string doc_no = doc_list[doc_id-1].doc_no;
		double value = score[i].first;
		if (i != len-1)
			fprintf(fp, "\t");
		fprintf(fp, "%s", doc_no.c_str());
	}
	fprintf(fp,"\n");
	return;
}