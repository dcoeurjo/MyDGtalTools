#include <DGtal/helpers/StdDefs.h>
#include <DGtal/topology/LightImplicitDigitalSurface.h>

#include <DGtal/dec/DiscreteExteriorCalculus.h>
#include <DGtal/dec/DiscreteExteriorCalculusSolver.h>
#include <DGtal/math/linalg/EigenSupport.h>
#include "DGtal/io/readers/GenericReader.h"
#include "DGtal/images/IntervalForegroundPredicate.h"

#include <DGtal/io/viewers/Viewer3D.h>
#include <DGtal/io/boards/Board3D.h>
#include <QtGui/qapplication.h>
#include <DGtal/topology/SetOfSurfels.h>
#include <DGtal/topology/DigitalSurface.h>

//#include <Eigen/SPQRSupport>

using namespace DGtal;
using namespace Z3i;

template <typename TPoint3>
struct ImplicitDigitalEllipse3 {
  typedef TPoint3 Point;
  inline
  ImplicitDigitalEllipse3( double a, double b, double c )
  : myA( a ), myB( b ), myC( c )
  {}
  inline
  bool operator()( const TPoint3 & p ) const
  {
    double x = ( (double) p[ 0 ] / myA );
    double y = ( (double) p[ 1 ] / myB );
    double z = ( (double) p[ 2 ] / myC );
    return ( x*x + y*y + z*z ) <= 1.0;
  }
  double myA, myB, myC;
};



template <typename Shape>
int testShape(int argc, char **argv)
{
  typedef LightImplicitDigitalSurface<KSpace,Shape> Boundary;
  typedef typename Boundary::SurfelConstIterator ConstIterator;
  typedef typename Boundary::Tracker Tracker;
  typedef typename Boundary::Surfel Surfel;
  typedef DigitalSetSelector< Domain, BIG_DS+HIGH_BEL_DS >::Type DigitalSet;
  typedef SurfelAdjacency<KSpace::dimension> MySurfelAdjacency;


  QApplication application(argc, argv);
  
  typedef ImageSelector<Domain, unsigned char>::Type Image;

  

  Image image = GenericReader<Image>::import (argv[1] );
  typedef functors::IntervalForegroundPredicate<Image> ThresholdedImage;
  ThresholdedImage thresholdedImage( image, 0, 255 );

  trace.beginBlock( "Construct the Khalimsky space from the image domain." );
    Domain domain = image.domain();
    KSpace ks;
    bool space_ok = ks.init( domain.lowerBound(), domain.upperBound(), true );
    if (!space_ok)
      {
	trace.error() << "Error in the Khamisky space construction."<<std::endl;
	return 2;
      }
    trace.endBlock();
    //! [3dVolBoundaryViewer-KSpace]

   
    //! [3dVolBoundaryViewer-ExtractingSurface]
    trace.beginBlock( "Extracting boundary by scanning the space. " );
    typedef KSpace::SurfelSet SurfelSet;
    typedef SetOfSurfels< KSpace, SurfelSet > MySetOfSurfels;
    typedef DigitalSurface< MySetOfSurfels > MyDigitalSurface;
    MySurfelAdjacency surfAdj( true ); // interior in all directions.
    MySetOfSurfels theSetOfSurfels( ks, surfAdj );
    Surfaces<KSpace>::sMakeBoundary( theSetOfSurfels.surfelSet(),
				     ks, thresholdedImage,
				     domain.lowerBound(),
				     domain.upperBound() );
    MyDigitalSurface boundary( theSetOfSurfels );
    trace.info() << "Digital surface has " << boundary.size() << " surfels."
		 << std::endl;
    trace.endBlock();


    
  // Point p1( -10, -10, -10 );
  // Point p2( 10, 10, 10);
  // KSpace K;
  // K.init( p1, p2, true );
  
  // //Surface
  // //Shape shape( 30.0, 20.5, 15.4 );
  // Shape shape( 6.0, 4.5, 3.4 );
  // Surfel bel = Surfaces<KSpace>::findABel( K, shape, 10000 );
  // Boundary boundary( K, shape,
  //                   SurfelAdjacency<KSpace::dimension>( true ), bel );
  // unsigned int nbsurfels = 0;
  // for ( ConstIterator it = boundary.begin(), it_end = boundary.end();
  //      it != it_end; ++it )
  // {
  //   ++nbsurfels;
  // }
  // trace.info() << nbsurfels << " surfels found." << std::endl;





  //DEC
  typedef DiscreteExteriorCalculus<3, EigenLinearAlgebraBackend> DEC;
  DEC calculus;
  
  //Creating the structure
  for ( MyDigitalSurface::ConstIterator it = boundary.begin(), it_end = boundary.end();
       it != it_end; ++it )
  {
    KSpace::SCells oneNeig = ks.sLowerIncident(*it);
    calculus.insertSCell(*it);
    for(KSpace::SCells::ConstIterator itt = oneNeig.begin(), ittend = oneNeig.end(); itt != ittend; ++itt)
    {
      calculus.insertSCell(*itt);
      KSpace::SCells oneNeig2 = ks.sLowerIncident(*itt);
      for(KSpace::SCells::ConstIterator ittt = oneNeig2.begin(), itttend = oneNeig2.end(); ittt != itttend; ++ittt)
        calculus.insertSCell(*ittt);
    }
  }
  trace.info() << calculus<<std::endl;
  
  //Setting a dirac on a 0-cell ==> 0-form
  DEC::DualForm1 dirac(calculus);
  dirac.myContainer( 50 )   = 1;
  
  //Laplace operator
  DEC::DualDerivative1 dp1 = calculus.derivative<1,DUAL>();
  DEC::DualHodge2 hodg2 = calculus.dualHodge<2>();
  DEC::PrimalDerivative1 d1 = calculus.derivative<1, PRIMAL>();
  DEC::PrimalHodge2 phodg2 = calculus.primalHodge<2>();
  
  //Diffusion-like operator
  DEC::DualIdentity1 laplace=   calculus.identity<1, DUAL>() - phodg2*d1*hodg2*dp1 ; //calculus.primalLaplace() ;
  DEC::DualIdentity1 lap2 = laplace*laplace;
  DEC::DualIdentity1 lap4 = lap2*lap2;
  DEC::DualIdentity1 lap8 = lap4*lap4;
  DEC::DualIdentity1 lap16 = lap8*lap8;
  
  trace.info() << "laplace = " << laplace << endl;
  
  //Solver
  typedef EigenLinearAlgebraBackend::SolverSimplicialLLT LinearAlgebra;
  typedef DiscreteExteriorCalculusSolver<DEC, LinearAlgebra, 1, DUAL, 1, DUAL> Solver;
  Solver solver;
  solver.compute(lap16);
  DEC::DualForm1 result = solver.solve(dirac);
  
  trace.info() << "Solver done"<<std::endl;

  //Viewer
  Viewer3D<Space,KSpace> viewer(ks);
  viewer.show();
  Board3D<Space,KSpace> board(ks);
  
  Display3DFactory<Space,KSpace>::draw(viewer,result);
  Display3DFactory<Space,KSpace>::draw(board,result);
  board.saveOBJ("diffusion.obj");
  
  
/*  for ( ConstIterator it = boundary.begin(), it_end = boundary.end();
       it != it_end; ++it )
  {
    KSpace::SCells oneNeig = ks.sLowerIncident(*it);
    viewer<< *it;
    for(KSpace::SCells::ConstIterator itt = oneNeig.begin(), ittend = oneNeig.end(); itt != ittend; ++itt)
    {
      viewer << *itt;
      KSpace::SCells oneNeig2 = ks.sLowerIncident(*itt);
      for(KSpace::SCells::ConstIterator ittt = oneNeig2.begin(), itttend = oneNeig2.end(); ittt != itttend; ++ittt)
        viewer<< *ittt;
    }
  }*/
  
  viewer << Viewer3D<Space, KSpace>::updateDisplay;
  application.exec();
  
  return 0;
}

int main(int argc, char **argv)
{
 
  testShape<ImplicitDigitalEllipse3<Point> >(argc,argv);
  
  return 0;
}
