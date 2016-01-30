Image HyperGraph ToolBox by T.BIRGUI (@Tramx)

This toolbox provides basic image processing methods applied on an image hypergraph representation.
The following methods are provided : 
   - IANH (Image Adaptative Neighborhood Hypergraph) based on the paper "Hypergraph-based image representation" by A.Bretto et al.
   - NLIH (Non-Local fashion Image Hypergraph) that uses non-local technics in the hyperedge building procesure. No known paper for this representation
   - NRIH (Naive Region based Image Hypergraph) that is a region based image hypergraph representation. No known paper for this representation.
   - Noise cancellation (S&P noise only) based on the paper "Application of Adaptive Hypergraph Model to Impulsive Noise Detection" by S.Rital et al.
   - Morphological operations on hypergraph based on the paper "Mathematical morphology on hypergraphs, application to similarity and positive kernel" by I.Bloch et al.
   - Image hypergraph segmentation based on the paper "Weighted adaptive neighborhood hypergraph partitioning for image segmentation" by S.Rital et al. Their method uses the hypergraph partitioning tool "hMetis" provided by G.Karypis et al.


   * Note: The index of a pixel is computed given its (x,y) coordinates and the width of the image.
   *       The index of the pixel at position (a,b) in the image of size NxM is an integer  : Index(a,b)=axM + b.


The basic image operation are handled thanks to OpenCv librarie. So make sure you have OpenCv installed.

.cpp files provide basic examples of how to use this toolbox.

By the way, the current version needs a lot of optimisation. Sorry about that ;)


// @Tramx
