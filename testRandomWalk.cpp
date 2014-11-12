#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>
#include <cmath>
#include <stdlib.h>

using namespace DGtal;
using namespace Z2i;

int main(int argc, char **argv)
{
  
  RealPoint p(0.0,0.0);
  Board2D board;

  unsigned int nbPts;

  if (argc == 2)
    nbPts = atoi(argv[1]);
  else
    nbPts = 100;
  
  board.drawDot(p[0],p[1]);
  for(unsigned int i = 0 ; i < nbPts ; ++i)
    {
      double theta = (double)rand()*2.0*M_PI/(double)RAND_MAX;
      
      p += RealPoint( cos(theta), sin(theta));
      board.drawDot(p[0],p[1]);
    }

  board.drawCircle(0.0,1.0,1.0);
  board.saveEPS("samples.eps");
}
