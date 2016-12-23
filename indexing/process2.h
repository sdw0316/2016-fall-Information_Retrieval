#pragma once
#include "structure.h"

typedef struct _aval{
	int doc_id;
	int TF;
} AVal;

typedef map<int, vector<AVal>> AMap;

void instertAMap(AMap *map, int key, AVal val);	//AMap�� key , value ����
void alignTF(FILE* TFpath[], int n); //n��° TF������ ����
void readTF(FILE* TFpath[], int n, float* docsum);	//n��° TF������ ���� �����صα�