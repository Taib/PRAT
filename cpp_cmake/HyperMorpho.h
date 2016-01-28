#ifndef HYPERMORPHO_H
#define HYPERMORPHO_H

#include "IANH.h"

namespace Hyper{

class StructElement{
public:
    StructElement(){}
};

template<class HyperRep = HyperGraph>
class HyperMorpho
{
public:
    inline HyperMorpho(){}
    static void hyperDilate(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement stel);
    static void hyperErode(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement stel);
    static void hyperOpen(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement stel);
    static void hyperClose(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement stel);
    static float hyperSimilarity(const HyperRep & _hyper_src, const HyperRep & _hyper_dst, StructElement stel);
};
////////////////////////Implementation //////////////////////////

template<class HyperRep>
float HyperMorpho<HyperRep>::hyperSimilarity(const HyperRep & _hyper_src, const HyperRep & _hyper_dst, StructElement stel){
    float all_sim = 0.0;
    throw "[HyperSimilarity] not implemented!";
}

template<class HyperRep>
void HyperMorpho<HyperRep>::hyperDilate(const HyperRep & _input_hyper, HyperRep &_output_hyper, StructElement stel){
    for(unsigned int i = 0 ; i <_input_hyper.getSize(); ++i)
    {
        HyperEdge *  curng      = new HyperEdge(_input_hyper.at(i));
        HyperEdge * searchField = new HyperEdge(_input_hyper.neighborHyperEdgeSets( *curng));
        std::vector<CvScalar> * values = new std::vector<CvScalar>(_input_hyper.neighborsValues( *searchField));
        //if(i%100 == 0)
        //std::cout<<i<<" :: "<<Utils::max(*values)<<" :: values  "<<*values<<std::endl;
        _output_hyper.setPixelValue(i, Utils::max(*values));
        delete values;
        free(curng);
        free(searchField);

    }
}
template<class HyperRep>
void HyperMorpho<HyperRep>::hyperErode(const HyperRep & _input_hyper, HyperRep &_output_hyper, StructElement stel){
    for(unsigned int i = 0 ; i <_input_hyper.getSize(); ++i)
    {
        HyperEdge *  curng      = new HyperEdge(_input_hyper.at(i));
        HyperEdge * searchField = new HyperEdge(_input_hyper.neighborHyperEdgeSets( *curng));

        std::vector<CvScalar> * values = new std::vector<CvScalar>(_input_hyper.neighborsValues( *searchField));

        _output_hyper.setPixelValue(i, Utils::min(*values));
        delete values;
        free(curng);
        free(searchField);

    }
}


template<class HyperRep>
void HyperMorpho<HyperRep>::hyperOpen(const HyperRep & _input_hyper, HyperRep &_output_hyper, StructElement stel){
    hyperErode(_input_hyper, _output_hyper, stel);
    HyperRep* cloning = _output_hyper.clone();
    hyperDilate(*cloning, _output_hyper, stel);
    free(cloning);
}

template<class HyperRep>
void HyperMorpho<HyperRep>::hyperClose(const HyperRep & _input_hyper,HyperRep& _output_hyper, StructElement stel){
    hyperDilate(_input_hyper, _output_hyper, stel);
    HyperRep* cloning = _output_hyper.clone();
    hyperErode(*cloning, _output_hyper, stel);
    free(cloning);
}

}
#endif // HYPERMORPHO_H

/*  @Tramx   */


