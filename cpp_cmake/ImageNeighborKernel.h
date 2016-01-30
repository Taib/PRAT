#ifndef IMAGENEIGHBORKERNEL_H
#define IMAGENEIGHBORKERNEL_H

#include "HyperGraph.h"

namespace Hyper {

namespace ImageNeighborKernel {
  /*
   * This namespace contains different image neighborhood kernel.
   * In this version, only few window size are provided.
   */
    enum WindowSizes{windowSizeS= 1, windowSizeM= 2, windowSizeL = 3, windowSizeXL = 4, windowSizeXXL = 5};
    // This class should not be use as kernel.
    // otherwise all parameters must be considered (has 2 more parameters than the normal kernel)
    class NeighborWindow {
    public:
        inline static void neighbors(long _num, CvPoint _size, int _windowSize, HyperEdge* _container){
            CvPoint pos = Utils::numToCoord(_num, _size.y);
            for(int i = -_windowSize; i<= _windowSize; i++){
                for(int j = -_windowSize; j<= _windowSize; j++){
                    int pi = (pos.x + i)>= 0?(pos.x + i)%_size.x:(_size.x + pos.x + i)%_size.x,
                        pj = (pos.y + j)>=0?(pos.y + j)%_size.y:(_size.y + pos.y + j)%_size.y;
                    _container->add(pi*_size.y +pj);
                }
            }
        }
    };
    
    // this class provides the smallest window centered on a given pixel. 3x3 window
    class NeighborSmall{
    public:
      /* method : neighbors provides the neighbors of a given pixel.
       * params : 
       *      _num       : the pixel position
       *      _size      : size of the image (in which the (x,w) coordinates of "_num" will be considered)
       *      _container : the result container, will contain the indices of the pixel at "_num" neighbors 
       *
       */
      inline static void neighbors(long _num, CvPoint _size, HyperEdge* _container){
	NeighborWindow::neighbors(_num, _size, windowSizeS, _container);
      }
    };

    // 5x5 window
    class NeighborMid: public NeighborWindow{
    public:
        inline static void neighbors(long _num, CvPoint _size, HyperEdge* _container){
            NeighborWindow::neighbors(_num, _size, windowSizeM,  _container);
        }
    };

    class NeighborLarge: public NeighborWindow{
    public:
        inline static void neighbors(long _num, CvPoint _size, HyperEdge* _container){
             NeighborWindow::neighbors(_num, _size, windowSizeL, _container);
        }
    };

    class NeighborExtraLarge: public NeighborWindow{
    public:
        inline static void neighbors(long _num, CvPoint _size, HyperEdge* _container){
            NeighborWindow::neighbors(_num, _size, windowSizeXL, _container);
        }
    };

    class NeighborExtraExtraLarge: public NeighborWindow{
    public:
        inline static void neighbors(long _num, CvPoint _size, HyperEdge* _container){
            NeighborWindow::neighbors(_num, _size, windowSizeXXL, _container);
        }
    };

}

}
#endif // IMAGENEIGHBORKERNEL_H

/*  @Tramx   */
