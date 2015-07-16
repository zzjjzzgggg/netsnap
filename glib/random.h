/////////////////////////////////////////////////
// Random (added by zhao)
class TSampler{
public:
	static int sample(const TFltV &list){
		Assert(!list.Empty());
		double sum=0, esp=1E-6;
		for(int i=0;i<list.Len();i++) sum+=list[i]+esp;
		double th=TInt::Rnd.GetUniDev()*sum;
		int id=0;
		sum=list[0]+esp;
		while(sum<th) sum+=list[++id]+esp;
		return id;
	}
};

namespace TRandom{

template<class TKey> TKey& Sample(const TVec<TKey>& samples, TKey& rst);
template<class TKey> TKey& Sample(const THashSet<TKey>& samples, TKey& rst);
template<class TKey> TKey& Sample(const THashSet<TKey, TFlt>& samples, TKey& rst);
template<class TKey> TKey& Sample(const THashSet<TKey, TInt>& samples, TKey& rst);
// choose without replacement from samples
template<class TKey> void Choose(const TVec<TKey>& samples, TVec<TKey>& rst, const int n);
template<class TKey> void Choose(const THashSet<TKey>& samples, THashSet<TKey>& rst, const int n);

// initialize uniform distribution of dist in range [pos_start:]
void InitUni(TFltV& dist, const int pos_start=0);
void InitPowerLaw(TFltV& dist, const double alpha, const int pos_start=0);

// Uniformly sample an item from samples
template<class TKey>
TKey& Sample(const TVec<TKey>& samples, TKey& rst){
	AssertR(samples.Len()!=0, TStr::Fmt("Empty samples cannot be sampled!"));
	rst=samples[TInt::Rnd.GetUniDevInt(samples.Len())];
	return rst;
}

// Uniformly sample an item from samples
template<class TKey>
TKey& Sample(const THashSet<TKey>& samples, TKey& rst){
	AssertR(samples.Len()!=0, TStr::Fmt("Empty samples cannot be sampled!"));
	rst=samples[TInt::Rnd.GetUniDevInt(samples.Len())];
	return rst;
}

// Sample an item from samples with probability proporation to its value
template<class TKey>
TKey& Sample(const THash<TKey, TFlt>& samples, TKey& rst){
	AssertR(!samples.Empty(), TStr::Fmt("I cannot sample from an empty hashmap."));
	double sum=0, esp=1E-6;
	TIntFltH::TIter it;
	for(it=samples.BegI(); it<samples.EndI(); it++) sum+=it.GetDat()+esp;
	double th=TInt::Rnd.GetUniDev()*sum;
	it=samples.BegI();
	sum=it.GetDat()+esp;
	while(sum<th) {
		it++;
		sum+=it.GetDat()+esp;
	}
	rst = it.GetKey();
	return rst;
}

// Sample an item from samples with probability proporation to its value
template<class TKey>
TKey& Sample(const THash<TKey, TInt>& samples, TKey& rst){
	AssertR(!samples.Empty(), TStr::Fmt("I cannot sample from an empty hashmap."));
	double sum=0, esp=1E-6;
	TIntIntH::TIter it;
	for(it=samples.BegI(); it<samples.EndI(); it++) sum+=it.GetDat()+esp;
	double th=TInt::Rnd.GetUniDev()*sum;
	it=samples.BegI();
	sum=it.GetDat()+esp;
	while(sum<th) {
		it++;
		sum+=it.GetDat()+esp;
	}
	rst = it.GetKey();
	return rst;
}

template<class TKey>
void ChooseWithReplacement(const TVec<TKey>& samples, TVec<TKey>& rst, const int n){
	int L=samples.Len();
	rst.Gen(n);
	for(int i=0; i<n; i++){
		rst[i] = samples[TInt::Rnd.GetUniDevInt(0, L-1)];
	}
}

template<class TKey>
void Choose(const TVec<TKey>& samples, TVec<TKey>& rst, const int n){
	int L=samples.Len();
	AssertR(n<=L, TStr::Fmt("%d > sample length %d!", n, L));
	rst.Clr();
	TIntH map(n);
	for(int i=0; i<n; i++){
		int r = TInt::Rnd.GetUniDevInt(i, L-1);
		rst.Add(samples[map.IsKey(r) ? map(r).Val : r]);
		if(r!=i) map(r) = map.IsKey(i) ? map(i).Val : i;
	}
}

template<class TKey>
void Choose(const THashSet<TKey>& samples, THashSet<TKey>& rst, const int n){
	int L=samples.Len();
	AssertR(n<=L, TStr::Fmt("%d > sample length %d!", n, L));
	rst.Clr();
	TIntH map(n);
	for(int i=0; i<n; i++){
		int r=TInt::Rnd.GetUniDevInt(i, L-1);
		rst.AddKey(samples[map.IsKey(r)?map(r).Val:r]);
		if(r!=i) map(r)=map.IsKey(i)?map(i).Val:i;
	}
}


}
