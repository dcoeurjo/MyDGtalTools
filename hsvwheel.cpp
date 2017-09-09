#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include <cmath>

#include <DGtal/io/colormaps/HueShadeColorMap.h>

using namespace DGtal;
using namespace Z2i;



void HSVtoRGB( double & r, double & g, double & b,
              const double h, const double s, const double v)
{
  int i;
  double f, p, q, t;
  if( s == 0 ) {                     // achromatic (gray)
    r = g = b = v;
    return;
  }
  i = static_cast<int>( floor( h / 60.0) );
  f = ( h / 60.0 ) - i;                        // factorial part of h
  p = v * ( 1.0 - s );
  q = v * ( 1.0 - s * f );
  t = v * ( 1.0 - s * ( 1.0 - f ) );
  switch( i ) {
    case 0:
      r = v; g = t; b = p;
      break;
    case 1:
      r = q; g = v; b = p;
      break;
    case 2:
      r = p; g = v; b = t;
      break;
    case 3:
      r = p; g = q; b = v;
      break;
    case 4:
      r = t;  g = p; b = v;
      break;
    default:    // case 5:
      r = v; g = p; b = q;
      break;
  }
}


template<typename Board>
void addHexa(const int i, const int j,  Board &board)
{
  std::vector<LibBoard::Point> poly;
  double r= 0.55;// 0.5*cos(M_PI/6.0);
  double di = 0.5*j + i;
  double dj = j*sqrt(3.0)/2.0;
  for(int ii = 0; ii < 6; ++ii)
  {
    double angle = 60*ii+30;
    poly.push_back( LibBoard::Point(di+r*cos( angle * M_PI/180.0),  dj+r*sin(angle * M_PI/180.0)));
  }
  double rr,gg,bb;
  double h = 360.0/(2.0*M_PI)*(M_PI + atan2(dj+0.001,di+0.001));
  double s= std::sqrt((di*di+dj*dj))/ 10.0;
  // s = 0.8;
 // if (s<=1.0)
  if (( ( abs(i)
         + abs( i + j)
         + abs(j)) / 2.0)< 10.0)
  {
    HSVtoRGB(	rr, gg, bb, h, s, 1.0);
    Color col(static_cast<int>( rr * 255),
              static_cast<int>( gg * 255),
              static_cast<int>( bb * 255));
    board.setPenColor( col );
    board.fillPolyline(poly);
  }
  
}


int main()
{
  Domain dom(Point(-100,-100), Point(100,100));
  
  const double eps=0.001;
  Board2D board;
  
  double r,g,b;
  for(auto i=-40; i<51; ++i)
    for(auto j=-40; j < 51; ++j)
    {
     
      addHexa(i,j,board);
      
    }
  
  
  /*for(auto p : dom )
  {
    double r,g,b;
    double h = 360.0/(2.0*M_PI)*(M_PI + atan2(p[1]+eps,p[0]+eps));
    double s= std::sqrt((p[0]*p[0]+p[1]*p[1]))/ 50.0;
   // s = 0.8;
    if (s>1.0) continue;
    HSVtoRGB(	r, g, b, h, s, 1.0);
    Color col(static_cast<int>( r * 255),
              static_cast<int>( g * 255),
              static_cast<int>( b * 255));
    board.setPenColor(col);
    board.setFillColor(col);
    board << CustomStyle( p.className(), new CustomColors( col, col ) ) << p;
    
  }*/
  
  board.saveCairo("hsvwheel.pdf", Board2D::CairoPDF);
  return 0;
  
}
