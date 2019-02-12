#include <string>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/io/readers/GenericReader.h>

using namespace DGtal;
using namespace Z2i;

const double f = 0.037;
const double k = 0.06;
const double dt = .4  ;
const double ra = 1.0;
const double rb = 0.5;

Point plus(const Point& a, const Point &d, const Domain &dom)
{
  Point c = a+d;
  Point l = dom.lowerBound();
  Point u = dom.upperBound();

  if (c[0] > u[0]) c[0] -= u[0]-l[0];
  if (c[0] < l[0]) c[0] += u[0]-l[0];
  
  if (c[1] > u[1]) c[1] -= u[1]-l[1];
  if (c[1] < l[1]) c[1] += u[1]-l[1];
  return c;
}


template<typename Image>
double lap(const Image &A, const Point &p)
{
  Point h(1,0);
  Point v(0,1);
  
  return A(plus(p,h,A.domain())) + A(plus(p,-h,A.domain())) + A(plus(p,-v,A.domain())) + A(plus(p,v,A.domain())) - 4.0*A(p);
 }
// A(x-1,y) + A(x+1,y) + A(x,y-1) + A(x,y+1) - 4*A(x,y)


template<typename Image>
void oneStep(const Domain &innerDomain,
             Image &A,
             Image &B)
{
  for(auto &p: A.domain())
  {
    double ap = ra * lap(A,p) - A(p)*B(p)*B(p)  + f*(1.0-A(p));
    double bp = rb * lap(B,p) + A(p)*B(p)*B(p)  - (f+k)*B(p);
  
    A.setValue(p, A(p) + dt*ap);
    B.setValue(p, B(p) + dt*bp);
  }
}

double clamp(const double a)
{
  if (a<0.0) return 0;
  if (a>0.7) return 1.0;
  return a;
}

template<typename Image, typename Im>
void exportMap(std::string &filename,
               const Image &A,
               const Image &B,
               Im &mix)
{

  for(auto &p: A.domain())
  {
    mix.setValue(p, Color(255-static_cast<unsigned char>(255*clamp(5*B(p))),
                          255-static_cast<unsigned char>(255*clamp(5*B(p))),
                          255-static_cast<unsigned char>(255*clamp(5*B(p)))));
  }
  trace.info() << "hop"<<std::endl;
  mix >> filename;
}

int main()
{
  Domain domain(Point(0,0), Point(511,511));
  Domain innerDomain(Point(1,1), Point(510,510));
  
  ImageContainerBySTLVector<Domain,double> A(domain);
  ImageContainerBySTLVector<Domain,double> B(domain);
  ImageContainerBySTLVector<Domain,Color> mix(A.domain());

  
  ImageContainerBySTLVector<Domain,int> logo = GenericReader< ImageContainerBySTLVector<Domain,int>>::import("allego.png");
  
  
  for(auto &v: A)
    v=1.0;
  for(auto &v: B)
    v=0.0;
  
  for(auto &p: B.domain())
    if (logo(p) < 254) B.setValue(p, 1.0);
  
//  for(auto w = 0; w <10 ; ++w)
//    for(auto ww = 0; ww <10 ; ++ww)
//      B.setValue(Point(250+w,250+ww), 1.0);

  int cpt=0;
  for(auto k=0; k < 50000; k++)
  {
    oneStep(innerDomain,A,B);
    if ((k % 100) == 0)
    {
      std::string filename="allreaction" + std::to_string(100000 + cpt) + ".png";
      exportMap(filename,A,B,mix);
      cpt++;
   }
  }
  return 0;
}
