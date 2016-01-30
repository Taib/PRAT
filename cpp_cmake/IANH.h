#ifndef IANH_H
#define IANH_H

#include <fstream>
#include "PixelHyperGraph.h"

#include "Utils.h"

namespace Hyper {

  /* We provide here an implementation of an image hypergraph representation technique defined in :
   * "Hypergraph based image representation " by A. Bretto et al.
   *
   * Its a pixel based representation that uses the standard deviation of pixel value in a neighborhood in the hyperedge building process.
   *
   */

template<class ImageNeighboursKernel, class PixelsDistanceKernel>
class IANH : public PixelHyperGraph
{

  /* The IANH (Image Adaptative Neighborhood Hypergraph) class needs 
   *     - a kernel to compute a pixel neighbors 
   *     - a distance kernel to compute the distance between two pixels
   */

protected:
  /* method : getEffectiveHyperEdge computes a hyperedge of a givin position in an image
   * params : 
   *     _img : the image
   *     _pos : a position in _img
   * 
   * output : a HyperEdge containing all indices of the neighbors of _pos that satisfy the similarity function.
   */
   HyperEdge getEffectiveHyperEdge(const IplImage * _img, long _pos);
public:
   typedef ImageNeighboursKernel ImgNeighourKernel;
   typedef PixelsDistanceKernel PxDistKernel;
   
   // Constructors
   inline IANH(){}
   IANH(const IplImage * _img);

    // to clone an hypergraph :: deprecated
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
    HyperEdge ng;
    // get the neighbor of the pixel at the position "_pos" in the image "_img" according to the given neighborhood kernel
    ImgNeighourKernel::neighbors(_pos, this->imSize, &ng);
    std::vector<CvScalar> values (ng.size());
    // get the intensity of the neighbors
    values =  this->neighborsValues( ng);
    // compute the standard deviation
    CvScalar alpha = Utils::standardDeviation(values);
    HyperEdge gamma;
    // get the (x,y) value that corresponds to "_pos" in the image "_img".
    CvPoint p   = Utils::numToCoord(_pos, imSize.y);
    for(int i = 0; i< ng.size(); i++){
      // get (x,y) of a neighbor
      CvPoint ngp = Utils::numToCoord(ng.at(i), imSize.y);
      // assign a neighbor of "_pos" only if it satisfy the similarity below 
      if (PxDistKernel::distance(cvGet2D(_img, p.x, p.y).val[0], cvGet2D(_img, ngp.x, ngp.y).val[0]) <= alpha.val[0]){
	gamma.add(ng.at(i));
      }
    }


    // For stats purpose
    // edgeStd.push_back(alpha.val[0]);
    // edgeCard.push_back(gamma.size());
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


