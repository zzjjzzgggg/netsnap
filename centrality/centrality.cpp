#include "stdafx.h"

void rank_undirected_graph(const TStr fi, const TStr fo, const TStr calcs) {
  printf("Loading %s...\n", fi.CStr());
  PUNGraph Graph = TSnap::LoadEdgeList<PUNGraph>(fi);
  printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
  TVec<TIntFltH> kvals;
  bool cdeg = calcs.SearchCh('d') != -1;
  bool cpag = calcs.SearchCh('p') != -1;
  bool cbet = calcs.SearchCh('b') != -1;
  bool cclo = calcs.SearchCh('c') != -1;
  TStr header("#NID");
  if (cdeg) {
    TIntFltH degCentr;
    printf("Degree centrality...\n");
    for (TUNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      const int NId = NI.GetId();
      const int DegCentr = Graph->GetNI(NId).GetDeg();
      degCentr(NId) = DegCentr;
    }
    kvals.Add(degCentr);
    header += "\tDeg";
  }
  if (cpag) {
    TIntFltH pagCentr;
    printf("PageRank centrality...\n");
    TSnap::GetPageRank(Graph, pagCentr);
    kvals.Add(pagCentr);
    header += "\tPageRank";
  }
  if (cbet) {
    TIntFltH betCentr;
    printf("Betweenness centrality...\n");
    TSnap::GetBetweennessCentr(Graph, betCentr, 0.2);
    kvals.Add(betCentr);
    header += "\tBetweenness";
  }
  if (cclo) {
    TIntFltH cloCentr;
    printf("Closeness centrality...\n");
    for (TUNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      const int NId = NI.GetId();
      cloCentr.AddDat(NId, TSnap::GetClosenessCentr(Graph, NId));
    }
    kvals.Add(cloCentr);
    header += "\tCloseness";
  }
  FILE *F = fopen(fo.CStr(), "w");
  fprintf(F, "%s\n", header.CStr());
  for (TUNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    const int NId = NI.GetId();
    fprintf(F, "%d", NId);
    for (int i = 0; i < kvals.Len(); i++)
      fprintf(F, "\t%.6f", kvals[i](NId).Val);
    fprintf(F, "\n");
  }
  fclose(F);
}

void rank_directed_graph(const TStr fi, const TStr fo, const TStr calcs) {
  printf("Loading %s...\n", fi.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(fi);
  printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
  TVec<TIntFltH> kvals;
  bool cdeg = calcs.SearchCh('d') != -1;
  bool cpag = calcs.SearchCh('p') != -1;
  bool cbet = calcs.SearchCh('b') != -1;
  bool cclo = calcs.SearchCh('c') != -1;
  TStr header("#NID");
  if (cdeg) {
    TIntFltH iDegCentr, oDegCentr, degCentr;
    printf("Degree centrality...\n");
    for (TNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
      const int NId = NI.GetId();
      iDegCentr(NId) = NI.GetInDeg();
      oDegCentr(NId) = NI.GetOutDeg();
      degCentr(NId) = NI.GetDeg();
    }
    kvals.Add(iDegCentr);
    kvals.Add(oDegCentr);
    kvals.Add(degCentr);
    header += "\tInDeg\tOutDeg\tDeg";
  }
  if (cpag) {
    TIntFltH pagCentr;
    printf("PageRank centrality...\n");
    TSnap::GetPageRank(Graph, pagCentr);
    kvals.Add(pagCentr);
    header += "\tPageRank";
  }
  FILE *F = fopen(fo.CStr(), "w");
  fprintf(F, "%s\n", header.CStr());
  for (TNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    const int NId = NI.GetId();
    fprintf(F, "%d", NId);
    for (int i = 0; i < kvals.Len(); i++)
      fprintf(F, "\t%.6f", kvals[i](NId).Val);
    fprintf(F, "\n");
  }
  fclose(F);
}

void rank_graph_core(const TStr InFNm, const TStr OutFNm,
                     const bool isdir = false) {
  if (!isdir) {
    PUNGraph G = TSnap::LoadEdgeList<PUNGraph>(InFNm);
    TKCore<PUNGraph> KCore(G);
    FILE *F = fopen(OutFNm.CStr(), "wt");
    fprintf(F, "#NID\tCore\n");
    while (KCore.GetNextCore() != 0) {
      const TIntV nodes = KCore.GetCoreV();
      const int core = KCore.GetCurK() - 1;
      for (int i = 0; i < nodes.Len(); i++)
        fprintf(F, "%d\t%d\n", nodes[i].Val, core);
    }
    fclose(F);
  } else {
    PNGraph G = TSnap::LoadEdgeList<PNGraph>(InFNm);
    TKCore<PNGraph> KCore(G);
    FILE *F = fopen(OutFNm.CStr(), "wt");
    fprintf(F, "#NID\tCore\n");
    while (KCore.GetNextCore() != 0) {
      const TIntV nodes = KCore.GetCoreV();
      const int core = KCore.GetCurK() - 1;
      for (int i = 0; i < nodes.Len(); i++)
        fprintf(F, "%d\t%d\n", nodes[i].Val, core);
    }
    fclose(F);
  }
  printf("\nrun time: (%s)\n", TSecTm::GetCurTm().GetTmStr().CStr());
}

void rank_directed_graph(const TStr InFNm, const TStr OutFNm) {
  printf("Loading %s...\n", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  // PNGraph Graph = TSnap::GenRndGnm<PNGraph>(10, 10);
  printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
  PUNGraph UGraph =
      TSnap::ConvertGraph<PUNGraph>(Graph); // undirected version of the graph
  TIntFltH BtwH, EigH, PRankH, CloseH, HubH, AuthH;

  printf("Computing...\n");
  printf("Treat graph as DIRECTED: \n");
  printf(" PageRank... \n");
  TSnap::GetPageRank(Graph, PRankH);
  printf(" Hubs&Authorities...\n");
  TSnap::GetHits(Graph, HubH, AuthH);
  printf("\nTreat graph as UNDIRECTED: \n");
  printf(" Eigenvector...\n");
  TSnap::GetEigenVectorCentr(UGraph, EigH);
  printf(" Betweenness (SLOW!)...\n");
  TSnap::GetBetweennessCentr(UGraph, BtwH, 0.01);
  printf(" Closeness (SLOW!)...\n");
  for (TUNGraph::TNodeI NI = UGraph->BegNI(); NI < UGraph->EndNI(); NI++) {
    const int nid = NI.GetId();
    CloseH.AddDat(nid, TSnap::GetClosenessCentr(UGraph, nid));
  }

  printf("\nDONE! saving...\n");
  FILE *F = fopen(OutFNm.CStr(), "wt");
  fprintf(F, "#Network: %s\n", InFNm.CStr());
  fprintf(F, "#Nodes: %d\tEdges: %d\n", Graph->GetNodes(), Graph->GetEdges());
  fprintf(F, "#NodeId\tInDegree\tOutDegree\tCloseness\tBetweennes\tEigenVector"
             "\tClustering\tPageRank\tHub\tAuthority\n");
  for (TUNGraph::TNodeI NI = UGraph->BegNI(); NI < UGraph->EndNI(); NI++) {
    const int NId = NI.GetId();
    const int ideg = NI.GetInDeg();
    const int odeg = NI.GetOutDeg();
    const double CloCentr = CloseH.GetDat(NId);
    const double BtwCentr = BtwH.GetDat(NId);
    const double EigCentr = EigH.GetDat(NId);
    const double PgrCentr = PRankH.GetDat(NId);
    const double HubCentr = HubH.GetDat(NId);
    const double AuthCentr = AuthH.GetDat(NId);
    fprintf(F, "%d\t%d\t%d\t%f\t%f\t%f\t%f\t%f\t%f\n", NId, ideg, odeg,
            CloCentr, BtwCentr, EigCentr, PgrCentr, HubCentr, AuthCentr);
  }
  fclose(F);
  printf("\nrun time: (%s)\n", TSecTm::GetCurTm().GetTmStr().CStr());
}

void rank_directed_graph_fast(const TStr InFNm, const TStr OutFNm) {
  printf("Loading %s...\n", InFNm.CStr());
  PNGraph Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
  //	PNGraph Graph = TSnap::GenRndGnm<PNGraph>(10, 10);
  printf("nodes:%d  edges:%d\n", Graph->GetNodes(), Graph->GetEdges());
  printf("Max eigenvalue... \n");
  double eig = util::getMxEig<PNGraph>(Graph);
  printf("Max eigenvalue=%f\n", eig);

  TIntFltH KatzH, PRankH;
  printf("Katz Centrality... \n");
  TSnap::GetKatzCentr<PNGraph>(Graph, KatzH, 1 / (eig + 0.001));
  printf("PageRank... \n");
  TSnap::GetPageRank<PNGraph>(Graph, PRankH);

  printf("\nDONE! saving...\n");
  FILE *F = fopen(OutFNm.CStr(), "w");
  fprintf(F, "#Network: %s\n", InFNm.CStr());
  fprintf(F, "#NodeId\tInDegree\tOutDegree\tKatz\tPageRank\n");
  for (TNGraph::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
    const int NId = NI.GetId();
    const int ideg = NI.GetInDeg();
    const int odeg = NI.GetOutDeg();
    const double KatzCentr = KatzH.GetDat(NId) * 1000;
    const double PgrCentr = PRankH.GetDat(NId) * 1000;
    fprintf(F, "%d\t%d\t%d\t%f\t%f\n", NId, ideg, odeg, KatzCentr, PgrCentr);
  }
  fclose(F);
}

int main(int argc, char *argv[]) {
  Env = TEnv(argc, argv, TNotify::StdNotify);
  Env.PrepArgs(TStr::Fmt("GraphInfo. build: %s, %s. Time: %s", __TIME__,
                         __DATE__, TExeTm::GetCurTm()));
  const TStr InFNm = Env.GetIfArgPrefixStr("-i:", "", "Input graph");
  const TBool IsDir = Env.GetIfArgPrefixBool("-d:", false, "Directed graph");
  const TStr OutFNm =
      Env.GetIfArgPrefixStr("-o:", "graph.centr", "Output file name");
  const TStr calcs = Env.GetIfArgPrefixStr("-c:", 0, "Calculate:\n\t"
                                                     "d: degree\n\t"
                                                     "p: pagerank\n\t"
                                                     "b: betweenness\n\t"
                                                     "c: closeness\n\t");
  if (InFNm.Empty())
    return 0;
  if (IsDir)
    rank_directed_graph(InFNm, OutFNm, calcs);
  else
    rank_undirected_graph(InFNm, OutFNm, calcs);
  return 0;
}
