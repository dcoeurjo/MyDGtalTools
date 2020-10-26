#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include "DGtal/geometry/curves/ArithmeticalDSS.h"
#include <random>
using namespace DGtal;
using namespace Z2i;



int main()
{
  auto seed= 12345;
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<double> distribution(-1.0,1.0);
  std::uniform_int_distribution<int> intdist(0,100);

  auto myrandom = [&](int i){ return intdist(generator) % i;};
  
  Board2D board;
  
  auto addline= [&](const double x, const double y,
                    const double xx, const double yy,const double eps=0.3) {
    board.drawLine(x+distribution(generator)*eps, y + distribution(generator)*eps,
                  xx + distribution(generator)*eps, yy + distribution(generator)*eps);};
  
  auto addcross = [&](const double x, const double y, const double radius){
    board.setPenColorRGBf(0.0, 0.0, 1.0,0.5);
    addline(x-radius,y-radius,x+radius,y+radius, radius/3.0);
    addline(x-radius,y+radius,x+radius,y-radius, radius/3.0) ;
  };
  
  auto nbStep = 20;
  auto addcircle = [&](const double x, const double y, const double radius){
    board.setPenColorRGBf(1.0, 0.0, 0.0,0.5);
    double rx= radius + distribution(generator)*radius*0.2;
    double ry= radius + distribution(generator)*radius*0.2;
    double phi = distribution(generator)*M_PI/2.0;
    double xp,yp,xpp,ypp;
    double theta=phi;
    double dec = 2.0*M_PI/(double)(nbStep);
    xp = x + (rx + radius*std::abs(distribution(generator))*0.6)*cos(theta) ;
    yp = y + (ry + radius*std::abs(distribution(generator))*0.6)*sin(theta);
    
    for(auto i=0 ; i < nbStep; ++i)
    {
      theta +=dec;
      xpp = x + rx*cos(theta);
      ypp = y + ry*sin(theta);
      board.drawLine(xp, yp, xpp, ypp);
      xp = xpp;
      yp = ypp;
    }
    theta+=dec;
    xpp = x + (rx + radius*std::abs(distribution(generator))*0.6)*cos(theta) ;
    ypp = y + (ry + radius*std::abs(distribution(generator))*0.6)*sin(theta);
    board.drawLine(xp,yp,xpp,ypp);
  };
  
  
  auto addStrikeVert = [&](const double x, const double y, const double size){
    addline(x, y, x, y+size,size*0.05);
  };
  auto addStrikeHoriz = [&](const double x, const double y, const double size){
    addline(x, y, x+size, y,size*0.05);
  };
  auto addStrikeDiagA = [&](const double x, const double y, const double size){
    addline(x, y, x+size, y+size,size*0.05);
  };
  auto addStrikeDiagB = [&](const double x, const double y, const double size){
    addline(x, y+size, x+size, y,size*0.05);
  };
  
  enum WIN { cross, circle, tie};
  
  auto addGame = [&](const double x, const double y, const double size)
  {
    std::vector<int> choices(9);
    // set some values:
    for (int i=0; i<9; ++i) choices[i]=i;
    std::shuffle ( choices.begin(), choices.end() , generator);
   
    
    
    auto dec = size / 3.0;
    bool finish=false;
    auto cpt=0;
    bool cross= (intdist(generator) % 2) ==0 ;
    
    board.setPenColorRGBf(0, 0, 0);
    addline(x+dec/2.0,y-dec/2.0,x+dec/2.0,y+size-dec/2.0,size*0.05);
    addline(x+3.0/2.0*dec,y-dec/2.0,x+3.0/2.0*dec,y+size-dec/2.0,size*0.05);
    addline(x-dec/2.0,y+.5*dec,x+size-dec/2.0, y+.5*dec,size*0.05);
    addline(x-dec/2.0,y+1.5*dec,x+size-dec/2.0, y+1.5*dec,size*0.05);
    
    std::array<unsigned char, 3> crossI({0,0,0}),crossJ({0,0,0});
    unsigned char crossDiagA=0, crossDiagB=0;
    unsigned char circleDiagA=0, circleDiagB=0;
    std::array<unsigned char, 3> circleI({0,0,0}),circleJ({0,0,0});
    bool playagain=true;
    bool crosswin;
    while ((playagain) && (cpt<9))
    {
      int i = choices[cpt] % 3;
      int j = choices[cpt] / 3;
      if (cross)
      {
        addcross(x+i*dec, y+j*dec, dec/4.);
        crossI[i]++;
        if (crossI[i]>=3) {addStrikeVert(x+i*dec, y, size-dec*0.7); return WIN::cross;}
        crossJ[j]++;
        if (crossJ[j]>=3) {addStrikeHoriz(x, y+j*dec, size-dec*0.7);return WIN::cross;}
        if (i==j) crossDiagA++;
        if (crossDiagA>=3) {addStrikeDiagA(x-dec*0.25, y-dec*0.25, size-dec*0.5); return WIN::cross;}
        if (i == 2-j) crossDiagB++;
        if (crossDiagB>=3) {addStrikeDiagB(x, y-dec*0.25, size-dec*0.5); return WIN::cross;}
        cross = !cross;
      }
      else
      {
        addcircle(x+i*dec, y+j*dec, dec/4.);
        circleI[i]++;
        if (circleI[i]>=3) {addStrikeVert(x+i*dec, y, size-dec*0.7); return WIN::circle;}
        circleJ[j]++;
        if (circleJ[j]>=3) {addStrikeHoriz(x, y+j*dec, size-dec*0.7); return WIN::circle;}
        if (i==j) circleDiagA++;
        if (circleDiagA>=3) {addStrikeDiagA(x-dec*0.25, y-dec*0.25, size-dec*0.5);return WIN::circle;}
        if (i == 2-j) circleDiagB++;
        if (circleDiagB>=3) {addStrikeDiagB(x, y-dec*0.25, size-dec*0.5); return WIN::circle;}
        cross = !cross;
      }
      ++cpt;
    }
    return WIN::tie;
  };
  
  board.setUnit ( LibBoard::Board::UCentimeter );
  double width = 29.7;
  double height = 21;
  std::array<unsigned int, 3> scores({0,0,0});
  double startx=1.0;
  double starty=1.0;
  double size = 0.5; //1.2
  double decx = size + 0.1;
  int gridx=20/decx;
  int gridy=22/decx;
  for(auto ii=0 ; ii < gridx; ++ii)
   for(auto jj=0 ; jj < gridy; ++jj)
     scores[addGame(startx + ii*decx,starty + jj*decx, size)]++;

  for(auto v: scores)
    std::cout<<v<<" ";
  std::cout<<std::endl;
  
  double x=1;
  double y= 24;
  decx = 8.0/(double)std::max(scores[0],scores[1]);
  board.setPenColorRGBf(0, 0, 1.0);
  for(auto ii=1; ii <= scores[0]; ++ii)
  {
    if (ii % 5 == 0)
    { addline(x, y, x - 4*decx, y+1.0, 0.05);  x+=decx;}
    else
      addline(x, y, x, y+1.0, 0.05);
    x += decx;
  }
  x+=1;
  board.setPenColorRGBf(1.0, 0, 0);
  for(auto ii=1; ii < scores[1]+1; ++ii)
  {
    if (ii % 5 == 0)
    { addline(x, y, x - 4*decx, y+1.0, 0.05);  x+=decx;}
    else
      addline(x, y, x, y+1.0, 0.05);
    x += decx;
  }
  
  x= 10.0 - (double)scores[2]*decx/2.0;
  y=25.5;
  board.setPenColorRGBf(0, 0, 0);
  for(auto ii=1; ii <= scores[2]; ++ii)
  {
    if (ii % 5 == 0)
    { addline(x, y, x - 4*decx, y+1.0, 0.05);  x+=decx;}
    else
      addline(x, y, x, y+1.0, 0.05);
    x += decx;
  }
  
  board.saveSVG("tictactoe.svg");
  return 0;
}
