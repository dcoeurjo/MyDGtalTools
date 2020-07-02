#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/io/boards/Board2D.h>

#include "DGtal/geometry/curves/ArithmeticalDSS.h"
#include <random>
#include <set>
#include <utility>
using namespace DGtal;
using namespace Z2i;

int main(int argc, char ** argv)
{
  Board2D board;
  std::default_random_engine generator;
  std::uniform_real_distribution<double> distribution(0.0,1.0);
  
  using Edge= std::pair<int,int> ;
  std::set<Edge> edges;
  
  std::ifstream in(argv[1], std::ios::in);
  if (!in)
  {
    std::cerr << "Cannot open " << argv[1] << std::endl;
    exit(1);
  }
  std::string line;
  double x,y,z;
  int a,b,c; //to store mesh index
  
  std::vector<std::vector<double>> vertices;
  
  while (std::getline(in, line))
  {
    //check v for vertices
    if (line.substr(0,2)=="v ")
    {
      std::istringstream v(line.substr(2));
      v>>x;v>>y;v>>z;
      if (y!= 0.0)
      {std::cout<<"ERROR"<<std::endl; exit(42);}
     
      vertices.push_back({x,z});
    }
    else if(line.substr(0,2)=="f ")
    {
      const char* chh=line.c_str();
      sscanf (chh, "f %i %i %i",&a,&b,&c); //here it read the line start with f and store the corresponding values in the variables
      a--;b--;c--;
    
      std::pair<int,int> ea(a,b), eb(b,c), ec(a,c);
      
    //  if (edges.find(std::pair<int,int>(b,a)) != edges.end())
        edges.insert(ea);
    //  if (edges.find(std::pair<int,int>(c,b)) != edges.end())
        edges.insert(eb);
    //  if (edges.find(std::pair<int,int>(c,a)) != edges.end())
        edges.insert(ec);
              
    }
  }
  
  for(auto edge: edges)
      board.drawLine(vertices[edge.first][0], vertices[edge.first][1], vertices[edge.second][0], vertices[edge.second][1]);
  
  std::cout<<edges.size()<<" Edges."<<std::endl;
  
  board.saveSVG("mesh.svg");
  
  
}
