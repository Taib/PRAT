
/*  @Tramx   */

#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <sstream>
#include <fstream>

#include "ImageNeighborKernel.h"
#include "SLIC_Hyper.h"

typedef Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> NRIH;


void segmentAfterHMETIS(const char* _segmentFile,int _kway, NRIH& _slic ){
    /*/ superpixel HyperGraph partition based segmentation /*/
    /*******************************************************/
    /// Read partition result from file
    std::ifstream part;
    part.open(_segmentFile);
    std::vector<Hyper::HyperEdge> blocks(_kway);
    std::string aux;
    if( !part.is_open()) {
        std::cout<<"No partition file found"<<std::endl;
        return;
    }
    for(long i = 0; i<_slic.getSize(); i++){
        part >> aux;
        blocks[atoi(aux.data())].add(i);
    }
    part.close();
    IplImage* segmentation = cvCreateImage(cvSize(_slic.getImageSize().y, _slic.getImageSize().x), IPL_DEPTH_8U, 1);
    std::cout<<"[main] init segmentation"<<std::endl;
    std::vector<int> mean_blocks;
    for(int j = 0; j<_kway; j++){
        double mean = 0.0;
        for(int k = 0; k<blocks.at(j).size(); k++)
            mean+=_slic.getSuperPixel(blocks.at(j).at(k)).getMeanValue().val[0];
        mean_blocks.push_back(uint( mean/blocks.at(j).size()));
    }
    std::cout<<"[main] mean blocks computed"<<std::endl;
    for(long i = 0; i < _slic.getSize(); i++){
        int kpart = -1;
        for(int j = 0; j<_kway;j++){
            if(blocks[j].setContains(i)) {
                kpart = j;
                break;
            }
        }
        const Hyper::SuperPixel & sp = _slic.getSuperPixel(i);
        for (int j = 0; j<sp.size(); j++){
            CvPoint pos = Hyper::Utils::numToCoord(sp.at(j), _slic.getImageSize().y);
            cvSet2D(segmentation, pos.x,pos.y, cvScalar(mean_blocks[kpart]));
        }

    }
    std::cout<<"[main] drawing"<<std::endl;
    cvShowImage("Segment super", segmentation);
    cvWaitKey(0);

    // std::string path = segmentFile;
    // path.append(".jpg");
    // cvSaveImage(path, segmentation);
    //*/

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

   int nbr_superPixels   = 1000,
     weight_factor       = 80,
     kNearestNeighbors   = 9;
   NRIH slic(nbr_superPixels, weight_factor, argv[1], kNearestNeighbors);
   slic.compute_hyper();

   stringstream path ;
   path<< argv[1]<<".hgr";
   slic.toWeightedFile(path.str().data());

   int kway = atoi(argv[2]);
   stringstream command;
   command<<"./shmetis "<<path.str().data()<<" "<<kway<<" "<<5;
   std::cout<<path.str()<<" ::: "<<command.str().data()<<std::endl;
   system(command.str().data()); 
   path<<".part."<<kway;
   segmentAfterHMETIS(path.str().data(), kway, slic);
   return 0;
}
