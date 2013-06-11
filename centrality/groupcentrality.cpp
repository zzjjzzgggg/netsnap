
#include "stdafx.h"
#include "advcentr.h"

int main(int argc, char* argv[]) {
	Env = TEnv(argc, argv, TNotify::StdNotify);
	Env.PrepArgs(TStr::Fmt("GraphInfo. build: %s, %s. Time: %s", __TIME__, __DATE__, TExeTm::GetCurTm()));
	const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "Input graph");
	const TStr attFnm = Env.GetIfArgPrefixStr("-a:", "", "Attributes of nodes");
	const bool IsDir = Env.GetIfArgPrefixBool("-d:", true, "Directed graph");
	const TStr OutFNm = Env.GetIfArgPrefixStr("-o:", "graph.centr", "Output file name");
	const double lambda = Env.GetIfArgPrefixFlt("-l:", log(2)/10, "Lambda");
	const int calcs = Env.GetIfArgPrefixInt("-c:", 0, "Calculate:\n\t"
				"0: degree\n\t"
				"1: in degree\n\t"
				"2: out degree\n\t"
				"3: weighted degree\n\t"
				"4: weighted in degree\n\t"
				"5: weighted out degree\n\t");
	const int maxN=Env.GetIfArgPrefixInt("-n:", -1, "top N nodes\n\t");
	if(InFNm.Empty()) return 0;
	Try
		if(IsDir){
			PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
			AdvCentr<PNGraph> advc(Graph, calcs, lambda);
			if(!attFnm.Empty()) advc.loadAttr(attFnm);
			advc.calculateDeg(maxN);
			advc.saveNodes(OutFNm);
		}
	CatchAll
	return 0;
}



