#include <DGtal/base/Common.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/topology/KhalimskySpaceND.h>



using namespace DGtal;
int main()
{
  //2D
  Z2i::KSpace K2;
  K2.init(  Z2i::Point(-4,-4),  Z2i::Point(4,4),true);
  Z2i::KSpace::SCell cell2= K2.sCell(Z2i::Point(1,1));
  trace.info() << cell2<<std::endl;
  
  Z2i::KSpace::SCells border2 = K2.sLowerIncident(cell2);
  for(auto it=border2.begin(), itend = border2.end(); it!=itend; ++it)
    trace.info() << "   "<< *it<<std::endl;

  
  //3D
  trace.info()<<std::endl;
  Z3i::KSpace K;
  K.init(  Z3i::Point(-4,-4,-4),  Z3i::Point(4,4,4),true);
  Z3i::KSpace::SCell cell3= K.sCell(Z3i::Point(1,1,1));
  trace.info() << cell3<<std::endl;
  
  Z3i::KSpace::SCells border = K.sLowerIncident(cell3);
  for(auto it=border.begin(), itend = border.end(); it!=itend; ++it)
    trace.info() << "   "<< *it<<std::endl;
  trace.info()<<std::endl;
  Z3i::KSpace::SCell surf = K.sCell(Z3i::KSpace::Point(1,0,1), Z3i::KSpace::NEG );
  Z3i::KSpace::SCells borders = K.sUpperIncident(surf);
  for(auto it=borders.begin(), itend = borders.end(); it!=itend; ++it)
    trace.info() << "   "<< *it<<std::endl;
  
  
  //4D
  trace.info()<<std::endl;
  KhalimskySpaceND<4> K4;
  K4.init(  KhalimskySpaceND<4>::Point(-4,-4,-4,-4),  KhalimskySpaceND<4>::Point(4,4,4,4),true);
  KhalimskySpaceND<4>::SCell cell4= K4.sCell(KhalimskySpaceND<4>::Point(1,1,1,1));
  trace.info() << cell4<<std::endl;
  
  KhalimskySpaceND<4>::SCells border4 = K4.sLowerIncident(cell4);
  for(auto it=border4.begin(), itend = border4.end(); it!=itend; ++it)
    trace.info() << "   "<< *it<<std::endl;
  

  
  return 0;
}