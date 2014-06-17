#include <iostream>
#include <vector>
#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/colormaps/HueShadeColorMap.h>

#include <DGtal/shapes/Shapes.h>
#include <DGtal/shapes/ShapeFactory.h>
#include <DGtal/shapes/GaussDigitizer.h>

//#include <DGtal/topology/helpers/Surfaces.h>
//#include <DGtal/geometry/curves/GridCurve.h>

using namespace DGtal;
using namespace Z2i;

template <typename P>
P Phi(const P & q)
{
  return q;
}

template <typename P>
P hash(const P & ap,  int M, int R, P& h1)
{

  P res = Point(ap[0] % M, ap[1] % M);

  h1 = Point(ap[0] % R, ap[1] %R);

  res += Phi(h1);
  res[0] %= M;
  res[1] %= M;
  
  //trace.info()<< ap <<" --> "<< res<<std::endl;
  return res;
}



int main(int argc, char **argv)
{
 
  //implicit digitization of a shape of type Flower 
  //into a digital space of type Space
  typedef Flower2D<Z2i::Space> Flower; 
  Flower2D<Z2i::Space> flower(Z2i::Point(0,0), 20, 5, 5, 0);
  double hh = 1; 
  GaussDigitizer<Z2i::Space,Flower> dig;  
  dig.attach( flower );
  dig.init( flower.getLowerBound()+Z2i::Vector(-1,-1),
            flower.getUpperBound()+Z2i::Vector(1,1), hh ); 
  Z2i::KSpace ks;
  ks.init( dig.getLowerBound(), dig.getUpperBound(), true );
  SurfelAdjacency<2> sAdj( true );
  Z2i::SCell bel = Surfaces<Z2i::KSpace>::findABel( ks, dig, 1000 );
  std::vector<Z2i::Point> boundaryPoints;
  Surfaces<Z2i::KSpace>
    ::track2DBoundaryPoints( boundaryPoints, ks, sAdj, dig, bel );
  
  Z2i::Curve c;
  c.initFromVector( boundaryPoints );  
  typedef Z2i::Curve::PointsRange Range; 
  Range r = c.getPointsRange(); 

  HueShadeColorMap<int,1> cmap( 0, r.size());
  

  Domain dom(Point(0,0), dig.getDomain().upperBound() - dig.getDomain().lowerBound());

  Point shiftN= dig.getDomain().lowerBound();

  unsigned int N = dom.upperBound()[0];
  unsigned int M = 17;
  unsigned int R = 11;

  Point shiftNM(N+5,0);
  Point One= Point::diagonal(1);
  
  Point shiftNR(N+5,M+5);

  Domain smalldom(Point(0,0)+shiftNM,Point(M,M)+shiftNM - One);
  
  Domain Rdom(Point(0,0)+shiftNR,Point(R,R)+shiftNR - One);
    
  
  Board2D board;
  Point h1;
  
  board << dom;
  board << smalldom;
  board << Rdom;
  int cpt = 0;
  Point prevh1, prevh, h;
   
  for(Curve::PointsRange::ConstIterator  it=r.begin(), itend=r.end(); it != itend; ++it)
    {
      board << CustomStyle( it->className(), 
                            new CustomPen( Color(180,180,180),cmap(cpt), 1.5, 
                                           Board2D::Shape::SolidStyle,
                                           Board2D::Shape::RoundCap,
                                           Board2D::Shape::RoundJoin ));
      board << *it - shiftN;
      h = hash<Point>( (*it) - shiftN ,M,R,h1) + shiftNM;
      h1 += shiftNR;
      board << h;
      board << h1 ;

      //  trace.info() << "H1: "<<prevh1<<"  -->  "<<h1<<std::endl;
     if (cpt != 0)
        {
          board.drawArrow((float)prevh[0], (float) prevh[1],
                          (float) h[0], (float) h[1],
                          true);

          board.drawArrow((float)prevh1[0], (float) prevh1[1],
                          (float) h1[0] , (float)h1[1],
                          true);
        }
      cpt++;
      prevh = h;
      prevh1 = h1 ;
    }

  board.saveEPS("perfect.eps");  
}
