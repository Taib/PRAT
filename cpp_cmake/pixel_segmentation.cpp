/*
 * Image hypergraph segmentation based on the hMetis hypergraph partitioning tool by G.Karypis et al.
 * The executable of "shmetis" is part of this hypergraph partitioning tool.
 *
 * This example is performed on a pixel based image hypergraph representation
 */
#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <sstream>
#include "IANH.h"
#include "NLIH.h"

#include "ImageNeighborKernel.h"
#include "DistanceKernel.h"


using namespace std;
typedef Hyper::IANH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> IANH;
typedef Hyper::NLIH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> NLIH;



/*
 * method : segmentAfterHMETS is a template function that aggregates the result the partitioning
 *          algorithm. Here the template class is either IANH or NLIH 
 * params :
 *     _segmentFile   : path to the partition result
 *     _noisy         : number of partition used to perform the partitioning process
 *     _hyper         : the hypergraph of the original image
 *
 * output : display the results
 */
template <class ImageHyper>
void segmentAfterHMETIS(const char* _segmentFile, int _kway, ImageHyper& _hyper){
  /* The Hmetis segmentation file must be accessible to execute the code below! */
  /****************************************/

  /// Read partition result from file
  std::ifstream part;
  part.open(_segmentFile);
  std::vector<Hyper::HyperEdge> blocks(_kway);
  std::string aux;
  if( !part.is_open()) {
      std::cout<<"No partition file found"<<std::endl;
      return;
  }
  // cluster construction 
  for(long i = 0; i<_hyper.getSize(); i++){
    part >> aux;
    blocks[atoi(aux.data())].add(i);
  }
  part.close();
  // init the result image
  IplImage * segment =cvCreateImage(cvSize(_hyper.getImage()->width, _hyper.getImage()->height), IPL_DEPTH_8U, 1);
  // compute the mean intensity value of all clusters
  std::vector<int> mean_blocks;
  for(int j = 0; j<_kway; j++){
    double mean = 0.0;
    for(int k = 0; k<blocks.at(j).size(); k++){
      CvPoint pos = Hyper::Utils::numToCoord(blocks.at(j).at(k), _hyper.getImage()->width);
      mean+=cvGet2D(_hyper.getImage(), pos.x, pos.y).val[0];
    }
    mean_blocks.push_back( mean/blocks.at(j).size());
  }
  // value assigment
  std::cout<<"[Segmentation] collecting Hmetis clusters... "<<std::endl;
  for(long i = 0; i < _hyper.getSize(); i++){
    int kpart = -1;
    for(int j = 0; j<_kway;j++){
      if(blocks[j].setContains(i)) {
	kpart = j;
	break;
      }
    }
    CvPoint pos = Hyper::Utils::numToCoord(i, _hyper.getImage()->width);
    cvSet2D(segment, pos.x, pos.y, cvScalar((mean_blocks[kpart] )));
    
  }
  
  // display process
  cvNamedWindow( "Original window", CV_WINDOW_NORMAL );
  cvShowImage("Original window", _hyper.getImage() );
  cvNamedWindow( "segmented window", CV_WINDOW_NORMAL );
  cvShowImage( "segmented window", segment );
  
  cvWaitKey(0);
  cvDestroyWindow("Original window");
  cvDestroyWindow("segmented window");
  //cvSaveImage("segment.jpg", segment); // uncomment to save the segmentation result
}



int main( int argc, char** argv )
{
    if( argc != 3)
    {
     cout <<" Usage: ./main ImagePath kway" << endl;
     return -1;
    }

   IplImage *  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

   if(! image->nSize)
    {
        cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
        return -1;
    }
   IANH hyper(image);
   std::cout<<"[Segmentation] card(hyper) = "<<hyper.getSize()<<"  "<<image->nSize<<std::endl;
 
   stringstream path ;
   path<< argv[1]<<".hgr";
   // building the weighted hypergraph 
   hyper.toWeightedFile(path.str().data());

   int kway = atoi(argv[2]); 
   stringstream command;
   command<<"./shmetis "<<path.str().data()<<" "<<kway<<" "<<5;
   std::cout<<path.str()<<" ::: "<<command.str().data()<<std::endl;
   // execute the partitioning tool : make sure the binary "shmetis" is accessible
   system(command.str().data()); 
   path<<".part."<<kway;
   // compute the aggregation
   segmentAfterHMETIS<IANH>(path.str().data(), kway, hyper);


   cvReleaseImage(&image);
   return 0;
}
