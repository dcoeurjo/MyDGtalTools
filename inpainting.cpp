#include <iostream>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/readers/GenericReader.h>
#include <DGtal/io/writers/GenericWriter.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include "DGtal/geometry/volumes/distance/DistanceTransformation.h"
#include <DGtal/kernel/BasicPointPredicates.h>
using namespace DGtal;
using namespace Z2i;


double sqdist(const Color &a, const Color &b)
{
  return ((double)a.red()-(double)b.red())*((double)a.red()-(double)b.red()) +
   ((double)a.green()-(double)b.green())*((double)a.green()-(double)b.green()) +
  ((double)a.blue()-(double)b.blue())*((double)a.blue()-(double)b.blue());
}


template<typename Image>
Color getColor(const Point &p,
               const Image &image,
               const Image &mask,
               const Domain &dom,
               const Domain &domimage)
{
  double minl2=1e23;
  Point minp;
  for(auto &q : domimage)
  {
    if (mask(q)== Color::White) continue;
    double l2=0.0;
    Point localq, localp;
    for(auto &local: dom)
    {
      localp = p + local;
      localq = q + local;
      if (mask(localq)==Color::White) {l2=1e23; continue;}
      if (mask(localp)==Color::Black)
        l2 += sqdist(image(localp), image(localq));
    }
    if (l2 < minl2)
    {
      minl2 = l2;
      minp = q;
    }
  }
  return image(minp);
}

template <typename Im>
struct CompPoint
{
  CompPoint(const Im & anIm): myIm(anIm){};
  bool operator()(const Point &a, const Point &b) const
  {
    return (myIm(a) < myIm(b));
  }
  Im myIm;
};

int main(int argc, char **argv)
{
  
  typedef ImageContainerBySTLVector<Z2i::Domain, DGtal::Color> Image;
  
  Image image = MagickReader<Image>::importImage(argv[1]);
  Image mask = MagickReader<Image>::importImage(argv[2]);

  
  DigitalSet set(mask.domain());
  for(auto &p: mask.domain())
    if (mask(p) == Color::White)
      set.insertNew(p);
  typedef DistanceTransformation<Z2i::Space, DigitalSet, Z2i::L2Metric> DTL2;
  DTL2 dt(image.domain(),set, Z2i::l2Metric );
  
  
  std::vector<Point> queue(set.size());
  std::copy(set.begin(), set.end(), queue.begin());
  CompPoint<DTL2> comp(dt);
  std::sort(queue.begin(), queue.end(), comp);
  trace.info()<<"Set: "<<set.size()<<std::endl;
  trace.info()<<"Queue: "<<queue.size()<<std::endl;
  
  
  const unsigned int size=11;
  Domain dom(Point::diagonal(-(int)size/2), Point::diagonal(size/2));
  Domain domimage(image.domain().lowerBound() + Point::diagonal(size/2),
                  image.domain().upperBound() - Point::diagonal(size/2));
  
  int cpt=0;
  for(auto &p : queue)
  {
    trace.progressBar(cpt, queue.size());
    Color c = getColor(p, image, mask, dom,domimage);
    image.setValue( p , c);
    mask.setValue(p, Color::Black);
    ++cpt;
  }

  //Writting the image to another format
  MagickWriter<Image>::exportMagick( "outImage.png", image );
  MagickWriter<Image>::exportMagick( "outMask.png", mask );

}

