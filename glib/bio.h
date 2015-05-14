/*
 * BIO.h
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

namespace BIO {

template<class TVal> void SaveVec(const TVec<TVal>& Vec, const TStr& Fnm, const TStr& Fmt, const TStr& anno);
void SaveIntV(const TIntV& IntV, const TStr& Fnm, const TStr& anno="");
void SaveFltV(const TFltV& FltV, const TStr& Fnm, const TStr& Fmt="%.6e", const TStr& anno="");

template<class TVal1, class TVal2> void SavePrV(const TVec< TPair<TVal1, TVal2> >& Pairs, const TStr& Fnm, const TStr& Fmt, const TStr& anno="");
void SaveIntPrV(const TIntPrV& IntPrV, const TStr& Fnm, const TStr& anno="");
void SaveFltPrV(const TFltPrV& FltPrV, const TStr& Fnm, const TStr& Fmt="%.6e\t%.6e", const TStr& anno="");

template<class TVal> void SaveVecWithIdx(const TVec<TVal>& Vec, const TStr& Fnm, const TStr& Fmt, const int StartIdx, const TStr& anno);
void SaveIntVWithIdx(const TIntV& IntV, const TStr& Fnm, const int StartIdx=0, const TStr& anno="");
void SaveFltVWithIdx(const TFltV& FltV, const TStr& Fnm, const int StartIdx=0, const TStr& anno="");

//////////////////////////////////////////////////////////////////////////////////////////////////

template<class TVal>
void SaveVec(const TVec<TVal>& Vec, const TStr& Fnm, const TStr& Fmt, const TStr& anno){
	PSOut FOutPt = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm) : TFOut::New(Fnm);
	FOutPt->PutStrLn(TStr::Fmt("# File: %s\n# Len: %d", Fnm.CStr(), Vec.Len()));
	if(!anno.Empty()) FOutPt->PutStrLn(anno);
	for(int i=0; i<Vec.Len(); i++) FOutPt->PutStrLn(TStr::Fmt(Fmt.CStr(), Vec[i].Val));
}

template<class TVal1, class TVal2>
void SavePrV(const TVec< TPair<TVal1, TVal2> >& Pairs, const TStr& Fnm, const TStr& Fmt, const TStr& anno){
	PSOut FOutPt = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm) : TFOut::New(Fnm);
	FOutPt->PutStrLn(TStr::Fmt("# File: %s\n# Len: %d", Fnm.CStr(), Pairs.Len()));
	if(!anno.Empty()) FOutPt->PutStrLn(anno);
	for(int i=0; i<Pairs.Len(); i++)
		FOutPt->PutStrLn(TStr::Fmt(Fmt.CStr(), Pairs[i].Val1.Val, Pairs[i].Val2.Val));
}

template<class TVal>
	void SaveVecWithIdx(const TVec<TVal>& Vec, const TStr& Fnm, const TStr& Fmt, const int StartIdx, const TStr& anno){
	PSOut FOutPt = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm) : TFOut::New(Fnm);
	FOutPt->PutStrLn(TStr::Fmt("# File: %s\n# Len: %d", Fnm.CStr(), Vec.Len()));
	if(!anno.Empty()) FOutPt->PutStrLn(anno);
	for(int i=0; i<Vec.Len(); i++) FOutPt->PutStrLn(TStr::Fmt(Fmt.CStr(), i+StartIdx, Vec[i].Val));
}


/////////////////////////////////////////////////////////////////////////////////////////////////

	void SaveIntFltKdV(const TIntFltKdV& data, const TStr& Fnm, const TStr& anno="");

	void SaveIntSet(const TIntSet& set, const TStr& Fnm, const TStr& anno="");
	void SaveIntPrSet(const TIntPrSet& hash, const TStr& Fnm, const TStr& anno="");

	void SaveIntH(const TIntH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveStrIntH(const TStrIntH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveIntFltH(const TIntFltH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveIntPrIntH(const TIntPrIntH& hash, const TStr& Fnm, const TStr& anno="");
	void SaveIntIntPrH(const TIntPrH& hash, const TStr& Fnm, const TStr& anno="");


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
