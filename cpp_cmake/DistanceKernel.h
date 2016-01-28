#ifndef DISTANCEKERNEL_H
#define DISTANCEKERNEL_H
#include <iostream>

// Note that all distance class must have a static method "distance"
// which takes at least 2 arguments

namespace Hyper {

namespace PixelDistanceKernel {
    enum DistanceType {BoolFashion = 0, StdFashion = 1};
    class AbsDistance1D {
    public:
      inline static int distance(int _a, int _b){
          return (_a - _b)>=0 ? (_a-_b):(-_a+_b); // abs
      }
    };

    class EuclidianDistance1D {
    public:
      inline static float distance(float _a, float _b){
          return std::pow(_a - _b, 2)*(1.0/sqrt(_a*_a + _b*_b));
      }
    };

    class SigmoidDistatnce1D {
    public:
      inline static float distance(float _a, float _b){
          return 0.0;//(1.0/(sqrt(2.0*3.14*_std*_std)))*exp(-pow(_a-_b, 2)/(2.0*pow(_std,2)));
      }
    };

}


}
#endif // DISTANCEKERNEL_H

/*  @Tramx   */
