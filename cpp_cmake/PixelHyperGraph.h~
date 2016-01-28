#ifndef PIXELHYPERGRAPH_H
#define PIXELHYPERGRAPH_H
#include <iostream>
#include <vector>
#include "HyperGraph.h"
namespace Hyper {

class PixelHyperGraph : public HyperGraph{
protected:
    virtual HyperEdge getEffectiveHyperEdge(const IplImage * _img, long _pos) = 0;
    CvPoint imSize;
    IplImage* image;
    // For stats purpose
    std::vector<double> edgeStd;
    std::vector<double> edgeCard;
public :
    inline PixelHyperGraph(){}
    inline ~PixelHyperGraph(){cvReleaseImage(&image);}
    void compute_hyper(const IplImage * _img);
    virtual PixelHyperGraph * clone() = 0;

    std::vector<CvScalar> neighborsValues(const HyperEdge & ng);
    std::vector<CvScalar> neighborsValues(const HyperEdge & ng)const;
    void neighborsValues(const HyperEdge & ng, std::vector<CvScalar>* _output);
    void neighborsValues(const HyperEdge & ng, std::vector<CvScalar>* _output)const;

    inline void setImage(const IplImage* _src ){this->image = cvCloneImage(_src); imSize = cvPoint(image->height, image->width);}
    void setPixelValue(long _hyperEdgeId, CvScalar _value);

    inline CvPoint getImageSize(){return imSize;}
    inline CvPoint getImageSize() const {return imSize;}
    inline const IplImage* getImage(){return image;}

    void toWeightedFile(const char * _outName, const IplImage * _img);
    // For stats purpose
    inline std::vector<double>& getEdgeStd(){return edgeStd;}
    inline std::vector<double>& getEdgeCard(){return edgeCard;}
};
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

void PixelHyperGraph::toWeightedFile(const char * _outName, const IplImage * _img)
{
    std::ofstream out (_outName, std::ios::out);
    out << hyper.size()<<" "<<hyper.size()<<" 10 \n";
    for(long i = 0; i<hyper.size(); i++){
        //double mean_edge = Utils::mean(Utils::neighborsValues(_img, hyper.at(i)));
        //out << (int)mean_edge <<" "<< hyper[i].toString() << "\n";
        out << hyper[i].toString() << "\n";
    }
    for(long i = 0; i<hyper.size(); i++){
        CvPoint p = Utils::numToCoord(i, this->imSize.y);
        out << cvGet2D(_img, p.x, p.y).val[0]<<"\n";
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

}
#endif // PIXELHYPERGRAPH_H
