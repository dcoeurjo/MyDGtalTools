#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/HueShadeColorMap.h>
#include <DGtal/helpers/StdDefs.h>  
#include <fstream>
#include <iostream>


using namespace DGtal;
using namespace Z2i;
using namespace std;

int main(int argc, char**argv)
{
  std::vector<double> x,y,p;
  Board2D board;
  double xx,yy,pp;
  ifstream myfile (argv[1], std::ifstream::in);
  board.setPenColor(Color::Red);
  board.setClippingRectangle(-4, 4, 8, 8);
  while (myfile.good())
  {
    myfile>> xx;
    myfile >>yy;
    myfile>> pp;
    x.push_back(xx);
    y.push_back(yy);
    p.push_back(pp);
    //trace.info()<<xx<<" "<<yy<<" "<<pp<<std::endl;
    if (pp> atof(argv[2]))
      board.drawCircle(xx, yy, atof(argv[3]));
    
  }
  
  
  board.saveCairo("peaks.pdf", Board2D::CairoPDF);
  board.clear();
  
  return 0;
  
}
