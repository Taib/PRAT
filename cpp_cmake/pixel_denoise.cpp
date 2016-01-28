
/*  @Tramx   */

#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include "IANH.h"
#include "NLIH.h"

#include "HyperDenoise.h"
#include "ImageNeighborKernel.h"
#include "DistanceKernel.h"


using namespace std;
typedef Hyper::IANH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> IANH;
typedef Hyper::NLIH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> NLIH;

IplImage* putNoise(IplImage* _image, int _thresW, int _thresB){
    cv::Mat img = cv::Mat(_image);
    cv::Mat saltpepper_noise = cv::Mat::zeros(img.rows, img.cols,CV_8U);
    randu(saltpepper_noise,0,255);

    cv::Mat black = saltpepper_noise < _thresB;
    cv::Mat white = saltpepper_noise > _thresW;

    cv::Mat saltpepper_img_mat = img.clone();
    saltpepper_img_mat.setTo(255,white);
    saltpepper_img_mat.setTo(0,black);

    IplImage  saltpepper_img = (IplImage)saltpepper_img_mat;
    return cvCloneImage(&saltpepper_img);
}


template<class ImageHyper>
IplImage *  denoising(IplImage * _image, IplImage* _noisy, char* _savePath){

    ImageHyper hyper(_noisy);
    //hyper.toFile("noisyHyper.txt");


    IplImage * imdenoise = cvCloneImage(_noisy);
    Hyper::HyperDenoise<ImageHyper >::hyperDenoise(hyper, imdenoise);
    double psnr = Hyper::HyperDenoise<ImageHyper >::compute_PSNR(_image, imdenoise);
    std::cout<<"[Denoise] PSNR = "<<psnr<<std::endl;
    
    
    std::vector<const IplImage *> imgs;
    imgs.push_back(_image);
    imgs.push_back(_noisy);
    imgs.push_back(imdenoise);

    std::vector<std::string> names;
    names.push_back("Original");
    names.push_back("Noisy");
    names.push_back("Denoise");
    Hyper::showImages(imgs, names);
    
    //if(_savePath != NULL)
    //Hyper::saveImages(imgs, names, _savePath);
}



int main( int argc, char** argv )
{
    if( argc != 3)
    {
     cout <<" Usage: ./main cleanImagePath noisePercentage(e.g. 5)" << endl;
     return -1;
    }

    IplImage *  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    if(! image->imageData)
    {
        cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
        return -1;
    }
    int thresWhite  = 255 - (int)255*0.5*atoi(argv[2])*0.01;
    int thresBlack  = (int)255*0.5*atoi(argv[2])*0.01;
    std::cout<<"W:"<<thresWhite<<" !!! B:"<<thresBlack<<"!!! atoi:"<<atoi(argv[2])<<" !!! multi:"<<255*0.5*atoi(argv[2])<<std::endl;
    IplImage* noisy = putNoise(image, thresWhite, thresBlack);

    denoising<IANH>(image, noisy, NULL);
    //denoising<NLIH>(image, noisy);

    return 0;
}

// @Tramx

