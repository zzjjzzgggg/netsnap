/*
 * BIO.h
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

#ifndef BIO_H_
#define BIO_H_

class BIO {
public:
	void static SaveIntVec(TIntV list, TStr fname, TStr anno=""){
		FILE* fw=fopen(fname.CStr(), "w");
		fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), list.Len());
		if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
		for(int i=0; i<list.Len(); i++) fprintf(fw, "%d\n", list[i].Val);
		fclose(fw);
	}
	void static SaveFltVec(TFltV list, TStr fname, TStr anno=""){
		FILE* fw=fopen(fname.CStr(), "w");
		fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), list.Len());
		if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
		for(int i=0; i<list.Len(); i++) fprintf(fw, "%.6f\n", list[i].Val);
		fclose(fw);
	}
	void static SaveIntSet(TIntSet set, TStr fname, TStr anno=""){
		TIntV keys;
		set.GetKeyV(keys);
		SaveIntVec(keys, fname, anno);
	}
	void static SaveIntH(TIntH hash, TStr fname, TStr anno=""){
		FILE* fw=fopen(fname.CStr(), "w");
		fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
		if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
		for(int i=0; i<hash.Len(); i++) fprintf(fw, "%d\t%d\n", hash.GetKey(i).Val, hash[i].Val);
		fclose(fw);
	}
	void static SaveIntFltH(TIntFltH hash, TStr fname, TStr anno=""){
		FILE* fw=fopen(fname.CStr(), "w");
		fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
		if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
		for(int i=0; i<hash.Len(); i++) fprintf(fw, "%d\t%.6f\n", hash.GetKey(i).Val, hash[i].Val);
		fclose(fw);
	}
	void static SaveIntPrIntH(TIntPrIntH hash, TStr fname, TStr anno=""){
		FILE* fw=fopen(fname.CStr(), "w");
		fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
		if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
		for(int i=0; i<hash.Len(); i++) {
			TIntPr pr=hash.GetKey(i);
			fprintf(fw, "%d\t%d\t%d\n", pr.Val1.Val, pr.Val2.Val, hash[i].Val);
		}
		fclose(fw);
	}
	void static SaveIntPrH(TIntPrH hash, TStr fname, TStr anno=""){
		FILE* fw=fopen(fname.CStr(), "w");
		fprintf(fw, "#File: %s\n#Len: %d\n", fname.CStr(), hash.Len());
		if(!anno.Empty()) fprintf(fw, "#%s\n", anno.CStr());
		for(int i=0; i<hash.Len(); i++) {
			fprintf(fw, "%d\t%d\t%d\n", hash.GetKey(i).Val, hash[i].Val1.Val, hash[i].Val2.Val);
		}
		fclose(fw);
	}

	void static LoadIntVec(const TStr fnm, TIntV& dat, const int col=0){
		dat.Clr();
		TSsParser Ss(fnm);
		while(Ss.Next()) dat.Add(Ss.GetInt(col));
	}
	void static LoadFltVec(const TStr fnm, TFltV& dat, const int col=0){
		dat.Clr();
		TSsParser Ss(fnm);
		while(Ss.Next()) dat.Add(Ss.GetFlt(col));
	}
	void static LoadIntH(const TStr fnm, TIntH& dat, const int c1=0, const int c2=1){
		dat.Clr();
		TSsParser Ss(fnm);
		while(Ss.Next()) dat.AddDat(Ss.GetInt(c1), Ss.GetInt(c2));
	}
	void static LoadIntFltH(const TStr fnm, TIntFltH& dat, const int c1=0, const int c2=1){
		dat.Clr();
		TSsParser Ss(fnm);
		while(Ss.Next()) dat.AddDat(Ss.GetInt(c1), Ss.GetFlt(c2));
	}
};

#endif /* BIO_H_ */
