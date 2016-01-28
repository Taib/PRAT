
/*  @Tramx   */

#include <opencv2/core/core.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>
#include "IANH.h"
#include "NLIH.h"



#include "HyperDenoise.h"
#include "HyperMorpho.h"
#include "ImageNeighborKernel.h"
#include "DistanceKernel.h"
#include "SLIC_Hyper.h"
#include <ctime>

using namespace std;
typedef Hyper::IANH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> ImageHyper;
typedef Hyper::NLIH<Hyper::ImageNeighborKernel::NeighborSmall, Hyper::PixelDistanceKernel::AbsDistance1D> NLIH;


void morphology(IplImage * image){
    //////////////////////// MORPHOLOGY ////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////

    ImageHyper hyper(image);
    NLIH hyperNl(image);
    //hyper.toFile("out2hyperSahara.txt");


/**/
    NLIH * dilateNl = hyperNl.clone();
    Hyper::HyperMorpho<NLIH>::hyperDilate(hyperNl, *dilateNl, Hyper::StructElement());

    ImageHyper * dilate1 = hyper.clone();
    Hyper::HyperMorpho<ImageHyper>::hyperDilate(hyper, *dilate1, Hyper::StructElement());

    ImageHyper * dilate2 = dilate1->clone();
    Hyper::HyperMorpho<ImageHyper>::hyperDilate(*dilate1, *dilate2, Hyper::StructElement());


    NLIH * erodeNl = hyperNl.clone();
    Hyper::HyperMorpho<NLIH>::hyperDilate(hyperNl, *erodeNl, Hyper::StructElement());

    ImageHyper * erode1 = hyper.clone();
    Hyper::HyperMorpho<ImageHyper>::hyperErode(hyper, *erode1 , Hyper::StructElement());

    ImageHyper * erode2 = erode1->clone();
    Hyper::HyperMorpho<ImageHyper>::hyperErode(*erode1, *erode2, Hyper::StructElement());


   IplImage * dilMer1 = cvCloneImage(hyper.getImage());
   cvAbsDiff(dilate1->getImage(), image, dilMer1);

   IplImage * cdilate = cvCloneImage(hyper.getImage());
    cvDilate(image, cdilate, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));

   IplImage * difDils = cvCloneImage(hyper.getImage());
    cvAbsDiff(dilate1->getImage(), dilateNl->getImage(), difDils);


   IplImage * difdifs = cvCloneImage(hyper.getImage());
    cvAbsDiff(erodeNl->getImage(), erode1->getImage(), difdifs);
//*/
    ImageHyper open1;
    open1.setImage(cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1));
    Hyper::HyperMorpho<ImageHyper >::hyperOpen(hyper, open1, Hyper::StructElement());

    ImageHyper close1;
    close1.setImage(cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1));
    Hyper::HyperMorpho<ImageHyper >::hyperClose(hyper, close1, Hyper::StructElement());

    std::vector<const IplImage *> imgs;
    imgs.push_back(image);
/**/
    imgs.push_back(dilate1->getImage());
    //imgs.push_back(dilate2->getImage());
    imgs.push_back(erode1->getImage());
    //imgs.push_back(erode2->getImage());
    //imgs.push_back(dilMer1);
    //imgs.push_back(cdilate);
    imgs.push_back(difDils);
    imgs.push_back(difdifs);
//*/
    //imgs.push_back(open1.getImage());
    //imgs.push_back(close1.getImage());

    std::vector<std::string> names;
    names.push_back("Original");
/**/
    names.push_back("Dilate_1");
    //names.push_back("Dilate_2");
    names.push_back("Erode_1");
    //names.push_back("Erode_2");
    //names.push_back("DilMer_1");
    //names.push_back("CDilate");
    names.push_back("DifDils");
    names.push_back("DifErs");
//*/
    //names.push_back("Open_1");
    //names.push_back("Close_1");
    Hyper::showImages(imgs, names);
    ///
    Hyper::saveImages(imgs, names, "morpho_barb_nlih/");
    //*/

}
// Returns a uniformly distributed random number
double uniform()
{
    return (rand()/(float)0x7fff);
}
double impulse(float amount)
{
    if(rand()>1-amount)
        return 255;
    if(uniform()<1-amount)
        return 0;

    return 0;
}
IplImage* generateNoise(IplImage* img, float amount=1)
{
    CvSize imgSize = cvGetSize(img);
    IplImage* imgTemp = cvCloneImage(img); // This will hold the noisy image
    // Go through each pixel
       for(int y=0;y<imgSize.height;y++)
       {
           for(int x=0;x<imgSize.width;x++)
           {
               int randomValue = (int)(impulse((float)(amount)));
               // Here we "apply" the noise to the current pixel

               int pixelValue = cvGetReal2D(imgTemp, y, x)+randomValue;

               // And set this value in our noisy image
               cvSetReal2D(imgTemp, y, x, pixelValue);
           }
       }

       // return
       return imgTemp;
}

void denoising(IplImage * image){
    //////////////////////// DENOISING  ////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
/****/
    cv::Mat img = cv::Mat(image);
    cv::Mat saltpepper_noise = cv::Mat::zeros(img.rows, img.cols,CV_8U);
    randu(saltpepper_noise,0,255);

    cv::Mat black = saltpepper_noise < 5;
    cv::Mat white = saltpepper_noise > 250;

    cv::Mat saltpepper_img_mat = img.clone();
    saltpepper_img_mat.setTo(255,white);
    saltpepper_img_mat.setTo(0,black);

    IplImage  saltpepper_img = (IplImage)saltpepper_img_mat;
    IplImage  original_img   = (IplImage)img;

    //cvShowImage("noise",&saltpepper_img);
    //cvWaitKey(0);
    //exit(0);

    ImageHyper hyper(&saltpepper_img);
    hyper.toFile("out2hyperSahara.txt");


    IplImage * imdenoise = cvCloneImage(&saltpepper_img);
    Hyper::HyperDenoise<ImageHyper >::hyperDenoise(hyper, imdenoise);
    double psnr = Hyper::HyperDenoise<ImageHyper >::compute_PSNR(&original_img, imdenoise);
    std::cout<<"[Denoise] PSNR = "<<psnr<<std::endl;

    ImageHyper open1;
    open1.setImage(cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1));
    Hyper::HyperMorpho<ImageHyper >::hyperOpen(hyper, open1, Hyper::StructElement());

    ImageHyper close1;
    close1.setImage(cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1));
    Hyper::HyperMorpho<ImageHyper >::hyperClose(hyper, close1, Hyper::StructElement());


    IplImage * cOpen = cvCloneImage(hyper.getImage());
    cvErode(hyper.getImage(), cOpen, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));
    cvDilate(cvCloneImage(cOpen), cOpen, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));


    IplImage * cClose = cvCloneImage(hyper.getImage());
    cvDilate(hyper.getImage(), cClose, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));
    cvErode(cvCloneImage(cClose), cClose, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));

    IplImage * difclos = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
    cvAbsDiff(close1.getImage(), cClose, difclos);

    IplImage * difopen = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
    cvAbsDiff(open1.getImage(), cOpen, difopen);

    IplImage * difopenNoisy = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
    cvAbsDiff(open1.getImage(), difopen, difopenNoisy);


    IplImage * cOpenO = cvCloneImage(hyper.getImage());
    cvErode(image, cOpenO, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));
    cvDilate(cvCloneImage(cOpenO), cOpenO, cvCreateStructuringElementEx(3, 3, 1,1, CV_SHAPE_RECT));

    IplImage * difopenOr = cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
    cvAbsDiff(cOpenO, difopenNoisy, difopenOr);

    std::vector<const IplImage *> imgs;
    imgs.push_back(image);
    imgs.push_back(hyper.getImage());
    imgs.push_back(cOpen);
    imgs.push_back(cClose);
    imgs.push_back(open1.getImage());
    imgs.push_back(close1.getImage());
    imgs.push_back(difclos);
    imgs.push_back(difopen);
    imgs.push_back(difopenNoisy);
    imgs.push_back(difopenOr);
    imgs.push_back(imdenoise);

    std::vector<std::string> names;
    names.push_back("Original");
    names.push_back("Noisy");
    names.push_back("cOpen");
    names.push_back("cClose");
    names.push_back("Open_1");
    names.push_back("Close_1");
    names.push_back("DifClose");
    names.push_back("DifOpen");
    names.push_back("DifopenNoisy");
    names.push_back("DifopenOr");
    names.push_back("Denoise");
    Hyper::showImages(imgs, names);

    for(int i = 0; i<imgs.size()-1; i++){
        std::string path = "denoising_2/";
        path.append(names.at(i));
        path.append(".jpg");
        cvSaveImage(path.data(), imgs.at(i));
    }
    // save with the PSNR value
    std::string path = "denoising_2/";
    path.append(names.at(imgs.size()-1));
    path.append("_");
    path.append(boost::lexical_cast<std::string>(psnr).data());
    path.append("_.jpg");
    cvSaveImage(path.data(), imgs.at(imgs.size()-1));
//*/
}

void segmentation(IplImage *image){
    //////////////////////// SEGMENTATION //////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    //image =IplImage *::ones(10,10,0);
    int kway = 30;
    ImageHyper hyper(image);
    std::cout<<"card(hyper) = "<<hyper.getSize()<<std::endl;
    hyper.toWeightedFile("weighed_ianh.hgr", image);
    //system("/home/tramx/Documents/hmetis-1.5-linux/shmetis ../../build_cpp_prat/weighted_ianh.hgr 40 5");
    /*****/
    std::ifstream part;
    part.open("weighed_ianh.hgr.part.30");
    std::vector<Hyper::HyperEdge> blocks(kway);
    std::string aux;
    if( !part.is_open()) {
        std::cout<<"No partition file found"<<std::endl;
        return;
    }
    for(long i = 0; i<hyper.getSize(); i++){
        part >> aux;
        blocks[atoi(aux.data())].add(i);
    }
    part.close();
   IplImage * segment =cvCreateImage(cvSize(image->width,image->height), IPL_DEPTH_8U, 1);
    std::vector<int> mean_blocks;
    for(int j = 0; j<kway; j++){
        mean_blocks.push_back(uint( Hyper::Utils::mean(hyper.neighborsValues( blocks[j])).val[0]));
    }
    std::cout<<"begin"<<std::endl;
    for(long i = 0; i < hyper.getSize(); i++){
        int kpart = -1;
        for(int j = 0; j<kway;j++){
            if(blocks[j].setContains(i)) {
                kpart = j;
                break;
            }
        }
        CvPoint pos = Hyper::Utils::numToCoord(i, image->width);
        //segment.at<uchar>(pos.x, pos.y) = uchar(uint(mean_blocks[kpart] ));
        cvSet2D(segment, pos.x, pos.y, cvScalar(uint(mean_blocks[kpart] )));

    }

    cvNamedWindow( "Original window", CV_WINDOW_NORMAL );
    cvShowImage("Original window", image );
    cvNamedWindow( "segmented window", CV_WINDOW_NORMAL );
    cvShowImage( "segmented window", segment );

    // std::cout<<"show"<<std::endl;
    cvWaitKey(0);
    cvDestroyWindow("Original window");
    cvDestroyWindow("segmented window");
    cvSaveImage("segment_irm_30.jpg", segment);
    //*/
}


void superpixel_morpho(char * _fname){
    Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> slic(1000, 80, _fname, 9);
    slic.compute_hyper();
    std::cout<<"[main] morpho on superpixel one   "<<slic.getImage()->alphaChannel<<std::endl;
    Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> * dilate1 = slic.clone();
    Hyper::HyperMorpho<Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> >::hyperDilate(slic, *dilate1, Hyper::StructElement());

    std::cout<<"[main] morpho on superpixel two"<<std::endl;
    //Hyper::SLIC_Hyper<Hyper::ImageNeighborKernel::NeighborSmall> * dilate2 = dilate1->clone();
    //Hyper::HyperMorpho<Hyper::SLIC_Hyper<Hyper::ImageNeighborKernel::NeighborSmall> >::hyperDilate(*dilate1, *dilate2, Hyper::StructElement());
    //dilate1->showSuperPixelNeighbors(10);


    ImageHyper hyper(slic.getImage());
    ImageHyper * dilatePixel = hyper.clone();
    Hyper::HyperMorpho<ImageHyper>::hyperDilate(hyper, *dilatePixel, Hyper::StructElement());

    IplImage * cdilate = cvCloneImage(hyper.getImage());
    cvDilate(slic.getImage(), cdilate, cvCreateStructuringElementEx(5, 5, 1,1, CV_SHAPE_RECT));

    IplImage* diff = cvCloneImage(slic.getImage());
    cvAbsDiff(slic.getImage(), dilate1->getImage(), diff);

    IplImage* diffDils = cvCloneImage(slic.getImage());
    cvAbsDiff(dilatePixel->getImage(), dilate1->getImage(), diffDils);

    IplImage* diffDils2 = cvCloneImage(slic.getImage());
    cvAbsDiff(cdilate, dilate1->getImage(), diffDils2);


    std::vector<const IplImage *> imgs;
    imgs.push_back(slic.getImage());
    imgs.push_back(slic.getContoursImage());
    imgs.push_back(dilate1->getImage());
    imgs.push_back(dilatePixel->getImage());
    imgs.push_back(cdilate);
    imgs.push_back(diff);
    imgs.push_back(diffDils);
    imgs.push_back(diffDils2);

    std::vector<std::string> names;
    names.push_back("Original");
    names.push_back("Contours");
    names.push_back("SLICDilate1");
    names.push_back("PixDilate1");
    names.push_back("ClassicDilate1");
    names.push_back("DiffSlicDilAndOriginal");
    names.push_back("DiffSlicDilAndPxDil");
    names.push_back("DiffSlicDilAndClassic");

    Hyper::showImages(imgs, names);
    Hyper::saveImages(imgs, names, "morpho_damier_slic/");

    free(dilate1);
    free(dilatePixel);
    //free(dilate2);
}

void superpixel(char* _fname){
    Hyper::NRIH<Hyper::ImageNeighborKernel::NeighborSmall> slic(1000, 80, _fname, 9);
    slic.compute_hyper();
    slic.toFile("superPixel_vet1.txt");
    slic.toWeightedFile("superPixelWeighted_sahara_vet1.hgr");
    slic.showSuperPixelNeighbors(100);
    /*/ superpixel HyperGraph partition based segmentation /*/
    /*******************************************************/
    /// Read partition result from file
    int kway = 3;
    std::ifstream part;
    part.open("superPixelWeighted_sahara_vet1.hgr.part.3");
    std::vector<Hyper::HyperEdge> blocks(kway);
    std::string aux;
    if( !part.is_open()) {
        std::cout<<"No partition file found"<<std::endl;
        return;
    }
    for(long i = 0; i<slic.getSize(); i++){
        part >> aux;
        blocks[atoi(aux.data())].add(i);
    }
    part.close();
    IplImage* segmentation = cvCreateImage(cvSize(slic.getImageSize().y, slic.getImageSize().x), IPL_DEPTH_8U, 1);
    //cv::Mat segmentation =IplImage *(slic.getImageSize().x, slic.getImageSize().y, CV_8U);
    std::cout<<"[main] init segmentation"<<std::endl;
    std::vector<int> mean_blocks;
    for(int j = 0; j<kway; j++){
        double mean = 0.0;
        for(int k = 0; k<blocks.at(j).size(); k++)
            mean+=slic.getSuperPixel(blocks.at(j).at(k)).getMeanValue().val[0];
        mean_blocks.push_back(uint( mean/blocks.at(j).size()));
    }
    std::cout<<"[main] mean blocks computed"<<std::endl;
    for(long i = 0; i < slic.getSize(); i++){
        int kpart = -1;
        for(int j = 0; j<kway;j++){
            if(blocks[j].setContains(i)) {
                kpart = j;
                break;
            }
        }
        const Hyper::SuperPixel & sp = slic.getSuperPixel(i);
        for (int j = 0; j<sp.size(); j++){
            CvPoint pos = Hyper::Utils::numToCoord(sp.at(j), slic.getImageSize().y);
            cvSet2D(segmentation, pos.x,pos.y, cvScalar(mean_blocks[kpart]));
            //segmentation.at<uchar>(pos.x, pos.y) = uchar(uint(mean_blocks[kpart] ));

        }

    }
    std::cout<<"[main] drawing"<<std::endl;
    //cv::imshow("segment", segmentation);
    cvShowImage("Segment super", segmentation);
    cvWaitKey(0);
    cvSaveImage("sahara_slic_3.jpg", segmentation);
    //*/

}

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: ./main ImageToLoadAndDisplay" << endl;
     return -1;
    }

   IplImage *  image = cvLoadImage(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

   if(! image->imageData)
    {
        cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
        return -1;
    }
    //morphology(image);
    /*// just doing some stats on IANH wrt to windowSize*
    time_t begin, end;
    time(&begin);
    ImageHyper hyper (image);
    std::ofstream outStd ("stdsSmall.txt", std::ios::out);
    printTo<std::vector<double>::iterator >(outStd, hyper.getEdgeStd().begin(), hyper.getEdgeStd().end());
    outStd.close();
    std::ofstream outCard ("cardsSmall.txt", std::ios::out);
    printTo<std::vector<double>::iterator >(outCard, hyper.getEdgeCard().begin(), hyper.getEdgeCard().end());
    outCard.close();
    time(&end);
    std::cout<<end-begin<<std::endl;
    exit(0);
    //*/
    //std::cout<<"before"<<std::endl;
   //denoising(image);
   //morphology(image);
   superpixel(argv[1]);
   //segmentation(image);
   //superpixel_morpho(argv[1]);
   //Hyper::Point2Di size(300, 300);
   // Hyper::HyperEdge ng = Hyper::ImageNeighbourKernel::NeighbourSmall::neighbour(450,size);
   // std::cout<<"ng : "<<ng.toString()<<std::endl;
    std::cout<<"Peace!!!"<<std::endl;
    return 0;
}

















//g++ -I/usr/include/opencv main.cpp -o main -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann
