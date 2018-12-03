#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/helpers/ShortcutsGeometry.h>

#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/viewers/Viewer3D.h>

using namespace DGtal;
using namespace Z3i;

const double f = 0.037;
const double k = 0.06;
const double dt = .4  ;
const double ra = 1.0;
const double rb = 0.5;



template<typename Image>
void oneStep(const Domain &innerDomain,
             Image &A,
             Image &B)
{
  for(auto &p: A.domain())
  {
    double ap = ra * lap(A,p) - A(p)*B(p)*B(p)  + f*(1.0-A(p));
    double bp = rb * lap(B,p) + A(p)*B(p)*B(p)  - (f+k)*B(p);
  
    A.setValue(p, A(p) + dt*ap);
    B.setValue(p, B(p) + dt*bp);
  }
}

double clamp(const double a)
{
  if (a<0.0) return 0;
  if (a>0.7) return 1.0;
  return a;
}

template<typename Image, typename Im>
void exportMap(std::string &filename,
               const Image &A,
               const Image &B,
               Im &mix)
{

  for(auto &p: A.domain())
  {
    mix.setValue(p, Color(255-static_cast<unsigned char>(255*clamp(5*B(p))),
                          255-static_cast<unsigned char>(255*clamp(5*B(p))),
                          255-static_cast<unsigned char>(255*clamp(5*B(p)))));
  }
  trace.info() << "hop"<<std::endl;
  mix >> filename;
}

int main()
{
  
  typedef Shortcuts<Z3i::KSpace> SH3;
  typedef ShortcutsGeometry<Z3i::KSpace> SHG3;
    
  auto params = SH3::defaultParameters() | SHG3::defaultParameters();
  // Set your own parameters with operator().
  params( "polynomial", "goursat" )( "gridstep", 0.5 )("r-radius","3.0");
  auto implicit_shape  = SH3::makeImplicitShape3D( params );
  auto digitized_shape = SH3::makeDigitizedImplicitShape3D( implicit_shape, params );
  auto K               = SH3::getKSpace( params );
  auto surface         = SH3::makeDigitalSurface( digitized_shape, K, params );
  auto surfels         = SH3::getSurfelRange( surface, params );
  SH3::Cell2Index  cellIndex;
  auto pointels        = SH3::getPointelRange(cellIndex,surface);
  SH3::Surfel2Index surfelIndex;
  auto polySurf        = SH3::makeDualPolygonalSurface(surfelIndex,surface);

  
  
  
  return 0;
}
