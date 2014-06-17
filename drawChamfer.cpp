#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

using namespace DGtal;
using namespace Z2i;

int main()
{
  Point a(0,0);
  Point da(1,0);
  Point db(1,1);
  Point dc(2,1);
  Point dd(3,1);
  Point de(3,2);
  Point df(4,3);
  Point dg(4,1);
 

  Color black(0,0,0);

  Domain dom(Point(-1,-1) , Point(10,3) );

  Board2D board;
  board << dom;
  

  board.setPenColorRGBi(0,0,0);
  board.setLineStyle( Board2D::Shape::SolidStyle );

  Point aa(3,0);
  Point aaa(6,0);

  Display2DFactory::draw( board,   da, a); 
  Display2DFactory::draw( board,   db, a); 
  Display2DFactory::draw( board,   da, aa); 
  Display2DFactory::draw( board,   db, aa); 
  Display2DFactory::draw( board,   dc, aa); 
 
  Display2DFactory::draw( board,   da, aaa); 
  Display2DFactory::draw( board,   db, aaa); 
  Display2DFactory::draw( board,   dc, aaa); 
  Display2DFactory::draw( board,   dd, aaa); 
  Display2DFactory::draw( board,   de, aaa); 
  Display2DFactory::draw( board,   df, aaa); 
  Display2DFactory::draw( board,   dg, aaa); 


  board.saveEPS("chamferVect.eps");


  board.clear();
  Domain domain(Point(0,0), Point(5,5));
  board << domain;
  
  board.saveEPS("chamferGraphDom.eps");

  board.setPenColorRGBi(0,0,0);
  board.setLineStyle( Board2D::Shape::SolidStyle );

  for(Domain::ConstIterator it = domain.begin(), itend=domain.end();
      it != itend; ++it)
    {
      Display2DFactory::draw( board,   da, (*it)); 
      Display2DFactory::draw( board,   db, (*it)); 
      Display2DFactory::draw( board,   dc, (*it)); 
      Display2DFactory::draw( board,   -da, (*it)); 
      Display2DFactory::draw( board,   -db, (*it)); 
      Display2DFactory::draw( board,   -dc, (*it)); 
      Display2DFactory::draw( board,   Point(-1*da[0],1*da[1]), (*it)); 
      Display2DFactory::draw( board,   Point(-1*db[0],1*db[1]), (*it)); 
      Display2DFactory::draw( board,   Point(-1*dc[0],1*dc[1]), (*it)); 
      Display2DFactory::draw( board,   Point(1*da[0],-1*da[1]), (*it)); 
      Display2DFactory::draw( board,   Point(1*db[0],-1*db[1]), (*it)); 
      Display2DFactory::draw( board,   Point(1*dc[0],-1*dc[1]), (*it)); 
      
    }
  board.saveEPS("chamferGraph.eps");


  board.clear();
  board.setPenColorRGBi(0,0,0);
  board.setLineStyle( Board2D::Shape::SolidStyle );
  board << Domain(Point(-1,-1), Point(3,2));
  board.setPenColorRGBi(0,0,0);
  board.setLineStyle( Board2D::Shape::SolidStyle );

  Display2DFactory::draw( board,   da, a); 
  Display2DFactory::draw( board,   db, a); 
  Display2DFactory::draw( board,   dc, a); 
  board.saveEPS("chamferMask.eps");
  

}

