#ifndef HYPERGRAPH_H
#define HYPERGRAPH_H

namespace Hyper{
template <class HyperEdges>
class HyperGraph
{
public:
    inline HyperGraph(){}
    inline HyperGraph(typename std::vector<HyperEdges> _hyper){setHyper(_hyper);}
    inline virtual const typename std::vector<HyperEdges> & getHyper() {return hyper;}
    inline virtual const typename std::vector<HyperEdges> & getHyper() const {return hyper;}
    inline virtual void setHyper(typename std::vector<HyperEdges> _hyper){this->hyper.clear(); this->hyper.assign(_hyper.begin(), _hyper.end());}
    inline virtual void addHyperEdge(HyperEdges _ng){this->hyper.push_back(_ng);}
    inline virtual long getSize(){return hyper.size();}
    inline virtual long getSize() const {return hyper.size();}
    static HyperEdges neighborEdgeSets(const HyperGraph<HyperEdges> & _hyper, HyperEdges _ng);
    static void neighborEdgeSets(const HyperGraph<HyperEdges> & _hyper, HyperEdges _ng, HyperEdges & _output);
    inline virtual HyperEdges at(long _i){return ((_i<hyper.size()) && (_i>= 0))?hyper[_i]:throw "we got pb here : hyper.at";}
    inline virtual HyperEdges at(long _i) const{return ((_i<hyper.size()) && (_i>= 0))?hyper[_i]:throw "we got pb here : hyper.at";}
protected:
    typename std::vector<HyperEdges> hyper;
};

/////////////////////////Implementation ///////////////////////////
template <class HyperEdges>
HyperEdges HyperGraph<HyperEdges>::neighborEdgeSets(const HyperGraph<HyperEdges> & _hyper, HyperEdges _ng){
    HyperEdges edges ;
    for(unsigned long i = 0; i < _ng.size(); i++){
        edges.setUnion(_hyper.at(_ng.at(i)));
    }
    return edges;
}

template <class HyperEdges>
void HyperGraph<HyperEdges>::neighborEdgeSets(const HyperGraph<HyperEdges> &_hyper, HyperEdges _ng, HyperEdges & _output){
    _output.clear();
    for(unsigned long i = 0; i < _ng.size(); i++){
        _output.setUnion(_hyper.at(_ng.at(i)));
    }
}
}
#endif // HYPERGRAPH_H
