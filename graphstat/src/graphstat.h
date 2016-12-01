#include "stdafx.h"

void SaveBinaryEdgelist(const TStr& graph_fnm,
                        const TStr& OutFNm) {
  PSOut pout = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm)
                                         : TFOut::New(OutFNm);
  TSsParser Ss(graph_fnm);
  while (Ss.Next()) {
    int src = Ss.GetInt(0), dst = Ss.GetInt(1);
    pout->Save(src);
    pout->Save(dst);
  }
}

template <class PGraph>
void saveGraph(const PGraph graph, const bool is_dir,
               const string& save_opt, const TStr& graph_fnm,
               const TStr& sufix = "") {
  TStr filename = sufix.Empty()
                      ? graph_fnm
                      : TStr::AddToFMid(graph_fnm, sufix);
  if (save_opt.find('e') != string::npos) {
    filename = is_dir ? TStr::AddToFMid(filename, "_digraph")
                      : TStr::AddToFMid(filename, "_graph");
    TSnap::SaveEdgeList(graph, filename);
  } else if (save_opt.find('b') != string::npos) {
    filename = is_dir ? TStr::AddToFMid(filename, "_bin_digraph")
                      : TStr::AddToFMid(filename, "_bin_graph");
    TSnap::SaveBinary(graph, filename);
  } else if (save_opt.find('B') != string::npos) {
    filename = is_dir ? TStr::AddToFMid(filename, "_be_digraph")
                      : TStr::AddToFMid(filename, "_be_graph");
    TSnap::SaveBinaryEdgelist(graph, filename);
  }
}

template <class PGraph>
void plotStatistics(const string& cmd_opt, const PGraph graph,
                    const bool is_dir, const TStr& graph_fnm) {
  // degree distritution
  if (cmd_opt.find('d') != string::npos) {
    if (is_dir) {
      TSnap::PlotOutDegDistr(
          graph, TStr::AddToFMid(graph_fnm, "_out_pdf"), "",
          false, false);
      TSnap::PlotInDegDistr(graph,
                            TStr::AddToFMid(graph_fnm, "_in_pdf"),
                            "", false, false);
    } else {
      TSnap::PlotOutDegDistr(graph,
                             TStr::AddToFMid(graph_fnm, "_pdf"),
                             "", false, false);
    }
  }
  // complementary cummulative distritution
  if (cmd_opt.find('c') != string::npos) {
    if (is_dir) {
      TSnap::PlotOutDegDistr(
          graph, TStr::AddToFMid(graph_fnm, "_out_ccdf"), "",
          true, false);
      TSnap::PlotInDegDistr(
          graph, TStr::AddToFMid(graph_fnm, "_in_ccdf"), "", true,
          false);
    } else {
      TSnap::PlotOutDegDistr(graph,
                             TStr::AddToFMid(graph_fnm, "_ccdf"),
                             "", true, false);
    }
  }
  // hop plot
  if (cmd_opt.find('h') != string::npos) {
    TSnap::PlotHops(graph, TStr::AddToFMid(graph_fnm, "_hops"),
                    "", false, 32);
  }
  // weakly connected component
  if (cmd_opt.find('w') != string::npos) {
    TSnap::PlotWccDistr(
        graph, TStr::AddToFMid(graph_fnm, "_wcc_dist"), "");
  }
  // strongly connected component
  if (cmd_opt.find('s') != string::npos) {
    TSnap::PlotSccDistr(
        graph, TStr::AddToFMid(graph_fnm, "_scc_dist"), "");
  }
  // clustering coefficient
  if (cmd_opt.find('C') != string::npos) {
    TSnap::PlotClustCf(
        graph, TStr::AddToFMid(graph_fnm, "_ccf_dist"), "");
  }
}

template <class PGraph>
void calcStatistics(const string cmd_opt, const string& save_opt,
                    const ArgsParser& parser, const PGraph graph,
                    const bool is_dir, const TStr& graph_fnm) {
  if (cmd_opt.find('b') != string::npos) {
    printf("nodes:%d edges:%d\n", graph->GetNodes(),
           graph->GetEdges());
    saveGraph(graph, is_dir, save_opt, graph_fnm);
  }
  if (cmd_opt.find('t') != string::npos) {
    int ctriads = TSnap::GetTriads(graph);
    printf("# closed triads: %d\n", ctriads);
  }
  if (cmd_opt.find('d') != string::npos) {
    int maxHops = parser.getInt("-hops", 2);
    double sample = parser.getFlt("-sample", 0.01);
    bool fo = parser.getBool("-out", true);
    bool fi = parser.getBool("-in", false);
    int n = parser.getInt("-n", 1);
    double rst =
        TSnap::GetDissAbility(graph, fo, fi, maxHops, sample, n);
    printf("Dissemination(hops=%d, sample=%.4f, n=%d): %.4f\n",
           maxHops, sample, n, rst);
  }
  if (cmd_opt.find('D') != string::npos) {
    int test_nodes = parser.getInt("-testnodes", 100);
    int dia = TSnap::GetBfsFullDiam(graph, test_nodes, true);
    printf("BFS diameter: %d\n", dia);
  }
  if (cmd_opt.find('C') != string::npos) {
    bool sample = parser.getBool("-sample", true);
    int samples = sample ? (int)(graph->GetNodes() * 0.05) : -1;
    double cc = TSnap::GetClustCf(graph, samples);
    printf("Avg. clustering coefficient(sampled: %d): %.6f\n",
           sample, cc);
  }
  if (cmd_opt.find('h') != string::npos) {
    double eff_diam = TSnap::GetAnfEffDiam(graph);
    printf("90%% effective diameter: %.4f\n", eff_diam);
  }
  if (cmd_opt.find('w') != string::npos) {
      PGraph wcc = TSnap::GetMxWcc(graph);
      printf("Number of nodes in WCC: %d\n", wcc->GetNodes());
      printf("Number of edges in WCC: %d\n", wcc->GetEdges());
      saveGraph(graph, is_dir, save_opt, graph_fnm, "wcc");
  }
}

void MapingNodes(const TStr& graph_fnm, const bool IsDir) {
  TStr OutFNm = TStr::AddToFMid(graph_fnm, "_mapped");
  PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm)
                                           : TFOut::New(OutFNm);

  TStrIntH nidmap;
  int uid = 0;
  TSsParser Ss(graph_fnm);
  TStr srcstr, dststr;
  while (Ss.Next()) {
    srcstr = Ss.GetFld(0);
    dststr = Ss.GetFld(1);
    if (!nidmap.IsKey(srcstr)) nidmap(srcstr) = (uid++);
    if (!nidmap.IsKey(dststr)) nidmap(dststr) = (uid++);
    FOutPt->PutStrLn(TStr::Fmt("%d\t%d", nidmap(srcstr).Val,
                               nidmap(dststr).Val));
  }
  BIO::SaveStrIntH(nidmap, TStr::AddToFMid(graph_fnm, "_nmap"),
                   "# Node\tNID");
  printf("Node range [0, %d]\n", uid - 1);
}

void SaveNodes(const TStr& graph_fnm, const int Type) {
  TIntSet nodes;
  int nedge = 0;
  if (Type == 0) {  // common edgelist
    TSsParser Ss(graph_fnm);
    while (Ss.Next()) {
      nodes.AddKey(Ss.GetInt(0));
      nodes.AddKey(Ss.GetInt(1));
      nedge++;
    }
  } else if (Type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm)
                   ? TZipIn::New(graph_fnm)
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

void ReverseEdgeDirection(const TStr& graph_fnm) {
  int u, v, n = 0;
  TStr OutFNm = TStr::AddToFMid(graph_fnm, "_reversed");
  PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm)
                                           : TFOut::New(OutFNm);
  TSsParser Ss(graph_fnm);
  while (Ss.Next()) {
    u = Ss.GetInt(0);
    v = Ss.GetInt(1);
    FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
    n++;
  }
  printf("total edges: %d\n", n);
}

void RemoveSelfLoops(const TStr& graph_fnm) {
  int u, v, n = 0;
  TStr OutFNm = TStr::AddToFMid(graph_fnm, "_looprmed");
  PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm)
                                           : TFOut::New(OutFNm);
  TSsParser Ss(graph_fnm);
  while (Ss.Next()) {
    u = Ss.GetInt(0);
    v = Ss.GetInt(1);
    if (u != v) {
      FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
      n++;
    }
  }
  printf("Number of edges without selfloops: %d\n", n);
}

void GetDirDegSeq(const TStr& graph_fnm, const int Type) {
  TIntH InDegSeq, OutDegSeq;
  int SrcNId, DstNId;
  if (Type == 0) {  // common edgelist
    TSsParser Ss(graph_fnm);
    while (Ss.Next()) {
      SrcNId = Ss.GetInt(0);
      DstNId = Ss.GetInt(1);
      OutDegSeq(SrcNId)++;
      InDegSeq(DstNId)++;
    }
  } else if (Type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm)
                   ? TZipIn::New(graph_fnm)
                   : TFIn::New(graph_fnm);
    while (!pin->Eof()) {
      pin->Load(SrcNId);
      pin->Load(DstNId);
      OutDegSeq(SrcNId)++;
      InDegSeq(DstNId)++;
    }
  }
  BIO::SaveIntH(InDegSeq,
                TStr::AddToFMid(graph_fnm, "_indegseq"));
  BIO::SaveIntH(OutDegSeq,
                TStr::AddToFMid(graph_fnm, "_outdegseq"));
}

void GetUnDirDegSeq(const TStr& graph_fnm, const int Type) {
  TIntH DegSeq;
  int SrcNId, DstNId;
  if (Type == 0) {  // common edgelist
    TSsParser Ss(graph_fnm);
    while (Ss.Next()) {
      SrcNId = Ss.GetInt(0);
      DstNId = Ss.GetInt(1);
      DegSeq(SrcNId)++;
      DegSeq(DstNId)++;
    }
  } else if (Type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm)
                   ? TZipIn::New(graph_fnm)
                   : TFIn::New(graph_fnm);
    while (!pin->Eof()) {
      pin->Load(SrcNId);
      pin->Load(DstNId);
      DegSeq(SrcNId)++;
      DegSeq(DstNId)++;
    }
  }
  BIO::SaveIntH(DegSeq, TStr::AddToFMid(graph_fnm, "_degseq"));
}

template <class PGraph>
void sampleNodes(const PGraph graph, const int num,
                 const TStr& graph_fnm) {
  TIntV samples, nodes;
  graph->GetNIdV(nodes);
  TRandom::ChooseWithReplacement(nodes, num, samples);
  BIO::SaveIntV(samples, TStr::AddToFMid(graph_fnm, "_samples"));
}
