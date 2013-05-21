/*
 * advcentr.h
 *
 *  Created on: Jun 7, 2012
 *      Author: jzzhao
 */

#ifndef ADVCENTR_H_
#define ADVCENTR_H_

#include "stdafx.h"

template<class PGraph>
class AdvCentr{
private:
	class Node{
	public:
		int uid;
		double val;
		bool flag;
	public:
		Node(){}
		Node(int u, bool f, double r){uid=u; flag=f; val=r;}
		bool operator<(const Node& n) const{return val<n.val;}
		bool operator>(const Node& n) const{return val>n.val;}
		Node& operator=(const Node& n){uid=n.uid; flag=n.flag; val=n.val; return *this;}
	};

	typedef TLst<Node> TNodeV;
	typedef typename TNodeV::PLstNd PNode;
	typedef typename PGraph::TObj::TNodeI NodeI;

	PGraph Graph;
	TIntFltH A, nbrs, attrs;
	int type;
	double lambda;
	bool useAttr;
private:
	void addNode(int nid, double ddeg){
		A.AddDat(nid, ddeg);
		nbrs(nid)=(nbrs.IsKey(nid)?nbrs(nid).Val:0) + 1;
		NodeI ni=Graph->GetNI(nid);
		for(int d=0; d<ni.GetDeg(); d++) {
			int nbid=ni.GetNbhNId(d);
			nbrs(nbid) = (nbrs.IsKey(nbid)?nbrs(nbid).Val:0) + 1;
		}
	}

	double getAttr(int nid, double init=0.5){
		if(!useAttr) return 1;
		else return (attrs.IsKey(nid)?attrs(nid).Val:0) + init;
	}

	double getWeight(int nid){
		return nbrs.IsKey(nid)?exp(-lambda*nbrs(nid)):1;
	}

	double getGroupDegree(int nid, bool isInDeg, bool isOutDeg){
		double val=0;
		NodeI ni=Graph->GetNI(nid);
		if(isInDeg){
			for(int d=0; d<ni.GetInDeg(); d++){
				int nbid=ni.GetInNId(d);
				if(!nbrs.IsKey(nbid)) val += getAttr(nbid);
			}
		}else if(isOutDeg){
			for(int d=0; d<ni.GetOutDeg(); d++){
				int nbid=ni.GetOutNId(d);
				if(!nbrs.IsKey(nbid)) val += getAttr(nbid);
			}
		}else{
			for(int d=0; d<ni.GetDeg(); d++){
				int nbid=ni.GetNbhNId(d);
				if(!nbrs.IsKey(nbid)) val += getAttr(nbid);
			}
		}
		return val;
	}

	double getWeightedGroupDegree(int nid, bool isInDeg, bool isOutDeg){
		double val=0;
		NodeI ni=Graph->GetNI(nid);
		if(isInDeg){
			for(int d=0; d<ni.GetInDeg(); d++){
				int nbid=ni.GetInNId(d);
				val += getWeight(nbid)*getAttr(nbid);
			}
		}else if(isOutDeg){
			for(int d=0; d<ni.GetOutDeg(); d++){
				int nbid=ni.GetOutNId(d);
				val += getWeight(nbid)*getAttr(nbid);
			}
		}else{
			for(int d=0; d<ni.GetDeg(); d++){
				int nbid=ni.GetNbhNId(d);
				val += getWeight(nbid)*getAttr(nbid);
			}
		}
		return val*getAttr(nid);
	}

	double getDeg(int nid){
		switch(type){
		case 0: return getGroupDegree(nid, false, false);// total degree
		case 1: return getGroupDegree(nid, true, false);	// in degree
		case 2: return getGroupDegree(nid, false, true);	// out degree
		case 3: return getWeightedGroupDegree(nid, false, false);	// total degree
		case 4: return getWeightedGroupDegree(nid, true, false);// in degree
		case 5: return getWeightedGroupDegree(nid, false, true);// out degree
		default: return getGroupDegree(nid, false, false);// total degree
		}
	}

public:
	AdvCentr(PGraph graph, const int type, const double lambda):
		Graph(graph), type(type), lambda(lambda), useAttr(false){}
	void loadAttr(TStr fnm){
		TSsParser ss(fnm);
		while(ss.Next()) attrs(ss.GetInt(0)) = ss.GetFlt(1);
		useAttr=true;
	}
	void calculateDeg(int maxN){
		TNodeV queue;
		for(NodeI ni=Graph->BegNI(); ni<Graph->EndNI(); ni++)
			queue.AddFrontSorted(Node(ni.GetId(), true, getDeg(ni.GetId())), false);
		int n=0;
		if(maxN<0) maxN=Graph->GetNodes();
		while(n++<maxN && !queue.Empty()){
			while(true){
				PNode pn=queue.First();
				Node s=pn->Val;
				queue.Del(pn);
				if(s.flag) {
					addNode(s.uid, s.val);
					break;
				}
				s.val=getDeg(s.uid);
				s.flag=true;
				queue.AddFrontSorted(s, false);
			}
			PNode p=queue.First();
			while(p){
				p->Val.flag=false;
				p=p->Next();
			}
		}
	}
	void saveNodes(TStr fnm){
		BIO::SaveIntFltH(A, fnm, TStr::Fmt("#NID\t%d", type));
	}
};


#endif /* ADVCENTR_H_ */
