#include <random>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board3DTo2D.h>
#include <DGtal/io/viewers/Viewer3D.h>
#include <DGtal/io/boards/Board3D.h>
#include <DGtal/helpers/Shortcuts.h>
//#include <DGtal/helpers/ShortcutsGeometry.h>

using namespace DGtal;
using namespace Z3i;
typedef Shortcuts<Z3i::KSpace>         SH3;
//typedef ShortcutsGeometry<Z3i::KSpace>         SHG3;

int main(int argc, char**argv)
{
  QApplication application(argc,argv);
  Viewer3D<> viewer;
  viewer.show();
  
  Board3D<Z3i::Space,Z3i::KSpace> board;
  size_t SIZEX=10;
  size_t SIZEY=15;
  size_t SIZEZ=8;
  size_t NB=9;
  size_t grid = (size_t)std::ceil(sqrt(NB));
  
  std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(1.0, 2.0);
  
  for(auto real=0; real < NB; ++real)
  {
    auto i = real % grid;
    auto j = real / grid;
    Point dec(i*(SIZEY+ SIZEX),j*(SIZEY+ SIZEX),0);
    Domain dom(Point(0,0,0),Point(SIZEX,SIZEY,SIZEZ));
    DigitalSet set(dom);
    uint cpt=0;
    
    double scale = dis(gen)/2.0;
    double scaley =  dis(gen);
    double phi = dis(gen)*M_PI;
    for(auto x=0; x < SIZEX; ++x)
    for(auto y=0; y < SIZEY; ++y)
    {
      int height = SIZEZ/2 + static_cast<int>((SIZEZ/2) * cos(phi+scale*x*M_PI/(double)SIZEX)*sin(phi+scaley*y*M_PI /(double) SIZEY));
      std::cout<<height<<std::endl;
   //   auto height =  1 + std::rand() % SIZEZ;
      for(auto z=0; z < height; ++z)
        set.insert( Point(x,y,z));
    }
    
    for(auto &v: set)
    {
      viewer << v + dec;
      board << v+dec;
    }
  }
  viewer << Viewer3D<>::updateDisplay;
  board.saveOBJ("cubes.obj");
  return application.exec();
  
}
