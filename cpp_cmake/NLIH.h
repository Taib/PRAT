#ifndef NLIH_H
#define NLIH_H

#include "PixelHyperGraph.h"

namespace Hyper {

template<class ImageNeighborhoodKernel, class PixelsDistanceKernel>
class NLIH : public PixelHyperGraph{
protected:
   HyperEdge getEffectiveHyperEdge(const IplImage* _img, long _pos);
public:
   typedef ImageNeighborhoodKernel ImgNeighourKernel;
   typedef PixelsDistanceKernel PxDistKernel;
   NLIH(){}
   NLIH(const IplImage* _img);
   NLIH<ImageNeighborhoodKernel , PixelsDistanceKernel >* clone();
};
template<class ImageNeighboursKernel , class PixelsDistanceKernel >
NLIH<ImageNeighboursKernel , PixelsDistanceKernel >::NLIH(const IplImage* _img ){
    compute_hyper(_img);
}
template<class ImageNeighboursKernel , class PixelsDistanceKernel >
HyperEdge NLIH<ImageNeighboursKernel , PixelsDistanceKernel >::getEffectiveHyperEdge(const IplImage * _img, long _pos){
    HyperEdge ng;
    ImgNeighourKernel::neighbors(_pos, imSize, &ng);
    std::vector<CvScalar>                  values  = this->neighborsValues(ng);
    std::vector<std::pair<CvScalar, int> > dots    = std::vector<std::pair<CvScalar, int> >(ng.size());

    HyperEdge gamma(ng.at(ng.indexOf(_pos)));
    for(int i = 0; i < ng.size(); i++){
        HyperEdge aux_ng;
        ImgNeighourKernel::neighbors(ng.at(i), imSize, &aux_ng);
        dots[i] = std::pair<CvScalar, int>(Utils::dot(values, this->neighborsValues(aux_ng)), ng.at(i));
    }
    std::vector<int> bestIds = Utils::adaptativeTresholding(dots);
    for(int i = 0; i< bestIds.size(); i++)
        gamma.add(bestIds.at(i));
    /*std::vector<Cluster> clusters = Utils::kMeans(dots);
    int bestCluster               = 0;
    for(int i = 0; i< clusters.size(); i++)
        if(clusters.at(i).indexes.setContains(ng.at(0)))
            bestCluster = i;
    for(int k = 0; k< clusters.at(bestCluster).indexes.size(); k++)
        gamma.add(clusters.at(bestCluster).indexes.at(k));
        */

    return gamma;
}
template<class ImageNeighborhoodKernel , class PixelsDistanceKernel >
NLIH<ImageNeighborhoodKernel , PixelsDistanceKernel >* NLIH<ImageNeighborhoodKernel , PixelsDistanceKernel >::clone(){
    NLIH<ImageNeighborhoodKernel , PixelsDistanceKernel >* res = new NLIH<ImageNeighborhoodKernel , PixelsDistanceKernel >();
    res->setHyper(this->getHyper());
    res->setImage(this->image);
    return res;
}

}
#endif // NLIH_H

/*  @Tramx   */
