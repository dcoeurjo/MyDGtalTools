#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/io/boards/Board2D.h>
#include <vector>

using namespace DGtal;
using namespace Z2i;

int main()
{
  Domain domain(Point(0,0), Point(2,2));
  
  //Testing auto in domain iteration
  trace.beginBlock("Domain iterator with auto");
  for(auto it = domain.begin(), itend=domain.end(); it != itend; ++it)
    trace.info()<< *it << " ";
  trace.info()<<std::endl;
  trace.endBlock();
  
  //For loop on a range
  trace.beginBlock("range-based for on the domain");
  for(auto point : domain)
    trace.info() <<point << " ";
  trace.info() <<std::endl;
  trace.endBlock();
  
  //For loop on an image range
  trace.beginBlock("image (const) iterator on values");
  ImageContainerBySTLVector<Domain, int> image(domain);
  for(auto value: image.range())
    trace.info() <<value << " ";
  trace.info() <<std::endl;
  trace.endBlock();
  
  //For loop on an image domain range
  trace.beginBlock("(const) iterator on image domain");
  for(auto value: image.domain())
    trace.info() <<value << " ";
  trace.info() <<std::endl;
  trace.endBlock();
  
  //R-W for loop on an image range
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
  
  //Testing lambda expressions
  trace.beginBlock("Lambda exps");
  std::for_each(image.range().begin(), image.range().end(), [](int a){ trace.info() <<a<<" "; });
  trace.info()<<std::endl;
  std::transform(image.range().begin(), image.range().end(), image.range().begin(), [](int a){return 2*a+1;});
  std::for_each(image.range().begin(), image.range().end(), [](int a){ trace.info() <<a<<" "; });
  trace.info() <<std::endl;
  trace.endBlock();
  
  //Lambda expression based ultimate homotopic thining
  trace.beginBlock("STL+Lambda homotopic thinning");
  Domain domain2( Point(-32,-32), Point(32,32));
  DigitalSet set(domain2);
  //constructing a torus
  std::for_each(domain2.begin(), domain2.end(), [&set](Point p){ if ((p.norm() < 10 ) && p.norm() > 5) set.insertNew(p); });
  trace.info() << set << std::endl;
  Board2D board;
  board << set;
  board.saveEPS("original.eps");

  Object4_8 object(dt4_8 , set);
  std::vector<Point> Q;
  auto stable = false;
  auto step = 0;
  DigitalSet & S = object.pointSet();
  while(!stable)
  {
    trace.info() << "step = "<<step;
    stable = true;
    std::for_each(S.begin(), S.end(),
                  [&Q,&object](Point p){ if (object.isSimple(p)) Q.push_back(p); });
    std::for_each(Q.begin(), Q.end(),
                  [&object,&stable,&S](Point p){ if (object.isSimple(p))  {stable = false; S.erase(p);} });
    Q.clear();
    trace.info() << "|S| = "<< S.size() << std::endl;
    step++;
  }
  trace.endBlock();
  board << CustomStyle( object.pointSet().className(), new CustomColors(Color::Black, Color::Red))
        << object.pointSet();
  board.saveEPS("skeleton.eps");
  return 0;
}
