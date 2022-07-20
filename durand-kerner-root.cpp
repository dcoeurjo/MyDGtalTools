#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <random>
#include <complex>


using namespace DGtal;
using namespace Z2i;
using Complex = std::complex<double>;

auto simpleP=[](const Complex &x) {
  return (x- Complex(1,0)) * (x- Complex(-1,0));
};

template <typename P>
std::vector<Complex> iterateDurandKerner(const P &poly, const std::vector<Complex> &Z, double lambda)
{
  std::vector<Complex> Zp = Z;
  for(auto i=0; i < Z.size(); ++i)
  {
    Complex denom(1,0);
    for(auto j=0; j < Z.size(); ++j)
      if (i!=j)
        denom *= (Z[i] - Z[j]);
    Zp[i] = Z[i]- lambda*poly(Z[i]) / denom ;
  }
  return Zp;
}


int main()
{
  Board2D board;
  const auto nbRoots=100  ;
  std::vector<Complex>  Z(nbRoots);
  std::vector<Complex>  Zp(nbRoots);

  std::vector<Complex> roots(nbRoots);
  for (auto &r : roots)
    r = Complex( 1-2.0*(rand() /(double) RAND_MAX),  1-2.0*(rand() /(double) RAND_MAX));
  
  auto randP=[&](const Complex &x) {
    Complex p(1,0);
    for(auto i=0; i < nbRoots; ++i)
      p *= (x - roots[i]);
    return p;
  };
  
  for(auto i=0; i < nbRoots; ++i)
  {
    Z[i] = 2.0*Complex( cos(i*2.0*M_PI/(double)nbRoots), sin(i*2.0*M_PI/(double)nbRoots)); //circle
 //   Z[i] = Complex( (i+1)/(double)nbRoots, 2.0); //line
    //Z[i] = Complex( 1-2.0*(rand() /(double) RAND_MAX),  1-2.0*(rand() /(double) RAND_MAX));
  }
  
  board.setPenColor(Color::Gray);
  for(auto &z:Z)
    board.fillCircle(z.real(), z.imag(), 0.03);
    
  
//  Z[0] = Complex(0.1,1.);
//  Z[1] = Complex(-0.1,-1.0);
  
  
  std::vector<DGtal::Color> cols= {Color::Red, Color::Blue};
  std::vector<std::vector<LibBoard::Point>> curves(nbRoots);

  
  for(auto k=0; k < 10000; k++)
  {
    for(auto i=0; i < nbRoots; ++i)
      curves[i].push_back(LibBoard::Point(Z[i].real(), Z[i].imag()));
    Zp = iterateDurandKerner(randP, Z, 0.05);
    std::swap(Zp,Z);
  }

  for(auto i=0; i < nbRoots; ++i)
  {
    board.setPenColor(cols[i % 2]);
    board.drawPolyline(curves[i]);
  }
  
  board.setPenColor(Color::Green);
  board.setFillColor(Color::Green);
  for(auto &r:roots)
    board.fillCircle(r.real(), r.imag(), 0.03);
  
  board.saveSVG("durandKerner.svg");
  std::cout<<"Ok."<<std::endl;
  return 0;
}
