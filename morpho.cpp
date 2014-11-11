#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/io/readers/PGMReader.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/ImageContainerBySTLVector.h>
#include <DGtal/io/writers/PGMWriter.h>
#include <string>

using namespace DGtal;
using namespace std;

template<typename Image,typename Domain>
void dilation(Image &imout, const Image &im, const Domain &dom)
{
  for(typename Image::Domain::ConstIterator it = im.domain().begin(),
      itend=im.domain().end(); it != itend; ++it)
  {
    typename Image::Value val=im(*it);
    for(typename Domain::ConstIterator itdom=dom.begin(), itenddom=dom.end(); itdom!=itenddom; ++itdom )
    {
     if (im.domain().isInside(*it+ *itdom))
         if (val < im(*it + *itdom)) val = im(*it + *itdom);
    }
    imout.setValue(*it, val);
  }
}


template<typename Image,typename Domain>
void erode(Image &imout, const Image &im, const Domain &dom)
{
  for(typename Image::Domain::ConstIterator it = im.domain().begin(),
      itend=im.domain().end(); it != itend; ++it)
  {
    typename Image::Value val=im(*it);
    for(typename Domain::ConstIterator itdom=dom.begin(), itenddom=dom.end(); itdom!=itenddom; ++itdom )
    {
      if (im.domain().isInside(*it+ *itdom))
        if (val > im(*it + *itdom)) val = im(*it + *itdom);
    }
    imout.setValue(*it, val);
  }
}



template<typename Image,typename Domain>
void opening(Image &imout, const Image &im, const Domain &dom)
{
  Image imouttemp(imout);
  erode(imouttemp,im,dom);
  dilation(imout, imouttemp,dom);
}


template<typename Image,typename Domain>
void closing(Image &imout, const Image &im, const Domain &dom)
{
  Image imouttemp(imout);
  dilation(imouttemp, im,dom);
  erode(imout,imouttemp,dom);
}


template<typename Image,typename Domain>
void Grad(Image &imout, const Image &im, const Domain &dom)
{
  Image ero(imout);
  Image dil(imout);
  erode(ero,im,dom);
  dilation(dil, im,dom);
  for(typename Image::Domain::ConstIterator it = im.domain().begin(),
      itend=im.domain().end(); it != itend; ++it)
    imout.setValue(*it, dil(*it) - ero(*it));
    
}



template<typename Image,typename Domain>
void Lap(Image &imout, const Image &im, const Domain &dom)
{
  Image ero(imout);
  Image dil(imout);
  erode(ero,im,dom);
  dilation(dil, im,dom);
  for(typename Image::Domain::ConstIterator it = im.domain().begin(),
      itend=im.domain().end(); it != itend; ++it)
  {
    imout.setValue(*it, static_cast<unsigned char>(128+0.5*(int)dil(*it) + (int)ero(*it) -2*(int)im(*it)));
  }
}





int main(int argc, char**argv)
{
  string filename=  argv[1];
  typedef ImageContainerBySTLVector<Z2i::Domain, unsigned char> Image;
  Image image= PGMReader<Image>::importPGM(filename);
  trace.info() << image << endl;
  Image imagebis(image);
  
  Z2i::Domain square( Z2i::Point().diagonal(-5), Z2i::Point().diagonal(5));
  trace.info()<<square<<std::endl;
  
  dilation(imagebis,image,square);
  PGMWriter<Image>::exportPGM("dilation.pgm",imagebis);
  
  erode(imagebis,image,square);
  PGMWriter<Image>::exportPGM("erode.pgm",imagebis);
  
  opening(imagebis,image,square);
  PGMWriter<Image>::exportPGM("opening.pgm",imagebis);
  
  closing(imagebis,image,square);
  PGMWriter<Image>::exportPGM("closing.pgm",imagebis);
  
  Grad(imagebis,image,square);
  PGMWriter<Image>::exportPGM("gradient_m.pgm",imagebis);
  
  
  Lap(imagebis,image,square);
  PGMWriter<Image>::exportPGM("laplacian_m.pgm",imagebis);
  
  
	return 0;
}
