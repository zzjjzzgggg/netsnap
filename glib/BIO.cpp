/*
 * BIO.cpp
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

namespace BIO{

void SaveInts(const TIntV& list, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), list.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<list.Len(); i++) fprintf(fw, "%d\n", list[i].Val);
	fclose(fw);
}
void SaveFlts(const TFltV& list, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), list.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<list.Len(); i++) fprintf(fw, "%.6f\n", list[i].Val);
	fclose(fw);
}
void SaveInts(const TIntSet& set, TStr fname, TStr anno){
	TIntV keys;
	set.GetKeyV(keys);
	SaveInts(keys, fname, anno);
}
void SaveIntH(const TIntH& hash, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<hash.Len(); i++) fprintf(fw, "%d\t%d\n", hash.GetKey(i).Val, hash[i].Val);
	fclose(fw);
}
void SaveIntFltH(const TIntFltH& hash, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<hash.Len(); i++) fprintf(fw, "%d\t%.6f\n", hash.GetKey(i).Val, hash[i].Val);
	fclose(fw);
}
void SaveIntPrIntH(const TIntPrIntH& hash, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<hash.Len(); i++) {
		TIntPr pr=hash.GetKey(i);
		fprintf(fw, "%d\t%d\t%d\n", pr.Val1.Val, pr.Val2.Val, hash[i].Val);
	}
	fclose(fw);
}
void SaveIntIntPrH(const TIntPrH& hash, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<hash.Len(); i++) {
		fprintf(fw, "%d\t%d\t%d\n", hash.GetKey(i).Val, hash[i].Val1.Val, hash[i].Val2.Val);
	}
	fclose(fw);
}
void SaveIntPrSet(const TIntPrSet& hash, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<hash.Len(); i++) {
		TIntPr pr=hash.GetKey(i);
		fprintf(fw, "%d\t%d\n", pr.Val1.Val, pr.Val2.Val);
	}
	fclose(fw);
}



// Load methods
void LoadInts(const TStr fnm, TIntV& dat, const int col){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.Add(Ss.GetInt(col));
}
void LoadInts(const TStr fnm, TIntSet& dat, const int col){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.AddKey(Ss.GetInt(col));
}
void LoadFlts(const TStr fnm, TFltV& dat, const int col){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.Add(Ss.GetFlt(col));
}
void LoadIntH(const TStr fnm, TIntH& dat, const int c1, const int c2){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.AddDat(Ss.GetInt(c1), Ss.GetInt(c2));
}
void LoadIntFltH(const TStr fnm, TIntFltH& dat, const int c1, const int c2){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.AddDat(Ss.GetInt(c1), Ss.GetFlt(c2));
}
void LoadIntPrSet(TStr fnm, TIntPrSet& dat, const int c1, const int c2){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.AddKey(TIntPr(Ss.GetInt(c1), Ss.GetInt(c2)));
}
void LoadIntPrVec(TStr fnm, TIntPrV& dat, const int c1, const int c2){
	dat.Clr();
	TSsParser Ss(fnm);
	while(Ss.Next()) dat.Add(TIntPr(Ss.GetInt(c1), Ss.GetInt(c2)));
}



void SaveIntArray(const int* list, const int len, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), len);
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<len; i++) fprintf(fw, "%d\n", list[i]);
	fclose(fw);
}
void SaveFltArray(const float* list, const int len, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), len);
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<len; i++) fprintf(fw, "%.6f\n", list[i]);
	fclose(fw);
}
void SaveDubArray(const double* list, const int len, TStr fname, TStr anno){
	FILE* fw=fopen(fname.CStr(), "w");
	fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), len);
	if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
	for(int i=0; i<len; i++) fprintf(fw, "%.6f\n", list[i]);
	fclose(fw);
}

}
