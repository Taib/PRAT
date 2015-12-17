#ifndef HYPERMORPHO_H
#define HYPERMORPHO_H

#include "IANH.h"

namespace Hyper{

class HyperMorpho
{
public:
    inline HyperMorpho(){struct_el = 1;}
    inline HyperMorpho(int _struct_el){this->struct_el = _struct_el;}
    void hyperDilate(const IANH & _input_hyper, const cv::Mat & _input_img, cv::Mat & _output_img);
    void hyperErode(const IANH & _input_hyper, const cv::Mat & _input_img, cv::Mat & _output_img);
    void hyperOpen(const IANH & _input_img, IANH & _output_img);
    void hyperClose(const IANH & _input_img, IANH & _output_img);
private:
    int struct_el;
};
////////////////////////Implementation //////////////////////////
/*
void HyperMorpho::hyperDilate(const IANH & _input_img, IANH & _output_img){
     for(long i = 0; i< _input_img.getSize(); i++){
         HyperEdge ng,
                 curng = _input_img.at(i);
        HyperEdge searchField = Hyper::HyperGraph<HyperEdge>::neighborEdgeSets(_input_img, curng);
        for(long j = 0; j < searchField.size(); j++){
            HyperEdge aux = _input_img.getHyper()[searchField.at(j)];
            if (aux.setIntersection(curng).size() != 0)
                ng.add(searchField.at(j));
        }
         _output_img.addHyperEdge(ng);
     }
}
*/

void HyperMorpho::hyperDilate(const IANH & _input_hyper, const cv::Mat & _input_img, cv::Mat & _output_img){
    for(unsigned int i = 0 ; i <_input_img.rows; ++i)
    {
        for(unsigned int j = 0 ; j < _input_img.cols; ++j)
        {
            HyperEdge *  curng      = new HyperEdge(_input_hyper.getHyper()[i*_input_img.cols + j]);
            HyperEdge * searchField = new HyperEdge(Hyper::HyperGraph<HyperEdge>::neighborEdgeSets(_input_hyper, *curng));

            std::vector<double> * values = new std::vector<double>(searchField->size());
            Utils::neighborsValues(_input_img, *searchField, values);

            _output_img.at<uchar>(i, j) =  uchar(uint(Utils::max<double>(*values)));//, _output_img.at<double>(p.x, p.y)));

            delete values;
            free(curng);
            free(searchField);
        }
    }
}

void HyperMorpho::hyperErode(const IANH & _input_hyper, const cv::Mat & _input_img, cv::Mat & _output_img){
    for(unsigned int i = 0 ; i <_input_img.rows; ++i)
    {
        for(unsigned int j = 0 ; j < _input_img.cols; ++j)
        {
            HyperEdge *  curng      = new HyperEdge(_input_hyper.getHyper()[i*_input_img.cols + j]);
            //HyperEdge * searchField = new HyperEdge(Hyper::HyperGraph<HyperEdge>::neighborEdgeSets(_input_hyper, *curng));

            std::vector<double> * values = new std::vector<double>(curng->size());
            Utils::neighborsValues(_input_img, *curng, values);

            _output_img.at<uchar>(i, j) =  uchar(uint(Utils::min<double>(*values)));//, _output_img.at<double>(p.x, p.y)));

            delete values;
            free(curng);
            //free(searchField);
        }
    }
}

void HyperMorpho::hyperOpen(const IANH & _input_img, IANH & _output_img){

}

void HyperMorpho::hyperClose(const IANH & _input_img, IANH & _output_img){

}

}
#endif // HYPERMORPHO_H
