/*
 * BIO.h
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

namespace BIO {

	void SaveInts(const TIntV& list, const TStr& fname, const TStr& anno="");
	void SaveFlts(const TFltV& list, const TStr& fname, const TStr& anno="");
	void SaveInts(const TIntSet& set, const TStr& fname, const TStr& anno="");
	void SaveIntH(const TIntH& hash, const TStr& fname, const TStr& anno="");
	void SaveIntFltH(const TIntFltH& hash, const TStr& fname, const TStr& anno="");
	void SaveIntPrIntH(const TIntPrIntH& hash, const TStr& fname, const TStr& anno="");
	void SaveIntIntPrH(const TIntPrH& hash, const TStr& fname, const TStr& anno="");
	void SaveIntPrSet(const TIntPrSet& hash, const TStr& fname, const TStr& anno="");
	void SaveIntFltKdV(const TIntFltKdV& data, const TStr& fname, const TStr& anno="");

	void SaveFltsWithIdx(const TFltV& list, const TStr& fname, const TStr& anno="");

	void LoadInts(const TStr& fnm, TIntV& dat, const int col=0);
	void LoadInts(const TStr& fnm, TIntSet& dat, const int col=0);
	void LoadFlts(const TStr& fnm, TFltV& dat, const int col=0);

	void LoadIntH(const TStr& fnm, TIntH& dat, const int c1=0, const int c2=1);
	void LoadIntFltH(const TStr& fnm, TIntFltH& dat, const int c1=0, const int c2=1);
	void LoadIntPrSet(const TStr& fnm, TIntPrSet& dat, const int c1=0, const int c2=1);
	void LoadIntPrVec(const TStr& fnm, TIntPrV& dat, const int c1=0, const int c2=1);


	void SaveIntArray(const int* list, const int len, const TStr& fname, const TStr& anno="");
	void SaveFltArray(const float* list, const int len, const TStr& fname, const TStr& anno="");
	void SaveDubArray(const double* list, const int len, const TStr& fname, const TStr& anno="");

}

