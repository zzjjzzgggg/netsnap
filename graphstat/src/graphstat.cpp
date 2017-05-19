#include "graphstat.h"

int main(int argc, char *argv[]) {
  TExeTm ExeTm;
  ArgsParser parser(argc, argv);
  TStr graph_fnm = parser.getStr("-graph", "graph.txt", "input graph").c_str();
  int type = parser.getInt("-type", 0,
                           "0: edgelist\n"
                           "1: binary edgelist\n"
                           "2: binary");
  bool is_dir = parser.getBool("-dir", true, "is directed?");
  string out_fnm = parser.getStr("-out", "", "ouput filename");

  string format_opt =
      parser.getStr("-format", "",
                    "How to format the graph: (no need to load into mem)\n"
                    "e: convert to binary edgelist\n"
                    "m: maping graph (assign nid from 0)\n"
                    "n: save nodes in a file\n"
                    "d: get degree sequence, e.g., (node degree)\n");
  string stat_opt = parser.getStr("-stat", "",
                                  "Graph statistics:\n"
                                  "c: CCDF\n"
                                  "d: degree distribution\n"
                                  "h: hop plot (diameter)\n"
                                  "w: dist. of weakly connected components\n"
                                  "s: dist. of strongly connected components\n"
                                  "C: clustering coefficient\n"
                                  "v: singular values\n"
                                  "V: left and right singular vector");
  string calc_opt =
      parser.getStr("-calcs", "",
                    "What statistics to calculate: (will load into mem)\n"
                    "b: basic statistics\n"
                    "t: count closed triads\n"
                    "D: BFS diameter (-testnodes:100)\n"
                    "C: average clustering coefficient (-sample true, "
                    "default 5%)\n"
                    "h: hops (10% effective diameter)\n"
                    "w: largest weakly connected components\n"
                    "s: largest strongly connected components");
  string sample_opt = parser.getStr("-sample", "",
                                    "Sample nodes in the graph\n"
                                    "  '-num <n>' specify sample size\n"
                                    "  '-wr true/false' indicate with or "
                                    "without replacement\n"
                                    "u: sample nodes uniformly\n"
                                    "d: sample nodes by degree");
  string save_opt = parser.getStr("-save", "",
                                  "Save graph:\n"
                                  "e: edgelist\n"
                                  "b: binary\n"
                                  "B: binary edgelist");
  if (parser.isEnd()) return 0;

  // format graph, no need to load graph file
  if (!format_opt.empty()) {
    // re-mapping nodes
    if (format_opt.find('m') != string::npos) {
      printf("Mapping nodes ...\n");
      MapingNodes(graph_fnm, is_dir);
    }
    // save nodes in the graph
    if (format_opt.find('n') != string::npos) SaveNodes(graph_fnm, type);
    // degree sequence
    if (format_opt.find('d') != string::npos) {
      printf("Geting degree sequence ...\n");
      getDegreeSequence(graph_fnm, is_dir);
    }
    // save binary edgelist
    if (format_opt.find('e') != string::npos) {
      printf("Formating to binary edgelist ...\n");
      SaveBinaryEdgelist(graph_fnm, TStr::AddToFMid(graph_fnm, "_be"));
    }
  } else {
    if (is_dir) {
      PNGraph graph;
      if (type == 0)
        graph = TSnap::LoadEdgeList<PNGraph>(graph_fnm);
      else if (type == 1)
        graph = TSnap::LoadBinaryEdgeList<PNGraph>(graph_fnm);
      else
        graph = TSnap::LoadBinary<PNGraph>(graph_fnm);
      printf("\nDirected graph loaded. Nodes:%d, Edges:%d\n", graph->GetNodes(),
             graph->GetEdges());

      if (!stat_opt.empty()) plotStatistics(stat_opt, graph, true, graph_fnm);
      if (!calc_opt.empty()) {
        calcStatistics(calc_opt, save_opt, parser, graph, true, graph_fnm);
        if (calc_opt.find('v') != string::npos) {
          const int Vals =
              graph->GetNodes() / 2 > 200 ? 200 : graph->GetNodes() / 2;
          TSnap::PlotSngValRank(graph, Vals, TStr::AddToFMid(graph_fnm, "_eig"),
                                "");
        }
        // singular vector
        if (calc_opt.find('V') != string::npos) {
          TSnap::PlotSngVec(graph, TStr::AddToFMid(graph_fnm, "_sng"), "");
        }
        if (calc_opt.find('s') != string::npos) {
          PNGraph scc = TSnap::GetMxScc<PNGraph>(graph);
          printf("Number of nodes in SCC: %d\n", scc->GetNodes());
          printf("Number of edges in SCC: %d\n", scc->GetEdges());
          saveGraph(graph, is_dir, save_opt, graph_fnm, "scc");
        }
      }
      if (!sample_opt.empty()) {
        int sample_size = parser.getInt("-num", 100, "sample size");
        bool wr = parser.getBool("-wr", false, "with replacement?");
        sampleNodes(graph, wr, sample_size, graph_fnm);
      }
    } else {
      PUNGraph graph;
      if (type == 0)
        graph = TSnap::LoadEdgeList<PUNGraph>(graph_fnm);
      else if (type == 1)
        graph = TSnap::LoadBinaryEdgeList<PUNGraph>(graph_fnm);
      else
        graph = TSnap::LoadBinary<PUNGraph>(graph_fnm);
      printf("\nUndirected graph loaded. Nodes:%d, Edges:%d\n",
             graph->GetNodes(), graph->GetEdges());

      if (!stat_opt.empty()) plotStatistics(stat_opt, graph, false, graph_fnm);
      if (!calc_opt.empty()) {
        calcStatistics(calc_opt, save_opt, parser, graph, false, graph_fnm);
        if (calc_opt.find('v') != string::npos) {
          const int Vals =
              graph->GetNodes() / 2 > 200 ? 200 : graph->GetNodes() / 2;
          TSnap::PlotEigValRank(graph, Vals, TStr::AddToFMid(graph_fnm, "_eig"),
                                "");
        }
      }
      if (!sample_opt.empty()) {
        int sample_size = parser.getInt("-num", 100, "sample size");
        bool wr = parser.getBool("-wr", false, "with replacement?");
        sampleNodes(graph, wr, sample_size, graph_fnm);
      }
    }
  }
  printf("\nTime duration: %s\n", ExeTm.GetTmStr());
  return 0;
}
