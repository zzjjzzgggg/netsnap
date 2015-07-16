/////////////////////////////////////////////////
// Mathematics-Utilities
class TMath {
public:
	static double E;
	static double Pi;
	static double LogOf2;
	static double Abs(const double& x) {return fabs(x);}
	static double Inv(const double& x) {IAssert(x!=0.0); return 1.0/x;}
	static double Sqr(const double& x) {return x*x;}
	static double Sqrt(const double& x) {IAssert(x>=0); return sqrt(x);}
	static double Log2(const double& Val) {return log(Val)/LogOf2;}
	static double Round(const double& Val) {return Val>0?floor(Val+0.5):ceil(Val-0.5);}
	static double Round(const double & Val, int Decs) {const double pwr=pow(10.0, Decs); return Round(Val*pwr)/pwr;}
	static int Fac(const int& Val) {if (Val<=1) return 1; else return Val * Fac(Val - 1);}
	// binomial coefficient
	// original implementation: return Fac(N)/(Fac(K)*Fac(N-K));
	static int Choose(const int& N, const int& K) {
		if (K<0 || K>N) return 0;
		if (K==0 || K==N) return 1;
		int k = min(K, N-K)+1;
		double c=1;
		while (--k>0) c = c*(N-k+1)/k;
		return (int)c;
	}
	static uint Pow2(const int& pow) {return uint(1u << pow);}
	static double Power(const double& Base, const double& Exponent) {return pow(Base, Exponent);}
	template<typename T>
	static int Sign(const T& Val) {return Val < 0 ? -1 : (Val > 0 ? 1 : 0);}
	template<class T>
	static const T& Mx(const T& LVal, const T& RVal) {return LVal>RVal?LVal:RVal;}
	template<class T>
	static const T& Mn(const T& LVal, const T& RVal) {return LVal<RVal?LVal:RVal;}
	template<class T>
	static const T& Mx(const T& Val1, const T& Val2, const T& Val3) {
		if (Val1 > Val2) {
			if(Val1 > Val3) return Val1;
			else return Val3;
		} else {
			if (Val2 > Val3) return Val2;
			else return Val3;
		}
	}
	template<class T>
	static const T& Mn(const T& Val1, const T& Val2, const T& Val3) {
		if(Val1 < Val2) {
			if(Val1 < Val3) return Val1;
			else return Val3;
		} else {
			if(Val2 < Val3) return Val2;
			else return Val3;
		}
	}
	template<class T>
	static const T& Median(const T& Val1, const T& Val2, const T& Val3) {
		if (Val1 < Val2) {
			if (Val2 < Val3) return Val2;
			else if (Val3 < Val1) return Val1;
			else return Val3;
		} else {
			if (Val1 < Val3) return Val1;
			else if (Val3 < Val2) return Val2;
			else return Val3;
		}
	}
	template<class T>
	static const T& InRange(const T& Val, const T& Mn, const T& Mx) {IAssert(Mn <= Mx); return Val<Mn?Mn:(Val>Mx?Mx:Val);}
	template<class T>
	static bool IsInRange(const T& Val, const T& Mn, const T& Mx) {IAssert(Mn <= Mx); return Val >= Mn && Val <= Mx;}
	template<class T>
	static bool IsInEps(const T& Val, const T& Eps) {return Val >= -Eps && Val <= Eps;}
};

/////////////////////////////////////////////////
// Special-Functions
class TSpecFunc {
public:
	static double Normal(double x, const double mu=0, const double sigma=1);
	static double Binomial(const int k, const int n, const double p);
	static double BetaBinomial(const int k, const int n, const double alpha, const double beta);
	static void GammaPSeries/*gser*/(double& gamser, const double& a, const double& x, double& gln);
	static void GammaQContFrac/*gcf*/(double& gammcf, const double& a, const double& x, double& gln);
	static double GammaQ/*gammq*/(const double& a, const double& x);
	static double LnGamma/*gammln*/(const double& xx);
	static double BetaCf/*betacf*/(const double& a, const double& b, const double& x);
	static double BetaI(const double& a, const double& b, const double& x);
	// Y = A + B*X
	static void LinearFit(const TVec<TFltPr>& XY, double& A, double& B, double& SigA, double& SigB, double& Chi2, double& R2);
	// Y = A * X^B
	static void PowerFit(const TVec<TFltPr>& XY, double& A, double& B, double& SigA, double& SigB, double& Chi2, double& R2);
	// Y = A + B*log(X)
	static void LogFit(const TVec<TFltPr>& XY, double& A, double& B, double& SigA, double& SigB, double& Chi2, double& R2);
	// Y = A * exp(B*X)
	static void ExpFit(const TVec<TFltPr>& XY, double& A, double& B, double& SigA, double& SigB, double& Chi2, double& R2);
	static double LnComb(const int& n, const int& k);
	static double Entropy(const TIntV& ValV);
	static double Entropy(const TFltV& ValV);
	static void EntropyFracDim(const TIntV& ValV, TFltV& EntropyV);
	static void EntropyFracDim(const TFltV& ValV, TFltV& EntropyV);
	static double EntropyBias(const double& B); // solves for p: B = p*log2(p)+(1-p)log2(1-p)
	//MLE of the power-law coefficient
	static double GetPowerCoef(const TFltV& XValV, double MinX = -1.0); // values (sampled from the distribution)
	static double GetPowerCoef(const TFltPrV& XValCntV, double MinX = -1.0); // (value, count) pairs
};

/////////////////////////////////////////////////
// Statistical-Moments
class TMom;
typedef TPt<TMom> PMom;
typedef TVec<PMom> TMomV;
class TMom{
private:
	TCRef CRef;
private:
	TBool DefP;
	TFltPrV ValWgtV;
	TFlt SumW, ValSumW;
	TInt Vals;
	TBool UsableP;
	TFlt UnusableVal;
	TFlt Mn, Mx;
	TFlt Mean, Vari, SDev, SErr;
	TFlt Median, Quart1, Quart3;
	TFltV DecileV; // 0=min 1=1.decile, ..., 9=9.decile, 10=max
	TFltV PercentileV; // 0=min 1=1.percentile, ..., 9=9.percentile, 10=max
public:
	TMom():
	DefP(false), ValWgtV(),
	SumW(), ValSumW(), Vals(),
	UsableP(false), UnusableVal(-1),
	Mn(), Mx(),
	Mean(), Vari(), SDev(), SErr(),
	Median(), Quart1(), Quart3(),
	DecileV(), PercentileV() {}
	TMom(const TMom& Mom):
	DefP(Mom.DefP), ValWgtV(Mom.ValWgtV),
	SumW(Mom.SumW), ValSumW(Mom.ValSumW), Vals(Mom.Vals),
	UsableP(Mom.UsableP), UnusableVal(Mom.UnusableVal),
	Mn(Mom.Mn), Mx(Mom.Mx),
	Mean(Mom.Mean), Vari(Mom.Vari), SDev(Mom.SDev), SErr(Mom.SErr),
	Median(Mom.Median), Quart1(Mom.Quart1), Quart3(Mom.Quart3),
	DecileV(Mom.DecileV), PercentileV(Mom.PercentileV) {}
	static PMom New() {
		return PMom(new TMom());
	}
	static void NewV(TMomV& MomV, const int& Moms) {
		MomV.Gen(Moms);
		for (int MomN = 0; MomN < Moms; MomN++) {
			MomV[MomN] = New();
		}
	}
	static void NewVV(TVVec<PMom>& MomVV, const int& XMoms, const int& YMoms) {
		MomVV.Gen(XMoms, YMoms);
		for (int XMomN = 0; XMomN < XMoms; XMomN++) {
			for (int YMomN = 0; YMomN < YMoms; YMomN++) {
				MomVV.At(XMomN, YMomN) = New();
			}
		}
	}
	TMom(const TFltV& _ValV);
	static PMom New(const TFltV& ValV) {
		return PMom(new TMom(ValV));
	}
	TMom(TSIn& SIn):
	DefP(SIn),
	ValWgtV(SIn),
	SumW(SIn), ValSumW(SIn), Vals(SIn),
	UsableP(SIn), UnusableVal(SIn),
	Mn(SIn), Mx(SIn),
	Mean(SIn), Vari(SIn), SDev(SIn), SErr(SIn),
	Median(SIn), Quart1(SIn), Quart3(SIn),
	DecileV(SIn), PercentileV(SIn) {}
	static PMom Load(TSIn& SIn) {
		return new TMom(SIn);
	}
	void Save(TSOut& SOut) const {
		DefP.Save(SOut);
		ValWgtV.Save(SOut);
		SumW.Save(SOut);
		ValSumW.Save(SOut);
		Vals.Save(SOut);
		UsableP.Save(SOut);
		UnusableVal.Save(SOut);
		Mn.Save(SOut);
		Mx.Save(SOut);
		Mean.Save(SOut);
		Vari.Save(SOut);
		SDev.Save(SOut);
		SErr.Save(SOut);
		Median.Save(SOut);
		Quart1.Save(SOut);
		Quart3.Save(SOut);
		DecileV.Save(SOut);
		PercentileV.Save(SOut);
	}

	TMom& operator=(const TMom& Mom) {
		Assert(!DefP);
		DefP = Mom.DefP;
		ValWgtV = Mom.ValWgtV;
		SumW = Mom.SumW;
		ValSumW = Mom.ValSumW;
		Vals = Mom.Vals;
		UsableP = Mom.UsableP;
		UnusableVal = Mom.UnusableVal;
		Mn = Mom.Mn;
		Mx = Mom.Mx;
		Mean = Mom.Mean;
		Vari = Mom.Vari;
		SDev = Mom.SDev;
		SErr = Mom.SErr;
		Median = Mom.Median;
		Quart1 = Mom.Quart1;
		Quart3 = Mom.Quart3;
		DecileV = Mom.DecileV;
		PercentileV = Mom.PercentileV;
		return *this;
	}
	bool operator==(const TMom& Mom) const {
		return Vals == Mom.Vals;
	}
	bool operator<(const TMom& Mom) const {
		return Vals < Mom.Vals;
	}

	// define
	void Def();
	static void DefV(TMomV& MomV) {
		for (int MomN = 0; MomN < MomV.Len(); MomN++) {
			MomV[MomN]->Def();
		}
	}
	static void DefVV(TVVec<PMom>& MomVV) {
		for (int XMomN = 0; XMomN < MomVV.GetXDim(); XMomN++) {
			for (int YMomN = 0; YMomN < MomVV.GetYDim(); YMomN++) {
				MomVV.At(XMomN, YMomN)->Def();
			}
		}
	}
	bool IsDef() const {
		return DefP;
	}

	// values
	void Add(const TFlt& Val, const TFlt& Wgt = 1) {
		Assert(!DefP);
		ValWgtV.Add(TFltPr(Val, Wgt));
		SumW += Wgt;
		ValSumW += Wgt * Val;
		Vals++;
	}
	double GetWgt() const {
		return SumW;
	}
	int GetVals() const {
		return Vals;
	}
	TFlt GetVal(const int& ValN) const {
		IAssert(!IsDef());
		return ValWgtV[ValN].Val1;
	}
	//const TFltV& GetValV() const {IAssert(!IsDef()); return ValV;} //J:

	// usability
	bool IsUsable() const {
		Assert( DefP);
		return UsableP;
	}
	static bool IsUsableV(const TMomV& MomV) {
		for (int MomN = 0; MomN < MomV.Len(); MomN++) {
			if (!MomV[MomN]->IsUsable()) {
				return false;
			}
		}
		return true;
	}
	static bool IsUsableVV(const TVVec<PMom>& MomVV) {
		for (int XMomN = 0; XMomN < MomVV.GetXDim(); XMomN++) {
			for (int YMomN = 0; YMomN < MomVV.GetYDim(); YMomN++) {
				if (!MomVV.At(XMomN, YMomN)->IsUsable()) {
					return false;
				}
			}
		}
		return true;
	}

	// moments
	double GetMn() const {
		Assert(DefP && UsableP);
		return Mn;
	}
	double GetMx() const {
		Assert(DefP && UsableP);
		return Mx;
	}
	double GetExtent() const {
		Assert(DefP && UsableP);
		return Mx - Mn;
	}
	double GetMean() const {
		Assert(DefP && UsableP);
		return Mean;
	}
	double GetVari() const {
		Assert(DefP && UsableP);
		return Vari;
	}
	double GetSDev() const {
		Assert(DefP && UsableP);
		return SDev;
	}
	double GetSErr() const {
		Assert(DefP && UsableP);
		return SErr;
	}
	double GetMedian() const {
		Assert(DefP && UsableP);
		return Median;
	}
	double GetQuart1() const {
		Assert(DefP && UsableP);
		return Quart1;
	}
	double GetQuart3() const {
		Assert(DefP && UsableP);
		return Quart3;
	}
	double GetDecile(const int& DecileN) const {
		Assert(DefP && UsableP);
		return DecileV[DecileN];
	}
	double GetPercentile(const int& PercentileN) const {
		Assert(DefP && UsableP);
		return PercentileV[PercentileN];
	}
	double GetByNm(const TStr& MomNm) const;
	TStr GetStrByNm(const TStr& MomNm, char* FmtStr = NULL) const;

	// strings
	TStr GetStr(const char& SepCh = ' ', const char& DelimCh = ':', const bool& DecileP = true, const bool& PercentileP = true,	const TStr& FmtStr = "%g") const;
	static TStr GetNmVStr(const TStr& VarPfx, const char& SepCh = '\t',	const bool& DecileP = true, const bool& PercentileP = true);
	TStr GetValVStr(const char& SepCh = '\t', const bool& DecileP = true, const bool& PercentileP = true) const;

	friend class TPt<TMom>;
};
typedef TVVec<PMom> TMomVV;
typedef THash<TInt, PMom> TIntMomH;
typedef THash<TInt, TMomV> TIntMomVH;
typedef THash<TInt, TMomVV> TIntMomVVH;

/////////////////////////////////////////////////
// Correlation
class TCorr;
typedef TPt<TCorr> PCorr;
class TCorr{
private:
	TCRef CRef;
	int ValVLen;
	double CorrCf;
	double CorrCfPrb;
	double FisherZ;
public:
	TCorr() {}
	TCorr(const TFltV& ValV1, const TFltV& ValV2);
	static PCorr New(const TFltV& ValV1, const TFltV& ValV2) {return PCorr(new TCorr(ValV1, ValV2));}
	TCorr(TSIn&) {Fail;}
	static PCorr Load(TSIn& SIn) {return new TCorr(SIn);}
	void Save(TSOut&) {Fail;}
	TCorr& operator=(const TCorr&) {Fail; return *this;}
	double GetCorrCf() const {return CorrCf;}
	double GetCorrCfPrb() const {return CorrCfPrb;}
	TStr GetStr() const;

	friend class TPt<TCorr>;
};


/////////////////////////////////////////////////
// Statistical Tests
class TStatTest {
private:
	static void AveVar(const TFltV& ValV, double& Ave, double& Var);
	static double KsProb(const double& Alam);
public:
	static void ChiSquareOne(const TFltV& ObservedBinV, const TFltV& ExpectedBinV,
	double& ChiSquareVal, double& SignificancePrb);
	static void ChiSquareTwo(const TFltV& ObservedBin1V,
	const TFltV& ObservedBin2V, double& ChiSquareVal, double& SignificancePrb);
	static void TTest(const TFltV& ValV1, const TFltV& ValV2, double& TTestVal,
	double& TTestPrb);
	// Kolmogorov-Smirnov (are two distributions the same)
	static void KsTest(const TFltV& ValV1, const TFltV& ValV2, double& DStat,
	double& PVal);
	static void KsTest(const TFltPrV& ValCntV1, const TFltPrV& ValCntV2,
	double& DStat, double& PVal);
};

/////////////////////////////////////////////////
// Combinations
class TComb;
typedef TPt<TComb> PComb;
class TComb{
private:
	TCRef CRef;
public:
	int Items,Order,CombN;
	TIntV ItemV;
public:
	TComb(): Items(-1), Order(-1), CombN(-1), ItemV() {}
	TComb(const int& _Items, const int& _Order):
	Items(_Items), Order(_Order), CombN(0), ItemV() {
	IAssert((Order>0)&&(Order<=Items));}
	static PComb New(const int& Items, const int& Order) {return PComb(new TComb(Items, Order));}
	~TComb() {}
	TComb(TSIn&) {Fail;}
	static PComb Load(TSIn& SIn) {return new TComb(SIn);}
	void Save(TSOut&) {Fail;}
	TComb& operator=(const TComb&) {Fail; return *this;}
	bool GetNext();
	TIntV& GetItemV() {return ItemV;}
	int GetCombN() const {return CombN;}
	int GetCombs() const;
	void Wr();

	friend class TPt<TComb>;
};

/////////////////////////////////////////////////
// Linear-Regression
class TLinReg;
typedef TPt<TLinReg> PLinReg;
class TLinReg{
private:
	TCRef CRef;
public:
	TFltVV XVV;
	TFltV YV;
	TFltV SigV;
	int Recs, Vars;
	TFltVV CovarVV; // 1 based
	TFltV CfV; // 1 based
	double ChiSq;
public:
	void GetXV(const int RecN, TFltV& VarV) const {VarV.Gen(Vars + 1); for(int VarN = 0; VarN < Vars; VarN++) VarV[VarN + 1] = XVV.At(RecN - 1, VarN);}
	double GetY(const int RecN) const {return YV[RecN - 1];}
	double GetSig(const int RecN) const {return SigV[RecN - 1];}
	void NR_covsrt(TFltVV& CovarVV, const int& Vars, const TIntV& ia,
	const int& mfit);
	void NR_gaussj(TFltVV& a, const int& n, TFltVV& b, const int& m);
	void NR_lfit();
	TLinReg() {}
	static PLinReg New(const TFltVV& XVV, const TFltV& YV,
	const TFltV& SigV = TFltV());
	~TLinReg() {}
	TLinReg(TSIn&) {Fail;}
	static PLinReg Load(TSIn& SIn) {return new TLinReg(SIn);}
	void Save(TSOut&) {Fail;}
	TLinReg& operator=(const TLinReg&) {Fail; return *this;}
	int GetRecs() const {return Recs;}
	int GetVars() const {return Vars;}
	double GetCf(const int& VarN) const {return CfV[VarN + 1];}
	double GetCfUncer(const int& VarN) const {return sqrt(double(CovarVV.At(VarN + 1, VarN + 1)));}
	double GetCovar(const int& VarN1, const int& VarN2) const {return CovarVV.At(VarN1 + 1, VarN2 + 1);}
	double GetChiSq() const {return ChiSq;}
	static double LinInterp(const double& x1, const double& y1, const double& x2, const double& y2, const double& AtX)_CMPWARN {
		if (x1 == x2) return (y1+y2)/2.0;
		const double k = (y2 - y1) / (x2 - x1);
		return k*(AtX - x1) + y1;
	}
	void Wr() const;

	friend class TPt<TLinReg>;
};

/////////////////////////////////////////////////
// Singular-Value-Decomposition
class TSvd;
typedef TPt<TSvd> PSvd;
class TSvd{
private:
	TCRef CRef;
public:
	TFltVV XVV;
	TFltV YV, SigV;
	int Recs, Vars;
	TFltVV CovarVV; // 1 based
	TFltV CfV; // 1 based
	double ChiSq;
	void GetXV(const int RecN, TFltV& VarV) const {VarV.Gen(Vars + 1);for (int VarN = 0; VarN < Vars; VarN++) VarV[VarN + 1] = XVV.At(RecN - 1, VarN);}
	double GetY(const int RecN) const {return YV[RecN - 1];}
	double GetSig(const int RecN) const {return SigV[RecN - 1];}
	static double NR_SIGN(double a, double b) {return b >= 0.0 ? fabs(a) : -fabs(a);}
	static double NR_FMAX(double maxarg1, double maxarg2) {return maxarg1 > maxarg2 ? maxarg1 : maxarg2;}
	static int NR_IMIN(int iminarg1, int iminarg2) {return iminarg1 < iminarg2 ? iminarg1 : iminarg2;}
	static double NR_pythag(double a, double b);
	static void NR_svdcmp(TFltVV& a, int m, int n, TFltV& w, TFltVV& v);
	void NR_svbksb(TFltVV& u, TFltV& w, TFltVV& v, int m, int n, TFltV& b, TFltV& x);
	void NR_svdvar(TFltVV& v, int ma, TFltV& w, TFltVV& cvm);
	void NR_svdfit();
public:
	TSvd() {}
	static PSvd
	New(const TFltVV& XVV, const TFltV& YV, const TFltV& SigV = TFltV());
	~TSvd() {}
	TSvd(TSIn&) {Fail;}
	static PSvd Load(TSIn& SIn) {return new TSvd(SIn);}
	void Save(TSOut&) {Fail;}
	TSvd& operator=(const TSvd&) {Fail;return *this;}
	int GetRecs() const {return Recs;}
	int GetVars() const {return Vars;}
	double GetCf(const int& VarN) const {return CfV[VarN + 1];}
	double GetCfUncer(const int& VarN) const {return sqrt(double(CovarVV.At(VarN + 1, VarN + 1)));}
	double GetCovar(const int& VarN1, const int& VarN2) const {return CovarVV.At(VarN1 + 1, VarN2 + 1);}
	double GetChiSq() const {return ChiSq;}
	void GetCfV(TFltV& _CfV);
	void GetCfUncerV(TFltV& CfUncerV);
	static void Svd(const TFltVV& InMtx, TFltVV& LSingV, TFltV& SingValV,
	TFltVV& RSingV);
	static void Svd1Based(const TFltVV& InMtx1, TFltVV& LSingV, TFltV& SingValV,
	TFltVV& RSingV);
	void Wr() const;

	friend class TPt<TSvd>;
};

/////////////////////////////////////////////////
// Histogram
class THist {
private:
	TInt Vals;
	TInt BucketSize;
	TIntV BucketV;
public:
	THist() {}
	THist(const int& _BucketSize, const double& MxVal): BucketSize(_BucketSize), BucketV(int(ceil(MxVal/_BucketSize)+1)){}
	void Add(const double& Val) {
		const int BucketN = int(floor(Val/BucketSize));
		IAssert(BucketN >= 0);
		if (BucketN < BucketV.Len()) BucketV[BucketN]++;
		else BucketV.Last()++;
		Vals++;
	}
	void SaveStat(const TStr& ValNm, TSOut& FOut) const {
		FOut.PutStrLn("#"+ValNm+": "+Vals.GetStr());
		const int Buckets=BucketV.Len()-1;
		for(int BucketN=0; BucketN<Buckets; BucketN++) FOut.PutStrLn(TStr::Fmt("%d-%d\t%d", BucketSize*BucketN, BucketSize*(BucketN+1), BucketV[BucketN]()));
		if(BucketV.Last()>0) FOut.PutStrLn(TStr::Fmt("%d-\t%d", BucketSize*Buckets, BucketV.Last()()));
	}
	void SaveTxt(const TStr& ValNm, const TStr& FNm) const {
		TFOut FOut(FNm);
		SaveStat(ValNm, FOut);
	}
};


/////////////////////////////////////////////////
// Sparse matrix
template<class TVal>
class TSparseMatrix{
public:
	typedef TTriple<int, int, TVal> Tr;
	typedef TVec<Tr> Vec;
	typedef typename Vec::TIter TIter;

	class TCmpR{// comparator on row
	public:
		bool operator()(const Tr& x, const Tr& y) const {
			if (x.Val1<y.Val1 || (x.Val1==y.Val1 && x.Val2<y.Val2)) return true;
			else return false;
		}
	};
	class TCmpC{// comparator on col
		public:
			int operator()(const Tr& x, const Tr& y) const {
				if (x.Val2<y.Val2 || (x.Val2==y.Val2 && x.Val1<y.Val1)) return -1;
				else if (x.Val2>y.Val2 || (x.Val2==y.Val2 && x.Val1>y.Val1)) return 1;
				else return 0;
			}
		};
private:
	TCmpR cmpr;
	TCmpC cmpc;
	const TVal zero;
	Vec dat;
	TIntH rowIndex, colIndex; // row or col No. ---> position in dat
	int NR,NC;
public:
	TSparseMatrix(){NR=NC=0;}
	int Len() const {return dat.Len();}
	bool Empty() const{return dat.Len()==0;}
	void Clear(){dat.Clr(); rowIndex.Clr(); colIndex.Clr(); NR=NC=0;}
	const Tr& operator[](const int k) const {
		Assert(0<=k && k<Len());
		return dat[k];
	}
	Tr& operator[](const int k) {
		Assert(0<=k && k<Len());
		return dat[k];
	}
	const TVal& operator()(const int i, const int j) const {
		Assert(0<=i && i<=NR && 0<=j && j<=NC);
		if(!rowIndex.IsKey(i)) return zero;
		int k=rowIndex(i), L=Len();
		while(k<L && i==dat[k].Val1) {
			if(j==dat[k].Val2) return dat[k].Val3;
			k++;
		}
		return zero;
	}
	TVal& operator()(const int i, const int j) {
		Assert(0<=i && i<=NR && 0<=j && j<=NC);
		if(!rowIndex.IsKey(i)) return zero;
		int k=rowIndex(i), L=Len();
		while(k<L && i==dat[k].Val1) {
			if(j==dat[k].Val2) return dat[k].Val3;
			k++;
		}
		return zero;
	}
	void Add(int i, int j, const TVal& val){
		dat.Add(Tr(i, j, val));
		if(i>=NR) NR=i+1;
		if(j>=NC) NC=j+1;
	}
	void Sort(bool byRow=true){
		AssertR(Len()!=0, TStr("Matrix is empty!"));
		if(byRow) dat.SortCmp(cmpr);
		else dat.SortCmp(cmpc);
	}
	bool IsSorted(bool byRow=true){
		if(byRow) return dat.IsSortedCmp(cmpr);
		else return dat.IsSortedCmp(cmpc);
	}
 	void Index(bool byRow=true){
		AssertR(Len()!=0, TStr("Matrix is empty!"));
		for(int i=0;i<Len();i++){
			int row=dat[i].Val1;
			if(!rowIndex.IsKey(row)) rowIndex(row)=i;
		}
		rowIndex.SortByKey();
	}
	const TVal& GetVal(const int i, const int j) const {return operator()(i,j);}
	TVal& GetVal(const int i, const int j) {return operator()(i,j);}
	void GetRow(const int i, TIntV& datIds){
		AssertR(Len()!=0, TStr("Matrix is empty!"));
		datIds.Clr(false);
		if(!rowIndex.IsKey(i)) return;
		int k=rowIndex(i), L=Len();
		while(k<L && i==dat[k].Val1) datIds.Add(k++);
	}
	void GetRowI(int row, TIter& begI, TIter& endI) const{
		begI=endI=NULL;
		AssertR(Len()!=0, TStr("Matrix is empty!"));
		if(!rowIndex.IsKey(row)) return;
		begI=dat.GetI(rowIndex.GetDat(row));
		int keyId=rowIndex.GetKeyId(row);
		if(keyId<rowIndex.Len()-1) endI=dat.GetI(rowIndex[keyId+1]);
		else endI=dat.EndI();
	}
	void RowNormal(){
		Assert(Len()!=0);
		double sum=0;
		int si=0, ei=0, lr=-1;
		for(int i=0; i<Len(); i++){
			if(dat[i].Val1!=lr){
				if(sum>1E-9) for(int j=si; j<ei; j++) dat[j].Val3/=sum;
				lr=dat[i].Val1;
				si=ei=i;
				sum=0;
			}
			ei++;
			sum+=dat[i].Val3;
		}
		for(int j=si; j<ei; j++) dat[j].Val3/=sum;
	}
	void Format(){Sort(); Index(); RowNormal();}
	void Print(int top=-1){
		if(top==-1)	for(int k=0; k<dat.Len(); k++)	printf("(%d, %d)	%.9f\n", dat[k].Val1, dat[k].Val2, dat[k].Val3.Val);
		else for(int k=0; k<min(top, dat.Len()); k++) printf("(%d, %d)	%.9f\n", dat[k].Val1, dat[k].Val2, dat[k].Val3.Val);
	}
	void Info(){
		printf("Row: %d, Col: %d, Items: %d, Sorted:%d\n", NR, NC, Len(), IsSorted());
	}
};

typedef TSparseMatrix<TFlt> SparseMatrix;
typedef TSparseMatrix<TInt> SparseMatrixInt;
typedef TSparseMatrix<TFltV> SparseMatrixFltV;
