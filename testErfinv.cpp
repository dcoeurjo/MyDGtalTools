#include <iostream>
#include <random>


// Inverse error function
float ErfInv(float x)
{
  float w, p;
  w = -log((1.0f - x) * (1.0f + x));
  if (w < 5.000000f)
  {
    w = w - 2.500000f;
    p = 2.81022636e-08f;
    p = 3.43273939e-07f + p * w;
    p = -3.5233877e-06f + p * w;
    p = -4.39150654e-06f + p * w;
    p = 0.00021858087f + p * w;
    p = -0.00125372503f + p * w;
    p = -0.00417768164f + p * w;
    p = 0.246640727f + p * w;
    p = 1.50140941f + p * w;
  }
  else
  {
    w = sqrt(w) - 3.000000f;
    p = -0.000200214257f;
    p = 0.000100950558f + p * w;
    p = 0.00134934322f + p * w;
    p = -0.00367342844f + p * w;
    p = 0.00573950773f + p * w;
    p = -0.0076224613f + p * w;
    p = 0.00943887047f + p * w;
    p = 1.00167406f + p * w;
    p = 2.83297682f + p * w;
  }
  return p * x;
}

int main()
{
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.,1.0);
  std::normal_distribution<float> Ndistribution(0.5,1.0/6.0);
  auto N=1000000.0;
  double mean=0.0;
  double smean = 0.0;
  for(auto i=0;i<N;++i)
  {
    auto v = 0.5 + 1.0/6.0*sqrt(2.0)*ErfInv(2*distribution(generator)- 1.0);
    smean += v*v;
    mean +=v;
  }
  mean /=N;
  smean /=N;
  std::cout<<"Mean = "<< mean<<"  var= "<<( smean - mean*mean ) <<std::endl;
  
  mean=0.0;
  smean=0.0;
  for(auto i=0;i<N;++i)
  {
    auto v = Ndistribution(generator);
    smean += v*v;
    mean +=v;
  }
  mean /=N;
  smean /=N;
  std::cout<<"Mean = "<< mean<<"  var= "<< smean - mean*mean  <<std::endl;
}
