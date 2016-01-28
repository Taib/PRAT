#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Set.h"
#include <string>
namespace Hyper {

#define Hyper_Kmeans_Iteration 10

inline CvScalar operator- (CvScalar _sc1, CvScalar _sc2){
    return cvScalar(_sc1.val[0] - _sc2.val[0],_sc1.val[1] - _sc2.val[1],_sc1.val[2] - _sc2.val[2],_sc1.val[3] - _sc2.val[3]);
}
inline CvScalar operator+ (CvScalar _sc1, CvScalar _sc2){
    return cvScalar(_sc1.val[0] + _sc2.val[0],_sc1.val[1] + _sc2.val[1],_sc1.val[2] + _sc2.val[2],_sc1.val[3] + _sc2.val[3]);
}
inline CvScalar operator* (CvScalar _sc1, CvScalar _sc2){
    return cvScalar(_sc1.val[0] * _sc2.val[0],_sc1.val[1] * _sc2.val[1],_sc1.val[2] * _sc2.val[2],_sc1.val[3] * _sc2.val[3]);
}
inline CvScalar operator/ (CvScalar _sc1, CvScalar _sc2){
    return cvScalar(_sc1.val[0] / _sc2.val[0],_sc1.val[1] / _sc2.val[1],_sc1.val[2] / _sc2.val[2],_sc1.val[3] / _sc2.val[3]);
}
inline bool operator== (CvScalar _sc1, CvScalar _sc2){
    return (_sc1.val[0] ==_sc2.val[0]) &&(_sc1.val[1] == _sc2.val[1] )&&(_sc1.val[2] == _sc2.val[2]) &&(_sc1.val[3] == _sc2.val[3]);
}
inline CvScalar operator/ (CvScalar _sc1, int _denom){
    return cvScalar(_sc1.val[0] /_denom,_sc1.val[1] /_denom,_sc1.val[2]/_denom,_sc1.val[3]/_denom);
}

inline std::ostream & operator<< (std::ostream & output, const CvPoint & _pt) {
    output <<"("<< _pt.x << ", " << _pt.y << ")";
    return output;
}

inline std::ostream & operator<< (std::ostream & output, const CvScalar & _sca) {
    for(int i = 0; i< 4; i++)
        output <<_sca.val[i]<<" ";
    return output;
}

template<class T>
inline std::ostream & operator<< (std::ostream & output, const std::vector<T> & _vec) {
    for(int i = 0; i< _vec.size(); i++)
        output <<_vec[i]<<" ";
    return output;
}

template<class T>
void printTo(std::ostream & _output, T _begin, T _end){
    for(;_begin != _end; _begin ++){
        _output << *_begin<<"\n";
    }
}

void showImages(std::vector<const IplImage *> images, std::vector<std::string> names){
    for(int i = 0; i < images.size(); i++){
        //cv::namedWindow( names[i].data(), cv::WINDOW_NORMAL );
        cvNamedWindow(names[i].data(), CV_WINDOW_FREERATIO);
        cvShowImage(names[i].data(), images.at(i) );
    }
    cv::waitKey(0);
    for(int i = 0; i < images.size(); i++){
        cv::destroyWindow(names.at(i).data());
    }
}
void saveImages(std::vector<const IplImage *> imgs, std::vector<std::string> names, char* directory){
    for(int i = 0; i<imgs.size(); i++){
        std::string path = directory;
        path.append(names.at(i));
        path.append(".jpg");
        cvSaveImage(path.data(), imgs.at(i));
    }
}
CvScalar pow(CvScalar _sca, int _pow){
    return cvScalar(std::pow(_sca.val[0], _pow), std::pow(_sca.val[1], _pow), std::pow(_sca.val[2], _pow), std::pow(_sca.val[3], _pow));
}

struct Cluster{
    std::vector<CvScalar > values;
    Set<int> indexes;
    CvScalar mean;
};

namespace Utils
{
    template<class T>
    inline static T abs(T _t){return _t<0?-_t:_t;}

    static double mean(std::vector<double> data){
        float sum=0.0;
        for(long i=0; i<data.size();++i)
            sum+=data[i];
        return (double)sum/data.size();
    }
    static double standardDeviation(std::vector<double> data){
        float mean=0.0, sum_deviation=0.0;
        mean = Utils::mean(data);
        for(long i=0; i<data.size();++i)
            sum_deviation+=(data[i]-mean)*(data[i]-mean);
        return sqrt(sum_deviation/data.size());
    }

    static CvScalar mean(std::vector<CvScalar> data){
        CvScalar sum = cvScalarAll(0.0);
        for(long i=0; i<(int)data.size();++i)
            sum = sum + data.at(i);
        return sum/data.size();
    }
    static CvScalar standardDeviation(std::vector<CvScalar> data){
        CvScalar mean=cvScalarAll(0.0), sum_deviation=cvScalarAll(0.0);
        mean = Utils::mean(data);
        for(long i=0; i<data.size();++i)
            sum_deviation=pow(data.at(i) - mean, 2);
        sum_deviation = sum_deviation/data.size();
        return cvScalar(sqrt(sum_deviation.val[0]), sqrt(sum_deviation.val[1]), sqrt(sum_deviation.val[2]), sqrt(sum_deviation.val[3]));
    }

    static CvScalar mean(std::vector<std::pair<CvScalar, int> > data){
        CvScalar sum = cvScalarAll(0.0);
        for(long i=0; i<(int)data.size();++i)
            sum = sum + data.at(i).first;
        return sum/data.size();
    }
    static CvScalar standardDeviation(std::vector<std::pair<CvScalar, int> > data){
        CvScalar mean         = Utils::mean(data),
                sum_deviation = cvScalarAll(0.0);
        for(long i=0; i< (int)data.size();++i)
            sum_deviation = pow(data.at(i).first - mean, 2);
        sum_deviation = sum_deviation/data.size();
        return cvScalar(sqrt(sum_deviation.val[0]), sqrt(sum_deviation.val[1]), sqrt(sum_deviation.val[2]), sqrt(sum_deviation.val[3]));
    }

    static void sort(std::vector<double>& _values){
        for(long i = 0; i<_values.size(); i++){
            double m = i;
            for(long j = i; j<_values.size(); j++){
                if(_values.at(m) >= _values.at(j))
                    m = j;
            }
            double aux = _values[i];
            _values[i] = _values[m];
            _values[m] = aux;
        }
    }


    static void sort(std::vector<CvScalar>& _values, int _dim = 0){
        for(long i = 0; i<_values.size(); i++){
            double m = i;
            for(long j = i; j<_values.size(); j++){
                if(_values.at(m).val[_dim] >= _values.at(j).val[_dim])
                    m = j;
            }
            CvScalar aux = _values[i];
            _values[i] = _values[m];
            _values[m] = aux;
        }
    }

    void sortId(std::vector<std::pair<double, int> > & _values){
        for(long i = 0; i<_values.size(); i++){
            double m = i;
            for(long j = i; j<_values.size(); j++){
                if(_values.at(m).first >= _values.at(j).first)
                    m = j;
            }
            std::pair<double, int>  aux =  _values.at(i);
            _values[i]                  = _values.at(m);
            _values[m]                  = aux;
        }
    }

    static CvPoint numToCoord(long _num, long _nb_columns){
        return cvPoint(_num/_nb_columns, _num%_nb_columns);
    }

    inline CvScalar min(const std::vector<CvScalar> & _vect, int _dimension = 0){
        CvScalar res = _vect.at(0);
        for(int i = 1; i< _vect.size(); i++){
            if(res.val[_dimension] > _vect.at(i).val[_dimension])
                res = _vect.at(i);
        }
        return res;
    }
    inline CvScalar max(const std::vector<CvScalar> & _vect, int _dimension = 0){
        CvScalar res = _vect.at(0);
        for(int i = 1; i< _vect.size(); i++){
            if(res.val[_dimension] < _vect.at(i).val[_dimension])
                res = _vect.at(i);
        }
        return res;
    }

    std::vector<std::pair<double, int> > knn(const std::vector<CvPoint>& _pts, CvPoint _pt, int _knn = 9){
        std::vector<std::pair<double, int> > res;
        std::vector<std::pair<double, int> > dist;
        for(int i = 0; i< _pts.size(); i++){
            dist.push_back(std::pair<double, int>((double)sqrt(std::pow(_pts.at(i).x-_pt.x, 2) + std::pow(_pts.at(i).y-_pt.y, 2)), i));
        }
        sortId(dist);
        for(int i = 0; i< _knn; i++){
            res.push_back(dist.at(i));
        }
        return res;
    }


    CvScalar dot(std::vector<CvScalar> _values1, std::vector<CvScalar> _values2){
        CvScalar res = cvScalarAll(0.0);
        for (int var = 0; var < (int)_values1.size(); ++var) {
            for(int d = 0; d<4; d++){
                res.val[d] += _values1.at(var).val[d]*_values2.at(var).val[d];
            }
        }
        return res;
    }

    std::vector<int> adaptativeTresholding(std::vector<std::pair<CvScalar, int> > _vect, int _dimension = 0){
        CvScalar mean = Utils::mean(_vect),
                stds  = Utils::standardDeviation(_vect);
        std::vector<int> res;
        for(int i = 0; i< _vect.size(); i++){
            if(std::abs(_vect.at(i).first.val[_dimension] - mean.val[_dimension]) <= stds.val[_dimension])
                res.push_back(_vect.at(i).second);
        }
        return res;
    }

    // Deprecated
    std::vector<Cluster> kMeans(std::vector<std::pair<CvScalar, int> > _data, int _nb_cluster = 2, int _dim = 0){
        std::vector<Cluster> result = std::vector<Cluster> (_nb_cluster);
        for(int k = 0; k< _nb_cluster; k++)
            for(int i = (int)k*_data.size()/_nb_cluster;( i< (int)k*_data.size()/_nb_cluster + _data.size()/_nb_cluster) && (i<_data.size()); i++){
                result.at(k).values.push_back(_data.at(i).first);
                result.at(k).indexes.add(_data.at(i).second);
            }
        for(int i = 0; i< Hyper_Kmeans_Iteration; i++){
            for(int k = 0; k< _nb_cluster; k++){
                result[k].mean = Utils::mean(result[k].values);
                result[k].values.clear();
                result[k].indexes.clear();
            }
            for(int j = 0; j<_data.size(); j++){
                int minId = 0;
                for(int k = 1; k< _nb_cluster; k++){
                    if( std::abs(result.at(minId).mean.val[_dim] - _data.at(j).first.val[_dim]) > std::abs(result.at(k).mean.val[_dim] - _data.at(j).first.val[_dim])  )
                        minId = k;
                }
                result.at(minId).values.push_back(_data.at(j).first);
                result.at(minId).indexes.add(_data.at(j).second);
            }
        }
    }

}
}

#endif // UTILS_H

/*  @Tramx   */


