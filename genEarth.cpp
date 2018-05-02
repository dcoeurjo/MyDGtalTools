#include <iostream>
#include <set>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>

#include <DGtal/shapes/parametric/Ball3D.h>
#include <DGtal/shapes/GaussDigitizer.h>
#include <DGtal/topology/LightImplicitDigitalSurface.h>
#include <DGtal/topology/DigitalSurface.h>
#include <DGtal/io/writers/GenericWriter.h>

using namespace DGtal;
using namespace Z3i;

double mix(const double x,const double y,const double a)
{
  return (x*(1-a)+y*a);
}

double hash(double n)
{
  return (sin(n) * 1e4)- std::floor((sin(n) * 1e4));
}

double noise(RealPoint &x)
{
  const RealPoint step(110, 241, 171);
  RealPoint i(std::floor(x[0]),std::floor(x[1]), std::floor(x[2]));
  RealPoint f = x - i;
  float n = i.dot(step);
  RealPoint u(f[0] * f[0] * (3.0 - 2.0 * f[0]),
              f[1] * f[1] * (3.0 - 2.0 * f[1]),
              f[2] * f[2] * (3.0 - 2.0 * f[2]));
  
  return mix(mix(mix( hash(n + step.dot(RealPoint(0, 0, 0))), hash(n + step.dot(RealPoint(1, 0, 0))), u[0]), mix( hash(n + step.dot(RealPoint(0, 1, 0))), hash(n + step.dot(RealPoint(1, 1, 0))), u[0]), u[1]), mix(mix( hash(n + step.dot(RealPoint(0, 0, 1))), hash(n + step.dot(RealPoint(1, 0, 1))), u[0]), mix( hash(n + step.dot(RealPoint(0, 1, 1))), hash(n + step.dot(RealPoint(1, 1, 1))), u[0]), u[1]), u[2]);
}




double FBM5(RealPoint &x)
{
  double v = 0.0;
  double a = 0.5;
  RealPoint shift(100,100,100);
  for (int i = 0; i < 5; ++i)
  {
    v += a * noise(x);
    x = x * 2.0 + shift;
    a *= 0.5;
  }
  
  return v;
}


int main(int argc, char **argv)
{
  const double radius = atof(argv[1]);
  const double water = radius*1.45;
  const double core = radius*1.2;
  Domain domain(Point(-2*radius,-2*radius,-radius*2), Point(radius*2,radius*2,radius*2));
  ImageContainerBySTLVector<Domain, unsigned char> image(domain);

  RealPoint dir(rand(), rand(), rand());
  dir = dir.getNormalized();
  
  std::vector<RealPoint> dirs;
  for(auto i=0; i < 7; ++i)
  {
    RealPoint dir(rand() - RAND_MAX/2, rand()- RAND_MAX/2, rand()- RAND_MAX/2);
    dirs.push_back(dir.getNormalized());
    trace.info()<<dir<<std::endl;
  }
  
  for(auto &p: domain)
  {
    RealPoint x = p;
    RealPoint y = 1.0*x.getNormalized();
    double mountain = pow(1.0 - FBM5(y), 3.0);
    //const double water = radius*0.85;
    y = x.getNormalized();
    
    double dist=(x - radius*y).norm() - 0.6*mountain*radius;
    if ((dist < 0.3*radius ) || (p.norm() < radius))
      image.setValue(p,128);
    
    //water
    if ((p.norm() < water) && (image(p)==0))
      image.setValue(p,64);
    
    //Volcanos + core
     if (image(p)!=0)
    {
      for(auto i=0; i < 7; ++i)
      {
        double d =(y-dirs[i]).norm();
        double dec = exp(-d*d * 3*core);
        if( ((p.norm() - 0.4*core*dec ) < core))
          image.setValue(p,32);
      }
    }

  }
  
  GenericWriter<ImageContainerBySTLVector<Domain, unsigned char> >::exportFile("earth.vol", image);
  
  return 0;
}
