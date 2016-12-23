#pragma once
#include "structure.h"

typedef struct _aval{
	int doc_id;
	int TF;
} AVal;

typedef map<int, vector<AVal>> AMap;

void instertAMap(AMap *map, int key, AVal val);	//AMap에 key , value 삽입
void alignTF(FILE* TFpath[], int n); //n번째 TF파일을 정렬
void readTF(FILE* TFpath[], int n, float* docsum);	//n번째 TF읽으며 정보 저장해두기