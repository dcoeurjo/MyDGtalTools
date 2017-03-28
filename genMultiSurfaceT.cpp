#include <iostream>
#include <set>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>

#include <DGtal/shapes/parametric/Ball3D.h>
#include <DGtal/shapes/GaussDigitizer.h>
#include <DGtal/topology/LightImplicitDigitalSurface.h>
#include <DGtal/topology/DigitalSurface.h>


#include <DGtal/io/viewers/Viewer3D.h>

using namespace DGtal;
using namespace Z3i;

int main(int argc, char ** argv)
{
 
  QApplication app(argc,argv);
  
  Domain domain(Point::zero , Point::diagonal(40) );
  
  Z3i::KSpace kspace;
  kspace.init( domain.lowerBound(), domain.upperBound(), true );
  trace.info()<< domain <<std::endl;
  
  typedef Z3i::KSpace::Surfel Surfel;
  
  Viewer3D<Space,KSpace> viewer(kspace);
  viewer.show();
  
  trace.beginBlock( "Make first digital surface..." );

  std::set< std::pair<Surfel,RealPoint> > storage;

  for(auto i=0; i<5;++i)
    for(auto j=0; j<10;++j)
    {
      Surfel s  = kspace.sCell(Point(i*2+1,j*2+1,20));
      RealPoint n(i,j,20);
      storage.insert( std::make_pair(s ,n ));
      viewer << s;
    }
  
  for(auto i=5; i<10;++i)
    for(auto j=0; j<10;++j)
    {
      Surfel s  = kspace.sCell(Point(i*2+1,j*2+1,20));
      RealPoint n(i,j,20  );
      storage.insert( std::make_pair(s ,n ));
      viewer << s;
    }
  
  for(auto j=0; j<10;++j)
    for(auto k=5; k<10;++k)
    {
      Surfel s  = kspace.sCell(Point(10,j*2+1,k*2+1));
      RealPoint n(1,0,0);
      storage.insert( std::make_pair(s ,n ));
      viewer << s;
    }
  
  viewer << Viewer3D<>::updateDisplay;
  
  
  std::ofstream handle("output.csv");
  trace.beginBlock("Exporting");
  for(auto pa : storage)
  {
    Surfel cell = pa.first;
    RealPoint normal = pa.second;
    
    const Point p = kspace.sKCoords(cell);
    const KSpace::Sign sign = kspace.sSign(cell);
    
    //Normal
    //const RealPoint normal ;
 
    for (int dim=0; dim<3; dim++)
      handle << p[dim] << " ";
    handle << ( sign == KSpace::POS ) << " ";
    for (int dim=0; dim<3; ++dim)
      handle << normal[dim] <<" ";
    handle << std::endl;
  }
  trace.endBlock();
  
  
  handle.close();
  
  return app.exec();
  trace.endBlock();
}
