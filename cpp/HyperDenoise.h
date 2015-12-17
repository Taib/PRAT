#ifndef HYPERDENOISE_H
#define HYPERDENOISE_H

#include "IANH.h"

namespace Hyper{
class HyperDenoise
{
public:
     inline HyperDenoise(){}
     void hyperDenoise(IANH & _hyper, const cv::Mat & _img, cv::Mat & output);

private:
    HyperEdge openNeighborhood(IANH & _hyper, long _x);
    bool openTest(IANH & _hyper, HyperEdge * _ng,   const unsigned long _x);
    HyperEdge * noisyHyperEdges(IANH & _hyper);
};

/////////////////////////Implementation ///////////////////////////


void HyperDenoise::hyperDenoise(IANH &  _hyper, const cv::Mat & _img, cv::Mat & output){
    std::cout<<"Hyper denoising..."<<std::endl;
    HyperEdge * nh = new HyperEdge(this->noisyHyperEdges(_hyper)->getDataCopy());
    for(unsigned long i = 0; i < nh->size(); i++){
        HyperEdge ng = this->openNeighborhood(_hyper, nh->at(i));
        if(true){
            ng = Utils::neighbors8(nh->at(i), _hyper.getImageSize());
        }
        ng.remove(ng.indexOf(nh->at(i)));
        std::vector<double> * values = new std::vector<double>(Utils::neighborsValues(_img, ng));
        values = Utils::sort(*values);
        Point2Di pos  = Utils::numToCoord(nh->at(i), _hyper.getImageSize().y);
        output.at<uchar>(pos.x, pos.y) = uchar((*values)[(int)values->size()/2]);
        delete values;
    }
    delete nh;
    std::cout<<"Hyper denoising finish."<<std::endl;
}


HyperEdge HyperDenoise::openNeighborhood(IANH & _hyper, long  _x){
    Point2Di ngi  = Utils::numToCoord(_x, _hyper.getImageSize().y);
    return Hyper::HyperGraph<HyperEdge>::neighborEdgeSets(_hyper,  _hyper.getHyper()[ngi.x*_hyper.getImageSize().y + ngi.y]);
}

bool HyperDenoise::openTest(IANH & _hyper, HyperEdge * _ng,  const unsigned long  _x){
    HyperEdge on = this->openNeighborhood(_hyper, _ng->at(_x));
    //for(unsigned long i = 0; i<_ng->size(); i++){
    //    if(_ng->setContains(on.at(i))){
    //        return true;
    //    }
    //}
    for(long i = 0; i < on.size(); i++){
        if(on.at(i)!=_x && _hyper.at(on.at(i)).size() == 1)
            return true;
    }
    return false;
}

HyperEdge* HyperDenoise::noisyHyperEdges(IANH & _hyper){
    HyperEdge* nh = new HyperEdge();
    HyperEdge* is = new HyperEdge();
    const std::vector<HyperEdge> & hyper = _hyper.getHyper();

    // Determination of isolated hyperedges of Hyper
    for(unsigned long i = 0; i<_hyper.getImageSize().x; i++){
        for(unsigned long j = 0; j<_hyper.getImageSize().y; j++){

            HyperEdge ex = hyper[i*_hyper.getImageSize().y + j];
            HyperEdge ep = Hyper::HyperGraph<HyperEdge>::neighborEdgeSets(_hyper, ex);
            if(ep == ex){
                is->add(i*_hyper.getImageSize().y + j);
            }
        }
    }
    std::cout<<"IS completed.  "<<is->size()<<std::endl;
    // Detection of noise hyperedges
    for(unsigned long i = 0; i<is->size(); i++){
        Point2Di ngi  = Utils::numToCoord(is->at(i), _hyper.getImageSize().y);
        HyperEdge aux = hyper[ngi.x*_hyper.getImageSize().y + ngi.y];
        if( (aux.size() == 1) || this->openTest(_hyper, is, i))
            nh->add(is->at(i));
    }
    std::cout<<"NH completed.  "<<nh->size()<<std::endl;
    delete is;
    return nh;
}

}
#endif // HYPERDENOISE_H
