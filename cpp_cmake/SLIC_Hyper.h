#ifndef SLIC_HYPER_H
#define SLIC_HYPER_H

#include "SLIC.h"
#include "Utils.h"
#include <fstream>
#include "HyperGraph.h"
namespace Hyper {
  /**
   * We provide in this file, a class to build a region based image hypergraph.
   * The regions are computed from the SLIC algorithm, implemented by [Pascal Mettes].
   *
   * This file contains two classes : 
   *        - SuperPixel : to handle a superpixel operations
   *        - NRIH       : that implements a region based image hypergraph
   */

class SuperPixel :public Set<long>{
  /*
   * Here, we implement a superpixel as a set of pixels indices.
   * Note: The index of a pixel is computed given its (x,y) coordinates and the width of the image.
   *       The index of the pixel at position (a,b) in the image of size NxM is an integer Index(a,b)=axM + b.
   *
   */
protected:
  // values of the pixels belonging to this superpixel
  std::vector<CvScalar> values;
  // the mean of the intensity value in the superpixel
  CvScalar mean;
  // the standard deviation of the intensity value in the superpixel
  CvScalar std_dev;
  // the position (x,y) of the center of the superpixel
  CvPoint position;
public:
  // constructors
  SuperPixel(){}
  SuperPixel(Set<long> _pixels, std::vector<CvScalar> _values){
    this->data = _pixels.getDataCopy(); values = _values;
  }
  // getters
  CvScalar getMeanValue() const{return mean;}
  CvScalar getStdvalue() const{return std_dev;}
  CvPoint getPosition() const{return position;}
  CvScalar getMeanValue(){return mean;}
  CvScalar getStdvalue(){return std_dev;}
  CvPoint getPosition(){return position;}
  std::vector<CvScalar> getValues(){return values;}
  // setters
  void setPosition(CvPoint _pos){position = _pos;}
  // after the assigment of pixels to a superpixel, this function store the values of the pixels in the superpixel
  void setValues(const IplImage * _img);
  // deprecated :  should compute the dot product of two superpixel with different sizes
  static CvScalar dotProduct(SuperPixel _sp1, SuperPixel _sp2);
  //void add(long _pixel, CvScalar _value){data.add(_pixel); values.push_back(_value);}
};

void SuperPixel::setValues(const IplImage* _img){
    for(int i = 0; i<data.size(); i++){
        CvPoint pos = Utils::numToCoord(data.at(i), _img->width);
        values.push_back(cvGet2D(_img, pos.x, pos.y));
    }
    mean    = Utils::mean(values);
    std_dev = Utils::standardDeviation(values);
}

// deprecated
CvScalar SuperPixel::dotProduct(SuperPixel _sp1, SuperPixel _sp2){
    int tour                    = std::max(_sp1.size(), _sp2.size()) - std::min(_sp1.size(), _sp2.size());
    CvScalar result             = cvScalarAll(99999999.0);
    std::vector<CvScalar> val1  = _sp1.getValues(),
                          val2  = _sp2.getValues();
    if(_sp1.size()< _sp2.size()){
        for(int i = 0; i<tour; i++){
            std::vector<CvScalar> slider;
            for(int j = i; j<_sp1.size(); j++)
                slider.push_back(val2.at(j));
            CvScalar res = Utils::dot(val1, slider);
            if(res.val[0]<result.val[0])
                result = res;
        }
        return result;
    }
    for(int i = 0; i<tour; i++){
        std::vector<CvScalar> slider;
        for(int j = i; j<_sp1.size(); j++)
            slider.push_back(val2.at(j));
        CvScalar res = Utils::dot(val1, slider);
        if(res.val[0]<result.val[0])
            result = res;
    }
    return result;
}

template <class ImageNeighborhoodKernel>
class NRIH : public HyperGraph{
  /*
   * The class NRIH (Naive Region-based Image Hypergraph) needs a kernel to compute the neighbors of a superpixel. 
   *In this version, the kernel is not really used, instead the number of neighbor is given in the constructor of the class.
   *
   */
public:

    inline NRIH(){}
    /*
     * method : NRIH constructor
     * params : 
     *     _nbr_superpixels   : the maximal number of superpixel computed by the SLIC algorithm
     *     _nc                : the weight factor used by the SLIC algorithm
     *     _fname             : the image path
     *     _kNearestNeighbors : the number of neighbor of a superpixel to be considered in the hyperedge computation. 
     */
    NRIH(int _nbr_superpixels, int _nc, char*_fname, int _kNearestNeighbors);
    inline ~NRIH(){cvReleaseImage(&image);}
    
    // basic method from the class HyperGraph : computes the hypergraph of the image
    void compute_hyper();
    
    // getters
    inline CvPoint getImageSize(){return imSize;}
    inline CvPoint getImageSize() const {return imSize;}
    inline const SuperPixel& getSuperPixel(long _id){return superPixels.at(_id);}

    // return a hyperedge neighbors values 
    std::vector<CvScalar> neighborsValues(const HyperEdge & ng);
    void neighborsValues(const HyperEdge & ng, std::vector<CvScalar>* _output) const;
    std::vector<CvScalar> neighborsValues(const HyperEdge & ng)const;
    void neighborsValues(const HyperEdge & ng, std::vector<CvScalar>* _output);


    
    // Creates a file "_outName" that contains the weighted image hypergraph. 
    // Here only the vertices(superpixel) are weighted (with their mean intensity values).
    // The output file "_outName" can be an input of the hMetis hypergraph partitioning tool. 
    void toWeightedFile(const char * _outName);

    // show the superpixel given its index "_i" in the image
    void showSuperPixelNeighbors(int _i);
    
    // actually this function should be called "setSuperPixelValue".
    // it affect a given value to all pixels in the given hyperedge index
    void setPixelValue(long _hyperEdgeId, CvScalar _value);

    // get/set superpixels
    inline void setSuperPixels(std::vector<SuperPixel> _spixels){superPixels =_spixels;}
    inline std::vector<SuperPixel> getSuperPixels(){return superPixels;}
    // clone the current class
    NRIH<ImageNeighborhoodKernel>* clone();
    // get/set the image
    inline void setImage(const IplImage* _img){image = cvCloneImage(_img); imSize = cvPoint(image->height, image->width);}
    inline const IplImage* getImage(){return image;}
    // return an image containing the superpixels
    inline IplImage* getContoursImage(){return contourImage;}

private:
    IplImage* contourImage;
    std::vector<SuperPixel> superPixels;
    CvPoint imSize;
    int nbr_superPixels ;
    int nc ;
    int knn;
    double step;
    IplImage * image;
    std::string fname;

    std::vector<CvPoint> getSuperPixelsPositions();

};
/////////////////////////////////////////Implementation/////////////////////////////////////////////////////////////////////////
///
///

template <class ImageNeighborhoodKernel>
NRIH<ImageNeighborhoodKernel>::NRIH(int _nbr_superpixels, int _nc, char*_fname, int _kNearestNeighbors){
    std::cout<<"[SLIC_SuperPixel] constructor."<<std::endl;
    nbr_superPixels = _nbr_superpixels;
    nc              = _nc;
    knn             = _kNearestNeighbors;
    fname.assign(_fname);
    image = cvLoadImage(_fname, CV_LOAD_IMAGE_GRAYSCALE);

}

template <class ImageNeighborhoodKernel>
NRIH<ImageNeighborhoodKernel>* NRIH<ImageNeighborhoodKernel>::clone(){
    NRIH<ImageNeighborhoodKernel>* res = new NRIH<ImageNeighborhoodKernel>();
    res->setHyper(this->getHyper());
    res->setImage(this->image);
    res->setSuperPixels(this->getSuperPixels());
    return res;
}
template <class ImageNeighborhoodKernel>
void NRIH<ImageNeighborhoodKernel>::compute_hyper(){
    std::cout<<"[SLIC_Hyper] computation..."<<std::endl;
    // Load the image and convert to Lab colour space.
    IplImage * lab_image = cvCloneImage(image);

    imSize = cvPoint(image->height, image->width);
    step   = sqrt((imSize.x * imSize.y) / (double) nbr_superPixels);

    /* Perform the SLIC superpixel algorithm. */
    Slic slic;
    slic.generate_superpixels(lab_image, step, nc);
    std::cout<<"start"<<std::endl;
    slic.create_connectivity(lab_image);
    std::cout<<"[SLIC_Hyper] computed"<<std::endl;

    /* save the contours image.*/
    contourImage = cvCloneImage(lab_image);
    slic.display_contours(contourImage, CV_RGB(255,0,0));
    // get the centers and clusters of superpixels
    std::vector<std::vector<double> > centers = slic.getCenters();
    std::vector<std::vector<int> > clusters = slic.getClusters();
    std::cout<<"[SLIC_Hyper] superpixels begin "<<centers.size()<<std::endl;
    superPixels = std::vector<SuperPixel>(centers.size());
    // create the superpixels
    for(int j = 0; j < image->width ; j++){
        for(int i = 0; i < image->height; i++){
            int id = clusters[j][i];
            superPixels[id].add(i*image->width +j);
        }
    }
    // save the superpixels' positions
    std::cout<<"[SLIC_Hyper] superpixels positioning "<<superPixels.at(0).size()<<std::endl;
    for(int i = 0; i<centers.size(); i++){
        superPixels[i].setPosition(cvPoint(centers[i][3], centers[i][4]));
        superPixels.at(i).setValues(image);
    }
    std::cout<<"[SLIC_Hyper] superpixels computed"<<std::endl;

    // dumb way to finally get their position
    std::vector<CvPoint> superPixelsPositions = getSuperPixelsPositions();
    // for each superpixel
    for(int i = 0; i< superPixels.size(); i++){
      // compute its neighbors : k nearest neighbor fashion.
      std::vector<std::pair<double, int> > ng = Utils::knn(superPixelsPositions, superPixels.at(i).getPosition());
      // compute its mean intesity value
      std::vector<double> means;
      for(int j = 0; j< ng.size(); j++){
	means.push_back(superPixels.at(ng.at(j).second).getMeanValue().val[0]);
      }
      // compute the standard deviation
      std::vector<double> stds;
      for(int j = 0; j< ng.size(); j++){
	stds.push_back(superPixels.at(ng.at(j).second).getStdvalue().val[0]);
      }
      // compute the standard deviation of the mean intensity values
      double alpha    = Utils::standardDeviation(means);
      // hyperedge construction
      HyperEdge gamma;
      for(int j = 0; j< ng.size(); j++ ){
	if(Utils::abs<double>(superPixels.at(ng.at(0).second).getMeanValue().val[0] - superPixels.at(ng.at(j).second).getMeanValue().val[0])<=alpha)
	  gamma.add(ng.at(j).second);
      }
      this->hyper.push_back(gamma);
    }
}

template <class ImageNeighborhoodKernel>
void NRIH<ImageNeighborhoodKernel>::setPixelValue(long _hyperEdgeId, CvScalar _value){
    SuperPixel sp = superPixels.at(_hyperEdgeId);
    for(int j = 0; j<sp.size(); j++){
        CvPoint pos = Utils::numToCoord(sp.at(j), image->width);
        cvSet2D(image, pos.x, pos.y, _value);
    }
}

template <class ImageNeighborhoodKernel>
void NRIH<ImageNeighborhoodKernel>::showSuperPixelNeighbors(int _id){
    IplImage* aux = cvCloneImage(image);
    std::vector<std::pair<double, int> > ng = Utils::knn(getSuperPixelsPositions(), superPixels.at(_id).getPosition());
    for(int i = 0; i<1; i++){
        SuperPixel sp = superPixels.at(ng.at(i).second);
        for(int j = 0; j<sp.size(); j++){
            CvPoint pos = Utils::numToCoord(sp.at(j), aux->width);
            cvSet2D(aux,pos.x, pos.y, cvScalar(255));//sp.getMeanValue().val[0]));
        }
    }
    for(int i = 1; i<ng.size(); i++){
        SuperPixel sp = superPixels.at(ng.at(i).second);
        for(int j = 0; j<sp.size(); j++){
            CvPoint pos = Utils::numToCoord(sp.at(j), aux->width);
            cvSet2D(aux,pos.x, pos.y, cvScalar(0));//sp.getMeanValue().val[0]));
        }
    }
    cvShowImage("showSuperPixel", aux);
    cvWaitKey(0);
}

template <class ImageNeighborhoodKernel>
std::vector<CvPoint> NRIH<ImageNeighborhoodKernel>::getSuperPixelsPositions(){
    std::vector<CvPoint> res;
    for(int i = 0; i< superPixels.size(); i++)
        res.push_back(superPixels.at(i).getPosition());
    return res;
}

template <class ImageNeighborhoodKernel>
std::vector<CvScalar> NRIH<ImageNeighborhoodKernel>::neighborsValues( const HyperEdge & ng) const
{
    std::vector<CvScalar> values (ng.size());
    for(long i = 0; i< values.size(); i++){
        SuperPixel sp = superPixels.at(ng.at(i));
        for(int j = 0; j< sp.size(); j++){
            CvPoint p = Utils::numToCoord(sp.at(j), image->width);
            values[i] = (cvGet2D(image, p.x, p.y));
        }
    }
    return values;
}

template <class ImageNeighborhoodKernel>
void NRIH<ImageNeighborhoodKernel>::neighborsValues( const HyperEdge & ng, std::vector<CvScalar>* _output) const
{
    for(long i = 0; i< ng.size(); i++){
        SuperPixel sp = superPixels.at(ng.at(i));
        for(int j = 0; j< sp.size(); j++){
            CvPoint p = Utils::numToCoord(sp.at(j), image->width);
            _output->push_back(cvGet2D(image, p.x, p.y));
        }

    }
}

template <class ImageNeighborhoodKernel>
std::vector<CvScalar> NRIH<ImageNeighborhoodKernel>::neighborsValues( const HyperEdge & ng)
{
    std::vector<CvScalar> values (ng.size());
    for(long i = 0; i< values.size(); i++){
        SuperPixel sp = superPixels.at(ng.at(i));
        for(int j = 0; j< sp.size(); j++){
            CvPoint p = Utils::numToCoord(sp.at(j), image->width);
            values[i] = (cvGet2D(image, p.x, p.y));
        }
    }
    return values;
}

template <class ImageNeighborhoodKernel>
void NRIH<ImageNeighborhoodKernel>::neighborsValues( const HyperEdge & ng, std::vector<CvScalar>* _output)
{
    for(long i = 0; i< ng.size(); i++){
        SuperPixel sp = superPixels.at(ng.at(i));
        for(int j = 0; j< sp.size(); j++){
            CvPoint p = Utils::numToCoord(sp.at(j), image->width);
            _output->push_back(cvGet2D(image, p.x, p.y));
        }

    }
}

template <class ImageNeighborhoodKernel>
void NRIH<ImageNeighborhoodKernel>::toWeightedFile(const char * _outName)
{
    std::ofstream out (_outName, std::ios::out);
    out << hyper.size()<<" "<<hyper.size()<<" 10 \n";
    for(long i = 0; i<hyper.size(); i++){
        out << hyper[i].toString() << "\n";
    }
    for(long i = 0; i<superPixels.size(); i++){
        out << (int)superPixels.at(i).getMeanValue().val[0]<<"\n";
    }
    out.close();
}
}


#endif // SLIC_HYPER_H

/*  @Tramx   */

