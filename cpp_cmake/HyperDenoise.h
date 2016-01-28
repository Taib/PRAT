#ifndef HYPERDENOISE_H
#define HYPERDENOISE_H

#include "IANH.h"
/**
 * This class is an implementation of a denoising algorithm on hypergraphs.
 * More details on the algorithm can be found in : "Application of Adaptive Hypergraph Model to Impulsive Noise Detection" by Rital et al.
 */
namespace Hyper{

template<class HyperRep>
class HyperDenoise
{
private:
    typedef typename HyperRep::ImgNeighourKernel ImgNeighbourKernel;
    typedef typename HyperRep::PxDistKernel PxDistKernel;

    static bool openTest(const HyperRep &_hyper, HyperEdge * _ng,   long _x);
    static HyperEdge * noisyHyperEdges(HyperRep& _hyper);

public:
     inline HyperDenoise(){}
     static void hyperDenoise(HyperRep & _hyper, IplImage * _output);
     static double compute_PSNR(const IplImage* _im1, const IplImage* _im2);

};

/////////////////////////Implementation ///////////////////////////


template<class HyperRep>
void HyperDenoise<HyperRep>::hyperDenoise(HyperRep & _hyper, IplImage * _output)
{
    std::cout<<"Hyper denoising..."<<std::endl;
    HyperEdge * nh = new HyperEdge(noisyHyperEdges(_hyper)->getDataCopy());
    std::vector<CvScalar> * values = new std::vector<CvScalar>(0);
    HyperEdge  edge;// = new HyperEdge();
    HyperEdge  ng  ;// = new HyperEdge();
    for(unsigned int i = 0; i < nh->size(); i++){
        if(true){
            ImgNeighbourKernel::neighbors(nh->at(i), _hyper.getImageSize(), &edge);
            ng = _hyper.neighborHyperEdgeSets( edge);//, ng);
        }
        ng.remove(ng.indexOf(nh->at(i)));
        CvPoint pos  = Utils::numToCoord(nh->at(i), _hyper.getImageSize().y);
        /// May be good to consider only different neighbours
        /**/
        for(int j = 0; j < ng.size(); j++){
            CvPoint pos_aux  = Utils::numToCoord(ng.at(j), _hyper.getImageSize().y);
            if(cvGet2D(_hyper.getImage(), pos_aux.x, pos_aux.y).val[0] == cvGet2D(_hyper.getImage(), pos.x, pos.y).val[0])
                ng.remove(j);
        }
        //*/
        _hyper.neighborsValues( ng, values);
        cvSet2D(_output, pos.x, pos.y, cvScalar((int)Utils::mean(*values).val[0]));
        values->clear();
        ng.clear();
        edge.clear();
    }
    std::cout<<"Hyper denoising finish."<<std::endl;
    delete nh;
}


template<class HyperRep>
double HyperDenoise<HyperRep>::compute_PSNR(const IplImage* _im1,const  IplImage* _im2){
    if((_im1->height != _im2->height ) || (_im1->width != _im2->width))
        return -1.0;
    IplImage* diff = cvCreateImage(cvSize(_im1->width, _im1->height), IPL_DEPTH_8U, 1);
    cvAbsDiff(_im1, _im2, diff);
    double mini    = 0.0;
    double maxi    = 0.0;
    double eqm     = 0.0;
    for(int l = 0; l < _im1->height; l++){
        for(int c = 0; c < _im1->height; c++){
            eqm += cvGet2D(diff, l,c).val[0]*cvGet2D(diff, l,c).val[0];
            mini = std::min(std::min(mini, cvGet2D(_im1, l,c).val[0]), cvGet2D(_im2, l,c).val[0]) ;
            maxi = std::max(std::min(maxi, cvGet2D(_im1, l,c).val[0]), cvGet2D(_im2, l,c).val[0]) ;
        }
    }
    double d = maxi - mini +1;
    eqm = eqm*(1.0/(_im1->height*_im1->width));
    std::cout<<"max "<<maxi<<" ::: mini "<<mini<<" ::: eqm "<<eqm<<std::endl;
    return log10(255*255/std::max(eqm,1.0)) * 10.0;
}

template<class HyperRep>
bool HyperDenoise<HyperRep>::openTest(const HyperRep & _hyper, HyperEdge * _ng, long  _x)
{
    long k = _ng->at(_x);
    HyperEdge on = _hyper.openNeighborhood(k);
    for(long i = 0; i < on.size(); i++){
        if(on.at(i)!=_x && _hyper.at(on.at(i)).size() == 1){
            return true;
        }
    }
    return false;
}

template<class HyperRep>
HyperEdge* HyperDenoise<HyperRep>::noisyHyperEdges(HyperRep & _hyper)
{
    HyperEdge* nh = new HyperEdge();
    HyperEdge* is = new HyperEdge();

    // Determination of isolated hyperedges of Hyper
    for(unsigned long i = 0; i<_hyper.getImageSize().x; i++){
        for(unsigned long j = 0; j<_hyper.getImageSize().y; j++){

            HyperEdge ex = _hyper.at(i*_hyper.getImageSize().y + j);
            HyperEdge ep = _hyper.neighborHyperEdgeSets( ex);
            if(ep == ex){
                is->add(i*_hyper.getImageSize().y + j);
            }
        }
    }
    std::cout<<"IS completed.  "<<is->size()<<std::endl;
    // Detection of noise hyperedges
    for(unsigned long i = 0; i<is->size(); i++){
        CvPoint ngi  = Utils::numToCoord(is->at(i), _hyper.getImageSize().y);
        HyperEdge aux  = _hyper.at(ngi.x*_hyper.getImageSize().y + ngi.y);
        if( (aux.size() == 1) || openTest(_hyper, is, i))
            nh->add(is->at(i));
    }
    std::cout<<"NH completed.  "<<nh->size()<<std::endl;
    delete is;
    return nh;
}

}
#endif // HYPERDENOISE_H

/*  @Tramx   */

