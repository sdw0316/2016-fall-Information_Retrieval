#pragma once
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define BUF 128

using namespace std;

typedef pair<unsigned int, unsigned int> TFpair;	//TFpair = { docID, TF }

typedef struct _wordVal {
	vector<TFpair> TFvec;
	unsigned int CF;
} WVal;

typedef struct _TFval{
	unsigned int docid;
	unsigned int TF;
	unsigned int offset;
}TFVal;

typedef map<string, WVal> WMap;	//WMap = {"word" => {TFvec, CF}}	id는 그냥 사전순
typedef map<string, TFVal> TFMap;

extern WMap wordinfo;	//모든 문서에 대한 word_info 저장
extern int totalN;	//모든 document의 수 N
 
//--------------------functions----------------------//
void extractTF(char* path,FILE* TFpath[]);	//path 문서의 document를 돌며 추출한 TF를 TFpath에 저장
void checkTF(char* word);	//해당 word가 doc_info에서 어떤 TF를 가진지 check하고 TFMap에 인설트
void clearDocInfo(FILE* TFpath[]);	//해당 TFMap의 정보를 TFpath에 쓰고, 초기화
