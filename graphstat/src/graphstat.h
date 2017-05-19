#include "stdafx.h"

void SaveBinaryEdgelist(const TStr& graph_fnm, const TStr& OutFNm) {
  PSOut pout =
      TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm) : TFOut::New(OutFNm);
  TSsParser ss(graph_fnm);
  while (ss.Next()) {
    int src = ss.GetInt(0), dst = ss.GetInt(1);
    pout->Save(src);
    pout->Save(dst);
  }
}

template <class PGraph>
void saveGraph(const PGraph graph, const bool is_dir, const string& save_opt,
               const TStr& graph_fnm, const TStr& sufix = "") {
  TStr filename = sufix.Empty() ? graph_fnm : TStr::AddToFMid(graph_fnm, sufix);
  if (save_opt.find('e') != string::npos) {
    TSnap::SaveEdgeList(graph, filename);
  } else if (save_opt.find('b') != string::npos) {
    filename = TStr::AddToFMid(filename, "_bin");
    TSnap::SaveBinary(graph, filename);
  } else if (save_opt.find('B') != string::npos) {
    filename = TStr::AddToFMid(filename, "_be");
    TSnap::SaveBinaryEdgelist(graph, filename);
  } else {  // do not save
    return;
  }
  printf("Saved to %s.\n", filename.CStr());
}

template <class PGraph>
void plotStatistics(const string& cmd_str, const PGraph graph,
                    const bool is_dir, const TStr& graph_fnm) {
  TStr prefix = graph_fnm.GetFMid();
  // degree distritution
  if (cmd_str.find('d') != string::npos) {
    if (is_dir) {
      TSnap::PlotOutDegDistr(graph, prefix + "_out_dist.dat", false);
      TSnap::PlotInDegDistr(graph, prefix + "_in_dist.dat", false);
    } else
      TSnap::PlotOutDegDistr(graph, prefix + "_dist.dat", false);
  }
  // CCDF
  if (cmd_str.find('c') != string::npos) {
    if (is_dir) {
      TSnap::PlotOutDegDistr(graph, prefix + "_out_ccdf.dat", true);
      TSnap::PlotInDegDistr(graph, prefix + "_in_ccdf.dat", true);
    } else
      TSnap::PlotOutDegDistr(graph, prefix + "_ccdf.dat", true);
  }
  // hop plot
  if (cmd_str.find('h') != string::npos)
    TSnap::PlotHops(graph, prefix + "_hops.dat", "", false, 32);
  // weakly connected component
  if (cmd_str.find('w') != string::npos)
    TSnap::PlotWccDistr(graph, prefix + "_wcc_dist.dat", "");
  // strongly connected component
  if (cmd_str.find('s') != string::npos)
    TSnap::PlotSccDistr(graph, prefix + "_scc_dist.dat", "");
  // clustering coefficient
  if (cmd_str.find('C') != string::npos)
    TSnap::PlotClustCf(graph, prefix + "_ccf_dist.dat", "");
}

template <class PGraph>
void calcStatistics(const string cmd_str, const string& save_opt,
                    const ArgsParser& parser, const PGraph graph,
                    const bool is_dir, const TStr& graph_fnm) {
  if (cmd_str.find('b') != string::npos) {
    printf("nodes:%d edges:%d\n", graph->GetNodes(), graph->GetEdges());
    saveGraph(graph, is_dir, save_opt, graph_fnm);
  }
  if (cmd_str.find('t') != string::npos) {
    int ctriads = TSnap::GetTriads(graph);
    printf("# closed triads: %d\n", ctriads);
  }
  if (cmd_str.find('d') != string::npos) {
    int maxHops = parser.getInt("-hops", 2);
    double sample = parser.getFlt("-sample", 0.01);
    bool fo = parser.getBool("-out", true);
    bool fi = parser.getBool("-in", false);
    int n = parser.getInt("-n", 1);
    double rst = TSnap::GetDissAbility(graph, fo, fi, maxHops, sample, n);
    printf("Dissemination(hops=%d, sample=%.4f, n=%d): %.4f\n", maxHops, sample,
           n, rst);
  }
  if (cmd_str.find('D') != string::npos) {
    int test_nodes = parser.getInt("-testnodes", 100);
    int dia = TSnap::GetBfsFullDiam(graph, test_nodes, true);
    printf("BFS diameter: %d\n", dia);
  }
  if (cmd_str.find('C') != string::npos) {
    bool sample = parser.getBool("-sample", true);
    int samples = sample ? (int)(graph->GetNodes() * 0.05) : -1;
    double cc = TSnap::GetClustCf(graph, samples);
    printf("Avg. clustering coefficient(sampled: %d): %.6f\n", sample, cc);
  }
  if (cmd_str.find('h') != string::npos) {
    double eff_diam = TSnap::GetAnfEffDiam(graph);
    printf("90%% effective diameter: %.4f\n", eff_diam);
  }
  if (cmd_str.find('w') != string::npos) {
    PGraph wcc = TSnap::GetMxWcc(graph);
    printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
    printf("Number of edges in WCC: %d\n", wcc->GetEdges());
    saveGraph(wcc, is_dir, save_opt, graph_fnm, "_wcc");
  }
}

void MapingNodes(const TStr& graph_fnm, const bool IsDir) {
  TStr OutFNm = TStr::AddToFMid(graph_fnm, "_mapped");
  PSOut FOutPt =
      TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm) : TFOut::New(OutFNm);
  TStrIntH nidmap;
  int uid = 0;
  TSsParser ss(graph_fnm);
  TStr srcstr, dststr;
  while (ss.Next()) {
    srcstr = ss.GetFld(0);
    dststr = ss.GetFld(1);
    if (!nidmap.IsKey(srcstr)) nidmap(srcstr) = (uid++);
    if (!nidmap.IsKey(dststr)) nidmap(dststr) = (uid++);
    FOutPt->PutStrLn(
        TStr::Fmt("%d\t%d", nidmap(srcstr).Val, nidmap(dststr).Val));
  }
  BIO::SaveStrIntH(nidmap, TStr::AddToFMid(graph_fnm, "_nmap"), "# Node\tNID");
  printf("Node range [0, %d]\n", uid - 1);
}

void SaveNodes(const TStr& graph_fnm, const int type) {
  TIntSet nodes;
  int nedge = 0;
  if (type == 0) {  // common edgelist
    TSsParser ss(graph_fnm);
    while (ss.Next()) {
      nodes.AddKey(ss.GetInt(0));
      nodes.AddKey(ss.GetInt(1));
      nedge++;
    }
  } else if (type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm) ? TZipIn::New(graph_fnm)
                                           : TFIn::New(graph_fnm);
    int node;
    while (!pin->Eof()) {
      pin->Load(node);
      nodes.AddKey(node);
      pin->Load(node);
      nodes.AddKey(node);
      nedge++;
    }
  }
  printf("Nodes: %d, Edges: %d\n", nodes.Len(), nedge);
  BIO::SaveIntSet(nodes, TStr::AddToFMid(graph_fnm, "_nodes"));
}

void getDegreeSequence(const TStr& graph_fnm, const bool is_dir) {
  int src, dst;
  TSsParser ss(graph_fnm);
  if (is_dir) {
    TIntH in_deg_seq, out_deg_seq;
    while (ss.Next()) {
      src = ss.GetInt(0);
      dst = ss.GetInt(1);
      out_deg_seq(src)++;
      in_deg_seq(dst)++;
    }
    BIO::SaveIntH(in_deg_seq, TStr::AddToFMid(graph_fnm, "_indegseq"));
    BIO::SaveIntH(out_deg_seq, TStr::AddToFMid(graph_fnm, "_outdegseq"));
  } else {
    TIntH deg_seq;
    while (ss.Next()) {
      src = ss.GetInt(0);
      dst = ss.GetInt(1);
      deg_seq(src)++;
      deg_seq(dst)++;
    }
    BIO::SaveIntH(deg_seq, TStr::AddToFMid(graph_fnm, "_degseq"));
  }
}

template <class PGraph>
void sampleNodes(const PGraph graph, const bool wr, const int num,
                 const TStr& graph_fnm) {
  TIntV samples, nodes;
  graph->GetNIdV(nodes);
  if (wr)
    TRandom::ChooseWithReplacement(nodes, num, samples);
  else
    TRandom::Choose(nodes, num, samples);
  TStr sufix = TStr::Fmt("_samples_%d", num);
  TStr fnm = TStr::AddToFMid(graph_fnm, sufix);
  BIO::SaveIntV(samples, fnm);
  printf("\nsaved to %s\n", fnm.CStr());
}
