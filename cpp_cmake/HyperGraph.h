#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H
#include <iostream>
#include <vector>
#include <fstream>
#include "Utils.h"
/***
 * We define in this file the following element : 
 * HyperGraphAbstract : Abstract class which contains basic methods to build a hypergraph
 * HyperEdge         : is a set of integers
 * HyperGraph       : is also an abstract class but implements some basic methods
 */
namespace Hyper{

template <class HyperEdgeSet >
class HyperGraphAbstract
{
public:
    inline HyperGraphAbstract(){}
    virtual const typename std::vector<HyperEdgeSet> & getHyper() = 0;
    virtual const typename std::vector<HyperEdgeSet> & getHyper() const = 0;
    virtual void setHyper(typename std::vector<HyperEdgeSet> _hyper) = 0;
    virtual void addHyperEdge(HyperEdgeSet _ng) = 0;
    virtual void clear() = 0;
    virtual long getSize() = 0;
    virtual long getSize() const  = 0;
    virtual HyperEdgeSet at(unsigned int  _i) = 0;
    virtual HyperEdgeSet at(unsigned int  _i) const = 0;
    virtual void  neighborHyperEdgeSets(const HyperEdgeSet& _ng, HyperEdgeSet& _output) const = 0;
    virtual HyperEdgeSet neighborHyperEdgeSets(const HyperEdgeSet& _ng)const = 0;
    virtual HyperEdgeSet openNeighborhood(long & _edgeIndex) = 0;
    virtual HyperEdgeSet openNeighborhood(HyperEdgeSet& _ng)  = 0;
    virtual HyperEdgeSet openNeighborhood(long & _edgeIndex) const = 0;
    virtual HyperEdgeSet openNeighborhood(HyperEdgeSet& _ng) const = 0;
    //TODO: Iterator methods
protected:
    // a hypergraph is just a vector of hyperedge
    typename std::vector<HyperEdgeSet> hyper;
};

//////////// HyperEdge definition
typedef Set<long> HyperEdge;


//////////// HyperGraph definition
class HyperGraph //: public HyperGraphAbstract<HyperEdge>
{
  /* We define here a hypergraph as a vector of HyperEdge.
   * Basically, the HyperGraph class provides the main operations needed on a usual hypergraph : add ahyperedge, get the size,...
   */
protected:
    std::vector<HyperEdge> hyper;
public:
    inline HyperGraph(){}
    inline void clear(){hyper.clear();}

    inline std::vector<HyperEdge> &       getHyper()       {return hyper;}
    inline const std::vector<HyperEdge> & getHyper() const {return hyper;}

    inline void setHyper(typename std::vector<HyperEdge> _hyper){this->hyper.clear(); this->hyper.assign(_hyper.begin(), _hyper.end());}
    inline void addHyperEdge(HyperEdge _ng){this->hyper.push_back(_ng);}

    inline long getSize(){return hyper.size();}
    inline long getSize() const {return hyper.size();}

    inline HyperEdge at(unsigned int _i){return ((_i<hyper.size()))?hyper.at(_i):throw "we got pb here : hyper.at";}
    inline HyperEdge at(unsigned int _i) const{return ((_i<hyper.size()))?hyper.at(_i):throw "we got pb here : hyper.at";}

    void toFile(const char *);
    void  neighborHyperEdgeSets( const HyperEdge& _ng, HyperEdge& _output) const;
    HyperEdge  neighborHyperEdgeSets(const HyperEdge& _ng)const;

    HyperEdge openNeighborhood(long & _edgeIndex) const;
    HyperEdge openNeighborhood(long & _edgeIndex) ;
    HyperEdge openNeighborhood(HyperEdge & _ng) const;
    HyperEdge openNeighborhood(HyperEdge & _ng) ;
    
    // Abstract methods
    virtual void setPixelValue(long _hyperEdgeId, CvScalar _value) = 0;
    virtual HyperGraph * clone() = 0;
};

std::ostream & operator<< (std::ostream & output, const HyperGraph & _hyper) {
    output << _hyper.getSize()<<"\n";
    for(long i = 0; i<_hyper.getSize(); i++)
            output << _hyper.at(i).toString() << "\n";
    return output;
}

HyperEdge HyperGraph::neighborHyperEdgeSets(const HyperEdge&  _ng)const{
    HyperEdge edges ;
    try{
        for(long i = 0; i < _ng.size(); i++){
            edges.setUnion(this->at(_ng.at(i)));
        }
    }
    catch(std::exception e){
        std::cout<<"[HyperGraph] : "<<e.what()<<std::endl;
    }

    return edges;
}

void HyperGraph::neighborHyperEdgeSets(const HyperEdge&  _ng, HyperEdge& _output)const{
    try{
        for(long i = 0; i < _ng.size(); i++){
            _output.setUnion(this->at(_ng.at(i)));
        }
    }
    catch(std::exception e){
        std::cout<<"[HyperGraph] : "<<e.what()<<std::endl;
    }
}

HyperEdge HyperGraph::openNeighborhood(HyperEdge & _ng) const{
    HyperEdge res;
    for(long i=0; i<_ng.size(); i++){
        this->neighborHyperEdgeSets(this->at(_ng.at(i)), res);
        res.remove(res.indexOf(_ng.at(i))); // open neighborhood principle
    }
    return res;
}

HyperEdge HyperGraph::openNeighborhood(long & _edgeIndex) const{
    HyperEdge res;
    this->neighborHyperEdgeSets(this->at(_edgeIndex), res);
    res.remove(res.indexOf(_edgeIndex));
    return res;
}


HyperEdge HyperGraph::openNeighborhood(HyperEdge & _ng) {
    HyperEdge res;
    for(unsigned int i=0; i<_ng.size(); i++){
        this->neighborHyperEdgeSets(this->at(_ng.at(i)), res);
        res.remove(res.indexOf(_ng.at(i))); // open neighborhood principle
    }
    return res;
}

HyperEdge HyperGraph::openNeighborhood(long & _edgeIndex) {
    HyperEdge res;
    this->neighborHyperEdgeSets(this->at(_edgeIndex), res);
    res.remove(res.indexOf(_edgeIndex));
    return res;
}

void HyperGraph::toFile(const char * _fname)
{
    std::ofstream out (_fname, std::ios::out);
    out << *this;
    out.close();
}

}
#endif // HYPERGRAPH_H

/*  @Tramx   */

