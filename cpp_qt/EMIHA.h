#ifndef EMIHA_H
#define EMIHA_H

#include "PixelHyperGraph.h"
#include "Utils.h"

namespace Hyper {

template<class ImageNeighboursKernel , class PixelsDistanceKernel >
class EMIHA : public PixelHyperGraph {
protected:
   HyperEdge getEffectiveHyperEdge(const IplImage * _img, long _pos);
   int closest(std::vector<CvScalar> _container, CvScalar _value);
public:
   typedef ImageNeighboursKernel ImgNeighourKernel;
   typedef PixelsDistanceKernel PxDistKernel;

    inline EMIHA(){}
    EMIHA(const IplImage * _img);
    EMIHA* clone();

};

template<class ImageNeighboursKernel , class PixelsDistanceKernel >
EMIHA<ImageNeighboursKernel , PixelsDistanceKernel >::EMIHA(const IplImage * _img){
    compute_hyper(_img);
}

template<class ImageNeighboursKernel , class PixelsDistanceKernel >
std::vector<CvScalar> compute_gmm(std::vector<CvScalar> _values){
    std::vector<CvScalar> res;
    //TODO: Apply GMM algorith on _values; should return just two mean values in res.
    return res;
}

template<class ImageNeighboursKernel , class PixelsDistanceKernel >
int EMIHA<ImageNeighboursKernel , PixelsDistanceKernel >::closest(std::vector<CvScalar> _container, CvScalar _value){
    int res = 0;
    for(int i =0; i<_container.size(); i++){
        if(Utils::abs(_value -_container[i])< Utils::abs(value-_container[res]))
            res = i;
    }
}


template<class ImageNeighboursKernel , class PixelsDistanceKernel >
HyperEdge EMIHA<ImageNeighboursKernel , PixelsDistanceKernel >::getEffectiveHyperEdge(const IplImage * _img, long _pos){
    HyperEdge ng;
    ImgNeighourKernel::neighbors(_pos, imSize, ng);
    std::vector<CvScalar> values = this->neighborsValues(_img, ng);
    std::vector<double> means    = this->compute_gmm(values);
    int best_mean                = closest(means, values[0]);
    HyperEdge gamma(_pos);
    for(int i = 1; i< ng.size(); i++){
        if(best_mean == closest(means, values[i]))
            gamma.add(ng.at(i));
    }
    return gamma;
}


template<class ImageNeighboursKernel , class PixelsDistanceKernel >
EMIHA* EMIHA<ImageNeighboursKernel , PixelsDistanceKernel >::clone(){
    EMIHA* res = new EMIHA();
    res->setHyper(this->getHyper());
    res->setImage(this->image);
    return res;
}

}
#endif // EMIHA_H

/*  @Tramx   */

