#include "graphstat.h"

int main(int argc, char *argv[]) {
  TExeTm ExeTm;
  ArgsParser parser(argc, argv);
  TStr InFNm =
      parser.getStr("-graph", "graph.txt", "input graph").c_str();
  int type = parser.getInt("-type", 0,
                           "0: edgelist\n"
                           "1: binary edgelist\n"
                           "2: binary");
  bool isDirected = parser.getBool("-dir", false, "is directed?");

  string Plot =
      parser.getStr("-plot", "",
                    "What statistics to plot:\n"
                    "c: cummulative degree distribution\n"
                    "d: degree distribution\n"
                    "h: hop plot (diameter)\n"
                    "w: dist. of weakly connected components\n"
                    "s: dist. of strongly connected components\n"
                    "C: clustering coefficient\n"
                    "v: singular values\n"
                    "V: left and right singular vector");
  string Calcs = parser.getStr(
      "-calcs", "",
      "What statistics to calculate: (will load into mem)\n"
      "b: basic statistics\n"
      "t: count closed triads\n"
      "d: dissemination ability (-hops 2 -sample 0.01 "
      "-out true -in false -n 1)\n"
      "D: BFS diameter (-testnodes:100)\n"
      "C: average clustering coefficient (-sample true, "
      "default 5%)\n"
      "h: hops (10% effective diameter)\n"
      "w: largest weakly connected components\n"
      "s: largest strongly connected components");
  string Fmts = parser.getStr(
      "-format", "",
      "How to format the graph: (no need to load into mem)\n"
      "b: convert to binary format (for the purpose of fast "
      "loading)\n"
      "e: convert to binary edgelist\n"
      "m: maping graph (assign nid from 0)\n"
      "l: remove self-loops\n"
      "n: save nodes in a file\n"
      "d: get degree sequence, e.g., (node degree)\n"
      "r: reverse the edge direction");
  string Save = parser.getStr("-save", "",
                              "Save graph:\n"
                              "e: edgelist\n"
                              "b: binary");

  bool PlotDD = Plot.find('d') != std::string::npos;
  bool PlotCDD = Plot.find('c') != std::string::npos;
  bool PlotHop = Plot.find('h') != std::string::npos;
  bool PlotWcc = Plot.find('w') != std::string::npos;
  bool PlotScc = Plot.find('s') != std::string::npos;
  bool PlotClustCf = Plot.find('C') != std::string::npos;
  bool PlotSVal = Plot.find('v') != std::string::npos;
  bool PlotSVec = Plot.find('V') != std::string::npos;

  bool Cal_b = Calcs.find('b') != std::string::npos;
  bool Cal_t = Calcs.find('t') != std::string::npos;
  bool Cal_d = Calcs.find('d') != std::string::npos;
  bool Cal_D = Calcs.find('D') != std::string::npos;
  bool Cal_C = Calcs.find('C') != std::string::npos;
  bool Cal_h = Calcs.find('h') != std::string::npos;
  bool Cal_w = Calcs.find('w') != std::string::npos;
  bool Cal_s = Calcs.find('s') != std::string::npos;

  // need to load graph
  bool Fmt_b = Fmts.find('b') != std::string::npos;
  bool Fmt_e = Fmts.find('e') != std::string::npos;
  bool Fmt_m = Fmts.find('m') != std::string::npos;
  bool Fmt_l = Fmts.find('l') != std::string::npos;
  bool Fmt_n = Fmts.find('n') != std::string::npos;
  bool Fmt_r = Fmts.find('r') != std::string::npos;
  bool Fmt_d = Fmts.find('d') != std::string::npos;

  bool Sav_e = Save.find('e') != std::string::npos;
  bool Sav_b = Save.find('b') != std::string::npos;
  if (parser.isEnd()) return 0;

  // format graph, no need to load graph file
  if (!Fmts.empty() && !Fmt_b) {
    // re-mapping nodes
    if (Fmt_m) {
      printf("Mapping nodes ...\n");
      MapingNodes(InFNm, isDirected);
    }
    // reverse edge direction
    if (Fmt_r) {
      printf("Reversing the edge direction ...\n");
      ReverseEdgeDirection(InFNm);
    }
    // remove self-loops
    if (Fmt_l) {
      printf("Removing self-loops ...\n");
      RemoveSelfLoops(InFNm);
    }
    // save nodes in the graph
    if (Fmt_n) SaveNodes(InFNm, type);

    if (Fmt_d) {
      printf("Geting degree sequence ...\n");
      if (isDirected)
        GetDirDegSeq(InFNm, type);
      else
        GetUnDirDegSeq(InFNm, type);
    }

    if (Fmt_e) {
      printf("Formating to binary edgelist ...\n");
      FormatBinaryEdgelist(InFNm, TStr::AddToFMid(InFNm, "_be"));
    }

  } else {
    if (isDirected) {
      PNGraph Graph;
      if (type == 0)
        Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
      else if (type == 1)
        Graph = TSnap::LoadBinaryEdgeList<PNGraph>(InFNm);
      else
        Graph = TSnap::LoadBinary<PNGraph>(InFNm);
      printf(
          "============================================"
          "\n\n"
          "Directed graph is loaded. Nodes:%d, "
          "Edges:%d\n\n",
          Graph->GetNodes(), Graph->GetEdges());
      if (PlotDD) {
        TSnap::PlotOutDegDistr(Graph,
                               TStr::AddToFMid(InFNm, "_out_pdf"),
                               "", false, false);
        TSnap::PlotInDegDistr(Graph,
                              TStr::AddToFMid(InFNm, "_in_pdf"),
                              "", false, false);
      }
      if (PlotCDD) {
        TSnap::PlotOutDegDistr(
            Graph, TStr::AddToFMid(InFNm, "_out_ccdf"), "", true,
            false);
        TSnap::PlotInDegDistr(Graph,
                              TStr::AddToFMid(InFNm, "_in_ccdf"),
                              "", true, false);
      }
      if (PlotHop)
        TSnap::PlotHops(Graph, TStr::AddToFMid(InFNm, "_hops"),
                        "", false, 32);
      if (PlotWcc)
        TSnap::PlotWccDistr(
            Graph, TStr::AddToFMid(InFNm, "_wcc_dist"), "");
      if (PlotScc)
        TSnap::PlotSccDistr(
            Graph, TStr::AddToFMid(InFNm, "_scc_dist"), "");
      if (PlotClustCf)
        TSnap::PlotClustCf(Graph, TStr::AddToFMid(InFNm, "_ccf"),
                           "");
      if (PlotSVal) {
        const int Vals = Graph->GetNodes() / 2 > 200
                             ? 200
                             : Graph->GetNodes() / 2;
        TSnap::PlotSngValRank(Graph, Vals,
                              TStr::AddToFMid(InFNm, "_sng"), "");
      }
      if (PlotSVec)
        TSnap::PlotSngVec(Graph, TStr::AddToFMid(InFNm, "_sng"),
                          "");
      if (Cal_b) {
        printf("nodes:%d edges:%d\n", Graph->GetNodes(),
               Graph->GetEdges());
        if (Sav_e)
          TSnap::SaveEdgeList(Graph,
                              TStr::AddToFMid(InFNm, "_digraph"));
        else if (Sav_b)
          TSnap::SaveBinary(
              Graph, TStr::AddToFMid(InFNm, "_bin_digraph"));
      }
      if (Cal_t) {
        int ctriads = TSnap::GetTriads(Graph);
        printf("# closed triads: %d\n", ctriads);
      }
      if (Cal_d) {
        int maxHops = parser.getInt("-hops", 2);
        double sample = parser.getFlt("-sample", 0.01);
        bool fo = parser.getBool("-out", true);
        bool fi = parser.getBool("-in", false);
        int n = parser.getInt("-n", 1);
        double rst = TSnap::GetDissAbility(Graph, fo, fi, maxHops,
                                           sample, n);
        printf(
            "Dissemination ability(hops=%d, "
            "sample=%.4f, "
            "n=%d): %.4f\n",
            maxHops, sample, n, rst);
      }
      if (Cal_D) {
        int test_nodes = parser.getInt("-testnodes", 100);
        int dia = TSnap::GetBfsFullDiam<PNGraph>(
            Graph, test_nodes, true);
        printf("BFS diameter: %d\n", dia);
      }
      if (Cal_C) {
        bool sample = parser.getBool("-sample", true);
        int samples =
            sample ? (int)(Graph->GetNodes() * 0.05) : -1;
        double cc = TSnap::GetClustCf(Graph, samples);
        printf(
            "Average clustering coefficient(sampled: "
            "%d): "
            "%.6f\n",
            sample, cc);
      }
      if (Cal_h) {
        double eff_diam = TSnap::GetAnfEffDiam(Graph);
        printf("90%% effective diameter: %.4f\n", eff_diam);
      }
      if (Cal_w) {
        PNGraph wcc = TSnap::GetMxWcc<PNGraph>(Graph);
        printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
        printf("Number of edges in WCC: %d\n", wcc->GetEdges());
        if (Sav_e)
          TSnap::SaveEdgeList(wcc,
                              TStr::AddToFMid(InFNm, "_wcc"));
        else if (Sav_b)
          TSnap::SaveBinary(wcc,
                            TStr::AddToFMid(InFNm, "_wcc_bin"));
      }
      if (Cal_s) {
        PNGraph scc = TSnap::GetMxScc<PNGraph>(Graph);
        printf("Number of nodes in SCC: %d\n", scc->GetNodes());
        printf("Number of edges in SCC: %d\n", scc->GetEdges());
        if (Sav_e)
          TSnap::SaveEdgeList(scc,
                              TStr::AddToFMid(InFNm, "_scc"));
        else if (Sav_b)
          TSnap::SaveBinary(scc,
                            TStr::AddToFMid(InFNm, "_scc_bin"));
      }
      if (Fmt_b) {
        printf("Converting to binary format ...\n");
        TSnap::SaveBinary<PNGraph>(
            Graph, TStr::AddToFMid(InFNm, "_bin"));
      }
    } else {
      PUNGraph Graph;
      if (type == 0)
        Graph = TSnap::LoadEdgeList<PUNGraph>(InFNm);
      else if (type == 1)
        Graph = TSnap::LoadBinaryEdgeList<PUNGraph>(InFNm);
      else
        Graph = TSnap::LoadBinary<PUNGraph>(InFNm);
      printf(
          "============================================"
          "===="
          "=="
          "=====\n\n"
          "Undirected graph is loaded. Nodes:%d, "
          "Edges:%d\n\n",
          Graph->GetNodes(), Graph->GetEdges());
      if (PlotDD)
        TSnap::PlotOutDegDistr(Graph,
                               TStr::AddToFMid(InFNm, "_pdf"), "",
                               false, false);
      if (PlotCDD)
        TSnap::PlotOutDegDistr(Graph,
                               TStr::AddToFMid(InFNm, "_ccdf"),
                               "", true, false);
      if (PlotHop)
        TSnap::PlotHops(Graph, TStr::AddToFMid(InFNm, "_hops"),
                        "", false, 32);
      if (PlotWcc)
        TSnap::PlotWccDistr(
            Graph, TStr::AddToFMid(InFNm, "_wcc_dist"), "");
      if (PlotClustCf)
        TSnap::PlotClustCf(
            Graph, TStr::AddToFMid(InFNm, "_ccf_dist"), "");
      if (PlotSVal) {
        const int Vals = Graph->GetNodes() / 2 > 200
                             ? 200
                             : Graph->GetNodes() / 2;
        TSnap::PlotEigValRank(Graph, Vals,
                              TStr::AddToFMid(InFNm, "_eig"), "");
      }
      if (Cal_b) {
        printf("nodes:%d  edges:%d\n", Graph->GetNodes(),
               Graph->GetEdges());
        if (Sav_e)
          TSnap::SaveEdgeList(Graph,
                              TStr::AddToFMid(InFNm, "_graph"));
        else if (Sav_b)
          TSnap::SaveBinary(Graph,
                            TStr::AddToFMid(InFNm, "_bin_graph"));
      }
      if (Cal_t) {
        int ctriads = TSnap::GetTriads(Graph);
        printf("# closed triads: %d\n", ctriads);
      }
      if (Cal_d) {
        int maxHops = parser.getInt("-hops", 2);
        double sample = parser.getFlt("-sample", 0.01);
        bool fo = parser.getBool("-out", true);
        bool fi = parser.getBool("-in", false);
        int n = parser.getInt("-n", 1);
        double rst = TSnap::GetDissAbility(Graph, fo, fi, maxHops,
                                           sample, n);
        printf(
            "Dissemination ability(hops=%d, "
            "sample=%.4f, "
            "n=%d): %.4f\n",
            maxHops, sample, n, rst);
      }
      if (Cal_D) {
        int test_nodes = parser.getInt("-testnodes", 100);
        int dia =
            TSnap::GetBfsFullDiam<PUNGraph>(Graph, test_nodes);
        printf("BFS diameter: %d\n", dia);
      }
      if (Cal_C) {
        bool sample = parser.getBool("-sample", true);
        int samples =
            sample ? (int)(Graph->GetNodes() * 0.05) : -1;
        double cc = TSnap::GetClustCf(Graph, samples);
        printf(
            "Average clustering coefficient(sampled: "
            "%d): "
            "%.6f\n",
            sample, cc);
      }
      if (Cal_h) {
        double eff_diam = TSnap::GetAnfEffDiam(Graph);
        printf("90%% effective diameter: %.4f\n", eff_diam);
      }
      if (Cal_w) {
        PUNGraph wcc = TSnap::GetMxWcc<PUNGraph>(Graph);
        printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
        printf("Number of edges in WCC: %d\n", wcc->GetEdges());
        if (Sav_e)
          TSnap::SaveEdgeList(wcc,
                              TStr::AddToFMid(InFNm, "_wcc"));
        else if (Sav_b)
          TSnap::SaveBinary(wcc,
                            TStr::AddToFMid(InFNm, "_wcc_bin"));
      }
      if (Fmt_b) {
        printf("Converting to binary format ...\n");
        TSnap::SaveBinary<PUNGraph>(
            Graph, TStr::AddToFMid(InFNm, "_bin"));
      }
    }
  }
  printf("\nTime duration: %s\n", ExeTm.GetTmStr());
  return 0;
}
