
namespace TRandom{

void InitUni(TFltV& dist, const int pos_start){
	TRnd rnd;
	double sum = 0;
	for (int i=pos_start; i<dist.Len(); i++) {
		dist[i] = rnd.GetUniDev();
		sum += dist[i];
	}
	for (int i=pos_start; i<dist.Len(); i++) dist[i] /= sum;
}

}