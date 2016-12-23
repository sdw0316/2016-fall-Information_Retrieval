/*************************************************************
**********data structures to store word, doc data************
**************************************************************/
#include <string>
#include <unordered_map>

#define WORD_MAX 501076
#define DOC_MAX 554028

using namespace std;

typedef struct _table_value {
	int id;
	int DF;
	unsigned int offset;
}TableVal;
typedef struct _doc_value {
	string doc_no;
	int length;
}DocVal;

typedef unordered_map<string, TableVal> WordTable;	//wordtable => { term : (wordId, DF, offset) } 의 해쉬 테이블

extern WordTable table;
extern DocVal doc_list[DOC_MAX];
extern double aval;	//docment average length

void loadWord();	//wordTable 에 word.dat 저장
void loadDocs();	//doc배열에 doc_no, length 저장