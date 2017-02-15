/*
 * BIO.cpp
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

namespace BIO {

  void SaveIntV(const TIntV& IntV, const TStr& Fnm,
                const TStr& anno) {
    SaveVec(IntV, Fnm, "%d", anno);
  }
  void SaveFltV(const TFltV& FltV, const TStr& Fnm,
                const TStr& anno) {
    SaveVec(FltV, Fnm, "%.6e", anno);
  }

  void SaveIntPrV(const TIntPrV& IntPrV, const TStr& Fnm,
                  const TStr& anno) {
    SavePrV<TInt, TInt>(IntPrV, Fnm, "%d\t%d", anno);
  }
  void SaveFltPrV(const TFltPrV& FltPrV, const TStr& Fnm,
                  const TStr& anno) {
    SavePrV<TFlt, TFlt>(FltPrV, Fnm, "%.6e\t%.6e", anno);
  }

  void SaveFltIntPrV(const TFltIntPrV& FltIntPrV,
                     const TStr& Fnm, const TStr& anno) {
    SavePrV<TFlt, TInt>(FltIntPrV, Fnm, "%.6e\t%d", anno);
  }
  void SaveIntVWithIdx(const TIntV& IntV, const TStr& Fnm,
                       const int StartIdx, const TStr& anno) {
    SaveVecWithIdx<TInt>(IntV, Fnm, "%d\t%d", StartIdx, anno);
  }
  void SaveFltVWithIdx(const TFltV& FltV, const TStr& Fnm,
                       const int StartIdx, const TStr& anno) {
    SaveVecWithIdx<TFlt>(FltV, Fnm, "%d\t%.6e", StartIdx,
                         anno);
  }

  void SaveIntFltKdV(const TIntFltKdV& list, const TStr& Fnm,
                     const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < list.Len(); i++)
      Fw->PutStrLn(TStr::Fmt("%d\t%.6e", list[i].Key.Val,
                             list[i].Dat.Val));
  }

  void SaveIntH(const TIntH& hash, const TStr& Fnm,
                const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < hash.Len(); i++)
      Fw->PutStrLn(TStr::Fmt("%d\t%d", hash.GetKey(i).Val,
                             hash[i].Val));
  }
  void SaveStrIntH(const TStrIntH& hash, const TStr& Fnm,
                   const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < hash.Len(); i++)
      Fw->PutStrLn(TStr::Fmt("%s\t%d", hash.GetKey(i).CStr(),
                             hash[i].Val));
  }
  void SaveIntFltH(const TIntFltH& hash, const TStr& Fnm,
                   const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < hash.Len(); i++)
      Fw->PutStrLn(TStr::Fmt("%d\t%.6e", hash.GetKey(i).Val,
                             hash[i].Val));
  }
  void SaveIntPrIntH(const TIntPrIntH& hash, const TStr& Fnm,
                     const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < hash.Len(); i++) {
      TIntPr pr = hash.GetKey(i);
      Fw->PutStrLn(TStr::Fmt("%d\t%d\t%d", pr.Val1.Val,
                             pr.Val2.Val, hash[i].Val));
    }
  }
  void SaveIntIntPrH(const TIntPrH& hash, const TStr& Fnm,
                     const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < hash.Len(); i++)
      Fw->PutStrLn(TStr::Fmt("%d\t%d\t%d", hash.GetKey(i).Val,
                             hash[i].Val1.Val,
                             hash[i].Val2.Val));
  }

  void SaveIntSet(const TIntSet& set, const TStr& Fnm,
                  const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < set.Len(); i++) {
      Fw->PutInt(set[i]);
      Fw->PutLn();
    }
  }
  void SaveIntPrSet(const TIntPrSet& hash, const TStr& Fnm,
                    const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < hash.Len(); i++) {
      TIntPr pr = hash.GetKey(i);
      Fw->PutStrLn(
          TStr::Fmt("%d\t%d", pr.Val1.Val, pr.Val2.Val));
    }
  }

  void SaveIntArray(const int* list, const int len,
                    const TStr& Fnm, const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < len; i++)
      Fw->PutStrLn(TStr::Fmt("%d", list[i]));
  }
  void SaveFltArray(const float* list, const int len,
                    const TStr& Fnm, const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < len; i++)
      Fw->PutStrLn(TStr::Fmt("%.6e", list[i]));
  }
  void SaveDubArray(const double* list, const int len,
                    const TStr& Fnm, const TStr& anno) {
    PSOut Fw = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm)
                                      : TFOut::New(Fnm);
    if (!anno.Empty()) Fw->PutStrLn(anno);
    for (int i = 0; i < len; i++)
      Fw->PutStrLn(TStr::Fmt("%.6f", list[i]));
  }

  // Load methods
  void LoadIntV(const TStr& fnm, TIntV& dat, const int col) {
    TSsParser Ss(fnm);
    while (Ss.Next()) dat.Add(Ss.GetInt(col));
  }
  void LoadIntV(const TStr& fnm, std::vector<int>& dat,
                const int col) {
    TSsParser Ss(fnm);
    while (Ss.Next()) dat.push_back(Ss.GetInt(col));
  }
  void LoadIntSet(const TStr& fnm, TIntSet& dat,
                  const int col) {
    TSsParser Ss(fnm);
    while (Ss.Next()) dat.AddKey(Ss.GetInt(col));
  }
  void LoadFltV(const TStr& fnm, TFltV& dat, const int col) {
    TSsParser Ss(fnm);
    while (Ss.Next()) dat.Add(Ss.GetFlt(col));
  }
  void LoadIntH(const TStr& fnm, TIntH& dat, const int c1,
                const int c2) {
    TSsParser Ss(fnm);
    while (Ss.Next())
      dat.AddDat(Ss.GetInt(c1), Ss.GetInt(c2));
  }
  void LoadIntFltH(const TStr& fnm, TIntFltH& dat,
                   const int c1, const int c2) {
    TSsParser Ss(fnm);
    while (Ss.Next())
      dat.AddDat(Ss.GetInt(c1), Ss.GetFlt(c2));
  }
  void LoadIntPrSet(const TStr& fnm, TIntPrSet& dat,
                    const int c1, const int c2) {
    TSsParser Ss(fnm);
    while (Ss.Next())
      dat.AddKey(TIntPr(Ss.GetInt(c1), Ss.GetInt(c2)));
  }
  void LoadIntPrV(const TStr& fnm, TIntPrV& dat, const int c1,
                  const int c2) {
    TSsParser Ss(fnm);
    while (Ss.Next())
      dat.Add(TIntPr(Ss.GetInt(c1), Ss.GetInt(c2)));
  }
  void LoadIntFltPrV(const TStr& fnm, TIntFltPrV& dat,
                     const int c1, const int c2) {
    TSsParser Ss(fnm);
    while (Ss.Next())
      dat.Add(TIntFltPr(Ss.GetInt(c1), Ss.GetFlt(c2)));
  }
}
