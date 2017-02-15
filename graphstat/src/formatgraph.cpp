#include "stdafx.h"

void mappingNodes(const string& graph_fnm, const bool is_dir,
                  const string& out_fnm_opt) {
  string out_fnm =
      out_fnm_opt.empty()
          ? strutils::insertMiddle(graph_fnm, "_mapped")
          : out_fnm_opt;
  PSOut FOutPt = TZipOut::IsZipFNm(out_fnm.c_str())
                     ? TZipOut::New(out_fnm.c_str())
                     : TFOut::New(out_fnm.c_str());
  TStrIntH nidmap;
  int uid = 0;
  TSsParser Ss(graph_fnm.c_str());
  TStr srcstr, dststr;
  while (Ss.Next()) {
    srcstr = Ss.GetFld(0);
    dststr = Ss.GetFld(1);
    if (!nidmap.IsKey(srcstr)) nidmap(srcstr) = (uid++);
    if (!nidmap.IsKey(dststr)) nidmap(dststr) = (uid++);
    FOutPt->PutStrLn(TStr::Fmt("%d\t%d", nidmap(srcstr).Val,
                               nidmap(dststr).Val));
  }
  BIO::SaveStrIntH(
      nidmap,
      strutils::insertMiddle(graph_fnm, "_nmap").c_str(),
      "# Node\tNID");
  printf("Node range [0, %d]\n", uid - 1);
}

void saveNodes(const string& graph_fnm, const int Type) {
  TIntSet nodes;
  int nedge = 0;
  if (Type == 0) {  // common edgelist
    TSsParser Ss(graph_fnm.c_str());
    while (Ss.Next()) {
      nodes.AddKey(Ss.GetInt(0));
      nodes.AddKey(Ss.GetInt(1));
      nedge++;
    }
  } else if (Type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm.c_str())
                   ? TZipIn::New(graph_fnm.c_str())
                   : TFIn::New(graph_fnm.c_str());
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
  BIO::SaveIntSet(
      nodes,
      strutils::insertMiddle(graph_fnm, "_nodes").c_str());
}

void ReverseEdgeDirection(const string& graph_fnm) {
  int u, v, n = 0;
  string out_fnm =
      strutils::insertMiddle(graph_fnm, "_reversed");
  PSOut FOutPt = TZipOut::IsZipFNm(out_fnm.c_str())
                     ? TZipOut::New(out_fnm.c_str())
                     : TFOut::New(out_fnm.c_str());
  TSsParser Ss(graph_fnm.c_str());
  while (Ss.Next()) {
    u = Ss.GetInt(0);
    v = Ss.GetInt(1);
    FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
    n++;
  }
  printf("total edges: %d\n", n);
}

void removeSelfLoops(const string& graph_fnm) {
  int u, v, n = 0;
  string out_fnm =
      strutils::insertMiddle(graph_fnm, "_looprmed");
  PSOut FOutPt = TZipOut::IsZipFNm(out_fnm.c_str())
                     ? TZipOut::New(out_fnm.c_str())
                     : TFOut::New(out_fnm.c_str());
  TSsParser Ss(graph_fnm.c_str());
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

void getDirDegSeq(const string& graph_fnm, const int Type) {
  TIntH InDegSeq, OutDegSeq;
  int SrcNId, DstNId;
  if (Type == 0) {  // common edgelist
    TSsParser Ss(graph_fnm.c_str());
    while (Ss.Next()) {
      SrcNId = Ss.GetInt(0);
      DstNId = Ss.GetInt(1);
      OutDegSeq(SrcNId)++;
      InDegSeq(DstNId)++;
    }
  } else if (Type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm.c_str())
                   ? TZipIn::New(graph_fnm.c_str())
                   : TFIn::New(graph_fnm.c_str());
    while (!pin->Eof()) {
      pin->Load(SrcNId);
      pin->Load(DstNId);
      OutDegSeq(SrcNId)++;
      InDegSeq(DstNId)++;
    }
  }
  BIO::SaveIntH(
      InDegSeq,
      strutils::insertMiddle(graph_fnm, "_indegseq").c_str());
  BIO::SaveIntH(OutDegSeq, strutils::insertMiddle(
                               graph_fnm, "_outdegseq")
                               .c_str());
}

void getUnDirDegSeq(const string& graph_fnm, const int Type) {
  TIntH DegSeq;
  int SrcNId, DstNId;
  if (Type == 0) {  // common edgelist
    TSsParser Ss(graph_fnm.c_str());
    while (Ss.Next()) {
      SrcNId = Ss.GetInt(0);
      DstNId = Ss.GetInt(1);
      DegSeq(SrcNId)++;
      DegSeq(DstNId)++;
    }
  } else if (Type == 1) {  // binary edgelist
    PSIn pin = TZipIn::IsZipFNm(graph_fnm.c_str())
                   ? TZipIn::New(graph_fnm.c_str())
                   : TFIn::New(graph_fnm.c_str());
    while (!pin->Eof()) {
      pin->Load(SrcNId);
      pin->Load(DstNId);
      DegSeq(SrcNId)++;
      DegSeq(DstNId)++;
    }
  }
  BIO::SaveIntH(
      DegSeq,
      strutils::insertMiddle(graph_fnm, "_degseq").c_str());
}

int main(int argc, char* argv[]) {
  Timer tm;
  ArgsParser parser(argc, argv);
  string graph_fnm =
      parser.getStr("-graph", "graph.txt", "input graph");

  int type = parser.getInt("-type", 0,
                           "0: edgelist\n"
                           "1: binary edgelist\n"
                           "2: binary");
  bool is_dir = parser.getBool("-dir", true, "is directed?");
  string out_fnm =
      parser.getStr("-out", "", "ouput filename");
  string format_opt = parser.getStr(
      "-format", "",
      "How to format the graph: (no need to load into mem)\n"
      "e: convert to binary edgelist\n"
      "m: maping graph (assign nid from 0)\n"
      "l: remove self-loops\n"
      "n: save nodes in a file\n"
      "d: get degree sequence, e.g., (node degree)\n"
      "r: reverse the edge direction");
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
      mappingNodes(graph_fnm, is_dir, out_fnm);
    }
    // reverse edge direction
    if (format_opt.find('r') != string::npos) {
      printf("Reversing the edge direction ...\n");
      ReverseEdgeDirection(graph_fnm);
    }
    // remove self-loops
    if (format_opt.find('l') != string::npos) {
      printf("Removing self-loops ...\n");
      removeSelfLoops(graph_fnm);
    }
    // save nodes in the graph
    if (format_opt.find('n') != string::npos)
      saveNodes(graph_fnm, type);
    // degree sequence
    if (format_opt.find('d') != string::npos) {
      printf("Geting degree sequence ...\n");
      if (is_dir)
        getDirDegSeq(graph_fnm, type);
      else
        getUnDirDegSeq(graph_fnm, type);
    }
    // save binary edgelist
    if (format_opt.find('e') != string::npos) {
      printf("Formating to binary edgelist ...\n");
      toBinaryEdgelist(graph_fnm, strutils::insertMiddle(
                                      graph_fnm, "_be"));
    }

    printf("\nTime duration: %s\n", tm.getStr().c_str());
    return 0;
  }
