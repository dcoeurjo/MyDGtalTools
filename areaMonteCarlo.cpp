#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/colormaps/HueShadeColorMap.h>

#include <DGtal/shapes/Shapes.h>
#include <DGtal/shapes/ShapeFactory.h>
#include <DGtal/shapes/GaussDigitizer.h>

#include <boost/random/uniform_real_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/ranlux.hpp>

using namespace DGtal;
using namespace Z2i;


 boost::random::uniform_real_distribution<double> Unif[2];

template <typename Shape, typename RealPoint>
double areaMonteCarlo(const Shape & aShape, 
                      const RealPoint &lower, 
                      const RealPoint &upper,
                      unsigned int N)
{
  double sum = 0;
  unsigned int cpt = 0;
  RealPoint RP;
  RealPoint p;
  
 
  boost::random::ranlux64_4  seed;
  Board2D board;

  board << SetMode( p.className(), "Paving" );
  
  while (cpt<N)
    {
      
      for(DGtal::Dimension d=0; d < Domain::dimension ; d++)
        p[d] = Unif[d](seed);
      
#ifdef VERBOSE
      board << CustomStyle( p.className(), new CustomColors( Color(0,0,0),
                                                             Color(0,0,0)));
      board << p;
      if (( cpt % 100) == 0)
        trace.progressBar(cpt,N);
#endif

      
      if ( aShape.orientation(p) == DGtal::INSIDE)
        {
#ifdef VERBOSE
          board << CustomStyle( p.className(), new CustomColors( Color(255,0,0),
                                                                 Color(255,0,0)))
                << p;
                                
#endif
          sum ++;
        }

      cpt++;
    }
#ifdef VERBOSE
  board.saveEPS("montecarlo.eps");
  trace.info()<<std::endl;
  trace.info() << "===> "<<cpt<<" probes and "<<sum<< " successes."<<std::endl;
#endif
  return sum;
}

template <typename Shape, typename RealPoint>
double areaGauss(const Shape & aShape, 
                 const RealPoint &lower, 
                 const RealPoint &upper,
                 double h)
{
  double sum = 0;
  unsigned int cpt = 0;
  RealPoint RP;
  RealPoint p;
  
  Domain dom(Point(lower[0]/h , lower[1]/h),
             Point(upper[0]/h , upper[1]/h));
  
#ifdef VERBOSE
   Board2D board;
   board << SetMode( p.className(), "Paving" );
   trace.info() << dom<<std::endl;
#endif
  
  for(Domain::ConstIterator it = dom.begin(),itend = dom.end();
      it != itend; ++it)
    {
      if ( aShape.orientation( RealPoint((*it)[0]*h, (*it)[1]*h)) == DGtal::INSIDE)
        {
#ifdef VERBOSE
          board << CustomStyle( p.className(), new CustomColors( Color(255,0,0),
                                                                 Color(255,0,0)));
          board << RealPoint((*it)[0]*h, (*it)[1]*h);
#endif
          sum ++;
        }
      
      cpt++;
    }
#ifdef VERBOSE
  board.saveEPS("gauss.eps");
  trace.info() << "===> "<<cpt<<" probes and "<<sum<< " successes."<<std::endl;
#endif
  return sum;
}


int main(int argc, char **argv)
{
 
  //implicit digitization of a shape of type Flower 
  //into a digital space of type Space
  typedef Flower2D<Z2i::Space> Flower; 
  Flower2D<Z2i::Space> flower(Z2i::Point(0,0), 20, 5, 5, 0);
 
  Flower2D<Z2i::Space>::RealPoint2D pareadomain = flower.getUpperBound()- flower.getLowerBound();
  double areaDomain = pareadomain[0]*pareadomain[1];

  //New sample
  for(DGtal::Dimension d=0; d < 2 ; d++)
    Unif[d] = boost::random::uniform_real_distribution<double>(flower.getLowerBound()[d],
                                                               flower.getUpperBound()[d] );
  

  
  double h = 1;
  double N, areaMC, areaG, timeMC, timeG;
  while (h> 0.001)
    {
      N = areaDomain/h/h;
      
      trace.info() << "===== h="<<h<<" N= "<<N<<std::endl;

      trace.beginBlock("MonteCarlo");
      areaMC =  areaMonteCarlo(flower, flower.getLowerBound(),
                               flower.getUpperBound(), 
                                      N);
      trace.info() << "areaMC = "<< areaMC/N * areaDomain << std::endl;
      timeMC = trace.endBlock();
      
      trace.beginBlock("Gauss");
      areaG =  areaGauss(flower, flower.getLowerBound(),
                                flower.getUpperBound(), 
                                h);
      trace.info() << "areaG = "<< areaG*h*h << std::endl;
      timeG = trace.endBlock();

      std::cout<< areaMC/N * areaDomain  << " " << areaG*h*h << " "<< timeMC<<" "<<timeG<< " "<<N<<std::endl;
       h/=1.5;
      
    
    }

  return 0;
}
