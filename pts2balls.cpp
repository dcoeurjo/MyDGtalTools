#include <iostream>
#include <fstream>
#include <vector>

#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/viewers/Viewer3D.h>
#include <DGtal/io/boards/Board3D.h>

using namespace DGtal;

int main(int argc, char**argv)
{
  
  //Loading point set  argv[1]==filename argv[2]==radius of small spheres argv[3]=output
  ifstream myfile (argv[1], std::ifstream::in);
  double smallrad = atof(argv[2]);
  //QApplication app(argc,argv);
  // Viewer3D<> viewer;
  Board3D<> board;
  double x,y,z;
  // viewer.show();
  // viewer.addBall(Z3i::RealPoint(0,0,0), 1.0);
  // viewer << CustomColors3D(Color::Red, Color::Red);

  //removing the unit ball in OBJ export
  //board.addBall(Z3i::RealPoint(0,0,0), 1.0, 50);
  board << CustomColors3D(Color(255,150,12),Color(255,150,12));
  while (myfile.good())
  {
    myfile>> x;
    myfile >>y;
    myfile>> z;
    // viewer.addBall(Z3i::RealPoint(x,y,z), smallrad);
    board.addBall(Z3i::RealPoint(x,y,z), smallrad,6);
  }
  
  board.saveOBJ(argv[3]);
  myfile.close();
  return 0;
  // return app.exec();
}

