#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include "HyperGraph.h"
#include "IANH.h"
#include "HyperDenoise.h"
#include "HyperMorpho.h"

using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: ./main ImageToLoadAndDisplay" << endl;
     return -1;
    }

    cv::Mat  image = cv::Mat( cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE));
/*
    image = cv::Mat::ones(100,100,0);
    for(int i =0; i<100; i++){
        for(int j = 0; j<100; j++){
        //    image.at<uchar>(i,j) = 100*rand()+j;
            std::cout<<image.at<uint>(i,j)<<"  vs  "<<image.at<uchar>(i, j)<<"    vs    "<<uint(image.at<uchar>(i,j))<<std::endl;
        }
    }*/
    if(! (image).data )
    {
        cout <<  "Could not open or find the image : "<<argv[1] << std::endl ;
        return -1;
    }

    Hyper::IANH hyper(image);
    std::cout<<"card(hyperedges) = "<<hyper.getHyper().size()<<std::endl;
    //hyper.toFile("out2hyperSahara.txt");

    //Hyper::HyperDenoise hypernoise;
    //cv::Mat imdenoise = cv::Mat::ones(image.rows, image.cols, CV_LOAD_IMAGE_GRAYSCALE);
    //image.copyTo(imdenoise);
    //hypernoise.hyperDenoise(hyper, image, imdenoise);
    Hyper::HyperMorpho morpho;
    cv::Mat dilate1 = cv::Mat::ones(image.rows, image.cols, CV_LOAD_IMAGE_GRAYSCALE);
    image.copyTo(dilate1);
    morpho.hyperDilate(hyper, image, dilate1);
    //imwrite( "../images/dilate1.jpg", dilate1 );
    cv::Mat erode = cv::Mat::ones(image.rows, image.cols, CV_LOAD_IMAGE_GRAYSCALE);
    //cv::dilate(image, dilate2, cv::InputArray(cv::Mat::ones(3,3,1)));
    //cv::absdiff(image, dilate1, dilate2);
    morpho.hyperErode(hyper, image, erode);
    //imwrite( "../images/dilate2.jpg", dilate2 );


    cv::namedWindow( "Noised window", cv::WINDOW_NORMAL );
    cv::imshow( "Noised window", image );

    // std::cout<<"named"<<std::endl;
    cv::namedWindow( "dilate1 window", cv::WINDOW_NORMAL);
    cv::imshow( "dilate1 window", dilate1);
    cv::namedWindow( "erode window", cv::WINDOW_NORMAL);
    cv::imshow( "erode window", erode);

    // std::cout<<"show"<<std::endl;
    cv::waitKey(0);
    cv::destroyWindow("Noised window");
    cv::destroyWindow("dilate1 window");
    cv::destroyWindow("erode window");

    std::cout<<"Peace!!!"<<std::endl;
    return 0;
}

















//g++ -I/usr/include/opencv main.cpp -o main -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann
