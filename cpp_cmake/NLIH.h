#ifndef NLIH_H
#define NLIH_H

#include "PixelHyperGraph.h"


namespace Hyper {

/* The class NLIH (Non-Local fashion Image Hypergraph) is a pixel based image hypergraph representation. 
 *It uses the similarity between a small patch around pixels as comparaison function.
 * 
 * NLIH needs a kernel to compute a pixel neighbors.
 *
 */

template<class ImageNeighborhoodKernel, class PixelsDistanceKernel>
class NLIH : public PixelHyperGraph{
protected:
  /* method : getEffectiveHyperEdge computes a hyperedge of a givin position in an image
   * params : 
   *     _img : the image
   *     _pos : a position in _img
   * 
   * output : a HyperEdge containing all indices of the neighbors of _pos that satisfy the similarity function.
   */
   HyperEdge getEffectiveHyperEdge(const IplImage* _img, long _pos);
public:
   typedef ImageNeighborhoodKernel ImgNeighourKernel;
   typedef PixelsDistanceKernel PxDistKernel;
   
   // Constructors
   NLIH(){}
   NLIH(const IplImage* _img);

   // to clone an hypergraph :: deprecated
   NLIH<ImageNeighborhoodKernel , PixelsDistanceKernel >* clone();
};
template<class ImageNeighboursKernel , class PixelsDistanceKernel >
NLIH<ImageNeighboursKernel , PixelsDistanceKernel >::NLIH(const IplImage* _img ){
    compute_hyper(_img);
}
template<class ImageNeighboursKernel , class PixelsDistanceKernel >
HyperEdge NLIH<ImageNeighboursKernel , PixelsDistanceKernel >::getEffectiveHyperEdge(const IplImage * _img, long _pos){
    HyperEdge ng;
    // get the neighbor of the pixel at the position "_pos" in the image "_img" according to the given neighborhood kernel
    ImgNeighourKernel::neighbors(_pos, imSize, &ng);
    // get the intensity of the neighbors
    std::vector<CvScalar>                  values  = this->neighborsValues(ng);

    /* Computing the dot product between patches*/
    // allocation
    std::vector<std::pair<CvScalar, int> > dots    = std::vector<std::pair<CvScalar, int> >(ng.size());
    // dot product between each neighbor's patch
    for(int i = 0; i < ng.size(); i++){
        HyperEdge aux_ng;
        ImgNeighourKernel::neighbors(ng.at(i), imSize, &aux_ng);
        dots[i] = std::pair<CvScalar, int>(Utils::dot(values, this->neighborsValues(aux_ng)), ng.at(i));
    }
    // compute the indices of the similar neighbors
    std::vector<int> bestIds = Utils::adaptativeTresholding(dots);
    // create the hyperedge
    HyperEdge gamma(ng.at(ng.indexOf(_pos)));
    for(int i = 0; i< bestIds.size(); i++)
        gamma.add(bestIds.at(i));
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
