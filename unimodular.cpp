#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

using namespace DGtal;
using namespace Z2i;

int main()
{
  Point a(0,0);
  Point aa(6,6);
  Point b(16,8);
  Point p(1,1);
  Point dq(0,1);
  Point dp(1,1);

  Point m(5,1);
  Point dm(4,1);
  Point dmm(3,1);
  

  Point n(5,4);
  Point dn(4,1);
  Point dnn(3,2);
  

  Color black(0,0,0);

  Domain dom(a,b);
  Domain dom2(a,aa);
  
  Board2D board2;
  board2  << SetMode( dom2.className(), "Paving" )
          << dom2;
  board2.saveEPS("grid.eps");


  Board2D board;
  board << dom;
  board.setPenColorRGBi(0,0,0);
  board.setLineStyle( Board2D::Shape::SolidStyle );
  Display2DFactory::draw( board,   dq, a); 
  Display2DFactory::draw( board,   Point(1,0), a); 
  board.saveEPS("domain.eps");

  board.clear();
  board << dom;
  
  Display2DFactory::draw( board,   dq, p); 
  Display2DFactory::draw( board,   dp, p); 

  Display2DFactory::draw( board,   dm, m); 
  Display2DFactory::draw( board,   dmm, m); 

  Display2DFactory::draw( board,   dn, n); 
  Display2DFactory::draw( board,   dnn, n); 

  board.setPenColorRGBi(12,200,12);
  board.setLineStyle( Board2D::Shape::SolidStyle );

  Display2DFactory::draw( board,   dp, dq+p); 
  Display2DFactory::draw( board,   dq, dp+p); 

  Display2DFactory::draw( board,   dm, m+dmm); 
  Display2DFactory::draw( board,   dmm, m+dm); 

  Display2DFactory::draw( board,   dn, n+dnn); 
  Display2DFactory::draw( board,   dnn, n+dn); 



  board.saveEPS("unimodular.eps");

}
