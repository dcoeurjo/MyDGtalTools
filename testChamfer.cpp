#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#define A 3
#define B 4

template <typename Point>
int d (Point &p, Point &q)
{
  int dx = abs(q[0] - p[0]);
  int dy = abs(q[1] - p[1]);
  if (dx > dy)
    return B*dy + (dx -dy)*A;
  else
    return B*dx + (dy -dx)*A;
}

using namespace DGtal;
using namespace Z2i;

int main()
{
  Point a(0,0);
  Point b(16,16);
  Point p(3,5);
  Point q(5,0);

  Color red(255,0,0);
  Color blue(0,0,255);
  Color black(0,0,0);

  Domain dom(a,b);

  Board2D board;
  board << dom;
  for(Domain::ConstIterator it = dom.begin(), itend=dom.end();
      it != itend;
      ++it)
    {
      if ( d( *it, p) < d(*it,q))
        board << CustomStyle( p.className(), new CustomColors(blue,blue));
      else
        board << CustomStyle( p.className(), new CustomColors(red,red));
      board << *it;
    }

  board << CustomStyle( p.className(), new CustomColors(black,black));
  board << p << q;
  board.saveEPS("chamfer.eps");

}
