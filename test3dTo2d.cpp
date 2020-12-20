#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board3DTo2D.h>
#include <DGtal/helpers/Shortcuts.h>

using namespace DGtal;
using namespace Z3i;
typedef Shortcuts<Z3i::KSpace>         SH3;

int main()
{
  size_t SIZE=5;
  size_t NB=SIZE*SIZE*SIZE;
  size_t grid = (size_t)std::ceil(sqrt(NB));
  
  Board3DTo2D<Space,KSpace> board;
  board.setCameraDirection(-1,-1,-1);
  auto params = SH3::defaultParameters();
  board.setCameraPosition(100*SIZE+3, 100*SIZE+3,100*SIZE+3);
  KSpace Kp;
  Kp.init( Point(-4000,-4000,-4000), Point(4000,4000,4000) ,true);
  for(auto real=0; real <= NB; ++real)
  {
    auto i = real % grid;
    auto j = real / grid;
    Point dec(Point(2*i*((5*SIZE)/2),0,-2*j*((5*SIZE)/2)));
    Domain dom(Point(0,0,0),Point(SIZE,SIZE,SIZE));
    DigitalSet set(dom);
    uint cpt=0;
    for(auto x=0; x < SIZE; ++x)
    for(auto y=0; y < SIZE; ++y)
    for(auto z=0; z < SIZE; ++z)
    {
      if ( cpt <= (int)((int)SIZE*SIZE*SIZE-(int)real))
        set.insert( Point(x,y,z));
      cpt++;
    }
    auto  binary_image   = SH3::makeBinaryImage( dom );
    for(auto &v: set)
      binary_image->setValue(v,true);
    auto K               = SH3::getKSpace( binary_image );
    auto surface         = SH3::makeDigitalSurface( binary_image, K, params );
    auto surfels         = SH3::getSurfelRange( surface, params );
    
    board <<  SetMode3D( surfels.begin()->className(), "Basic" );
    for(auto &s: surfels)
    {
      auto P = K.sKCoords(s);
      board << Kp.sCell(P+dec);
    }
    
  }
  auto real=555;
  std::string fnamep="test3d2d-"+std::to_string(real)+".png";
  std::string fnames="test3d2d-"+std::to_string(real)+".svg";
  board.saveCairo(fnamep.c_str(),Board3DTo2D<Space,KSpace>::CairoPNG,2970,2100);
  board.saveCairo(fnames.c_str(),Board3DTo2D<Space,KSpace>::CairoSVG,2970,2100);
  
  return 0;
}
