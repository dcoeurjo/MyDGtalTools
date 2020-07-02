#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include "DGtal/geometry/curves/ArithmeticalDSS.h"
#include <random>
using namespace DGtal;
using namespace Z2i;

int main()
{
  Point a(0,0);
  Point b(16,16);

  Board2D board;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  
  std::vector<LibBoard::Point> line;
  double x=0,y=0,xprev=0,yprev=0;
  for(auto i= 0; i < 100; ++i)
  {
    if ((i%20)>17) continue;
    xprev =i;
    yprev = 0;
    for(auto j=0; j < 100 ; ++j)
    {
      x= i + (j/50.0 * (i/100.0))*distribution(generator);
      y= j+10;
      
      if ((j % 20) > 16)
      {
        board.drawPolyline(line);
        line.clear();
      }//board.drawLine(xprev, yprev, x, y);

      line.push_back({x,y});
      xprev = x;
      yprev = y;
    }
    board.drawPolyline(line);
    line.clear();
  }
  
  board.saveSVG("grid.svg");
  
  
}
