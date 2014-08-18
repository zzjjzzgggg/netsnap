#include "graphstat.h"

int main(int argc, char* argv[]) {
	TExeTm ExeTm;
	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(TStr::Fmt("GraphStat. Build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
	Try
		const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "graph.txt", "Input graph");
		const bool IsEdgeList = Env.GetIfArgPrefixBool("-e:", true, "Edgelist");
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
		const TStr Calcs = Env.GetIfArgPrefixStr("-c:", "", "What statistics to calculate:"
			"\n\tb: basic statistics"
			"\n\tt: count closed triads"
			"\n\td: dissemination ability(-hops:2 -sample:0.01 -fo:T -fi:F -n:1)"
			"\n\tD: BFS diameter"
			"\n\tC: average clustering coefficient(-sample:T, default 5%)"
			"\n\th: hops (10% effective diameter)"
			"\n\tn: save all nodes in the graph to a file"
			"\n\tw: largest weakly connected components"
			"\n\ts: largest strongly connected components\n\t");
		const TStr Fmts = Env.GetIfArgPrefixStr("-f:", "", "How to format the graph:"
			"\n\tb: convert to binary format (for the purpose of fast loading)"
			"\n\tm: remaping graph (assign uid from 0)"
			"\n\tl: remove self-loops"
			"\n\tn: save all nodes in the graph to a file"
			"\n\tr: reverse the edge direction\n\t");
		const TStr Save = Env.GetIfArgPrefixStr("-s:", "", "Save graph:"
				"\n\te: edgelist"
				"\n\tb: binary");

		bool PlotDD = Plot.SearchCh('d') != -1;
		bool PlotCDD = Plot.SearchCh('c') != -1;
		bool PlotHop = Plot.SearchCh('h') != -1;
		bool PlotWcc = Plot.SearchCh('w') != -1;
		bool PlotScc = Plot.SearchCh('s') != -1;
		bool PlotClustCf = Plot.SearchCh('C') != -1;
		bool PlotSVal = Plot.SearchCh('v') != -1;
		bool PlotSVec = Plot.SearchCh('V') != -1;

		bool Cal_b = Calcs.SearchCh('b') != -1;
		bool Cal_t = Calcs.SearchCh('t') != -1;
		bool Cal_d = Calcs.SearchCh('d') != -1;
		bool Cal_D = Calcs.SearchCh('D') != -1;
		bool Cal_C = Calcs.SearchCh('C') != -1;
		bool Cal_h = Calcs.SearchCh('h') != -1;
		bool Cal_n = Calcs.SearchCh('n') != -1;
		bool Cal_w = Calcs.SearchCh('w') != -1;
		bool Cal_s = Calcs.SearchCh('s') != -1;

		bool Fmt_b = Fmts.SearchCh('b') != -1;  // need to load graph
		bool Fmt_m = Fmts.SearchCh('m') != -1;
		bool Fmt_l = Fmts.SearchCh('l') != -1;
		bool Fmt_n = Fmts.SearchCh('n') != -1;
		bool Fmt_r = Fmts.SearchCh('r') != -1;

		bool Sav_e = Save.SearchCh('e') != -1;
		bool Sav_b = Save.SearchCh('b') != -1;
		if (Env.IsEndOfRun()) return 0;

		// format graph, no need to load graph file
		if(Fmts.Len()!=0) {
			// re-mapping nodes
			if(Fmt_m) {
				printf("Re-mapping nodes ...\n");
				MapingNodes(InFNm, OutFNm, IsDir);
			}
			// reverse edge direction
			if(Fmt_r){
				printf("Reversing the edge direction ...\n");
				ReverseEdgeDirection(InFNm, OutFNm+".reversed");
			}
			// remove self-loops
			if(Fmt_l){
				printf("Removing self-loops ...\n");
				RemoveSelfLoops(InFNm, OutFNm+".removed");
			}
			// save nodes in the graph
			if(Fmt_n) SaveNodes(InFNm, OutFNm+".nodes");

			if(!Fmt_b) return 0;
		}

		if(IsDir){
			PNGraph Graph;
			if (IsEdgeList) Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
			else Graph = TSnap::LoadBinary<PNGraph>(InFNm);
			printf("=======================================================\n\n"
					"Directed graph is loaded. Nodes:%d, Edges:%d\n\n", Graph->GetNodes(), Graph->GetEdges());
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
			if(Cal_b){
				printf("nodes:%d edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
				if(Sav_e) TSnap::SaveEdgeList(Graph, OutFNm+".digraph");
				else if(Sav_b) TSnap::SaveBinary(Graph,OutFNm+"_bin.digraph.gz");
			}
			if(Cal_t){
				int ctriads=TSnap::GetTriads(Graph);
				printf("# closed triads: %d\n", ctriads);
			}
			if(Cal_d){
				int maxHops=Env.GetIfArgPrefixInt("-hops:",2);
				double sample=Env.GetIfArgPrefixFlt("-sample:", 0.01);
				bool fo=Env.GetIfArgPrefixBool("-fo:",true);
				bool fi=Env.GetIfArgPrefixBool("-fi:",false);
				int n=Env.GetIfArgPrefixInt("-n:", 1);
				double rst=TSnap::GetDissAbility(Graph, fo, fi, maxHops, sample, n);
				printf("Dissemination ability(hops=%d, sample=%.4f, n=%d): %.4f\n", maxHops, sample, n, rst);
			}
			if(Cal_D){
				int dia=TSnap::GetBfsFullDiam<PNGraph>(Graph, 1, true);
				printf("BFS diameter: %d\n", dia);
			}
			if(Cal_C){
				bool sample=Env.GetIfArgPrefixBool("-sample:", true);
				int samples=sample?(int)(Graph->GetNodes()*0.05):-1;
				double cc=TSnap::GetClustCf(Graph, samples);
				printf("Average clustering coefficient(sampled: %d): %.6f\n", sample, cc);
			}
			if(Cal_h){
				double eff_diam=TSnap::GetAnfEffDiam(Graph);
				printf("90%% effective diameter: %.4f\n", eff_diam);
			}
			if(Cal_n){
				TIntV nodes;
				Graph->GetNIdV(nodes);
				BIO::SaveInts(nodes, OutFNm+".nodes");
			}
			if(Cal_w){
				PNGraph wcc=TSnap::GetMxWcc<PNGraph>(Graph);
				printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
				printf("Number of edges in WCC: %d\n", wcc->GetEdges());
				if(Sav_e) TSnap::SaveEdgeList(wcc, OutFNm+"_wcc.digraph");
				else if(Sav_b) TSnap::SaveBinary(wcc,OutFNm+"_wcc_bin.digraph.gz");
			}
			if(Cal_s){
				PNGraph scc=TSnap::GetMxScc<PNGraph>(Graph);
				printf("Number of nodes in SCC: %d\n", scc->GetNodes());
				printf("Number of edges in SCC: %d\n", scc->GetEdges());
				if(Sav_e) TSnap::SaveEdgeList(scc,OutFNm+"_scc.digraph");
				else if(Sav_b) TSnap::SaveBinary(scc,OutFNm+"_scc_bin.digraph.gz");
			}
			if(Fmt_b){
				printf("Converting to binary format ...\n");
				TSnap::SaveBinary<PNGraph>(Graph, OutFNm+"_bin.digraph.gz");
			}
		}else{
			PUNGraph Graph;
			if (IsEdgeList) Graph = TSnap::LoadEdgeList<PUNGraph>(InFNm);
			else Graph = TSnap::LoadBinary<PUNGraph>(InFNm);
			printf("=======================================================\n\n"
					"Undirected graph is loaded. Nodes:%d, Edges:%d\n\n", Graph->GetNodes(), Graph->GetEdges());
			if(PlotDD)	TSnap::PlotOutDegDistr(Graph, OutFNm, Desc, false, false);
			if(PlotCDD) TSnap::PlotOutDegDistr(Graph, OutFNm, Desc, true, false);
			if(PlotHop) TSnap::PlotHops(Graph, OutFNm, Desc, false, 32);
			if(PlotWcc) TSnap::PlotWccDistr(Graph, OutFNm, Desc);
			if(PlotClustCf) TSnap::PlotClustCf(Graph, OutFNm, Desc);
			if(PlotSVal){
				const int Vals = Graph->GetNodes()/2 > 200 ? 200 : Graph->GetNodes()/2;
				TSnap::PlotEigValRank(Graph, Vals, OutFNm, Desc);
			}
			if(Cal_b){
				printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
				if(Sav_e) TSnap::SaveEdgeList(Graph, OutFNm+".graph");
				else if(Sav_b) TSnap::SaveBinary(Graph,OutFNm+"_bin.graph.gz");
			}
			if(Cal_t){
				int ctriads=TSnap::GetTriads(Graph);
				printf("# closed triads: %d\n", ctriads);
			}
			if(Cal_d){
				int maxHops=Env.GetIfArgPrefixInt("-hops:",2);
				double sample=Env.GetIfArgPrefixFlt("-sample:", 0.01);
				bool fo=Env.GetIfArgPrefixBool("-fo:",true);
				bool fi=Env.GetIfArgPrefixBool("-fi:",false);
				int n=Env.GetIfArgPrefixInt("-n:", 1);
				double rst=TSnap::GetDissAbility(Graph, fo, fi, maxHops, sample, n);
				printf("Dissemination ability(hops=%d, sample=%.4f, n=%d): %.4f\n", maxHops, sample, n, rst);
			}
			if(Cal_D){
				int dia=TSnap::GetBfsFullDiam<PUNGraph>(Graph);
				printf("BFS diameter: %d\n", dia);
			}
			if(Cal_C){
				bool sample=Env.GetIfArgPrefixBool("-sample:", true);
				int samples=sample?(int)(Graph->GetNodes()*0.05):-1;
				double cc=TSnap::GetClustCf(Graph, samples);
				printf("Average clustering coefficient(sampled: %d): %.6f\n", sample, cc);
			}
			if(Cal_h){
				double eff_diam=TSnap::GetAnfEffDiam(Graph);
				printf("90%% effective diameter: %.4f\n", eff_diam);
			}
			if(Cal_n){
				TIntV nodes;
				Graph->GetNIdV(nodes);
				BIO::SaveInts(nodes, OutFNm+".nodes");
			}
			if(Cal_w){
				PUNGraph wcc=TSnap::GetMxWcc<PUNGraph>(Graph);
				printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
				printf("Number of edges in WCC: %d\n", wcc->GetEdges());
				if(Sav_e) TSnap::SaveEdgeList(wcc, OutFNm+"_wcc.graph");
				else if(Sav_b) TSnap::SaveBinary(wcc,OutFNm+"_wcc_bin.graph.gz");
			}
			if(Fmt_b){
				printf("Converting to binary format ...\n");
				TSnap::SaveBinary<PUNGraph>(Graph, OutFNm+"_bin.graph.gz");
			}
		}
	CatchAll
	printf("\nTime duration: %s\n", ExeTm.GetTmStr());
	return 0;
}

