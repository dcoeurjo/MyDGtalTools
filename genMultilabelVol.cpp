#include <DGtal/base/Common.h>
#include <DGtal/shapes/GaussDigitizer.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/shapes/ShapeFactory.h>
#include <DGtal/shapes/Shapes.h>

using namespace DGtal;
using namespace Z3i;


int main()
{
  ImplicitBall<Space> ball(RealPoint(0,0,0), 20);
  Domain domain(Point::diagonal(0), Point::diagonal(128));
  
  GaussDigitizer<Space, ImplicitBall<Space> > gauss;
  gauss.init( ball.getLowerBound(), ball.getUpperBound(), 1.0);
  gauss.attach( ball );
  
  DigitalSet aSet( gauss.getDomain() );
  Shapes<Domain>::digitalShaper( aSet, gauss );
  
  ImageContainerBySTLMap<Domain, unsigned char> image(domain);
  for(auto p: aSet)
    image.setValue(p+ Point(32,64,64), 64);
  
  for(auto p: aSet)
    image.setValue(p+Point(64,64,64), 128);
  
  for(auto p: aSet)
    image.setValue(p+ Point(100,100,100), 32);
  
  image >> "output-labels.vol";
  return 0;
}
