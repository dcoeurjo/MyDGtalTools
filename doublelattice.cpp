#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

using namespace DGtal;
using namespace Z2i;

int main()
{
  Point a(0,0);
  Point b(64,64);
 
  Point du(1,0);
  Point dv(0,1);

  Point dv1(3,2);
  Point dv2(1,4);
  
  Point dw1(4,3);
  Point dw2(2,5);
  
  Domain dom(a,b);

  Board2D board;
  board << dom;
  
  board.setPenColorRGBi(0,0,0);
  board.setLineStyle( Board2D::Shape::SolidStyle );

  //du x dv
  Display2DFactory::draw( board,   du, a);
  Display2DFactory::draw( board,   dv, a);

  board.setPenColorRGBi(12,200,12);
  board.setLineStyle( Board2D::Shape::SolidStyle );
  for(int x=0; x < 8; ++x)
  {
    for(int y=0; y < 8; ++y)
    {
      Display2DFactory::draw( board,   dv1, a + x*dv1 + y*dv2);
      Display2DFactory::draw( board,   dv2, a + x*dv1 + y*dv2);
    }
  }
  
  board.setPenColorRGBi(12,12,200);
  board.setLineStyle( Board2D::Shape::SolidStyle );
  for(int x=0; x < 8; ++x)
  {
    for(int y=0; y < 8; ++y)
    {
      Display2DFactory::draw( board,   dw1, a + x*dw1 + y*dw2);
      Display2DFactory::draw( board,   dw2, a + x*dw1 + y*dw2);
    }
  }
  board.saveEPS("doublelattice.eps");

}
