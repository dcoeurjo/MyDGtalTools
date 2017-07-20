#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>

//Export
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/io/writers/VolWriter.h>

//Voronoi
#include <DGtal/geometry/volumes/distance/VoronoiMap.h>

using namespace DGtal;
using namespace Z3i;

int main()
{
  const unsigned int S = 256;
  Domain domain(Point(0,0,0), Point(S,S,S));
  
  DigitalSet set(domain);
  
  //Sites
  for(auto p:domain)
    set.insertNew(p);
  for(auto i= 0; i < S; ++i)
    set.erase( Point( rand() % S, rand() % S, rand() %S));
                 
    
  VoronoiMap<Space, DigitalSet, L2Metric> voronoi(domain, set, l2Metric);
  
  ImageContainerBySTLVector<Domain, unsigned char> image(domain);
  
  for(auto p :voronoi.domain())
  {
    image.setValue( p , ((int)voronoi(p)[0]*123 + (int)voronoi(p)[1]*11 + (int)voronoi(p)[2]*7)  % 255);
  }
  trace.info() << image<<std::endl;
  VolWriter<ImageContainerBySTLVector<Domain, unsigned char> >::exportVol( "voronoi.vol", image);
  
  return 0;
}
