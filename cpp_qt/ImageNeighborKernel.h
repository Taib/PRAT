#ifndef IMAGENEIGHBORKERNEL_H
#define IMAGENEIGHBORKERNEL_H

#include "HyperGraph.h"

namespace Hyper {

namespace ImageNeighborKernel {
    enum WindowSizes{windowSizeS= 1, windowSizeM= 2, windowSizeL = 3, windowSizeXL = 4, windowSizeXXL = 5};

    class NeighborWindow {
    public:
        inline static void neighbors(long _num, CvPoint _size, int _windowSize, HyperEdge* _container){
            CvPoint pos = Utils::numToCoord(_num, _size.y);
            for(int i = -_windowSize; i<= _windowSize; i++){
                for(int j = -_windowSize; j<= _windowSize; j++){
                    int pi = (pos.x + i)>= 0?(pos.x + i)%_size.x:(_size.x + pos.x + i)%_size.x,
                        pj = (pos.y + j)>=0?(pos.y + j)%_size.y:(_size.y + pos.y + j)%_size.y;
                    //if((pi < 0) || (pi >= _size.x) || (pj < 0) || (pj >= _size.y))
                    //    continue;
                    _container->add(pi*_size.y +pj);
                }
            }
        }
    };

    class NeighborSmall{
    public:
        inline static void neighbors(long _num, CvPoint _size, HyperEdge* _container){
            NeighborWindow::neighbors(_num, _size, windowSizeS, _container);
        }
    };

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
