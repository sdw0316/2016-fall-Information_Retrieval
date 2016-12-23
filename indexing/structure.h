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

typedef map<string, WVal> WMap;	//WMap = {"word" => {TFvec, CF}}	id�� �׳� ������
typedef map<string, TFVal> TFMap;

extern WMap wordinfo;	//��� ������ ���� word_info ����
extern int totalN;	//��� document�� �� N
 
//--------------------functions----------------------//
void extractTF(char* path,FILE* TFpath[]);	//path ������ document�� ���� ������ TF�� TFpath�� ����
void checkTF(char* word);	//�ش� word�� doc_info���� � TF�� ������ check�ϰ� TFMap�� �μ�Ʈ
void clearDocInfo(FILE* TFpath[]);	//�ش� TFMap�� ������ TFpath�� ����, �ʱ�ȭ
