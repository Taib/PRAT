
/*  @Tramx   */

#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

#include "HyperMorpho.h"
#include "ImageNeighborKernel.h"
#include "SLIC_Hyper.h"
#include <ctime>



void superpixel_morpho(char * _fname){
    int nbr_superPixels   = 1000,
        weight_factor     = 80,
        kNearestNeighbors = 9;
    Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> slic(nbr_superPixels, weight_factor, _fname, kNearestNeighbors);
    slic.compute_hyper();
    
    Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> * dilate1 = slic.clone(); // Should free the pointer after using clone function!
    Hyper::HyperMorpho<Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> >::hyperDilate(slic, *dilate1, Hyper::StructElement());

    //Hyper::SLIC_Hyper<Hyper::ImageNeighborKernel::NeighborSmall> * dilate2 = dilate1->clone();
    //Hyper::HyperMorpho<Hyper::SLIC_Hyper<Hyper::ImageNeighborKernel::NeighborSmall> >::hyperDilate(*dilate1, *dilate2, Hyper::StructElement());
    //dilate1->showSuperPixelNeighbors(10);


    IplImage * cDilate = cvCloneImage(slic.getImage());
    cvDilate(slic.getImage(), cDilate, cvCreateStructuringElementEx(5, 5, 1,1, CV_SHAPE_RECT));

    IplImage* diffDilations = cvCloneImage(slic.getImage());
    cvAbsDiff(dilate1->getImage(), cDilate, diffDilations);


    std::vector<const IplImage *> imgs;
    imgs.push_back(slic.getImage());
    imgs.push_back(slic.getContoursImage());
    imgs.push_back(dilate1->getImage());
    imgs.push_back(cDilate);
    imgs.push_back(diffDilations);

    std::vector<std::string> names;
    names.push_back("Original");
    names.push_back("Contours");
    names.push_back("SLICDilate1");
    names.push_back("ClassicDilate1");
    names.push_back("Diff Dilatations");

    Hyper::showImages(imgs, names);
    //Hyper::saveImages(imgs, names, "morpho_damier_slic/");

    free(dilate1);    
}

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: ./main ImagePath" << endl;
     return -1;
    }

   IplImage *  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

   if(! image->imageData)
    {
        cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
        return -1;
    }
   superpixel_morpho(argv[1]);
    return 0;
}

// @Tramx
