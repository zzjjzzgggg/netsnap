/////////////////////////////////////////////////
// Graph Utilities
class TGUtil {
public:
  static void GetCdf(const TIntPrV& PdfV, TIntPrV& CdfV);
  static void GetCdf(const TFltPrV& PdfV, TFltPrV& CdfV);
  static void GetCdf(const TIntFltKdV& PdfV, TIntFltKdV& CdfV);
  static TIntPrV GetCdf(const TIntPrV& PdfV);
  static TFltPrV GetCdf(const TFltPrV& PdfV);

  static void GetCCdf(const TIntPrV& PdfV, TIntPrV& CCdfV);
  static void GetCCdf(const TFltPrV& PdfV, TFltPrV& CCdfV);
  static void GetCCdf(const TIntFltKdV& PdfV, TIntFltKdV& CCdfV);
  static TIntPrV GetCCdf(const TIntPrV& PdfV);
  static TFltPrV GetCCdf(const TFltPrV& PdfV);

  static void GetPdf(const TIntPrV& CdfV, TIntPrV& PdfV);
  static void GetPdf(const TFltPrV& CdfV, TFltPrV& PdfV);
  static void GetPdf(const TIntFltKdV& CdfV, TIntFltKdV& PdfV);

  static void Normalize(TFltPrV& PdfV);
  static void Normalize(TIntFltKdV& PdfV);

  static void MakeExpBins(const TFltPrV& XYValV, TFltPrV& ExpXYValV, const double& BinFactor = 2, const double& MinYVal = 1);
  static void MakeExpBins(const TFltKdV& XYValV, TFltKdV& ExpXYValV, const double& BinFactor = 2, const double& MinYVal = 1);
  static void MakeExpBins(const TFltV& YValV, TFltV& ExpYValV, const double& BinFactor = 1.01);
  static void MakeExpBins(const TIntV& YValV, TIntV& ExpYValV, const double& BinFactor = 1.01);
};

/////////////////////////////////////////////////
// String helper functions and utilities
class TStrUtil {
public:
  static TChA& GetXmlTagVal(TXmlLx& XmlLx, const TChA& TagNm);
  static void GetXmlTagNmVal(TXmlLx& XmlLx, TChA& TagNm, TChA& TagVal);
  static bool GetXmlTagNmVal2(TXmlLx& XmlLx, TChA& TagNm, TChA& TagVal, const bool& TakeTagNms);
  static TChA GetDomNm(const TChA& UrlChA);  // www.cs.cmu.edu
  static TChA GetDomNm2(const TChA& UrlChA); // also strip starting www.
  static TChA GetWebsiteNm(const TChA& UrlChA); // get website (GetDomNm2 or blog url)
  static bool GetNormalizedUrl(const TChA& UrlIn, const TChA& BaseUrl, TChA& UrlOut);
  static bool StripEnd(const TChA& Str, const TChA& SearchStr, TChA& NewStr);
  //int GetNthOccurence(const TChA& Url, const int& Count, const char Ch = '/');

  static TChA GetShorStr(const TChA& LongStr, const int MaxLen=50);
  static TChA GetCleanStr(const TChA& ChA);
  static TChA GetCleanWrdStr(const TChA& ChA);
  static int CountWords(const char* CStr);
  static int CountWords(const TChA& ChA);
  static int CountWords(const TChA& ChA, const TStrHash<TInt>& StopWordH);
  static int SplitWords(TChA& ChA, TVec<char *>& WrdV, const bool& SplitOnWs=true);
  static int SplitOnCh(TChA& ChA, TVec<char *>& WrdV, const char& Ch, const bool& SkipEmpty=false);
  static int SplitLines(TChA& ChA, TVec<char *>& LineV, const bool& SkipEmpty=false);
  static int SplitSentences(TChA& ChA, TVec<char *>& SentenceV);
  static void RemoveHtmlTags(const TChA& HtmlStr, TChA& TextStr);
  static bool IsLatinStr(const TChA& Str, const double& MinAlFrac);
  static void GetWIdV(const TStrHash<TInt>& StrH, const char *CStr, TIntV& WIdV);
  static void GetAddWIdV(TStrHash<TInt>& StrH, const char *CStr, TIntV& WIdV);
  static bool GetTmFromStr(const char* TmStr, TSecTm& Tm);

  // Name Splitter and Standardizer
  static TStr GetStdName(TStr AuthorName);
  static void GetStdNameV(TStr AuthorNames, TStrV& StdNameV);
};

class TAvg {
private:
	TIntFltVH sum, sqr;
	TIntH Nums;
public:
	void Add(int key, TFltV item){
		if(!sum.IsKey(key)){
			sum(key)=TFltV(item.Len());
			sqr(key)=TFltV(item.Len());
		}
		for(int i=0; i<item.Len(); i++){
			sum(key)[i]+=item[i];
			sqr(key)[i]+=item[i]*item[i];
		}
		Nums(key)+=1;
	}

	void Add(int key, double val){
		if(!sum.IsKey(key)){
			sum(key)=TFltV(1);
			sqr(key)=TFltV(1);
		}
		sum(key)[0]+=val;
		sqr(key)[0]+=val*val;
		Nums(key)+=1;
	}

	void Print(){
		TIntV keys;
		sum.GetKeyV(keys);
		keys.Sort();
		for(int i=0; i<keys.Len(); i++){
			int key=keys[i];
			printf("%d", key);
			for(int j=0; j<sum(key).Len(); j++) {
				double mean=sum(key)[j]/Nums(key);
				double var=sqrt(fabs(sqr(key)[j]/Nums(key)-mean*mean));
				printf("\t%.4f(%.4f)", mean, var);
			}
			printf("\n");
		}
	}

	void Save(TStr fnm, bool echo=true){
		TIntV keys;
		sum.GetKeyV(keys);
		keys.Sort();
		FILE* fw=fopen(fnm.CStr(), "w");
		for(int i=0; i<keys.Len(); i++){
			int key=keys[i];
			fprintf(fw, "%d", key);
			if(echo) printf("%d", key);
			for(int j=0; j<sum(key).Len(); j++) {
				double mean=sum(key)[j]/Nums(key);
				double var=sqrt(fabs(sqr(key)[j]/Nums(key)-mean*mean));
				fprintf(fw, "\t%.4f\t%.4f", mean, var);
				if(echo) printf("\t%.4f(%.4f)", mean, var);
			}
			fprintf(fw, "\n");
			if(echo) printf("\n");
		}
		fclose(fw);
	}
};


/////////////////////////////////////////////////
namespace Utils{
// max eigvalue and its eigvector
template<class PGraph> double getMxEig(const PGraph& Graph, const double& eps=1E-6, const int& maxIter=10000);

template<class PGraph> double getMxEig(const PGraph& Graph, const double& eps, const int& maxIter){
	const int N=Graph->GetNodes();
	TIntIntH nmap;
	nmap.Gen(N);
	int j=0;
	for(typename PGraph::TObj::TNodeI ni=Graph->BegNI(); ni<Graph->EndNI(); ni++,j++) nmap.AddDat(ni.GetId(), j);
	TFltV x,y;
	x.Gen(N); y.Gen(N);
	double mxNew, mxOld=1;
	for(int i=0;i<N;i++) x[i]=1;
	for(int iter=0;iter<maxIter;iter++){
		j=0; mxNew=TFlt::Mn;
		for(typename PGraph::TObj::TNodeI ni=Graph->BegNI(); ni<Graph->EndNI(); ni++,j++){
			y[j]=0;
			for(int e=0; e<ni.GetInDeg(); e++) y[j]+=x[nmap.GetDat(ni.GetInNId(e))];
			if(y[j]>mxNew) mxNew=y[j];
		}
		if(fabs(mxNew-mxOld)<eps) {
			printf("Iteration number=%d\n", iter);
			break;
		}
		for(int i=0;i<N;i++) x[i]=y[i]/mxNew;
		mxOld=mxNew;
	}
	return mxNew;
}

};
