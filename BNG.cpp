#include <DGtal/base/Common.h>
#include <DGtal/io/boards/Board2D.h>
#include <DGtal/io/colormaps/HueShadeColorMap.h>
#include <DGtal/helpers/StdDefs.h>  

using namespace DGtal;
using namespace Z2i;


int main()
{
  const unsigned int n=64;
  const unsigned int m=8; //size of blocks
  std::vector<double> sx(n*n);
  std::vector<double> sy(n*n);
  const Domain dom(Point(0,0), Point(n-1,n-1));
  Board2D board;
  HueShadeColorMap<unsigned int, 1> cmap(0,n*n);
  
  //randomshift
  std::vector<unsigned int> rshifti(n/m);
  for(auto i=0;i<n/m;i++)
    rshifti[i]=i;
  std::random_shuffle(rshifti.begin(), rshifti.end());
  //randomshift
  std::vector<unsigned int> rshiftj(n/m);
  for(auto i=0;i<n/m;i++)
    rshiftj[i]=i;
  std::random_shuffle(rshiftj.begin(), rshiftj.end());
  
  for(auto i: rshifti)
    trace.info()<< i;
  trace.info()<<std::endl;
  
  //Init shuffle
  std::vector<unsigned int> shufflei(n*n);
  std::vector<unsigned int> shufflej(n*n);
  for(auto i=0; i<n;++i)
    for(auto j=0; j < n; ++j)
    {
      int bloci = (i/m), blocj=(j/m);
      int ibloci = i - (i/m)*m, iblocj = j - (j/m)*m;
      trace.info()<< "i="<<i<<" j="<<j<<" bloci="<<bloci<<" nextbloci="<< rshifti[bloci]
      <<" postnext="<<  rshifti[bloci]*m<<" iboci="<<ibloci<<std::endl;
      shufflei[i+j*n] = (rshifti[bloci] )*(m) + ibloci ;
      shufflej[i+j*n] = (rshiftj[blocj])*(m) + iblocj ;
    }
  
  board << SetMode( dom.className(), "Paving" )
    << dom;
  
  double alpha = sqrt(3.0);
  Point p;
  
  //fillup
  for(auto i=0; i<n;++i)
    for(auto j=0; j < n; ++j)
    {
      sx[i+j*n] = alpha*i - floor(alpha*i);
      sy[i+j*n] = alpha*j   -floor(alpha*j);
    }
  
  //PointSet
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      //board.drawArrow(i-0.5,j-0.5,i+sy[i+j*n]-0.5, j+sx[i+j*n]-0.5);
      board.setPenColor(Color(255,150,0,128));
      board.drawLine(i-0.5,j-0.5,i+sy[i+j*n]-0.5, j+sx[i+j*n]-0.5);
      board.setPenColor(Color::Red);
      board.fillCircle(i+sy[i+j*n]-0.5, j+sx[i+j*n]-0.5, 0.1);
    }
  board.saveCairo("bng-ptset.pdf", Board2D::CairoPDF);
  
  board.clear();
  board << SetMode( dom.className(), "Paving" )
  << dom;
  
  //PointSet-x
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      //board.drawArrow(i-0.5,j-0.5,i+sy[i+j*n]-0.5, j-0.5);
      board.setPenColor(Color(255,150,0));
      board.drawLine(i-0.5,j-0.5,i+sy[i+j*n]-0.5, j-0.5);
      board.setPenColor(Color::Red);
      board.fillCircle(i+sy[i+j*n]-0.5, j-0.5, 0.1);
    }
  board.saveCairo("bng-ptset-x.pdf", Board2D::CairoPDF);
  board.clear();
  board << SetMode( dom.className(), "Paving" )
  << dom;
  
  //PointSet-y
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      //board.drawArrow(i-0.5,j-0.5,i-0.5, j+sx[i+j*n]-0.5);
      board.setPenColor(Color(255,150,0));
      board.drawLine(i-0.5,j-0.5,i-0.5, j+sx[i+j*n]-0.5);
      board.setPenColor(Color::Red);
      board.fillCircle(i-0.5, j+sx[i+j*n]-0.5, 0.1);
    }
  board.saveCairo("bng-ptset-y.pdf", Board2D::CairoPDF);
  board.clear();
  board << SetMode( dom.className(), "Paving" )
  << dom;
  
  
  //Color
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      board.fillCircle(i+sy[i+j*n]-0.5, j+sx[i+j*n]-0.5, 0.1);
    }
  
  board.saveCairo("bng-orig.pdf", Board2D::CairoPDF);
  board.clear();
  
  //Color
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
      board << CustomStyle( p.className(), new CustomFillColor( cmap(i+j*n)))
            << Point(i,j);
  
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      board.fillCircle(i+sy[i+j*n]-0.5, j+sx[i+j*n]-0.5, 0.1);
    }
  
  board.saveCairo("bng-shuffle-orig.pdf", Board2D::CairoPDF);
  board.clear();
  
  //Scrambling
  board << SetMode( dom.className(), "Paving" )
        << dom;
  unsigned int ii,jj;
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      ii = shufflei[i+j*n];
      jj = shufflej[i+j*n];
      trace.info()<< Point(ii,j)<<" ";
      board << CustomStyle( p.className(), new CustomFillColor( cmap(i+j*n)))
            << Point(ii,j);
    }
  board.saveCairo("bng-shuffle-x.pdf", Board2D::CairoPDF);
  board.clear();
 
  board << SetMode( dom.className(), "Paving" )
  << dom;
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      ii = shufflei[i+j*n];
      jj = shufflej[i+j*n];
      trace.info()<< Point(ii,jj)<<" ";
      board << CustomStyle( p.className(), new CustomFillColor( cmap(i+j*n)))
      << Point(ii,jj);
    }
  board.saveCairo("bng-shuffle-xy.pdf", Board2D::CairoPDF);
  board.clear();
  
  //Final set
  board << SetMode( dom.className(), "Paving" )
  << dom;
  board.setPenColor(Color::Red);
  board.setFillColor(Color::Red);
  for(auto i=0; i<n;++i)
    for(auto j=0; j<n; ++j)
    {
      ii = shufflei[i+j*n];
      jj = shufflej[i+j*n];
      board.fillCircle(i+sy[ii+jj*n]-0.5, j+sx[ii+jj*n]-0.5, 0.1);
    }
  
  board.saveCairo("bng-final.pdf", Board2D::CairoPDF);
  board.clear();
  
  return 0;
  
}