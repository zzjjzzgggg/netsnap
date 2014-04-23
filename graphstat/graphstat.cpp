#include "graphstat.h"

int main(int argc, char* argv[]) {
	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(TStr::Fmt("GraphInfo. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
	Try
		const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.txt", "Input graph");
		const bool IsDir = Env.GetIfArgPrefixBool("-d:", true, "Directed graph");
		const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "graph", "Output file prefix");
		const TStr Desc = Env.GetIfArgPrefixStr("-t:", "", "Title");
		const TStr Plot = Env.GetIfArgPrefixStr("-p:", "", "What statistics to plot:"
			"\n\tc: cummulative degree distribution"
			"\n\td: degree distribution"
			"\n\th: hop plot (diameter)"
			"\n\tw: distribution of weakly connected components"
			"\n\ts: distribution of strongly connected components"
			"\n\tC: clustering coefficient"
			"\n\tv: singular values"
			"\n\tV: left and right singular vector\n\t");
		const TStr calcs = Env.GetIfArgPrefixStr("-c:", "", "What statistics to calculate:"
			"\n\tb: basic statistics"
			"\n\tt: count closed triads"
			"\n\td: dissemination ability(-hops:2 -sample:0.01 -fo:T -fi:F -n:1)"
			"\n\tD: BFS diameter"
			"\n\tC: average clustering coefficient(-sample:T, default 5%)"
			"\n\th: hops (10% effective diameter)"
			"\n\tw: largest weakly connected components"
			"\n\ts: largest strongly connected components\n\t");
		const TStr fmts = Env.GetIfArgPrefixStr("-f:", "", "How to format the graph:"
			"\n\tm: map graph(assign uid from 0)"
			"\n\tl: remove selfloops"
			"\n\tn: save all the nodes"
			"\n\tr: reverse the edge direction\n\t");
		const TBool save = Env.GetIfArgPrefixBool("-s:", false, "Save graph");

		bool PlotDD = Plot.SearchCh('d') != -1;
		bool PlotCDD = Plot.SearchCh('c') != -1;
		bool PlotHop = Plot.SearchCh('h') != -1;
		bool PlotWcc = Plot.SearchCh('w') != -1;
		bool PlotScc = Plot.SearchCh('s') != -1;
		bool PlotClustCf = Plot.SearchCh('C') != -1;
		bool PlotSVal = Plot.SearchCh('v') != -1;
		bool PlotSVec = Plot.SearchCh('V') != -1;
		bool cb = calcs.SearchCh('b') != -1;
		bool ct = calcs.SearchCh('t') != -1;
		bool da = calcs.SearchCh('d') != -1;
		bool bd = calcs.SearchCh('D') != -1;
		bool cC = calcs.SearchCh('C') != -1;
		bool ch = calcs.SearchCh('h') != -1;
		bool cw = calcs.SearchCh('w') != -1;
		bool cs = calcs.SearchCh('s') != -1;
		bool mg = fmts.SearchCh('m') != -1;
		bool sl = fmts.SearchCh('l') != -1;
		bool sn = fmts.SearchCh('n') != -1;
		bool fr = fmts.SearchCh('r') != -1;
		if (Env.IsEndOfRun()) return 0;

		if(mg) {
			MapNodes(InFNm, OutFNm, IsDir);
			return 0;
		}

		if(IsDir){
			PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
			printf("Directed graph is loaded. Nodes:%d, Edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
			if(PlotDD){
				TSnap::PlotOutDegDistr(Graph, OutFNm, Desc, false, false);
				TSnap::PlotInDegDistr(Graph, OutFNm, Desc, false, false);
			}
			if(PlotCDD){
				TSnap::PlotOutDegDistr(Graph, OutFNm, Desc, true, false);
				TSnap::PlotInDegDistr(Graph, OutFNm, Desc, true, false);
			}
			if(PlotHop) TSnap::PlotHops(Graph, OutFNm, Desc, false, 32);
			if(PlotWcc) TSnap::PlotWccDistr(Graph, OutFNm, Desc);
			if(PlotScc) TSnap::PlotSccDistr(Graph, OutFNm, Desc);
			if(PlotClustCf) TSnap::PlotClustCf(Graph, OutFNm, Desc);
			if(PlotSVal){
				const int Vals = Graph->GetNodes()/2>200?200:Graph->GetNodes()/2;
				TSnap::PlotSngValRank(Graph, Vals, OutFNm, Desc);
			}
			if(PlotSVec) TSnap::PlotSngVec(Graph, OutFNm, Desc);
			if(cb){
				printf("nodes:%d edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
				if(save) TSnap::SaveEdgeList(Graph, OutFNm+".digraph");
			}
			if(ct){
				int ctriads=TSnap::GetTriads(Graph);
				printf("# closed triads: %d\n", ctriads);
			}
			if(da){
				int maxHops=Env.GetIfArgPrefixInt("-hops:",2);
				double sample=Env.GetIfArgPrefixFlt("-sample:", 0.01);
				bool fo=Env.GetIfArgPrefixBool("-fo:",true);
				bool fi=Env.GetIfArgPrefixBool("-fi:",false);
				int n=Env.GetIfArgPrefixInt("-n:", 1);
				double rst=TSnap::GetDissAbility(Graph, fo, fi, maxHops, sample, n);
				printf("Dissemination ability(hops=%d, sample=%.4f, n=%d): %.4f\n", maxHops, sample, n, rst);
			}
			if(bd){
				int dia=TSnap::GetBfsFullDiam<PNGraph>(Graph, 1, true);
				printf("BFS diameter: %d\n", dia);
			}
			if(cC){
				bool sample=Env.GetIfArgPrefixBool("-sample:", true);
				int samples=sample?(int)(Graph->GetNodes()*0.05):-1;
				double cc=TSnap::GetClustCf(Graph, samples);
				printf("Average clustering coefficient(sampled: %d): %.4f\n", sample, cc);
			}
			if(ch){
				double eff_diam=TSnap::GetAnfEffDiam(Graph);
				printf("90%% effective diameter: %.4f\n", eff_diam);
			}
			if(cw){
				PNGraph wcc=TSnap::GetMxWcc<PNGraph>(Graph);
				printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
				printf("Number of edges in WCC: %d\n", wcc->GetEdges());
				if(save) TSnap::SaveEdgeList(wcc, OutFNm+"_wcc.digraph");
			}
			if(cs){
				PNGraph scc=TSnap::GetMxScc<PNGraph>(Graph);
				printf("Number of nodes in SCC: %d\n", scc->GetNodes());
				printf("Number of edges in SCC: %d\n", scc->GetEdges());
				if(save) TSnap::SaveEdgeList(scc,OutFNm+"_scc.digraph");
			}
			if(sl){
				printf("Removing self edges...\n");
				TSnap::DelSelfEdges(Graph);
				TSnap::SaveEdgeList(Graph, OutFNm, "", true);
			}
			if(fr){
				printf("Reversing the edge direction...\n");
				TSnap::SaveEdgeList(Graph, OutFNm, "", true);
			}
			if(sn) SaveNodes(Graph, OutFNm+".nodes");
		}else{
			PUNGraph Graph = TSnap::LoadEdgeList<PUNGraph>(InFNm);
			printf("Undirected graph is loaded. Nodes:%d, Edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
			if(PlotDD)	TSnap::PlotOutDegDistr(Graph, OutFNm, Desc, false, false);
			if(PlotCDD) TSnap::PlotOutDegDistr(Graph, OutFNm, Desc, true, false);
			if(PlotHop) TSnap::PlotHops(Graph, OutFNm, Desc, false, 32);
			if(PlotWcc) TSnap::PlotWccDistr(Graph, OutFNm, Desc);
			if(PlotClustCf) TSnap::PlotClustCf(Graph, OutFNm, Desc);
			if(PlotSVal){
				const int Vals = Graph->GetNodes()/2 > 200 ? 200 : Graph->GetNodes()/2;
				TSnap::PlotEigValRank(Graph, Vals, OutFNm, Desc);
			}
			if(cb){
				printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
				if(save) TSnap::SaveEdgeList(Graph, OutFNm+".graph");
			}
			if(ct){
				int ctriads=TSnap::GetTriads(Graph);
				printf("# closed triads: %d\n", ctriads);
			}
			if(da){
				int maxHops=Env.GetIfArgPrefixInt("-hops:",2);
				double sample=Env.GetIfArgPrefixFlt("-sample:", 0.01);
				bool fo=Env.GetIfArgPrefixBool("-fo:",true);
				bool fi=Env.GetIfArgPrefixBool("-fi:",false);
				int n=Env.GetIfArgPrefixInt("-n:", 1);
				double rst=TSnap::GetDissAbility(Graph, fo, fi, maxHops, sample, n);
				printf("Dissemination ability(hops=%d, sample=%.4f, n=%d): %.4f\n", maxHops, sample, n, rst);
			}
			if(bd){
				int dia=TSnap::GetBfsFullDiam<PUNGraph>(Graph);
				printf("BFS diameter: %d\n", dia);
			}
			if(cC){
				bool sample=Env.GetIfArgPrefixBool("-sample:", true);
				int samples=sample?(int)(Graph->GetNodes()*0.05):-1;
				double cc=TSnap::GetClustCf(Graph, samples);
				printf("Average clustering coefficient(sampled: %d): %.4f\n", sample, cc);
			}
			if(ch){
				double eff_diam=TSnap::GetAnfEffDiam(Graph);
				printf("90%% effective diameter: %.4f\n", eff_diam);
			}
			if(cw){
				PUNGraph wcc=TSnap::GetMxWcc<PUNGraph>(Graph);
				printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
				printf("Number of edges in WCC: %d\n", wcc->GetEdges());
				if(save) TSnap::SaveEdgeList(wcc, OutFNm+"_wcc.graph");
			}
			if(sl){
				printf("Removing self edges...\n");
				TSnap::DelSelfEdges(Graph);
				TSnap::SaveEdgeList(Graph, OutFNm, "", true);
			}
			if(sn) SaveNodes(Graph, OutFNm+".nodes");
		}
	CatchAll
	return 0;
}

