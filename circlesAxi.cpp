#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <random>

using namespace DGtal;
using namespace Z2i;

int main()
{
  
  Board2D board;
  board.setUnit ( LibBoard::Board::UCentimeter );

  auto nbCircles=50;
  auto dec = 0.5;
  auto rad = 1.0;
  
  board.setPenColorRGBf(1.0, 0, 0);
  auto cx=0.0, cy=10.5;
  double theta,phi;
  for(auto i = 0; i < nbCircles; ++i)
  {
    double dinf = rad*rad - cy*cy;
    if (dinf<0.0)
    {
      theta=-M_PI/2.0;
      phi = M_PI/2.0;
    }
    else
    {
      theta=3*M_PI/2.0+ (acos(10.5/rad));
      phi=M_PI/2.0-(acos(10.5/rad));
    }
    
    board.drawArc(cx, cy, rad,theta, phi,false);
    rad += dec;
  }
  
  board.setPenColorRGBf(0.0, 0, 1.0);
  cx=29.7, cy=10.5;
  rad=1.0;
  for(auto i = 0; i < nbCircles; ++i)
  {
    double dinf = rad*rad - cy*cy;
    if (dinf<0.0)
    {
      theta=M_PI/2.0;
      phi = 3*M_PI/2.0;
    }
    else
    {
      theta=M_PI/2.0+ (acos(10.5/rad));
      phi=3*M_PI/2.0-(acos(10.5/rad));
    }
    
    board.drawArc(cx, cy, rad,theta, phi,false);
    rad += dec;
  }
  
 // board.setClippingRectangle(0, 0, 29.7, 21.0);
  board.saveSVG("circlesAxi.svg");
  
  return 0;
}
