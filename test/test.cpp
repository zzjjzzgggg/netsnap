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

void test(){
//	double x=-0.1234;
	TIntV nodes(10);
	printf("%d\n", nodes.Len());
	nodes.Clr(false);

	printf("%d\n", nodes.Len());
}

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
	for(int i=0;i<10;i++) hash(i)=i;
//	hash.DelKey(2);
//	for(int i=0;i<9; i++) printf("%d\n", hash[i].Val);
//	for(TIntH::TIter it=hash.BegI(); it<hash.EndI(); it++) printf("%d, %d\n", it.GetKey().Val, it.GetDat().Val);
}

void testSet(){
	TIntSet set;
	for(int i=0;i<10;i++) set.AddKey(i);
	for(int i=0; i<5; i++) printf("%d\n", set.GetRndKey(TInt::Rnd).Val);
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

void test_graph(){
	PUNGraph G=PUNGraph::New();
	G->AddNode(1);
	G->AddNode(2);
	G->AddNode(3);
	G->AddNode(4);
	G->AddEdge(2, 1);
	G->AddEdge(3, 1);
	G->AddEdge(1, 3);
	TUNGraph::TNodeI ni=G->GetNI(1);
	printf("Nodes: %d, edges: %d, in-deg: %d, out-deg: %d, deg: %d\n",
			G->GetNodes(), G->GetEdges(), ni.GetInDeg(), ni.GetOutDeg(), ni.GetDeg());
//	int hops = TSnap::GetShortPath(G, 3, 4, false, 10);
//	printf("hops: %d\n", hops);
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

void count_tri(){
	PNGraph Graph=TSnap::LoadEdgeList<PNGraph>("/home/jzzhao/workspace/wiki-Vote.txt");
	int open, closed;
	TSnap::GetTriads(Graph, closed, open, -1);
	printf("%d\t%d\n", closed, open);
}

void test_fun(){
	printf("v=%.6f\n", TSpecFunc::Normal(0));
}

void test_avg(){
	TAvg avg;
	TFltV item1, item2, item3;
	item1.Add(1); item1.Add(2); item1.Add(3);
	item2.Add(1); item2.Add(2); item2.Add(3);
	item3.Add(1); item3.Add(2); item3.Add(3);
	avg.Add(1, item1);
	avg.Add(1, item2);
	avg.Add(1, item3);
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
	TStr gpgraph="/media/e/direct_social_structure.txt.bz2";
	TExeTm tm;
	tm.Tick();
	TSsParser ss(gpgraph);
	printf("\nstart reading...\n");
	int n=0, fid, tid, s;
	while(ss.Next()){
//		if(ss.GetInt(0, fid) && ss.GetInt(1, tid)) n+=1;
//		else printf("Error: %ld\n", ss.GetLineNo());
		if((n++)%1000000==0) printf("%d\n", n);
	}
	printf("n=%d, time=%.2fmins\n", n, tm.GetSecs()/60);
}

void test_katz(){
	PUNGraph Graph=TUNGraph::New();
	Graph->AddNode(1);Graph->AddNode(2);Graph->AddNode(3);Graph->AddNode(4);
	Graph->AddEdge(1,2);Graph->AddEdge(1,3);Graph->AddEdge(1,4); Graph->AddEdge(3,4);
	TIntFltH katzH;
	TSnap::GetKatzCentr(Graph, katzH, 0.3);
	for(int i=0; i<katzH.Len(); i++) printf("%d: %.4f\n", katzH.GetKey(i).Val, katzH[i].Val);
}

void test_gio(){
	PUNGraph G=TSnap::LoadEdgeList<PUNGraph>(TStr("/data/graphdata/wiki_vote.graph.bz2"));
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

void test_file(){
	FILE* fw=fopen("t.txt", "a");
	fprintf(fw, "asdf\n");
	fclose(fw);
}

void test_os(){
	RemoveFile("t.txt");
//	TStrV fnms;
//	ListDir(".", fnms);
//	for(int i=0; i<fnms.Len(); i++) printf("%s\n", fnms[i].CStr());

//	FILE* fr=open("tt.txt", "r");
}

void test_ptr(){
	TVec<uint64> BitV;
	int NNodes=5189809;
	int NBits = int(ceil(TMath::Log2(NNodes)))+8;
	int BytesPerNd = 32*NBits;
	BitV.Gen(NNodes*BytesPerNd/sizeof(uint64)+1); IAssert(BitV.BegI() != NULL);
	uchar* BitVPt = (uchar*) BitV.BegI();
	for(int i=0; i<BitV.Len()*sizeof(uint64);i++) *(BitVPt+i)=i;
	printf("%lu, %d, %lu\n", BitV.Len()*sizeof(uint64), BitV.Len(), sizeof(uint64));
//	*(BitVPt+853324352) = 0;
}

void gen_graphs(){
//	PUNGraph G = TSnap::GenRndGnm<PUNGraph>(100000, 1000000);
//	TSnap::SaveEdgeList<PUNGraph>(G, "ER_100K_1M.graph");

	PUNGraph G = TSnap::GenPrefAttach(100000, 10);
	TSnap::SaveEdgeList<PUNGraph>(G, "BA_100K_10.graph");
}

int main(void) {
	gen_graphs();
//	test();
//	test_tm();
//	testCmp();
//	test_rnd();
//	test_hash();
//	test_orth_lst();
//	test_graph();
//	test_sm();
//	test_bio();
//	test_dict_pair();
//	count_tri();
//	test_fun();
//	test_avg();
//	test_bignet();
//	test_zip();
//	test_katz();
//	test_gio();
//	testSet();
//	test_lst();
//	test_file();
//	test_os();
//	test_ptr();
	return 0;
}

