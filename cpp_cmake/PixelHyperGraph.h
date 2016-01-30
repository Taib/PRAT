#ifndef PIXELHYPERGRAPH_H
#define PIXELHYPERGRAPH_H
#include <iostream>
#include <vector>
#include "HyperGraph.h"
namespace Hyper {

class PixelHyperGraph : public HyperGraph{
  /* PixelHyperGraph is a template class that implements basic functions used in a pixel based image hypergraph.
   *
   */
protected: 
  /* method : getEffectiveHyperEdge computes a hyperedge of a given position in an image. It's an abstract method that
   * every pixel based image hypergraph must implement.
   * params : 
   *     _img : the image
   *     _pos : a position in _img
   * 
   * output : a HyperEdge containing all indices of the neighbors of _pos that satisfy the similarity function.
   */
    virtual HyperEdge getEffectiveHyperEdge(const IplImage * _img, long _pos) = 0;
    // store the image and its size 
    CvPoint imSize;
    IplImage* image;
    // For stats purpose : store the variations of hyperedges cardinals
    std::vector<double> edgeStd;
    std::vector<double> edgeCard;
public :
    inline PixelHyperGraph(){}
    inline ~PixelHyperGraph(){cvReleaseImage(&image);}
    
    // computes the hypergraph of a given image
    void compute_hyper(const IplImage * _img);
    // clones th class 
    virtual PixelHyperGraph * clone() = 0;
    
    // returns the intensity values of pixels contained in a given set "ng"
    std::vector<CvScalar> neighborsValues(const HyperEdge & ng);
    std::vector<CvScalar> neighborsValues(const HyperEdge & ng)const;
    void neighborsValues(const HyperEdge & ng, std::vector<CvScalar>* _output);
    void neighborsValues(const HyperEdge & ng, std::vector<CvScalar>* _output)const;

    // changes the current image to a given one
    inline void setImage(const IplImage* _src ){this->image = cvCloneImage(_src); imSize = cvPoint(image->height, image->width);}

    // assigns new values of a set of pixels in the image.
    void setPixelValue(long _hyperEdgeId, CvScalar _value);

    inline CvPoint getImageSize(){return imSize;}
    inline CvPoint getImageSize() const {return imSize;}
    inline const IplImage* getImage(){return image;}

    // Creates a file "_outName" that contains the weighted image hypergraph. 
    // Here only the vertices are weighted (with their intensity values).
    // The output file "_outName" can be an input of the hMetis hypergraph partitioning tool. 
    void toWeightedFile(const char * _outName);

    // For stats purpose
    inline std::vector<double>& getEdgeStd(){return edgeStd;}
    inline std::vector<double>& getEdgeCard(){return edgeCard;}
};
void PixelHyperGraph::toWeightedFile(const char * _outName)
{
    std::ofstream out (_outName, std::ios::out);
    out << hyper.size()<<" "<<hyper.size()<<" 10 \n";
    for(long i = 0; i<hyper.size(); i++){
        out << hyper[i].toString() << "\n";
    }
    for(long i = 0; i<hyper.size(); i++){
        CvPoint p = Utils::numToCoord(i, this->imSize.y);
        out << cvGet2D(image, p.x, p.y).val[0]<<"\n";
    }
    out.close();
}

void PixelHyperGraph::compute_hyper(const IplImage * _img){
    std::cout<<"[PixelHyperGraph] building..."<<std::endl;
    imSize = cvPoint(_img->height, _img->width);
    image  = cvCloneImage(_img);
    this->hyper.resize(imSize.x*imSize.y);

    for(long i = 0 ; i <imSize.x; ++i)
    {
        for(long j = 0 ; j < imSize.y; ++j)
        {
            hyper[i*imSize.y + j] = getEffectiveHyperEdge(_img, i*imSize.y + j);
        }
    }
    std::cout<<"[PixelHyperGraph] finish."<<std::endl;
}

void PixelHyperGraph::setPixelValue(long _hyperEdgeId, CvScalar _value){
    CvPoint pos = Utils::numToCoord(_hyperEdgeId, image->width);
    cvSet2D(image, pos.x, pos.y, _value);
}

std::vector<CvScalar> PixelHyperGraph::neighborsValues( const HyperEdge & ng)const
{
    std::vector<CvScalar> values (ng.size());
    for(long i = 0; i< values.size(); i++){
        CvPoint p = Utils::numToCoord(ng.at(i), image->width);
        values[i] = cvGet2D(image, p.x, p.y);
    }
    return values;
}

void PixelHyperGraph::neighborsValues( const HyperEdge & ng, std::vector<CvScalar>* _output) const
{
    for(long i = 0; i< ng.size(); i++){
        CvPoint p = Utils::numToCoord(ng.at(i), image->width);
        _output->push_back(cvGet2D(image, p.x, p.y));
    }
}

std::vector<CvScalar> PixelHyperGraph::neighborsValues( const HyperEdge & ng)
{
    std::vector<CvScalar> values (ng.size());
    for(long i = 0; i< values.size(); i++){
        CvPoint p = Utils::numToCoord(ng.at(i), image->width);
        values[i] = cvGet2D(image, p.x, p.y);
    }
    return values;
}

void PixelHyperGraph::neighborsValues( const HyperEdge & ng, std::vector<CvScalar>* _output)
{
    for(long i = 0; i< ng.size(); i++){
        CvPoint p = Utils::numToCoord(ng.at(i), image->width);
        _output->push_back(cvGet2D(image, p.x, p.y));
    }
}

}
#endif // PIXELHYPERGRAPH_H

// @Tramx
