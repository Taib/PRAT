#ifndef IANH_H
#define IANH_H

#include "Utils.h"

#include <fstream>
#include "HyperGraph.h"
namespace Hyper {
class IANH : public HyperGraph<HyperEdge>
{
public:
    inline IANH(){}
    IANH(const cv::Mat & _img);
    inline Point2Di getImageSize(){return size;}
    inline Point2Di getImageSize() const {return size;}
    inline const std::vector<HyperEdge> & getHyper(){return hyper;}
    inline const std::vector<HyperEdge> & getHyper() const {return hyper;}
    inline void setHyper(typename std::vector<HyperEdge> _hyper){this->hyper.clear(); this->hyper.assign(_hyper.begin(), _hyper.end());}
    inline void addHyperEdge(HyperEdge _ng){this->hyper.push_back(_ng);}
    inline long getSize(){return hyper.size();}
    inline long getSize() const {return hyper.size();}
    inline HyperEdge at(long _i){return ((_i<hyper.size()) && (_i>= 0))?hyper[_i]:throw "we got pb here : hyper.at";}
    inline HyperEdge at(long _i) const{return ((_i<hyper.size()) && (_i>= 0))?hyper[_i]:throw "we got pb here : hyper.at";}
    void toFile(const char *);
private:
   HyperEdge getEffectiveHyperEdge(const cv::Mat & _img, long _pos);
   Point2Di size;
};


/////////////////////////Implementation ///////////////////////////
void IANH::toFile(const char * _fname){
    std::ofstream out (_fname, std::ios::out);
    for(long i = 0; i<hyper.size(); i++)
        out <<i <<"   "<< hyper[i].toString() << "\n";
    out.close();
}

IANH::IANH( const cv::Mat & _img)
{
    std::cout<<"IANH building..."<<std::endl;
    size = Point2Di(_img.rows, _img.cols);

    this->hyper.resize(size.x*size.y);

    for(unsigned int i = 0 ; i <size.x; ++i)
    {
        for(unsigned int j = 0 ; j < size.y; ++j)
        {
            hyper[i*size.y + j] = getEffectiveHyperEdge(_img, i*size.y + j);
        }
    }
    std::cout<<"IANH finish."<<std::endl;
}

HyperEdge IANH::getEffectiveHyperEdge(const cv::Mat & _img, long _pos){
    HyperEdge ng = Utils::neighbors8(_pos, this->size);
    std::vector<double> values =  Utils::neighborsValues(_img, ng);
    double alpha = Utils::standardDeviation(values);
    HyperEdge gamma;
    Point2Di p   = Utils::numToCoord(_pos, size.y);
    for(int i = 0; i< ng.size(); i++){
        Point2Di ngp = Utils::numToCoord(ng.getData()[i], size.y);
        if (Utils::distance(uint(_img.at<uchar>(p.x, p.y)), uint(_img.at<uchar>(ngp.x, ngp.y))) <= alpha){
            gamma.add(ng.at(i));
        }
    }
    return gamma;
}


}
#endif // IANH_H
