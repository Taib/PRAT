/*
 * Image HyperGraph denoising example.
 * The denoising algorithm was designed for S&P noise.
 *
 */
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

// create a noisy image given a threshold white pixels "_thresW" and a threshold of black pixels "_thresB"
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

/*
 * method : denoising template function for denoising. Here the template class is either IANH or NLIH 
 * params :
 *     _image    : original clean image
 *     _noisy    : the noisy image
 *     _savePath : a path to save the result 
 *
 * output : display the denoised image
 */
template<class ImageHyper>
void  denoising(IplImage * _image, IplImage* _noisy, char* _savePath){
  // build the noisy image's hypergraph
  ImageHyper hyper(_noisy);
  //hyper.toFile("noisyHyper.txt");
  
  // init the result
  IplImage * imdenoise = cvCloneImage(_noisy);
  // start the denoising operation
  Hyper::HyperDenoise<ImageHyper >::hyperDenoise(hyper, imdenoise);
  // compute the PSNR between the clean image and the denoised one
  double psnr = Hyper::HyperDenoise<ImageHyper >::compute_PSNR(_image, imdenoise);
  std::cout<<"[Denoise] PSNR = "<<psnr<<std::endl;
    
  // Displaying the results
  std::vector<const IplImage *> imgs;
  imgs.push_back(_image);
  imgs.push_back(_noisy);
  imgs.push_back(imdenoise);
  
  std::vector<std::string> names;
  names.push_back("Original");
  names.push_back("Noisy");
  names.push_back("Denoise");
  Hyper::showImages(imgs, names);
    
  // Uncomment the following to save the results.
  //if(_savePath != NULL)
  //Hyper::saveImages(imgs, names, _savePath);
  cvReleaseImage(imdenoise);
}



int main( int argc, char** argv )
{
    if( argc != 3)
    {
     cout <<" Usage: ./main cleanImagePath noisePercentage([0,100])" << endl;
     return -1;
    }

    IplImage *  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    if(! image->imageData)
    {
        cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
        return -1;
    }
    if(atoi(argv[2]) < 0 || atoi(argv[2]) >100)
      cout << "Invalid noise percentage"<<endl;
    // transform an integer between 0 and 100 to a noise S&P percentage
    int thresWhite  = 255 - (int)255*0.5*atoi(argv[2])*0.01;
    int thresBlack  = (int)255*0.5*atoi(argv[2])*0.01;
    std::cout<<"W:"<<thresWhite<<" !!! B:"<<thresBlack<<"!!! atoi:"<<atoi(argv[2])<<" !!! multi:"<<255*0.5*atoi(argv[2])<<std::endl;
    IplImage* noisy = putNoise(image, thresWhite, thresBlack);

    denoising<IANH>(image, noisy, NULL);
    //denoising<NLIH>(image, noisy, NULL); 

    return 0;
}

// @Tramx

