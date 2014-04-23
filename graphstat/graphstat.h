#include "stdafx.h"

void MapNodes(const TStr& InFNm, const TStr& OutFNm, const bool IsDir){
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

template<class PGraph>
void SaveNodes(const PGraph& Graph, const TStr& OutFNm){
	TIntV nodes;
	Graph->GetNIdV(nodes);
	BIO::SaveInts(nodes, OutFNm);
}
