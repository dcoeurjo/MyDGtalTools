#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

using namespace DGtal;
using namespace Z2i;


int main(int argc, char** argv)
{
  Board2D board;

  double R = atof(argv[1]);
  trace.info()<< R <<std::endl;
  double k= std::floor(std::log2(sqrt(2.0)/2.0*R));
  
  board.drawCircle(0.0, 0.0, R);
  double x=0,y=0, cx=0,cy=0;
  
  double l = std::pow(2,k);
  board.drawRectangle(-l ,l,  2.*l, 2.*l);
  y = l;
  --k;
  
  bool stay;
  while (k>=1)
  {
    //l is half-length
    l /= 2;
    
    //right corner
    cx = x + 2*l;
    cy = y + 2*l;
    stay=true;
    while ((cx*cx+cy*cy)<(R*R))
    {
      stay=false;
      trace.info() << "Drawing "<<std::endl;
      
      //UpDown
      board.drawRectangle(cx - 2*l ,cy ,  2.*l, 2.*l);
      board.drawRectangle(-cx ,cy ,  2.*l, 2.*l);
      board.drawRectangle(cx - 2*l ,- cy + 2*l ,  2.*l, 2.*l);
      board.drawRectangle(-cx ,-cy+ 2*l ,  2.*l, 2.*l);

      //leftright
      board.drawRectangle(cy - 2*l ,cx ,  2.*l, 2.*l);
      board.drawRectangle(-cy ,cx ,  2.*l, 2.*l);
      board.drawRectangle(cy - 2*l  ,- cx + 2*l ,  2.*l, 2.*l);
      board.drawRectangle(-cy ,-cx+ 2*l ,  2.*l, 2.*l);
      
      cx += 2*l;
    }
    
    
    trace.info()<< "K = "<<k<< "   l= "<<l<<std::endl;
   
    if (!stay)
      y += 2*l;
    
    --k;
  }
  
  board << Point(0,0);
  board.saveCairo("mipmap.pdf", Board2D::CairoPDF);

}
