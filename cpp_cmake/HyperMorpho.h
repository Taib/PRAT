#ifndef HYPERMORPHO_H
#define HYPERMORPHO_H

#include "IANH.h"

namespace Hyper{
  /* We provide here a template class to perform morphological operations on an image hypergraph (region or pixel based).
   *
   * The algorithms are based on the paper : "Morphological operations on hypergraphs : an application" by I. Bloch et al.
   *
   * In this version, the standard structurant element we use maps a hyperedge to a set that contains all neighbors (the neighbors has non-empty intersection).
   *
   */

  //TODO : perform morphological operations according to a given structurant element.
class StructElement{
  /*
   * This class should contain different structurant element.
   */
public:
    StructElement(){}
};

template<class HyperRep = HyperGraph>
class HyperMorpho
{
public:
    inline HyperMorpho(){}
   
   /* method :  hyperDilate performs a dilation on a given hypergraph.
    * params : 
    *     _input_hyper  : the input hypergraph 
    *     _output_hyper : the output hypergraph
    *     _stel         : the structurant element
    * 
    */
    static void hyperDilate(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement _stel);
   
   /* method :  hyperErode performs an erosion on a given hypergraph.
    * params : 
    *     _input_hyper  : the input hypergraph 
    *     _output_hyper : the output hypergraph
    *     _stel         : the structurant element
    * 
    */
    static void hyperErode(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement _stel);
   /* method :  hyperOpen performs an opening on a given hypergraph.
    * params : 
    *     _input_hyper  : the input hypergraph 
    *     _output_hyper : the output hypergraph
    *     _stel         : the structurant element
    * 
    */
    static void hyperOpen(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement _stel);
   
   /* method :  hyperClose performs a closing on a given hypergraph.
    * params : 
    *     _input_hyper  : the input hypergraph 
    *     _output_hyper : the output hypergraph
    *     _stel         : the structurant element
    * 
    */
    static void hyperClose(const HyperRep & _input_hyper, HyperRep& _output_hyper, StructElement _stel);
   /* method :  hyperSimilarity (not implemented) performs the similarity between two hypergraph.
    * params : 
    *     _hyper_src  : the input hypergraph 
    *     _hyper_dst : the output hypergraph
    *     _stel    : the structurant element
    * 
    */
    static float hyperSimilarity(const HyperRep & _hyper_src, const HyperRep & _hyper_dst, StructElement _stel);
};
////////////////////////Implementation //////////////////////////

// TODO: hyperSimilarity implementation
template<class HyperRep>
float HyperMorpho<HyperRep>::hyperSimilarity(const HyperRep & _hyper_src, const HyperRep & _hyper_dst, StructElement _stel){
    float all_sim = 0.0;
    throw "[HyperSimilarity] not implemented!";
}

template<class HyperRep>
void HyperMorpho<HyperRep>::hyperDilate(const HyperRep & _input_hyper, HyperRep &_output_hyper, StructElement _stel){
  // for each pixel : note in our image representation, the number of vertices(pixel or region) eqauls to the number of hyperedge.
  for(unsigned int i = 0 ; i <_input_hyper.getSize(); ++i)
    {
      // store the current hyperedge
      HyperEdge *  curng      = new HyperEdge(_input_hyper.at(i));
      // compute its neighbors : searchField is a set that contains all vertices of the neighbors of the current hyperedge.
      HyperEdge * searchField = new HyperEdge(_input_hyper.neighborHyperEdgeSets( *curng));
      // get their values : of the vertices(pixels or regions) 
      std::vector<CvScalar> * values = new std::vector<CvScalar>(_input_hyper.neighborsValues( *searchField));
      // modifying the current vertex
      _output_hyper.setPixelValue(i, Utils::max(*values));
      delete values;
      free(curng);
      free(searchField);
      
    }
}

// Same technic used in the hyperDilate function
template<class HyperRep>
void HyperMorpho<HyperRep>::hyperErode(const HyperRep & _input_hyper, HyperRep &_output_hyper, StructElement _stel){
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
void HyperMorpho<HyperRep>::hyperOpen(const HyperRep & _input_hyper, HyperRep &_output_hyper, StructElement _stel){
    hyperErode(_input_hyper, _output_hyper, stel);
    HyperRep* cloning = _output_hyper.clone(); // must free after cloning (bad implementation!!!)
    hyperDilate(*cloning, _output_hyper, stel);
    free(cloning);
}

template<class HyperRep>
void HyperMorpho<HyperRep>::hyperClose(const HyperRep & _input_hyper,HyperRep& _output_hyper, StructElement _stel){
    hyperDilate(_input_hyper, _output_hyper, stel);
    HyperRep* cloning = _output_hyper.clone();
    hyperErode(*cloning, _output_hyper, stel);
    free(cloning);
}

}
#endif // HYPERMORPHO_H

/*  @Tramx   */


