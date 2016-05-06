#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/helpers/StdDefs.h>  

using namespace DGtal;
using namespace Z2i;

int main()
{
  const unsigned int n=4;
  const Domain dom(Point(0,0), Point(n-1,n-1));
  Board2D board;
  
  board << SetMode( dom.className(), "Paving" )
  << dom;
  
  double alpha = sqrt(3.0);
  
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      double dx= alpha*i - floor(alpha*i) ;
      double dy= alpha*j - floor(alpha*j) ;
      trace.info() << dx<<"  "<<dy<<std::endl;
      board.fillCircle(i+dy-0.5, j+dx-0.5, 0.1);
    }
  
  
  
  board.saveCairo("bng.pdf", Board2D::CairoPDF);

  return 0;
  
}