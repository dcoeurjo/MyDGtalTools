#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include "DGtal/geometry/curves/ArithmeticalDSS.h"
#include <random>
using namespace DGtal;
using namespace Z2i;

int main()
{
  
  Board2D boardRed;
  Board2D boardBlue;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(-1.0,1.0);
  
 std::bernoulli_distribution dist(0.4);
 
  
  std::vector<LibBoard::Point> line;
    
  double theta = M_PI/6.0;
  
  boardRed.setUnit ( LibBoard::Board::UCentimeter );
  boardBlue.setUnit ( LibBoard::Board::UCentimeter );
  double width = 29.7;
  double height = 21;
  double dd=.8;
  double dx = cos(theta);
  double dy = sin(theta) ;
  double lambda;
  double x=0,y=0;
  for(auto i= 0; i*dd < width; ++i)
  {
    x=dd*i;
    y=0;
    
    for(auto y = 0.0; y < height && x < width; )
    {
      lambda = 0.001*x*y;
      x += dx + lambda*distribution(generator);
      y += dy + lambda*distribution(generator);
      line.push_back({x,y});
      
    }
    if (dist(generator))
    {
      boardRed.setPenColorRGBf(1.0, 0.0, 0.0);
      boardRed.drawPolyline(line);
    }
    else
    {
      boardBlue.setPenColorRGBf(0.0, 0.0, 1.0);
      boardBlue.drawPolyline(line);
    }
    line.clear();
  }
  
  dd*=tan(theta);
  for(auto i= 1; i*dd < height; ++i)
  {
    x=0;
    y=dd*i;
    
    for(auto x = 0.0; y < height && x < width; )
    {
      lambda = 0.001*x*y;
      x += dx + lambda*distribution(generator);
      y += dy + lambda*distribution(generator);
      line.push_back({x,y});
      
    }
    if (dist(generator))
    {
      boardRed.setPenColorRGBf(1.0, 0.0, 0.0);
      boardRed.drawPolyline(line);
    }
    else
    {
      boardBlue.setPenColorRGBf(0.0, 0.0, 1.0);
      boardBlue.drawPolyline(line);
    }
     line.clear();
   }

 // board.setClippingRectangle(0, 0, width, width*tan(theta));
  
  boardRed.saveSVG("colorAxi-Red.svg");
  boardBlue.saveSVG("colorAxi-Blue.svg");

  return 0;
}
