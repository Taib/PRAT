#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Set.h"
#include <string>
namespace Hyper {
class Point2Di{
public:
    inline Point2Di(){}
    inline Point2Di(int _x, int _y){x = _x; y = _y;}
    int x;
    int y;
    inline bool operator == (Point2Di _pt){return (x == _pt.x && y == _pt.y);}
    inline bool operator != (Point2Di _pt){return (x != _pt.x || y != _pt.y);}
    /*
    inline std::ostream & operator<< (std::ostream & output, const Point2Di & _pt) {
        output <<"("<< _pt.x << ", " << _pt.y << ")";
        return output;
    }
    */
};

typedef Set<long> HyperEdge;

namespace Utils
{
    static double mean(std::vector<double> data);
    static double standardDeviation(std::vector<double> data);
    static std::vector<double> neighborsValues(const cv::Mat & _img, const HyperEdge & ng);
    void neighborsValues(const cv::Mat & _img, const HyperEdge & ng, std::vector<double> * _output);
    static HyperEdge neighbors8(Point2Di _xy, Point2Di _size);
    static HyperEdge neighbors8(long _num, Point2Di _size);
    static int distance(int _a, int _b);
    static std::vector<double> * sort(std::vector<double> _values);
    static Point2Di numToCoord(long _num, long _nb_columns);
    template <class T>
    inline T max(std::vector<T> & _vect){
           T mx = *(_vect.begin());
           for(typename std::vector<T>::iterator it = _vect.begin(); it != _vect.end(); it++){
               if(*it > mx)
                   mx = *it;
           }
           return mx;
    }
    template <class T>
    inline T min(std::vector<T> & _vect){
           T mn = *(_vect.begin());
           for(typename std::vector<T>::iterator it = _vect.begin(); it != _vect.end(); it++){
               if(*it < mn)
                   mn = *it;
           }
           return mn;
    }

}

/////////////////////////Implementation ///////////////////////////


std::vector<double> * Utils::sort(std::vector<double> _values){
    std::vector<double>* res = new std::vector<double>(_values.size());
    for(long i = 0; i<_values.size(); i++){
        double m = i;
        for(long j = i; j<_values.size(); j++){
            if(_values[m] >= _values[j])
                m = j;
        }
        res->push_back(_values[m]);
    }
    return res;
}

HyperEdge Utils::neighbors8(long _num, Point2Di _size){
    Point2Di pos = Utils::numToCoord(_num, _size.y);
    int i1 = ((int)pos.x -1) % _size.x ;
    int i2 = ((int)pos.x +1) % _size.x;
    int j1 = ((int)pos.y -1) % _size.y;
    int j2 = ((int)pos.y +1) % _size.y ;

    i1     = i1 >= 0 ? i1: i1 + _size.x;
    i2     = i2 >= 0 ? i2: i2 + _size.x;
    j1     = j1 >= 0 ? j1: j1 + _size.y;
    j2     = j2 >= 0 ? j2: j2 + _size.y;

    long r  = pos.x*_size.y + j2;
    long l  = pos.x*_size.y + j1;
    long t  = (i1)*_size.y + pos.y;
    long tl = (i1)*_size.y+ j1;
    long tr = (i1)*_size.y+ j2;
    long b  = (i2)*_size.y+ pos.y;
    long bl = (i2)*_size.y+ j1;
    long br = (i2)*_size.y + j2;
    HyperEdge ng;
    ng.resize(9);
    ng.change(0, _num);
    ng.change(1, l);
    ng.change(2, r);
    ng.change(3, t);
    ng.change(4, b);
    ng.change(5, tl);
    ng.change(6, tr);
    ng.change(7, bl);
    ng.change(8, br);
    return ng;
}

int Utils::distance( int _a,  int _b){
    return (_a - _b)>=0 ? (_a-_b):(-_a+_b); // abs
}

static double Utils::mean(std::vector<double> data){
    float sum=0.0;
    for(long i=0; i<data.size();++i)
        sum+=data[i];
    return (double)sum/data.size();
}

double Utils::standardDeviation(std::vector<double> data)
{
    float mean=0.0, sum_deviation=0.0;
    mean = Utils::mean(data);
    for(long i=0; i<data.size();++i)
        sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/data.size());
}

std::vector<double> Utils::neighborsValues(const cv::Mat & _img, const HyperEdge & ng){
    std::vector<double> values (ng.size());
    //unsigned char * input = (unsigned char*)(_img.data);
    for(long i = 0; i< values.size(); i++){
        Point2Di p = Utils::numToCoord(ng.at(i), _img.cols);
        values[i] = uint(_img.at<uchar>(p.x, p.y));//_img->at<unsigned long>(p.x, p.y);//atoi((char32_t)
    }
    return values;
}

void Utils::neighborsValues(const cv::Mat & _img, const HyperEdge & ng, std::vector<double>* _output){
    for(long i = 0; i< ng.size(); i++){
        Point2Di p = Utils::numToCoord(ng.at(i), _img.cols);
        _output->push_back(uint(_img.at<uchar>(p.x, p.y)));//(int)input[(int)(_img.step * p.x + p.y)];//atoi((char32_t)
    }
}


Point2Di Utils::numToCoord(long _num, long _nb_columns){
        return Point2Di(_num/_nb_columns, _num%_nb_columns);
}

}

#endif // UTILS_H
