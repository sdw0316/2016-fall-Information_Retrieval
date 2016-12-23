#include "structure.h"
#include "process2.h"
#include <math.h>
//process 2 for align each TF files with WordId

void instertAMap(AMap *map, int key, AVal val) {
	(*map)[key].push_back(val);
}

void alignTF(FILE* TFpath[], int n) {
	FILE *TF2;
	char path[BUF];

	clock_t start, end;

	if (TFpath[n] == NULL) {
		printf("file open error!\n");
		exit(1);
	}
	else {
		sprintf(path, "./Index/TF%d.txt", n);
		TF2 = fopen(path, "w");
	}

	start = clock();
	printf("./TFdat/TF%d.txt is processing....\n", n);

	AMap temp;
	if (TF2 != NULL) {
		char line[BUF];
		/*TF파일에서 라인별로 읽으며 sorting*/
		while (fgets(line, BUF, TFpath[n])) {
			int word_id;
			AVal val;
			sscanf(line, "%d\t%d\t%d", &val.doc_id, &word_id, &val.TF);

			instertAMap(&temp, word_id, val);
		}
	}

	for (AMap::iterator iter = temp.begin(); iter != temp.end(); ++iter) {
		for (int i = 0; i < (iter->second).size();i++) {
			fprintf(TF2, "%8d %6d %3d\n", iter->first, (iter->second[i]).doc_id, (iter->second[i]).TF);
		}
	}

	temp.clear();

	end = clock();	
	printf("\r%lf second over...\n", (double)(end - start) / 1000);

	fclose(TFpath[n]);
	fclose(TF2);
	return;
}

void readTF(FILE* TFpath[], int n, float* docsum) {
	char path[BUF];

	clock_t start, end;

	if (TFpath[n] == NULL) {
		printf("file open error!\n");
		exit(1);
	}

	start = clock();
	printf("./TFdat/TF%d.txt is reading....\n", n);

	char line[BUF];
	/*TF파일에서 라인별로 읽으며 sorting*/
	while (fgets(line, BUF, TFpath[n])) {
		int doc_id;
		int TF;
		char word[BUF];
		float sum;

		sscanf(line, "%d\t%s\t%d", &doc_id, &word, &TF);
		string key = word;

		if (wordinfo.find(key) == wordinfo.end()) {
			continue;
		}

		sum = (log10f((float)TF) + 1.0)*(log10f((float)totalN/(float)wordinfo[key].TFvec.size()));
		docsum[doc_id] += sum;
	}

	end = clock();
	printf("\r%lf second over...\n", (double)(end - start) / 1000);
	fclose(TFpath[n]);
	return;
}
