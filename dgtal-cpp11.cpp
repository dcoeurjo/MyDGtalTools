#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

using namespace DGtal;
using namespace Z2i;

int main()
{
  Domain domain(Point(0,0), Point(2,2));
  
  trace.beginBlock("Domain iterator with auto");
  for(auto it = domain.begin(), itend=domain.end(); it != itend; ++it)
    trace.info()<< *it << " ";
  trace.info()<<std::endl;
  trace.endBlock();
  
  trace.beginBlock("range-based for on the domain");
  for(auto point : domain)
    trace.info() <<point << " ";
  trace.info() <<std::endl;
  trace.endBlock();
  
  trace.beginBlock("image (const) iterator on values");
  ImageContainerBySTLVector<Domain, int> image(domain);
  for(auto value: image.range())
    trace.info() <<value << " ";
  trace.info() <<std::endl;
  trace.endBlock();
  
  trace.beginBlock("(const) iterator on image domain");
  for(auto value: image.domain())
    trace.info() <<value << " ";
  trace.info() <<std::endl;
  trace.endBlock();
  
  trace.beginBlock("image  iterator on values");
  int cpt=0;
  for(auto& value: image.range())
  {
    value = cpt;
    cpt++;
    trace.info() <<value << " ";
  }
  trace.info() <<std::endl;
  trace.endBlock();
  
  
  return 0;
}
