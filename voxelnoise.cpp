#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/helpers/Shortcuts.h>
#include <DGtal/io/writers/VolWriter.h>
#include <random>

using namespace DGtal;
using namespace Z3i;
typedef Shortcuts<Z3i::KSpace>  SH3;

int main()
{
  Point p(0,0,0);
  Point q(40,20,100);
  Domain dom(p,q);
  DigitalSet mySet(dom);

  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  
  ImageContainerBySTLVector<Domain, unsigned char> image(dom);

  for(const auto &voxel: dom )
    if (std::pow(distribution(generator),0.2  ) < (double)voxel[2]/100.0)
      image.setValue(voxel, 128);
  else
    image.setValue(voxel, 0);
  
  VolWriter<ImageContainerBySTLVector<Domain, unsigned char>>::exportVol("voxelnoise.vol", image);
  return 0;
}
