#include "stdafx.h"

void MapingNodes(const TStr& InFNm, const bool IsDir) {
    TStr OutFNm = TStr::AddToFMid(InFNm, "_mapped");
    PSOut FOutPt = TZipOut::IsZipFNm(OutFNm)
                       ? TZipOut::New(OutFNm)
                       : TFOut::New(OutFNm);

    TStrIntH nidmap;
    int uid = 0;
    TSsParser Ss(InFNm);
    TStr srcstr, dststr;
    while (Ss.Next()) {
        srcstr = Ss.GetFld(0);
        dststr = Ss.GetFld(1);
        if (!nidmap.IsKey(srcstr)) nidmap(srcstr) = (uid++);
        if (!nidmap.IsKey(dststr)) nidmap(dststr) = (uid++);
        FOutPt->PutStrLn(TStr::Fmt("%d\t%d",
                                   nidmap(srcstr).Val,
                                   nidmap(dststr).Val));
    }
    BIO::SaveStrIntH(nidmap, TStr::AddToFMid(InFNm, "_nmap"),
                     "# Node\tNID");
    printf("Node range [0, %d]\n", uid - 1);
}

void SaveNodes(const TStr& InFNm, const int Type) {
    TIntSet nodes;
    int nedge = 0;
    if (Type == 0) {  // common edgelist
        TSsParser Ss(InFNm);
        while (Ss.Next()) {
            nodes.AddKey(Ss.GetInt(0));
            nodes.AddKey(Ss.GetInt(1));
            nedge++;
        }
    } else if (Type == 1) {  // binary edgelist
        PSIn pin = TZipIn::IsZipFNm(InFNm)
                       ? TZipIn::New(InFNm)
                       : TFIn::New(InFNm);
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
    BIO::SaveIntSet(nodes, TStr::AddToFMid(InFNm, "_nodes"));
}

void ReverseEdgeDirection(const TStr& InFNm) {
    int u, v, n = 0;
    TStr OutFNm = TStr::AddToFMid(InFNm, "_reversed");
    PSOut FOutPt = TZipOut::IsZipFNm(OutFNm)
                       ? TZipOut::New(OutFNm)
                       : TFOut::New(OutFNm);
    TSsParser Ss(InFNm);
    while (Ss.Next()) {
        u = Ss.GetInt(0);
        v = Ss.GetInt(1);
        FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
        n++;
    }
    printf("total edges: %d\n", n);
}

void RemoveSelfLoops(const TStr& InFNm) {
    int u, v, n = 0;
    TStr OutFNm = TStr::AddToFMid(InFNm, "_looprmed");
    PSOut FOutPt = TZipOut::IsZipFNm(OutFNm)
                       ? TZipOut::New(OutFNm)
                       : TFOut::New(OutFNm);
    TSsParser Ss(InFNm);
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

void GetDirDegSeq(const TStr& InFNm, const int Type) {
    TIntH InDegSeq, OutDegSeq;
    int SrcNId, DstNId;
    if (Type == 0) {  // common edgelist
        TSsParser Ss(InFNm);
        while (Ss.Next()) {
            SrcNId = Ss.GetInt(0);
            DstNId = Ss.GetInt(1);
            OutDegSeq(SrcNId)++;
            InDegSeq(DstNId)++;
        }
    } else if (Type == 1) {  // binary edgelist
        PSIn pin = TZipIn::IsZipFNm(InFNm)
                       ? TZipIn::New(InFNm)
                       : TFIn::New(InFNm);
        while (!pin->Eof()) {
            pin->Load(SrcNId);
            pin->Load(DstNId);
            OutDegSeq(SrcNId)++;
            InDegSeq(DstNId)++;
        }
    }
    BIO::SaveIntH(InDegSeq,
                  TStr::AddToFMid(InFNm, "_indegseq"));
    BIO::SaveIntH(OutDegSeq,
                  TStr::AddToFMid(InFNm, "_outdegseq"));
}

void GetUnDirDegSeq(const TStr& InFNm, const int Type) {
    TIntH DegSeq;
    int SrcNId, DstNId;
    if (Type == 0) {  // common edgelist
        TSsParser Ss(InFNm);
        while (Ss.Next()) {
            SrcNId = Ss.GetInt(0);
            DstNId = Ss.GetInt(1);
            DegSeq(SrcNId)++;
            DegSeq(DstNId)++;
        }
    } else if (Type == 1) {  // binary edgelist
        PSIn pin = TZipIn::IsZipFNm(InFNm)
                       ? TZipIn::New(InFNm)
                       : TFIn::New(InFNm);
        while (!pin->Eof()) {
            pin->Load(SrcNId);
            pin->Load(DstNId);
            DegSeq(SrcNId)++;
            DegSeq(DstNId)++;
        }
    }
    BIO::SaveIntH(DegSeq, TStr::AddToFMid(InFNm, "_degseq"));
}

void FormatBinaryEdgelist(const TStr& InFNm,
                          const TStr& OutFNm) {
    PSOut pout = TZipOut::IsZipFNm(OutFNm)
                     ? TZipOut::New(OutFNm)
                     : TFOut::New(OutFNm);
    TSsParser Ss(InFNm);
    while (Ss.Next()) {
        int src = Ss.GetInt(0), dst = Ss.GetInt(1);
        pout->Save(src);
        pout->Save(dst);
    }
}
