#include <iostream>
#include <set>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>

#include <DGtal/shapes/parametric/Ball3D.h>
#include <DGtal/shapes/GaussDigitizer.h>
#include <DGtal/topology/LightImplicitDigitalSurface.h>
#include <DGtal/topology/DigitalSurface.h>

using namespace DGtal;
using namespace Z3i;

int main()
{
  const double h = 1;
  const double radiusBall = 12.0;
  
  trace.beginBlock( "Make parametric shape..." );
  
  typedef Ball3D< Z3i::Space > Shape;
  
  RealPoint center( 0.0, 0.0, 0.0 );
  Shape ball( center, radiusBall );
  RealPoint centerBis(18.0,0.0,0.0);
  Shape ballBis( centerBis, radiusBall);
  trace.endBlock();
  
  trace.beginBlock( "Make digital shape..." );
  typedef GaussDigitizer< Z3i::Space, Shape > DigitalShape;
  typedef DigitalShape::Domain Domain;
  
  DigitalShape digitalBall;
  digitalBall.attach( ball );
  digitalBall.init( ball.getLowerBound() - Z3i::RealPoint( 1.0, 1.0, 1.0 ),
                   ball.getUpperBound() + Z3i::RealPoint( 1.0, 1.0, 1.0 ),
                   h );
  DigitalShape digitalBallBis;
  digitalBallBis.attach( ballBis );
  digitalBallBis.init( ballBis.getLowerBound() - Z3i::RealPoint( 1.0, 1.0, 1.0 ),
                      ballBis.getUpperBound() + Z3i::RealPoint( 1.0, 1.0, 1.0 ),
                      h );
  
  Domain domain( digitalBall.getDomain().lowerBound().inf( digitalBallBis.getDomain().lowerBound() ),
                 digitalBall.getDomain().upperBound().sup( digitalBallBis.getDomain().upperBound() ));
  
  Z3i::KSpace kspace;
  kspace.init( domain.lowerBound(), domain.upperBound(), true );
  trace.info()<< domain <<std::endl;
  trace.endBlock();
  
  trace.beginBlock( "Make first digital surface..." );
  typedef LightImplicitDigitalSurface< Z3i::KSpace, DigitalShape > LightDigitalSurface;
  typedef DigitalSurface< LightDigitalSurface > DigitalSurface;
  typedef Z3i::KSpace::Surfel Surfel;
  
  Surfel bel = Surfaces< Z3i::KSpace >::findABel( kspace, digitalBall, 500 );
  SurfelAdjacency< Z3i::KSpace::dimension > surfelAdjacency( true );
  LightDigitalSurface lightDigitalSurface( kspace, digitalBall, surfelAdjacency, bel );
  DigitalSurface digitalSurface( lightDigitalSurface );

  bel = Surfaces< Z3i::KSpace >::findABel( kspace, digitalBallBis, 500 );
  LightDigitalSurface lightDigitalSurfaceBis( kspace, digitalBallBis, surfelAdjacency, bel );
  DigitalSurface digitalSurfaceBis( lightDigitalSurfaceBis );

  
  trace.endBlock();

  std::set<Surfel> storage;
  trace.beginBlock("Exporting the first ball");
  std::ofstream handle("output.csv");
  for(auto cell : digitalSurface)
  {
    const Point p = kspace.sKCoords(cell);
    
    if (p[0] < -7.5) continue;
    
    const KSpace::Sign sign = kspace.sSign(cell);
    for (int dim=0; dim<3; dim++)
      handle << p[dim] << " ";
    handle << ( sign == KSpace::POS ) << " ";
    for (int dim=0; dim<3; ++dim)
      handle << (p-center)[dim]<<" ";
    handle << std::endl;
    //std::cout << "exporting "<< cell<<std::endl;
    storage.insert( cell );
  }
  trace.endBlock();
  
  
  trace.beginBlock("Exporting the second ball");
  for(auto cell : digitalSurfaceBis)
  {
  
    if (storage.find(cell) != storage.end())
      continue;
    
    const Point p = kspace.sKCoords(cell);
    const KSpace::Sign sign = kspace.sSign(cell);
    // "2.0*" to map centerbis to Kspace
    const RealPoint normal = (p-2.0*centerBis).getNormalized();
    for (int dim=0; dim<3; dim++)
      handle << p[dim] << " ";
    handle << ( sign == KSpace::POS ) << " ";
    for (int dim=0; dim<3; ++dim)
      handle << normal[dim] <<" ";
    handle << std::endl;
    //std::cout << "exporting "<< cell<<std::endl;
  }
  trace.endBlock();
  
  
  handle.close();
  
  return 0;
}
