//============================================================================
// Name        : test.cpp
// Author      : jzzhao
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include "../snap/Snap.h"

class Node{
public:
	int uid;
	double reward;
	bool flag;
public:
	Node(){}
	Node(int u, bool f, double r){
		uid=u;	flag=f;  reward=r;
	}
	bool operator<(const Node& n) const{
		return reward<n.reward;
	}
	bool operator>(const Node& n) const{
		return reward>n.reward;
	}
	Node& operator=(const Node& n){
		uid=n.uid; flag=n.flag; reward=n.reward;
		return *this;
	}
};

void test_node_data_net(){
	TIntNNet net;
	net.AddNode(1);
	net.AddNode(2);
	net.AddEdge(1, 2);
	net.AddEdge(2, 1);
	net.AddEdge(1, 1);
	net.AddEdge(1, 1);

	for(TIntNNet::TEdgeI ei=net.BegEI();ei<net.EndEI();ei++){
		printf("%d-->%d\n", ei.GetSrcNId(), ei.GetDstNId());
	}
	for(TIntNNet::TNodeI ni=net.BegNI();ni<net.EndNI();ni++){
		printf("Deg:%d, InDeg:%d, OutDeg:%d\n", ni.GetDeg(), ni.GetInDeg(), ni.GetOutDeg());
	}
}

void test_tm(){
//	TExeTm2 tm;
//	tm.Tick();
////	test_node_data_net();
//	printf("tm=%.2f\n", tm.GetSecs());
//	for(int k=0; k<100; k++){
//		int i=0;
//		while(i++<1E9);
//		printf("%.2f\n", double(clock()));
//	}

//	TSecTm tm(2010, 1, 1);
//	printf("%d\n", tm.GetAbsSecs());

//	TTm tm=TTm::GetTmFromWebLogDateTimeStr("2012-2-1 23:34:12");
	TSecTm tm=TSecTm::GetCurTm();
	printf("%s\n%s\n%s\n", tm.GetDtYmdStr().CStr(), tm.GetDtTmSortStr().CStr(), tm.GetDtTmSortFNmStr().CStr());
	sleep(5);
	tm=TSecTm::GetCurTm();
	printf("%s\n%s\n%s\n", tm.GetDtYmdStr().CStr(), tm.GetDtTmSortStr().CStr(), tm.GetDtTmSortFNmStr().CStr());

//	TExeTm2 tm;
//	for(int i=0; i<10; i++){
//		sleep(5);
//		printf("%d\t%.4f, %lu\n", i, tm.GetSecs(), clock());
//	}
}

void test_ref(){
	Node* a=new Node(1,true,12.2);
	Node &b=*a;
	Node c=b;
	printf("%d,%d\n", c.uid, c.flag);
}

void test_rnd(){
//	TSampler sampler;
//	TFltV dat(5);
//	for(int i=0;i<5;i++) dat[i]=0;
	TIntSet samples, rst;
	for(int i=0;i<1;i++) samples.AddKey(i);
	TRandom::Choose(samples, rst, 1);
	for(int i=0; i<rst.Len(); i++) printf("%d ", rst.GetKey(i).Val);
	printf("\n");
	TInt tmp;
	int x=TRandom::Sample(samples, tmp);
	printf("%d\n", x);
}

class MyCMP{
public:
	bool operator()(const TInt& x, const TInt& y) const{
		if(x<y) return true;
		else return false;
	}
};

void testCmp(){
	TIntV x(10);
	TRnd rnd;
	MyCMP cmp;
	printf("%d\n", cmp(1,2));
	for(int i=0;i<x.Len();i++) x[i]=rnd.GetUniDevInt(100);
	x.SortCmp(cmp);
	for(int i=0;i<x.Len();i++) printf("%d, ", x[i].Val);
	printf("\nsorted: %d\n", x.IsSortedCmp<MyCMP>(cmp));
	if(0) printf("%d\n", 0);
	if(1) printf("%d\n", 1);
	if(-1) printf("%d\n", -1);
}

void test_hash(){
	TIntH hash;
	for(int i=0;i<10;i++) hash.AddDat(i,i);// hash(i)=i;
	for(int i=0; i<hash.Len(); i++) printf("%d -> %d\n", hash.GetKey(i).Val, hash[i].Val);
//	hash.DelKey(2);
//	for(int i=0;i<9; i++) printf("%d\n", hash[i].Val);
//	for(TIntH::TIter it=hash.BegI(); it<hash.EndI(); it++) printf("%d, %d\n", it.GetKey().Val, it.GetDat().Val);
}

void test_orth_lst(){
	typedef TOLstNd<Node>* POLstNd;
	typedef TOLstHdr<Node>* POLstHdr;

	TOLst<Node> lst(4,3);
	Node n1(1,false,1);
	Node n2(1,false,2);
	Node n3(1,false,0.5);
	lst.AddBack(1,1,n1);
	lst.AddBack(1,2,n2);
	lst.AddBack(1,0,n3);
	lst.AddBack(0,2,n1);
	lst.AddBack(0,0,n3);
	lst.AddBack(0,1,n3);
	lst.AddBack(3,1,n1);
	lst.AddBack(2,2,n2);

	printf("L=%d\n", lst.Len());
	for(int i=0;i<lst.LenH();i++) {
		POLstHdr hlist=lst.GetHLst(i);
		printf("LH=%d\n\t", hlist->Nds);
		POLstNd p=hlist->FirstNd;
		while(p!=NULL){
			printf("r=%.2f  ",p->Val.reward);
			p=p->HNextNd;
		}
		printf("\n");
	}
	printf("\n");
	for(int i=0;i<lst.LenV();i++) {
		POLstHdr vlist=lst.GetVLst(i);
		printf("LV=%d\n\t", vlist->Nds);
		POLstNd p=vlist->FirstNd;
		while(p!=NULL){
			printf("r=%.2f  ",p->Val.reward);
			p=p->VNextNd;
		}
		printf("\n");
	}
}

void test_bignet(){
	TBigNet<TBool, true> net(3, 2);
	net.AddNode(1, 0, 2);
	net.AddNode(2, 1, 0);
	net.AddNode(3, 1, 0);
	printf("Adding edges ...\n");
	net.AddEdge(1, 2);
	net.AddEdge(1, 3);
	printf("nodes: %d, edges: %ld\n", net.GetNodes(), net.GetEdges());
}
void test_bignet1(){
	TStr root="/data/SNSDATA/G+/gplus/imc12/";
	TIntPrH nodeH; int nedge =0;
	TSsParser ss(root+"gp_follow.digraph.gz");
	while(ss.Next()){
		int fid = ss.GetInt(0), tid = ss.GetInt(1);
		if(nodeH.IsKey(fid)) nodeH(fid).Val2++;	else nodeH(fid)=TIntPr(0, 1);
		if(nodeH.IsKey(tid)) nodeH(tid).Val1++;	else nodeH(tid)=TIntPr(1, 0);
		nedge++;
	}
	FILE* fw = fopen((root+"node_deg.dat").CStr(), "w");
	fprintf(fw, "%d\t%d\n", nodeH.Len(), nedge);
	for(int i=0; i<nodeH.Len(); i++) fprintf(fw, "%d\t%d\t%d\n", nodeH.GetKey(i).Val, nodeH[i].Val1.Val, nodeH[i].Val2.Val);
	fclose(fw);
	return;
}

void test_bignet2(){
	TStr root="/data/SNSDATA/G+/gplus/imc12/";
	TSsParser ss(root+"node_deg.dat.gz");
	ss.Next();
	int nodes = ss.GetInt(0), edges = ss.GetInt(1);
	printf("nodes: %d, edges: %d\nAdding nodes ...\n", nodes, edges);
	TBigNet<TBool, true> net(nodes, edges);
	while(ss.Next()) {
		int nid = ss.GetInt(0), ideg = ss.GetInt(1), odeg = ss.GetInt(2);
		net.AddNode(nid, ideg, odeg);
	}
	printf("Adding edges ...\n");
	TSsParser ess(root+"gp_follow.digraph.gz");
	while(ess.Next()){
		int fid = ess.GetInt(0), tid = ess.GetInt(1);
		net.AddEdge(fid, tid);
	}
	printf("nodes: %d, edges: %ld.\n", net.GetNodes(), net.GetEdges());
}

void test_sm(){
	SparseMatrix sm;
	sm.Add(0,2,1);
	sm.Add(1,3,3);
	sm.Add(2,1,4);
	sm.Add(1,0,2);
	sm.Add(0,1,1);
	sm.Sort();
	sm.Index();

//	sm.Print();
//	sm.Clear();
//	printf("After L=%d\n", sm.Len());
	sm.Print();


//	sm.Print();
	sm.RowNormal();
	sm.Print();
//	sm.Info();
//	TIntV datIds;
//	for(int r=0; r<sm.NR; r++) {
//		sm.GetRow(r, datIds);
//		double sum=0;
//		for(int j=0; j<datIds.Len(); j++) sum+=sm[datIds[j]].Val3;
//		printf("row: %d sum: %.4f\n", r, sum);
//	}
//	sm.GetRow(2, datIds);

//	SparseMatrix::TIter begI, endI;
//	sm.GetRowI(3, begI, endI);
//	for(; begI<endI; begI++) printf("%f\n", begI->Val3.Val);
//
//	TIntFltH sum;
//	sm.Sum(sum);
//	for(int i=0; i<sum.Len(); i++) printf("%d: %.2f\n", i, sum[i].Val);
}

void test_dict_pair(){
	TIntIntPrH dat;
	dat(1)=TIntPr(1,1);
	dat(2)=TIntPr(2,1);
	dat(3)=TIntPr(3,1);
	dat(4)=TIntPr(4,1);
	dat.SortByDat(false);
	for(int i=0; i<dat.Len(); i++) printf("%d[%d] -> (%d, %d)\n", dat.GetKey(i).Val, dat.GetKeyId(dat.GetKey(i)), dat[i].Val1.Val, dat[i].Val2.Val);
}

void test_zip(){
//	TStr fnm="/media/e/direct_social_structure.txt.bz2", line;
//	TZipIn zin(fnm);
//	printf("CMD: %s\nLen=%ld\n", zin.GetCmd(fnm).CStr(),TZipIn::GetFLen(fnm));
//	return;
//	while(!zin.GetNextLn(line)){
//		printf("%s\n", line.CStr());
//	}

//	TStr gpgraph="/data/SNSDATA/G+/gplus/imc12/gp_follow.digraph";
//	TStr gpgraph="/media/e/gp_follow.digraph.bz2";
//	TStr gpgraph="/media/e/direct_social_structure.txt.bz2";
//	TExeTm tm;
//	tm.Tick();
//	TSsParser ss(gpgraph);
//	printf("\nstart reading...\n");
//	int n=0, fid, tid, s;
//	while(ss.Next()){
////		if(ss.GetInt(0, fid) && ss.GetInt(1, tid)) n+=1;
////		else printf("Error: %ld\n", ss.GetLineNo());
//		if((n++)%1000000==0) printf("%d\n", n);
//	}
//	printf("n=%d, time=%.2fmins\n", n, tm.GetSecs()/60);

	TStr Fnm("t.txt.gz");
	PSOut FOutPt = TZipOut::IsZipFNm(Fnm) ? TZipOut::New(Fnm) : TFOut::New(Fnm);
	FOutPt->PutStrLn("hello");
	FOutPt->PutStrLn("hello again");
}

void test_lst(){
	typedef TLst<int>::PLstNd PNd;
	TLst<int> Queue;
	for(int i=0; i<5; i++) Queue.AddBackSorted(i,false);
	Queue.Pop();
	Queue.AddBackSorted(3, false);
	PNd ptr=Queue.First();
	while(ptr!=NULL){
		printf("%d\n", ptr->Val);
		ptr=ptr->Next();
	}
	ptr=Queue.Last();
	while(ptr!=NULL){
		printf("%d\n", ptr->Val);
		ptr=ptr->Prev();
	}
}

void fun(TIntV& vec){
	vec[0]=12;
}

void test_c11(){
	TIntV vecs[2];
	vecs[0].Gen(2);
	fun(vecs[0]);
	auto f = [&vecs] {return 10;};
	printf("%d\n%d\n", vecs[0][0].Val, f());
}

void test_fnm(){
	TStr fnm = "/home/jzzhao/git_project/netsnap/snap/Snap.h";
	printf("Base: %s\nPath: %s\nMid: %s\n", fnm.GetFBase().CStr(), fnm.GetFPath().CStr(), fnm.GetFMid().CStr());
}

void test_binom(){
	int i=10000;
	double p=0.1;
	int m = (int)(i*p);
	for (int j=m-100;j<=m+100;j++){
		double bij = TSpecFunc::Binomial(j, i, p);;
		printf("%d: %.6e  ", j, bij);
		if ((j+1)%5==0) printf("\n");
		Assert(bij>=0);
	}
}

const TStr GetFNm(const TStr& GFNm) {
	return GFNm.GetFPath();
}

int main(int argc, char* argv[]) {
//	test_c11();
//	test_fnm();
	test_binom();
//	test_zip();
	return 0;
}

