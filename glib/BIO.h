/*
 * BIO.h
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

namespace BIO {

	void SaveIntVec(const TIntV& list, TStr fname, TStr anno="");
	void SaveFltVec(const TFltV& list, TStr fname, TStr anno="");
	void SaveIntSet(const TIntSet& set, TStr fname, TStr anno="");
	void SaveIntH(const TIntH& hash, TStr fname, TStr anno="");
	void SaveIntFltH(const TIntFltH& hash, TStr fname, TStr anno="");
	void SaveIntPrIntH(const TIntPrIntH& hash, TStr fname, TStr anno="");
	void SaveIntIntPrH(const TIntPrH& hash, TStr fname, TStr anno="");
	void SaveIntPrSet(const TIntPrSet& hash, TStr fname, TStr anno="");

	void LoadIntVec(const TStr fnm, TIntV& dat, const int col=0);
	void LoadIntSet(const TStr fnm, TIntSet& dat, const int col=0);
	void LoadFltVec(const TStr fnm, TFltV& dat, const int col=0);
	void LoadIntH(const TStr fnm, TIntH& dat, const int c1=0, const int c2=1);
	void LoadIntFltH(const TStr fnm, TIntFltH& dat, const int c1=0, const int c2=1);
	void LoadIntPrSet(TStr fnm, TIntPrSet& dat, const int c1=0, const int c2=1);
	void LoadIntPrVec(TStr fnm, TIntPrV& dat, const int c1=0, const int c2=1);


	void SaveIntArray(const int* list, const int len, TStr fname, TStr anno="");
	void SaveFltArray(const float* list, const int len, TStr fname, TStr anno="");
	void SaveDubArray(const double* list, const int len, TStr fname, TStr anno="");
}

