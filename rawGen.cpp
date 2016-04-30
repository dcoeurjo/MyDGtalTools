#include<DGtal/base/Common.h>
#include<DGtal/helpers/StdDefs.h>
#include <DGtal/io/writers/RawWriter.h>
#include <DGtal/shapes/implicit/ImplicitBall.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

using namespace DGtal;
using namespace Z3i;

int main()
{
  const unsigned int N = 128;
  Domain domain(Point(0,0,0), Point(N-1,N-1,N-1));
  ImageContainerBySTLVector<Domain,unsigned char> image(domain);

  typedef ImplicitBall< Z3i::Space > MyEuclideanShape;
  MyEuclideanShape shapeA( RealPoint(0,0,0), 35.0 );
  for(auto it = domain.begin(), itend=domain.end(); it!=itend; ++it)
    if (shapeA( *it - RealPoint(N/2.0,N/2.0,N/3.0)) >0.0 )
     {
       image.setValue( *it, 5 );
}
  for(auto it = domain.begin(), itend=domain.end(); it!=itend; ++it)
    if (shapeA(*it - RealPoint(N/2.0,N/2.0,N*2.0/3.0))>0.0 )
          image.setValue( *it, 10 );

  RawWriter<ImageContainerBySTLVector<Domain,unsigned char> >::exportRaw8("labels.raw", image);

  return 0;
}
