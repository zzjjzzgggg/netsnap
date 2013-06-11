#ifndef CENTR_H
#define CENTR_H

namespace TSnap {

/////////////////////////////////////////////////
// Centrality measures (see: http://en.wikipedia.org/wiki/Centrality)
// Closeness centrality
double GetClosenessCentr(const PUNGraph& Graph, const int& NId);

// Betweenness centrality (exact and fast approximate calculations)
void GetBetweennessCentr(const PUNGraph& Graph, TIntFltH& NIdBtwH,	const double& NodeFrac = 1.0);
void GetBetweennessCentr(const PUNGraph& Graph, TIntPrFltH& EdgeBtwH, const double& NodeFrac = 1.0);
void GetBetweennessCentr(const PUNGraph& Graph, TIntFltH& NodeBtwH,	TIntPrFltH& EdgeBtwH, const double& NodeFrac = 1.0);
void GetBetweennessCentr(const PUNGraph& Graph, const TIntV& BtwNIdV, TIntFltH& NodeBtwH, const bool& DoNodeCent, TIntPrFltH& EdgeBtwH, const bool& DoEdgeCent);

// Eigenvector centrality
void GetEigenVectorCentr(const PUNGraph& Graph, TIntFltH& EigenH, const double& Eps = 1e-4, const int& MaxIter = 100);

// PageRank, HITS (Hubs and Authorities) and Katz
template<class PGraph> void GetPageRank(const PGraph& Graph, TIntFltH& PRankH, const double& C = 0.85, const double& Eps = 1e-6, const int& MaxIter = 200);
template<class PGraph> void GetHits(const PGraph& Graph, TIntFltH& NIdHubH,	TIntFltH& NIdAuthH, const int& MaxIter = 200);
template<class PGraph> void GetKatzCentr(const PGraph& Graph, TIntFltH& KatzH, const double& alpha, const double& Eps = 1e-3, const int& MaxIter = 500);

////////////////////////////////////////////////
// Page Rank -- there are two different implementations (uncomment the desired 2 lines):
//   Berkhin -- (the correct way) see Algorithm 1 of P. Berkhin, A Survey on PageRank Computing, Internet Mathematics, 2005
//		In Berkhin, sum of each iteration should equal alpha. This process can guarenteen sum 1 all the time.
//   iGraph -- iGraph implementation(which treats leaked PageRank in a funny way)
template<class PGraph>
void GetPageRank(const PGraph& Graph, TIntFltH& PRankH, const double& C, const double& Eps, const int& MaxIter) {
	const int NNodes = Graph->GetNodes();
	const double OneOver = 1.0/NNodes;
	PRankH.Gen(NNodes);
	for (typename PGraph::TObj::TNodeI NI=Graph->BegNI(); NI<Graph->EndNI(); NI++) PRankH.AddDat(NI.GetId(), OneOver);
	TFltV TmpV(NNodes);
	for (int iter=0; iter<MaxIter; iter++) {
		int j = 0; double sum=0;
		for (typename PGraph::TObj::TNodeI NI=Graph->BegNI(); NI<Graph->EndNI(); NI++, j++) {
			TmpV[j] = 0;
			for (int e=0; e<NI.GetInDeg(); e++) {
				const int InNId = NI.GetInNId(e);
				const int OutDeg = Graph->GetNI(InNId).GetOutDeg();
				if (OutDeg > 0) TmpV[j] += PRankH.GetDat(InNId)/OutDeg;
			}
			TmpV[j] = C*TmpV[j]; // Berkhin
			//TmpV[j] =  C*TmpV[j] + (1.0-C)*OneOver; // iGraph
			sum += TmpV[j];
		}
		double diff = 0, NewVal;
//		for (int i = 0; i < TmpV.Len(); i++) sum += TmpV[i];
		const double Leaked = (1.0 - sum)/NNodes;
		for (int i = 0; i < PRankH.Len(); i++) { // re-instert leaked PageRank
			NewVal = TmpV[i] + Leaked; // Berkhin
			//NewVal = TmpV[i]/sum;  // iGraph
			diff += fabs(NewVal - PRankH[i]);
			PRankH[i] = NewVal;
		}
		if (diff < Eps) {
			printf("Successed. (iter=%d)\n", iter);
			return;
		}
	}
	printf("Warning: Power iteration is not converged!\n");
}

// HITS: Hubs and Authorities (by J. Kleinberg, see http://en.wikipedia.org/wiki/HITS_algorithm)
template<class PGraph>
void GetHits(const PGraph& Graph, TIntFltH& NIdHubH, TIntFltH& NIdAuthH, const int& MaxIter) {
	const int NNodes = Graph->GetNodes();
	NIdHubH.Gen(NNodes);
	NIdAuthH.Gen(NNodes);
	for (typename PGraph::TObj::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
		NIdHubH.AddDat(NI.GetId(), 1.0);
		NIdAuthH.AddDat(NI.GetId(), 1.0);
	}
	double Norm = 0;
	for (int iter = 0; iter < MaxIter; iter++) {
		// update authority scores
		Norm = 0;
		for (typename PGraph::TObj::TNodeI NI = Graph->BegNI(); NI	< Graph->EndNI(); NI++) {
			double& Auth = NIdAuthH.GetDat(NI.GetId()).Val;
			Auth = 0;
			for (int e = 0; e < NI.GetInDeg(); e++) {
				Auth += NIdHubH.GetDat(NI.GetInNId(e));
			}
			Norm += Auth * Auth;
		}
		Norm = sqrt(Norm);
		for (int i = 0; i < NIdAuthH.Len(); i++) {
			NIdAuthH[i] /= Norm;
		}
		// update hub scores
		for (typename PGraph::TObj::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
			double& Hub = NIdHubH.GetDat(NI.GetId()).Val;
			Hub = 0;
			for (int e = 0; e < NI.GetOutDeg(); e++) {
				Hub += NIdAuthH.GetDat(NI.GetOutNId(e));
			}
			Norm += Hub * Hub;
		}
		Norm = sqrt(Norm);
		for (int i = 0; i < NIdHubH.Len(); i++) {
			NIdHubH[i] /= Norm;
		}
	}
}

// Katz centrality

// normalized Katz centrality
template<class PGraph>
void GetKatzCentrNorm(const PGraph& Graph, TIntFltH& KatzH, const double& alpha, const double& Eps, const int& MaxIter) {
	const int NNodes = Graph->GetNodes();
	const double OneOver = 1.0/NNodes;
	KatzH.Gen(NNodes);
	for (typename PGraph::TObj::TNodeI NI=Graph->BegNI(); NI<Graph->EndNI(); NI++) KatzH.AddDat(NI.GetId(), OneOver);
	TFltV TmpV(NNodes);
	for (int iter=0; iter<MaxIter; iter++) {
		int j = 0; double sum=0;
		for (typename PGraph::TObj::TNodeI NI=Graph->BegNI(); NI<Graph->EndNI(); NI++, j++) {
			TmpV[j] = 0;
			for (int e=0; e<NI.GetInDeg(); e++) {
				const int InNId = NI.GetInNId(e);
				TmpV[j] += KatzH.GetDat(InNId);
			}
			TmpV[j] = alpha*TmpV[j]+OneOver;
			sum += TmpV[j];
		}
		double diff = 0, NewVal;
//		for (int i=0; i<TmpV.Len(); i++) sum += TmpV[i];
		for (int i=0; i<KatzH.Len(); i++) {
			NewVal = TmpV[i];///sum;
			diff += fabs(NewVal - KatzH[i]);
			KatzH[i] = NewVal;
		}
		if (diff < Eps) {
			printf("Iteration number=%d\n", iter);
			return;
		}
	}
	printf("Warning: Power iteration is not converged!\n");
}

// unnormalized Katz centrality
template<class PGraph>
void GetKatzCentr(const PGraph& Graph, TIntFltH& KatzH, const double& alpha, const double& Eps, const int& MaxIter) {
	const int NNodes = Graph->GetNodes();
	KatzH.Gen(NNodes);
	for (typename PGraph::TObj::TNodeI NI=Graph->BegNI(); NI<Graph->EndNI(); NI++) KatzH.AddDat(NI.GetId(), TInt::Rnd.GetUniDev());
	TFltV TmpV(NNodes);
	for (int iter=0; iter<MaxIter; iter++) {
		int j = 0;
		for (typename PGraph::TObj::TNodeI NI=Graph->BegNI(); NI<Graph->EndNI(); NI++, j++) {
			TmpV[j] = 0;
			for (int e=0; e<NI.GetInDeg(); e++) {
				const int InNId = NI.GetInNId(e);
				TmpV[j] += KatzH.GetDat(InNId);
			}
			TmpV[j] = alpha*TmpV[j]+1;
		}
		double diff = 0;
		for (int i=0; i<KatzH.Len(); i++) {
			diff += fabs(TmpV[i] - KatzH[i]);
			KatzH[i] = TmpV[i];
		}
		if (diff < Eps) {
			printf("Iteration number=%d\n", iter);
			return;
		}
	}
	printf("Warning: Power iteration is not converged!\n");
}


}; // namespace TSnap

#endif
