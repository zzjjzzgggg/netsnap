#include "stdafx.h"

void MapingNodes(const TStr& InFNm, const bool IsDir){
	TStr OutFNm = TStr::AddToFMid(InFNm, "_mapped");
	PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ?
		TZipOut::New(OutFNm) : TFOut::New(OutFNm);

	TStrIntH nidmap;
	int uid=0;
	TSsParser Ss(InFNm);
	TStr srcstr, dststr;
	while(Ss.Next()){
		srcstr=Ss.GetFld(0);
		dststr=Ss.GetFld(1);
		if(!nidmap.IsKey(srcstr)) nidmap(srcstr)=(uid++);
		if(!nidmap.IsKey(dststr)) nidmap(dststr)=(uid++);
		FOutPt->PutStrLn(TStr::Fmt("%d\t%d", nidmap(srcstr).Val,
								   nidmap(dststr).Val));
	}
	BIO::SaveStrIntH(nidmap, TStr::AddToFMid(InFNm, "_nmap"), "# Node\tNID");
	printf("Node range [0, %d]\n", uid-1);
}

void SaveNodes(const TStr& InFNm){
	TIntSet nodes; int nedge=0;
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		nodes.AddKey(Ss.GetInt(0));
		nodes.AddKey(Ss.GetInt(1));
		nedge++;
	}
	printf("Nodes: %d, Edges: %d\n", nodes.Len(), nedge);
	BIO::SaveIntSet(nodes, TStr::AddToFMid(InFNm, "_nodes.gz"),
					TStr::Fmt("# Total: %d", nodes.Len()));
}

void ReverseEdgeDirection(const TStr& InFNm){
	int u, v, n=0;
	TStr OutFNm = TStr::AddToFMid(InFNm, "_reversed");
	PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ?
		TZipOut::New(OutFNm) : TFOut::New(OutFNm);
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		u = Ss.GetInt(0); v = Ss.GetInt(1);
		FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
		n++;
	}
	printf("total edges: %d\n", n);
}

void RemoveSelfLoops(const TStr& InFNm){
	int u, v, n=0;
	TStr OutFNm = TStr::AddToFMid(InFNm, "_looprmed");
	PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ?
		TZipOut::New(OutFNm) : TFOut::New(OutFNm);
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		u = Ss.GetInt(0); v = Ss.GetInt(1);
		if (u!=v) {
			FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
			n++;
		}
	}
	printf("Number of edges without selfloops: %d\n", n);
}


void GetDegSeq(const TStr& InFNm, const bool IsDir) {
	TSsParser Ss(InFNm);
	if(IsDir) {
		TIntH InDegSeq, OutDegSeq;
		while(Ss.Next()) {
			OutDegSeq(Ss.GetInt(0))++;
			InDegSeq(Ss.GetInt(1))++;
		}
		BIO::SaveIntH(InDegSeq, TStr::AddToFMid(InFNm, "_indegseq"),
					  "#node in-degree");
		BIO::SaveIntH(OutDegSeq, TStr::AddToFMid(InFNm, "_outdegseq"),
					  "#node out-degree");
	} else {
		printf("NOTE: Assume an edge is stored only once.\n");
		TIntH DegSeq;
		while(Ss.Next()) {
			DegSeq(Ss.GetInt(0))++;
			DegSeq(Ss.GetInt(1))++;
		}
		BIO::SaveIntH(DegSeq, TStr::AddToFMid(InFNm, "_degseq"),
					  "#node degree");
	}
}
