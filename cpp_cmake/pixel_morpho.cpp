/*
 * Morphological operations on an image hypergraph : Example on a pixel based representation.
 *
 */
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


#include <iostream>
#include "IANH.h"
#include "NLIH.h"

#include "HyperMorpho.h"
#include "ImageNeighborKernel.h"
#include "DistanceKernel.h"

using namespace std;
typedef Hyper::IANH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> IANH;
typedef Hyper::NLIH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> NLIH;

template<class ImageHyper>
void morphology(IplImage * _image){
  // Hypergraph construction 
  ImageHyper hyper(_image);
  
  
  // init the dilation result
  ImageHyper dilate1;
  dilate1.setImage(cvCreateImage(cvSize(_image->width,_image->height), IPL_DEPTH_8U, 1));
  // perform the dilation
  Hyper::HyperMorpho<ImageHyper>::hyperDilate(hyper, dilate1, Hyper::StructElement());

  //ImageHyper * dilate2 = dilate1->clone();
  //Hyper::HyperMorpho<ImageHyper>::hyperDilate(*dilate1, *dilate2, Hyper::StructElement());

  // init the erosion result
  ImageHyper erode1 ;
  erode1.setImage(cvCreateImage(cvSize(_image->width,_image->height), IPL_DEPTH_8U, 1));
  // perform the erosion per se
  Hyper::HyperMorpho<ImageHyper>::hyxperErode(hyper, erode1 , Hyper::StructElement());

  // ImageHyper * erode2 = erode1->clone();
  //Hyper::HyperMorpho<ImageHyper>::hyperErode(*erode1, *erode2, Hyper::StructElement());

  
  /* compute classical morphological operations */
  IplImage * cDilate = cvCloneImage(hyper.getImage());
  cvDilate(_image, cDilate, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));
  
  IplImage * cErode = cvCloneImage(hyper.getImage());
  cvErode(_image, cErode, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));

  /* Compute the differences */
  IplImage * difErodes = cvCloneImage(hyper.getImage());
  cvAbsDiff(erode1.getImage(), cErode, difErodes);
  
  IplImage * difDilations = cvCloneImage(hyper.getImage());
  cvAbsDiff(dilate1.getImage(), cDilate, difDilations);

  /* compute an opening and closing operation on the image hypergraph*/
  ImageHyper open1;
  open1.setImage(cvCreateImage(cvSize(_image->width,_image->height), IPL_DEPTH_8U, 1));
  Hyper::HyperMorpho<ImageHyper >::hyperOpen(hyper, open1, Hyper::StructElement());
  
  ImageHyper close1;
  close1.setImage(cvCreateImage(cvSize(_image->width,_image->height), IPL_DEPTH_8U, 1));
  Hyper::HyperMorpho<ImageHyper >::hyperClose(hyper, close1, Hyper::StructElement());



  /* Displaying process */
  std::vector<const IplImage *> imgs;
  imgs.push_back(_image);
  imgs.push_back(dilate1.getImage());
  imgs.push_back(erode1.getImage());
  imgs.push_back(difErodes);
  imgs.push_back(difDilations);
  imgs.push_back(open1.getImage());
  imgs.push_back(close1.getImage());
  
  std::vector<std::string> names;
  names.push_back("Original");
  
  names.push_back("Dilate_1");
  names.push_back("Erode_1");
  names.push_back("DifErodes");
  names.push_back("DifDilations");
  
  names.push_back("Open_1");
  names.push_back("Close_1");
  Hyper::showImages(imgs, names);
  

  // Uncomment the following to save the results.
  ///
  // Hyper::saveImages(imgs, names, ""); // the last parameters contains a directory path
  //*/
}


int main( int argc, char** argv )
{
  if( argc != 2)
    {
      cout <<" Usage: ./main Image" << endl;
      return -1;
    }
  
  IplImage *  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  
  if(! image->imageData)
    {
      cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
      return -1;
    }
  morphology<IANH>(image);
  //morphology<NLIH>(image);


  cvReleaseImage(image);
  return 0;
}

// @Tramx

