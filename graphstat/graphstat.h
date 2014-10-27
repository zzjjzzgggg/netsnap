#include "stdafx.h"

void MapingNodes(const TStr& InFNm, const bool IsDir){
	printf("Mapping nodes...\n");
	TStr OutFNm = TStr::Fmt("%s_mapped%s", InFNm.GetFPath().CStr(), InFNm.GetFExt().CStr());
	PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm) : TFOut::New(OutFNm);
	if (IsDir) FOutPt->PutStrLn("# Directed graph: "+OutFNm);
	else FOutPt->PutStrLn("# Undirected graph (each unordered pair of nodes is saved once): " + OutFNm);

	TStrIntH nidmap;
	int uid=0;
	TSsParser Ss(InFNm);
	TStr srcstr, dststr;
	while(Ss.Next()){
		srcstr=Ss.GetFld(0);
		dststr=Ss.GetFld(1);
		if(!nidmap.IsKey(srcstr)) nidmap(srcstr)=(uid++);
		if(!nidmap.IsKey(dststr)) nidmap(dststr)=(uid++);
		FOutPt->PutStrLn(TStr::Fmt("%d\t%d", nidmap(srcstr).Val, nidmap(dststr).Val));
	}
	BIO::SaveStrIntH(nidmap, InFNm.GetFPath()+".nmap", "# Node\tNID");
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
	BIO::SaveIntSet(nodes, InFNm.GetFPath()+"_nodes.gz", TStr::Fmt("# Total: %d", nodes.Len()));
}

void ReverseEdgeDirection(const TStr& InFNm){
	int u, v;
	TStr OutFNm = TStr::Fmt("%s_reversed%s", InFNm.GetFPath().CStr(), InFNm.GetFExt().CStr());
	PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm) : TFOut::New(OutFNm);
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		u = Ss.GetInt(0); v = Ss.GetInt(1);
		FOutPt->PutStrLn(TStr::Fmt("%d\t%d", v, u));
	}
}

void RemoveSelfLoops(const TStr& InFNm){
	int u, v, n=0;
	TStr OutFNm = InFNm.GetFPath() + TStr::Fmt("%s_looprmed%s", InFNm.GetFMid().CStr(), InFNm.GetFExt().CStr());
	PSOut FOutPt = TZipOut::IsZipFNm(OutFNm) ? TZipOut::New(OutFNm) : TFOut::New(OutFNm);
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
