
#include <stdio.h>
#include <stdlib.h>
#include "../../snap/Snap.h"

typedef TNodeEDatNet<TInt, TFlt> TCasdGraph;
typedef TPt<TCasdGraph> PCasdGraph;

PCasdGraph loadGraph(TStr fname, bool rowNorm=true){
	printf("Loading graph with flag=%d ...\n", rowNorm);
	TSsParser Ss(fname, ssfWhiteSep, true, true, true);
	PCasdGraph Graph = TCasdGraph::New();
	int SrcNId, DstNId, dat1, dat2;
	double weight;
	while (Ss.Next()) {
		if (!Ss.GetInt(0, SrcNId)||!Ss.GetInt(1, DstNId)||!Ss.GetInt(2, dat1)||!Ss.GetInt(3, dat2)) {
			printf("%s\n",Ss.DumpStr());
			continue;
		}
		Graph->AddNode(SrcNId);
		Graph->AddNode(DstNId);
		Graph->AddEdge(SrcNId, DstNId, dat2/(dat1+1E-6));
	}

	//normalize edge weight
	double sum;
	for(TCasdGraph::TNodeI ni = Graph->BegNI(); ni < Graph->EndNI(); ni++) {
		int nid=ni.GetId();
		sum=0;
		if(rowNorm){
			for(int e=0; e<ni.GetOutDeg(); e++) sum+=ni.GetOutEDat(e).Val;
			for(int e=0; e<ni.GetOutDeg(); e++) ni.GetOutEDat(e).Val/=sum;
		}else{
			for(int e=0; e<ni.GetInDeg(); e++) sum+=Graph->GetEDat(ni.GetInNId(e), nid);
			for(int e=0; e<ni.GetInDeg(); e++) Graph->GetEDat(ni.GetInNId(e), nid)/=sum;
		}
	}
	Graph->Defrag();
	printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
	return Graph;
}

void save(PCasdGraph casdGraph, TStr fname){
	printf("Saving...\n");
	FILE *F = fopen(fname.CStr(), "wt");
	fprintf(F, "# Cascade Trace Graph\n");
	fprintf(F, "# Nodes: %d Edges: %d\n", casdGraph->GetNodes(), casdGraph->GetEdges());
	fprintf(F, "# FId\tTId\tWeight\n");
	for (TCasdGraph::TEdgeI ei = casdGraph->BegEI(); ei < casdGraph->EndEI(); ei++)
		fprintf(F, "%d\t%d\t%.6f\n", ei.GetSrcNId(), ei.GetDstNId(), ei.GetDat().Val);
	fclose(F);
}

template<class PGraph>
void influenceRank(const PGraph& Graph, TIntFltH& PRankH, const double& C=0.85, const double& Eps=1E-6, const int& MaxIter=200) {
	const int NNodes = Graph->GetNodes();
	const double OneOver = 1.0/double(NNodes);
	PRankH.Gen(NNodes);
	for (typename PGraph::TObj::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
		PRankH.AddDat(NI.GetId(), 1.0/NNodes);
	}
	TFltV TmpV(NNodes);
	for (int iter = 0; iter < MaxIter; iter++) {
		int i = 0;
		for (typename PGraph::TObj::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, i++) {
			TmpV[i] = 0;
			for (int e = 0; e < NI.GetOutDeg(); e++) {
				const int OutNId = NI.GetOutNId(e);
				const int InDeg = Graph->GetNI(OutNId).GetInDeg();
				if (InDeg > 0) TmpV[i] += PRankH.GetDat(OutNId)/InDeg;
			}
			TmpV[i] = C * TmpV[i];
		}
		double diff = 0, sum = 0, NewVal;
		for (i = 0; i < TmpV.Len(); i++) sum += TmpV[i];
		const double Leaked = (1.0 - sum)/double(NNodes);
		for (i = 0; i < PRankH.Len(); i++) {
			NewVal = TmpV[i] + Leaked;
			diff += fabs(NewVal - PRankH[i]);
			PRankH[i] = NewVal;
		}
		if (diff < Eps) {
			printf("Influence Rank Successed. Iteration number=%d\n", iter);
			break;
		}
	}
}

void pageRankWithWeights(PCasdGraph& Graph, TIntFltH& PRankH, const double& C=0.85, const double& Eps=1E-6, const int& MaxIter=100) {
	const int NNodes = Graph->GetNodes();
	const double OneOver = 1.0/double(NNodes);
	PRankH.Gen(NNodes);
	for(TCasdGraph::TNodeI ni = Graph->BegNI(); ni < Graph->EndNI(); ni++) 	PRankH.AddDat(ni.GetId(), 1.0/NNodes);

	TFltV TmpV(NNodes);
	for (int iter = 0; iter < MaxIter; iter++) {
		int j = 0;
		for (TCasdGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, j++) {
			int dstNId=NI.GetId();
			TmpV[j] = 0;
			for (int e = 0; e < NI.GetInDeg(); e++) {
				const int srcNId = NI.GetInNId(e);
				const double pij = Graph->GetEDat(srcNId, dstNId).Val;
				if (pij > 0) TmpV[j] += PRankH.GetDat(srcNId)*pij;
			}
			TmpV[j] = C * TmpV[j]; // Berkhin
		}
		double diff = 0, sum = 0, NewVal;
		for (int i = 0; i < TmpV.Len(); i++) sum += TmpV[i];
		const double Leaked = (1.0 - sum)/double(NNodes);
		for (int i = 0; i < PRankH.Len(); i++) { // re-instert leaked PageRank
			NewVal = TmpV[i] + Leaked; // Berkhin
			diff += fabs(NewVal - PRankH[i]);
			PRankH[i] = NewVal;
		}
		if (diff < Eps) {
			printf("PageRank Successed. Iteration number=%d\n", iter);
			break;
		}
	}
}

void influenceRankWithWeights(PCasdGraph& Graph, TIntFltH& PRankH, const double& C=0.85, const double& Eps=1E-6, const int& MaxIter=100) {
	const int NNodes = Graph->GetNodes();
	const double OneOver = 1.0/double(NNodes);
	PRankH.Gen(NNodes);
	for(TCasdGraph::TNodeI ni = Graph->BegNI(); ni < Graph->EndNI(); ni++) 	PRankH.AddDat(ni.GetId(), 1.0/NNodes);

	TFltV TmpV(NNodes);
	for (int iter = 0; iter < MaxIter; iter++) {
		int i = 0;
		for (TCasdGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++, i++) {
			int srcNId=NI.GetId();
			TmpV[i] = 0;
			for (int e = 0; e < NI.GetOutDeg(); e++) {
				int dstNId=NI.GetOutNId(e);
				const double pij = NI.GetOutEDat(e);
				if (pij > 0) TmpV[i] += PRankH.GetDat(dstNId)*pij;
			}
			TmpV[i] = C * TmpV[i]; // Berkhin
		}
		double diff = 0, sum = 0, NewVal;
		for (i = 0; i < TmpV.Len(); i++) sum += TmpV[i];
		const double Leaked = (1.0 - sum)/double(NNodes);
		for (i = 0; i < PRankH.Len(); i++) { // re-instert leaked PageRank
			NewVal = TmpV[i] + Leaked; // Berkhin
			diff += fabs(NewVal - PRankH[i]);
			PRankH[i] = NewVal;
		}
		if (diff < Eps) {
			printf("PageRank Successed. Iteration number=%d\n", iter);
			break;
		}
	}
}

void rankDigraph(const TStr InFNm, const TStr OutFNm){
	printf("Loading %s...\n", InFNm.CStr());
	PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
	printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
	TIntFltH IRankH, PRankH;
	printf("\tPageRank... \n");					TSnap::GetPageRank(Graph, PRankH);
	printf("\tInfluence Rank...\n");			influenceRank(Graph, IRankH);
	printf("DONE! saving...\n");

	FILE *F = fopen(OutFNm.CStr(), "wt");
	fprintf(F, "#Network: %s\n", InFNm.CStr());
	fprintf(F, "#Nodes: %d\tEdges: %d\n", Graph->GetNodes(), Graph->GetEdges());
	fprintf(F, "#NId\tInDeg\tOutDeg\tPageRank\tInfRank\n");
	for (TNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
		const int NId = NI.GetId();
		const int inDeg = Graph->GetNI(NId).GetInDeg();
		const int outDeg = Graph->GetNI(NId).GetOutDeg();
		const double pr = PRankH.GetDat(NId);
		const double ir = IRankH.GetDat(NId);
		fprintf(F, "%d\t%d\t%d\t%.9f\t%.9f\n", NId, inDeg, outDeg, pr, ir);
	}
	fclose(F);
	printf("\nrun time: (%s)\n", TSecTm::GetCurTm().GetTmStr().CStr());
}

void rankGraph(const TStr ifname, const TStr ofname){
	printf("Loading %s...\n", ifname.CStr());
	PUNGraph Graph = TSnap::LoadEdgeList<PUNGraph>(ifname);
	printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
	TIntFltH IRankH, PRankH, CcfH;
	printf("\tPageRank... \n");					TSnap::GetPageRank(Graph, PRankH);
	printf("\tInfluence Rank...\n");			influenceRank(Graph, IRankH);
	printf("DONE! saving...\n");

	FILE *F = fopen(ofname.CStr(), "wt");
	fprintf(F, "#Network: %s\n", ifname.CStr());
	fprintf(F, "#Nodes: %d\tEdges: %d\n", Graph->GetNodes(), Graph->GetEdges());
	fprintf(F, "#NId\tDegree\tPageRank\tIinfluenceRank\n");
	for (TUNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
		const int NId = NI.GetId();
		const int deg = Graph->GetNI(NId).GetDeg();
		const double pr = PRankH.GetDat(NId);
		const double ir = IRankH.GetDat(NId);
		fprintf(F, "%d\t%d\t%.9f\t%.9f\n", NId, deg, pr, ir);
	}
	fclose(F);
	printf("\nrun time: (%s)\n", TSecTm::GetCurTm().GetTmStr().CStr());
}

void rankCasdGraph(const TStr InFNm, const TStr OutFNm){
	printf("Loading %s...\n", InFNm.CStr());
	PCasdGraph rowGraph = loadGraph(InFNm, true);
	PCasdGraph colGraph = loadGraph(InFNm, false);
	TIntFltH IRankH, PRankH;
	printf("\tPageRank... \n");					pageRankWithWeights(rowGraph, PRankH);
	printf("\tInfluence Rank...\n");			influenceRankWithWeights(colGraph, IRankH);
	printf("DONE! saving...\n");

	FILE *F = fopen(OutFNm.CStr(), "wt");
	fprintf(F, "#Network: %s\n", InFNm.CStr());
	fprintf(F, "#Nodes: %d\tEdges: %d\n", rowGraph->GetNodes(), rowGraph->GetEdges());
	fprintf(F, "#NId\tInDeg\tOutDeg\tPageRank\tInfRank\n");
	for (TCasdGraph::TNodeI NI = rowGraph->BegNI(); NI < rowGraph->EndNI(); NI++) {
		const int NId = NI.GetId();
		const int inDeg = rowGraph->GetNI(NId).GetInDeg();
		const int outDeg = rowGraph->GetNI(NId).GetOutDeg();
		const double pr = PRankH.GetDat(NId);
		const double ir = IRankH.GetDat(NId);
		fprintf(F, "%d\t%d\t%d\t%.9f\t%.9f\n", NId, inDeg, outDeg, pr, ir);
	}
	fclose(F);
	printf("\nrun time: (%s)\n", TSecTm::GetCurTm().GetTmStr().CStr());
}

void rank_cascade_graph(){
	const TStr ifname("/home/jzzhao/workspace/outbreak/data/weibo/cascade.digraph");
	const TStr ofname = ("pagerank_infrank.rank");
	rankCasdGraph(ifname, ofname);
}

void rank_base_graph(){
	const TStr ifname("/home/jzzhao/workspace/outbreak/data/renren/base(mapped).edgelist");
	const TStr ofname = ("base_graph.rank");
	rankGraph(ifname, ofname);
}

int main(int argc, char* argv[]) {
	rank_base_graph();
	return 0;
}
