/*
 * BIO.h
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

namespace BIO {

	void SaveIntV(const TIntV& list, const TStr& Fnm, const TStr& anno="");
	void SaveFltV(const TFltV& list, const TStr& Fnm, const TStr& anno="");
	void SaveIntPrV(const TIntPrV& list, const TStr& Fnm, const TStr& anno="");
	void SaveIntFltKdV(const TIntFltKdV& data, const TStr& Fnm, const TStr& anno="");

	void SaveIntSet(const TIntSet& set, const TStr& Fnm, const TStr& anno="");
	void SaveIntPrSet(const TIntPrSet& hash, const TStr& Fnm, const TStr& anno="");

	void SaveIntH(const TIntH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveStrIntH(const TStrIntH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveIntFltH(const TIntFltH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveIntPrIntH(const TIntPrIntH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveIntIntPrH(const TIntPrH& hash, const TStr& Fnm, const TStr& anno="");

	void SaveIntVWithIdx(const TIntV& list, const TStr& Fnm, const TStr& anno="");
	void SaveFltVWithIdx(const TFltV& list, const TStr& Fnm, const TStr& anno="");

	void LoadIntV(const TStr& fnm, TIntV& dat, const int col=0);
	void LoadIntSet(const TStr& fnm, TIntSet& dat, const int col=0);
	void LoadFltV(const TStr& fnm, TFltV& dat, const int col=0);

	void LoadIntH(const TStr& fnm, TIntH& dat, const int c1=0, const int c2=1);
	void LoadIntFltH(const TStr& fnm, TIntFltH& dat, const int c1=0, const int c2=1);
	void LoadIntPrSet(const TStr& fnm, TIntPrSet& dat, const int c1=0, const int c2=1);
	void LoadIntPrV(const TStr& fnm, TIntPrV& dat, const int c1=0, const int c2=1);
	void LoadIntFltPrV(const TStr& fnm, TIntFltPrV& dat, const int c1=0, const int c2=1);

	void SaveIntArray(const int* list, const int len, const TStr& Fnm, const TStr& anno="");
	void SaveFltArray(const float* list, const int len, const TStr& Fnm, const TStr& anno="");
	void SaveDubArray(const double* list, const int len, const TStr& Fnm, const TStr& anno="");

}

