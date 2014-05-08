#include "stdafx.h"

void MapingNodes(const TStr& InFNm, const TStr& OutFNm, const bool IsDir){
	printf("Mapping nodes...\n");
	TStr gfnm=OutFNm + (IsDir?".digraph":".graph");
	FILE *F = fopen(gfnm.CStr(), "wt");
	if (IsDir) fprintf(F, "# Directed graph: %s \n", gfnm.CStr());
	else fprintf(F, "# Undirected graph (each unordered pair of nodes is saved once): %s\n", gfnm.CStr());

	TStrIntH nidmap;
	int uid=0;
	TSsParser Ss(InFNm);
	TStr srcstr, dststr;
	while(Ss.Next()){
		srcstr=Ss.GetFld(0);
		dststr=Ss.GetFld(1);
		if(!nidmap.IsKey(srcstr)) nidmap(srcstr)=(uid++);
		if(!nidmap.IsKey(dststr)) nidmap(dststr)=(uid++);
		fprintf(F, "%d\t%d\n", nidmap(srcstr).Val, nidmap(dststr).Val);
	}
	fclose(F);
	F = fopen((OutFNm+".nmap").CStr(), "wt");
	fprintf(F, "#Node\tNID\n");
	for(int i=0;i<nidmap.Len();i++) fprintf(F, "%s\t%d\n", nidmap.GetKey(i).CStr(), nidmap[i].Val);
	fclose(F);
}

void SaveNodes(const TStr& InFNm, const TStr& OutFNm){
	TIntSet nodes; int nedge=0;
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		nodes.AddKey(Ss.GetInt(0));
		nodes.AddKey(Ss.GetInt(1));
		nedge++;
	}
	printf("Nodes: %d, Edges: %d\n", nodes.Len(), nedge);
	BIO::SaveInts(nodes, OutFNm, TStr::Fmt("Total: %d", nodes.Len()));
}

void ReverseEdgeDirection(const TStr& InFNm, const TStr& OutFNm){
	int u, v;
	FILE *F = fopen(OutFNm.CStr(), "wt");
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		u = Ss.GetInt(0); v = Ss.GetInt(1);
		fprintf(F, "%d\t%d\n", v, u);
	}
	fclose(F);
}

void RemoveSelfLoops(const TStr& InFNm, const TStr& OutFNm){
	int u, v, n=0;
	FILE *F = fopen(OutFNm.CStr(), "wt");
	TSsParser Ss(InFNm);
	while(Ss.Next()) {
		u = Ss.GetInt(0); v = Ss.GetInt(1);
		if (u!=v) {
			fprintf(F, "%d\t%d\n", u, v);
			n++;
		}
	}
	fclose(F);
	printf("Number of edges without selfloops: %d\n", n);
}
