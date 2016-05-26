#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include "DGtal/geometry/curves/ArithmeticalDSS.h"

using namespace DGtal;
using namespace Z2i;

int main()
{
  Point a(0,0);
  Point b(16,16);


  Domain dom(a,b);
  
  Board2D board;
  board  << SetMode( dom.className(), "Paving" )
          << dom;
  
  board << SetMode(a.className(), "Both");

  
  NaiveDSS8<Integer> segment(Point(1,1), Point(15,1));
  board << SetMode(segment.className(), "");
  board << segment;
  
  NaiveDSS8<Integer> segment2(Point(1,3), Point(15,5));
  board << segment2;
  
  NaiveDSS8<Integer> segment3(Point(1,5), Point(15,10));
  board << segment3;
  
  
  NaiveDSS8<Integer> segment4(Point(1,7), Point(15,15));
  board << segment4;
  

  board.saveCairo("grid.pdf", Board2D::CairoPDF);


}
