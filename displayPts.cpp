#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <fstream>

using namespace DGtal;
using namespace Z2i;

int main(int argc, char **argv)
{
  
  std::ifstream ifs;
  ifs.open (argv[1], std::ifstream::in);

  Board2D board;
  board.setUnit(Board2D::UCentimeter);
  double x,y, nx,ny;
  board.setFillColorRGBi(220,0,0);
  board.setPenColorRGBi(220,0,0);
  board.setLineWidth(4.0);
  while (ifs.good())
  {
    ifs >> x;
    if (!ifs.good()) continue;
    ifs >> y;
    ifs >> nx;
    ifs >> ny;
    x*=100;
    y*=100;
    nx*=10;
    ny*=10;
    board.fillCircle(x, y, 1);
    board.drawArrow(x, y, x+nx, y+ny);
    trace.info()<< x<<" "<<y<<" "<<nx<<" "<<ny<<std::endl;
  }
  ifs.close();
  
  ifs.open (argv[2], std::ifstream::in);
  board.setFillColorRGBi(0,220,0);
  board.setPenColorRGBi(0,220,0);
  while (ifs.good())
  {
    ifs >> x;
    if (!ifs.good()) continue;
    ifs >> y;
    ifs >> nx;
    ifs >> ny;
    x*=100;
    y*=100;
    nx*=10;
    ny*=10;
    board.fillCircle(x, y, 1);
    board.drawArrow(x, y, x+nx, y+ny);
    trace.info()<< x<<" "<<y<<" "<<nx<<" "<<ny<<std::endl;
  }
  
  board.saveCairo("pts.png",Board2D::CairoPNG);
 
  return 0;
}
