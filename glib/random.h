/////////////////////////////////////////////////
// Random (added by jzzhao)
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
template<class TKey> void Choose(const TVec<TKey>& samples, TVec<TKey>& rst, const int n);
template<class TKey> void Choose(const THashSet<TKey>& samples, THashSet<TKey>& rst, const int n);



template<class TKey>
TKey& Sample(const TVec<TKey>& samples, TKey& rst){
	AssertR(samples.Len()!=0, TStr::Fmt("Empty samples cannot be sampled!"));
	rst=samples[TInt::Rnd.GetUniDevInt(samples.Len())];
	return rst;
}

template<class TKey>
TKey& Sample(const THashSet<TKey>& samples, TKey& rst){
	AssertR(samples.Len()!=0, TStr::Fmt("Empty samples cannot be sampled!"));
	rst=samples[TInt::Rnd.GetUniDevInt(samples.Len())];
	return rst;
}

template<class TKey>
TKey& Sample(const THash<TKey, TFlt>& samples, TKey& rst){
	AssertR(!samples.Empty(), TStr::Fmt("I cannot sample from an empty set."));
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

template<class TKey>
void Choose(const TVec<TKey>& samples, TVec<TKey>& rst, const int n){
	int L=samples.Len();
	AssertR(n<=L, TStr::Fmt("%d > sample length %d!", n, L));
	rst.Clr();
	TIntH map(n);
	for(int i=0; i<n; i++){
		int r=TInt::Rnd.GetUniDevInt(i, L-1);
		rst.Add(samples[map.IsKey(r)?map(r).Val:r]);
		if(r!=i) map(r)=map.IsKey(i)?map(i).Val:i;
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

