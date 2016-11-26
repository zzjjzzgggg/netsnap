#include "graphstat.h"

int main(int argc, char *argv[]) {
    TExeTm ExeTm;
    Env = TEnv(argc, argv, TNotify::StdNotify);
    Env.PrepArgs(TStr::Fmt("Build: %s, %s. Time: %s",
                           __TIME__, __DATE__,
                           TExeTm::GetCurTm()));
    const TStr InFNm = Env.GetIfArgPrefixStr(
        "-i:", "graph.txt", "Input graph");
    const int Type = Env.GetIfArgPrefixInt(
        "-t:", 0,
        "0: edgelist, 1: binary edgelist, 2: binary");
    const bool IsDir =
        Env.GetIfArgPrefixBool("-d:", true, "Directed graph");
    const TStr Plot = Env.GetIfArgPrefixStr(
        "-p:", "",
        "What statistics to plot:"
        "\n\tc: cummulative degree distribution"
        "\n\td: degree distribution"
        "\n\th: hop plot (diameter)"
        "\n\tw: dist. of weakly connected components"
        "\n\ts: dist. of strongly connected components"
        "\n\tC: clustering coefficient"
        "\n\tv: singular values"
        "\n\tV: left and right singular vector\n\t");
    const TStr Calcs = Env.GetIfArgPrefixStr(
        "-c:", "",
        "What statistics to calculate: (will load into mem)"
        "\n\tb: basic statistics"
        "\n\tt: count closed triads"
        "\n\td: dissemination ability (-hops:2 -sample:0.01 "
        "-fo:T "
        "-fi:F -n:1)"
        "\n\tD: BFS diameter (-testnodes:100)"
        "\n\tC: average clustering coefficient (-sample:T, "
        "default "
        "5%)"
        "\n\th: hops (10% effective diameter)"
        "\n\tw: largest weakly connected components"
        "\n\ts: largest strongly connected components\n\t");
    const TStr Fmts = Env.GetIfArgPrefixStr(
        "-f:", "",
        "How to format the graph: (no need to load into mem)"
        "\n\tb: convert to binary format (for the purpose of "
        "fast "
        "loading)"
        "\n\te: convert to binary edgelist"
        "\n\tm: maping graph (assign nid from 0)"
        "\n\tl: remove self-loops"
        "\n\tn: save nodes in a file"
        "\n\td: get degree sequence, e.g., (node degree)"
        "\n\tr: reverse the edge direction\n\t");
    const TStr Save = Env.GetIfArgPrefixStr("-s:", "",
                                            "Save graph:"
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
    bool Cal_w = Calcs.SearchCh('w') != -1;
    bool Cal_s = Calcs.SearchCh('s') != -1;

    bool Fmt_b =
        Fmts.SearchCh('b') != -1;  // need to load graph
    bool Fmt_e = Fmts.SearchCh('e') != -1;
    bool Fmt_m = Fmts.SearchCh('m') != -1;
    bool Fmt_l = Fmts.SearchCh('l') != -1;
    bool Fmt_n = Fmts.SearchCh('n') != -1;
    bool Fmt_r = Fmts.SearchCh('r') != -1;
    bool Fmt_d = Fmts.SearchCh('d') != -1;

    bool Sav_e = Save.SearchCh('e') != -1;
    bool Sav_b = Save.SearchCh('b') != -1;
    if (Env.IsEndOfRun()) return 0;

    // format graph, no need to load graph file
    if (Fmts.Len() != 0 && !Fmt_b) {
        // re-mapping nodes
        if (Fmt_m) {
            printf("Mapping nodes ...\n");
            MapingNodes(InFNm, IsDir);
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
        if (Fmt_n) SaveNodes(InFNm, Type);

        if (Fmt_d) {
            printf("Geting degree sequence ...\n");
            if (IsDir)
                GetDirDegSeq(InFNm, Type);
            else
                GetUnDirDegSeq(InFNm, Type);
        }

        if (Fmt_e) {
            printf("Formating to binary edgelist ...\n");
            FormatBinaryEdgelist(
                InFNm, TStr::AddToFMid(InFNm, "_be"));
        }

    } else {
        if (IsDir) {
            PNGraph Graph;
            if (Type == 0)
                Graph = TSnap::LoadEdgeList<PNGraph>(InFNm);
            else if (Type == 1)
                Graph =
                    TSnap::LoadBinaryEdgeList<PNGraph>(InFNm);
            else
                Graph = TSnap::LoadBinary<PNGraph>(InFNm);
            printf(
                "============================================"
                "\n\n"
                "Directed graph is loaded. Nodes:%d, "
                "Edges:%d\n\n",
                Graph->GetNodes(), Graph->GetEdges());
            if (PlotDD) {
                TSnap::PlotOutDegDistr(
                    Graph, TStr::AddToFMid(InFNm, "_out_pdf"),
                    "", false, false);
                TSnap::PlotInDegDistr(
                    Graph, TStr::AddToFMid(InFNm, "_in_pdf"),
                    "", false, false);
            }
            if (PlotCDD) {
                TSnap::PlotOutDegDistr(
                    Graph,
                    TStr::AddToFMid(InFNm, "_out_ccdf"), "",
                    true, false);
                TSnap::PlotInDegDistr(
                    Graph, TStr::AddToFMid(InFNm, "_in_ccdf"),
                    "", true, false);
            }
            if (PlotHop)
                TSnap::PlotHops(
                    Graph, TStr::AddToFMid(InFNm, "_hops"),
                    "", false, 32);
            if (PlotWcc)
                TSnap::PlotWccDistr(
                    Graph,
                    TStr::AddToFMid(InFNm, "_wcc_dist"), "");
            if (PlotScc)
                TSnap::PlotSccDistr(
                    Graph,
                    TStr::AddToFMid(InFNm, "_scc_dist"), "");
            if (PlotClustCf)
                TSnap::PlotClustCf(
                    Graph, TStr::AddToFMid(InFNm, "_ccf"),
                    "");
            if (PlotSVal) {
                const int Vals = Graph->GetNodes() / 2 > 200
                                     ? 200
                                     : Graph->GetNodes() / 2;
                TSnap::PlotSngValRank(
                    Graph, Vals,
                    TStr::AddToFMid(InFNm, "_sng"), "");
            }
            if (PlotSVec)
                TSnap::PlotSngVec(
                    Graph, TStr::AddToFMid(InFNm, "_sng"),
                    "");
            if (Cal_b) {
                printf("nodes:%d edges:%d\n",
                       Graph->GetNodes(), Graph->GetEdges());
                if (Sav_e)
                    TSnap::SaveEdgeList(
                        Graph,
                        TStr::AddToFMid(InFNm, "_digraph"));
                else if (Sav_b)
                    TSnap::SaveBinary(
                        Graph, TStr::AddToFMid(
                                   InFNm, "_bin_digraph"));
            }
            if (Cal_t) {
                int ctriads = TSnap::GetTriads(Graph);
                printf("# closed triads: %d\n", ctriads);
            }
            if (Cal_d) {
                int maxHops =
                    Env.GetIfArgPrefixInt("-hops:", 2);
                double sample =
                    Env.GetIfArgPrefixFlt("-sample:", 0.01);
                bool fo =
                    Env.GetIfArgPrefixBool("-fo:", true);
                bool fi =
                    Env.GetIfArgPrefixBool("-fi:", false);
                int n = Env.GetIfArgPrefixInt("-n:", 1);
                double rst = TSnap::GetDissAbility(
                    Graph, fo, fi, maxHops, sample, n);
                printf(
                    "Dissemination ability(hops=%d, "
                    "sample=%.4f, "
                    "n=%d): %.4f\n",
                    maxHops, sample, n, rst);
            }
            if (Cal_D) {
                int test_nodes =
                    Env.GetIfArgPrefixInt("-testnodes:", 100);
                int dia = TSnap::GetBfsFullDiam<PNGraph>(
                    Graph, test_nodes, true);
                printf("BFS diameter: %d\n", dia);
            }
            if (Cal_C) {
                bool sample =
                    Env.GetIfArgPrefixBool("-sample:", true);
                int samples =
                    sample ? (int)(Graph->GetNodes() * 0.05)
                           : -1;
                double cc = TSnap::GetClustCf(Graph, samples);
                printf(
                    "Average clustering coefficient(sampled: "
                    "%d): "
                    "%.6f\n",
                    sample, cc);
            }
            if (Cal_h) {
                double eff_diam = TSnap::GetAnfEffDiam(Graph);
                printf("90%% effective diameter: %.4f\n",
                       eff_diam);
            }
            if (Cal_w) {
                PNGraph wcc = TSnap::GetMxWcc<PNGraph>(Graph);
                printf("Number of nodes in WCC: %d\n",
                       wcc->GetNodes());
                printf("Number of edges in WCC: %d\n",
                       wcc->GetEdges());
                if (Sav_e)
                    TSnap::SaveEdgeList(
                        wcc, TStr::AddToFMid(InFNm, "_wcc"));
                else if (Sav_b)
                    TSnap::SaveBinary(
                        wcc,
                        TStr::AddToFMid(InFNm, "_wcc_bin"));
            }
            if (Cal_s) {
                PNGraph scc = TSnap::GetMxScc<PNGraph>(Graph);
                printf("Number of nodes in SCC: %d\n",
                       scc->GetNodes());
                printf("Number of edges in SCC: %d\n",
                       scc->GetEdges());
                if (Sav_e)
                    TSnap::SaveEdgeList(
                        scc, TStr::AddToFMid(InFNm, "_scc"));
                else if (Sav_b)
                    TSnap::SaveBinary(
                        scc,
                        TStr::AddToFMid(InFNm, "_scc_bin"));
            }
            if (Fmt_b) {
                printf("Converting to binary format ...\n");
                TSnap::SaveBinary<PNGraph>(
                    Graph, TStr::AddToFMid(InFNm, "_bin"));
            }
        } else {
            PUNGraph Graph;
            if (Type == 0)
                Graph = TSnap::LoadEdgeList<PUNGraph>(InFNm);
            else if (Type == 1)
                Graph = TSnap::LoadBinaryEdgeList<PUNGraph>(
                    InFNm);
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
                TSnap::PlotOutDegDistr(
                    Graph, TStr::AddToFMid(InFNm, "_pdf"), "",
                    false, false);
            if (PlotCDD)
                TSnap::PlotOutDegDistr(
                    Graph, TStr::AddToFMid(InFNm, "_ccdf"),
                    "", true, false);
            if (PlotHop)
                TSnap::PlotHops(
                    Graph, TStr::AddToFMid(InFNm, "_hops"),
                    "", false, 32);
            if (PlotWcc)
                TSnap::PlotWccDistr(
                    Graph,
                    TStr::AddToFMid(InFNm, "_wcc_dist"), "");
            if (PlotClustCf)
                TSnap::PlotClustCf(
                    Graph,
                    TStr::AddToFMid(InFNm, "_ccf_dist"), "");
            if (PlotSVal) {
                const int Vals = Graph->GetNodes() / 2 > 200
                                     ? 200
                                     : Graph->GetNodes() / 2;
                TSnap::PlotEigValRank(
                    Graph, Vals,
                    TStr::AddToFMid(InFNm, "_eig"), "");
            }
            if (Cal_b) {
                printf("nodes:%d  edges:%d\n",
                       Graph->GetNodes(), Graph->GetEdges());
                if (Sav_e)
                    TSnap::SaveEdgeList(
                        Graph,
                        TStr::AddToFMid(InFNm, "_graph"));
                else if (Sav_b)
                    TSnap::SaveBinary(
                        Graph,
                        TStr::AddToFMid(InFNm, "_bin_graph"));
            }
            if (Cal_t) {
                int ctriads = TSnap::GetTriads(Graph);
                printf("# closed triads: %d\n", ctriads);
            }
            if (Cal_d) {
                int maxHops =
                    Env.GetIfArgPrefixInt("-hops:", 2);
                double sample =
                    Env.GetIfArgPrefixFlt("-sample:", 0.01);
                bool fo =
                    Env.GetIfArgPrefixBool("-fo:", true);
                bool fi =
                    Env.GetIfArgPrefixBool("-fi:", false);
                int n = Env.GetIfArgPrefixInt("-n:", 1);
                double rst = TSnap::GetDissAbility(
                    Graph, fo, fi, maxHops, sample, n);
                printf(
                    "Dissemination ability(hops=%d, "
                    "sample=%.4f, "
                    "n=%d): %.4f\n",
                    maxHops, sample, n, rst);
            }
            if (Cal_D) {
                int test_nodes =
                    Env.GetIfArgPrefixInt("-testnodes:", 100);
                int dia = TSnap::GetBfsFullDiam<PUNGraph>(
                    Graph, test_nodes);
                printf("BFS diameter: %d\n", dia);
            }
            if (Cal_C) {
                bool sample =
                    Env.GetIfArgPrefixBool("-sample:", true);
                int samples =
                    sample ? (int)(Graph->GetNodes() * 0.05)
                           : -1;
                double cc = TSnap::GetClustCf(Graph, samples);
                printf(
                    "Average clustering coefficient(sampled: "
                    "%d): "
                    "%.6f\n",
                    sample, cc);
            }
            if (Cal_h) {
                double eff_diam = TSnap::GetAnfEffDiam(Graph);
                printf("90%% effective diameter: %.4f\n",
                       eff_diam);
            }
            if (Cal_w) {
                PUNGraph wcc =
                    TSnap::GetMxWcc<PUNGraph>(Graph);
                printf("Number of nodes in WCC: %d\n",
                       wcc->GetNodes());
                printf("Number of edges in WCC: %d\n",
                       wcc->GetEdges());
                if (Sav_e)
                    TSnap::SaveEdgeList(
                        wcc, TStr::AddToFMid(InFNm, "_wcc"));
                else if (Sav_b)
                    TSnap::SaveBinary(
                        wcc,
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
