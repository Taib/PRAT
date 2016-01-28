#ifndef IANH_H
#define IANH_H

#include <fstream>
#include "PixelHyperGraph.h"

#include "Utils.h"

namespace Hyper {

enum Hyper_Build_Types{Hyper_Std_Build=0, Hyper_HistoEm_Build=1, Hyper_Patch_Build=2, Hyper_Kmeans_Build=3};

template<class ImageNeighboursKernel, class PixelsDistanceKernel>
class IANH : public PixelHyperGraph
{
protected:
   HyperEdge getEffectiveHyperEdge(const IplImage * _img, long _pos);
public:
   typedef ImageNeighboursKernel ImgNeighourKernel;
   typedef PixelsDistanceKernel PxDistKernel;

    inline IANH(){}
    IANH(const IplImage * _img);
    IANH<ImageNeighboursKernel, PixelsDistanceKernel>* clone();
};


/////////////////////////Implementation ///////////////////////////


template<class ImageNeighboursKernel, class PixelsDistanceKernel>
IANH<ImageNeighboursKernel, PixelsDistanceKernel>::IANH(const IplImage * _img)
{
    compute_hyper(_img);
}

template<class ImageNeighboursKernel, class PixelsDistanceKernel>
HyperEdge IANH<ImageNeighboursKernel, PixelsDistanceKernel>::getEffectiveHyperEdge(
        const IplImage * _img, long _pos)
{
    HyperEdge ng;//= new HyperEdge();
    ImgNeighourKernel::neighbors(_pos, this->imSize, &ng);
    std::vector<CvScalar> values (ng.size());
    values =  this->neighborsValues( ng);
    CvScalar alpha = Utils::standardDeviation(values);
    HyperEdge gamma;
    CvPoint p   = Utils::numToCoord(_pos, imSize.y);
    for(int i = 0; i< ng.size(); i++){
        CvPoint ngp = Utils::numToCoord(ng.at(i), imSize.y);
        if (PxDistKernel::distance(cvGet2D(_img, p.x, p.y).val[0], cvGet2D(_img, ngp.x, ngp.y).val[0]) <= alpha.val[0]){
            //(PxDistKernel::distance(uint(_img.at<uchar>(p.x, p.y)), uint(_img.at<uchar>(ngp.x, ngp.y))) <= alpha){
            gamma.add(ng.at(i));
        }
    }
    //delete ng;
    // For stats purpose
    edgeStd.push_back(alpha.val[0]);
    edgeCard.push_back(gamma.size());
    ng.clear();
    return gamma;
}

template<class ImageNeighboursKernel, class PixelsDistanceKernel>
IANH<ImageNeighboursKernel, PixelsDistanceKernel>* IANH<ImageNeighboursKernel, PixelsDistanceKernel>::clone(){
    IANH<ImageNeighboursKernel, PixelsDistanceKernel>* res = new IANH<ImageNeighboursKernel, PixelsDistanceKernel>();
    res->setHyper(this->getHyper());
    res->setImage(this->image);
    return res;
}




}
#endif // IANH_H

/*  @Tramx   */


