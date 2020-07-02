#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/helpers/ShortcutsGeometry.h>

#include <DGtal/math/linalg/EigenSupport.h>

#include <DGtal/dec/DiscreteExteriorCalculus.h>
#include <DGtal/dec/DiscreteExteriorCalculusFactory.h>
#include <DGtal/dec/DiscreteExteriorCalculusSolver.h>

#include <DGtal/geometry/surfaces/estimation/LocalEstimatorFromSurfelFunctorAdapter.h>
#include <DGtal/geometry/surfaces/estimation/IIGeometricFunctors.h>
#include <DGtal/geometry/surfaces/estimation/IntegralInvariantCovarianceEstimator.h>

#include <DGtal/io/viewers/Viewer3D.h>

using namespace DGtal;
using namespace Z3i;

const double f = 0.037;
const double k = 0.06;
const double dt = .4  ;
const double ra = 1.0;
const double rb = 0.5;

template<typename Form, typename Lap>
void onestep( Form &A,  Form &B, const Lap &laplacian)
{
  auto lapA = laplacian * A;
  auto lapB = laplacian * B;
  for(auto i = 0 ; i < A.myContainer.size(); ++i)
  {
    double ap = ra * lapA.myContainer(i) - A.myContainer(i)*B.myContainer(i)*B.myContainer(i)  + f*(1.0-A.myContainer(i));
    double bp = rb * lapB.myContainer(i) + A.myContainer(i)*B.myContainer(i)*B.myContainer(i)  - (f+k)*B.myContainer(i);
    
    A.myContainer(i) += dt*ap;
    B.myContainer(i) += dt*bp;
  }
}

int main(int argc, char** argv)
{
  QApplication application(argc,argv);
  
  typedef Shortcuts<Z3i::KSpace> SH3;
  typedef ShortcutsGeometry<Z3i::KSpace> SHG3;
  
  auto params = SH3::defaultParameters() | SHG3::defaultParameters();
  // Set your own parameters with operator().
  params( "polynomial", "goursat" )( "gridstep", 0.5 )("r-radius","4.0");
  auto implicit_shape  = SH3::makeImplicitShape3D( params );
  auto digitized_shape = SH3::makeDigitizedImplicitShape3D( implicit_shape, params );
  auto K               = SH3::getKSpace( params );
  auto surface         = SH3::makeDigitalSurface( digitized_shape, K, params );
  auto surfels         = SH3::getSurfelRange( surface, params );
  SH3::Surfel2Index surfelIndex;
  auto polySurf        = SH3::makeDualPolygonalSurface(surfelIndex,surface);
  
  //Geom
  
  typedef DiscreteExteriorCalculus<2, 3, EigenLinearAlgebraBackend> Calculus;
  typedef DiscreteExteriorCalculusFactory<EigenLinearAlgebraBackend> CalculusFactory;
  const Calculus calculus = CalculusFactory::createFromNSCells<2>(surface->begin(), surface->end());
  
  Calculus::DualForm0 A(calculus);
  Calculus::DualForm0 B(calculus);

//  const double h= 0.1;
//  const double t = 0.5 * pow(h, 2. / 3.);
//  const double Kt=  4.;
//  const Calculus::DualIdentity0 laplace = calculus.heatLaplace<DUAL>(h, t, Kt);
  const Calculus::DualIdentity0 laplace = -calculus.laplace<DUAL>();

  trace.info()<< laplace<<std::endl;
  for(auto i = 0 ; i < A.myContainer.size(); ++i)
  {
    A.myContainer( i ) = 1.0;
    B.myContainer( i ) = 0.0;
  }
  B.myContainer( 0 ) = 1.0;
  B.myContainer( 1 ) = 1.0;
  B.myContainer( 2 ) = 1.0;
  //v = laplace*u

  trace.info()<< laplace.myContainer.coeff(10,10)<<std::endl;
  
  const auto MaxIter = 10;
  for(auto k = 0 ; k < MaxIter ; ++k)
  {
    trace.progressBar(k, MaxIter);
    onestep(A,B,laplace);
  }
  
  trace.info() << B.myContainer.minCoeff()<<" x "<< B.myContainer.maxCoeff() <<std::endl;
  Viewer3D<Space,KSpace> viewer(K);
  viewer.show();
  Display3DFactory<Space, KSpace>::draw(viewer, B);
  viewer << Viewer3D<>::updateDisplay;
  application.exec();
  
  
  return 0;
}
