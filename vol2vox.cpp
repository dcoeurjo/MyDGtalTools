/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

/**
 * @file voAddBorder.cpp
 * @author David Coeurjolly (\c david.coeurjolly@liris.cnrs.fr )
 * Laboratoire d'InfoRmatique en Image et Systèmes d'information - LIRIS (CNRS, UMR 5205), CNRS, France
 *
 * @date 2012/05/01
 *
 *
 * This file is part of the DGtal library.
 */

#include <iostream>
#include <DGtal/base/Common.h>
#include <DGtal/io/readers/VolReader.h>
#include <DGtal/helpers/StdDefs.h>
#include <DGtal/images/Image.h>
#include <DGtal/images/ImageContainerBySTLVector.h>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>

using namespace std;
using namespace DGtal;
using namespace Z3i;

namespace po = boost::program_options;


/**
 * Missing parameter error message.
 *
 * @param param
 */
void missingParam ( std::string param )
{
  trace.error() <<" Parameter: "<<param<<" is required..";
  trace.info() <<std::endl;
  exit ( 1 );
}

template <typename Word>
static
std::ostream& write_word( std::ostream& outs, Word value )
{
  for (unsigned size = sizeof( Word ); size; --size, value >>= 8)
    outs.put( static_cast <char> (value & 0xFF) );
  return outs;
}


int main(int argc, char**argv)
{

  // parse command line ----------------------------------------------
  po::options_description general_opt ( "Allowed options are: " );
  general_opt.add_options()
    ( "help,h", "display this message." )
    ( "input,i", po::value<std::string>(), "Input vol file." )
    ( "output,o", po::value<string>(),"Output filename." );
  bool parseOK=true;
  po::variables_map vm;
  try{
    po::store(po::parse_command_line(argc, argv, general_opt), vm);
  }catch(const std::exception& ex){
    parseOK=false;
    trace.info()<< "Error checking program options: "<< ex.what()<< endl;
  }
  po::notify ( vm );
  if ( !parseOK || vm.count ( "help" ) ||argc<=1 )
    {
      trace.info() << "Fill the shape by filling-out the exterior (+complement)"<<std::endl
                   << std::endl << "Basic usage: "<<std::endl
                   << "\tvolFillExterior --input <volFileName> --o <volOutputFileName> "<<std::endl
                   << general_opt << "\n";
      return 0;
    }

  //Parse options
  if ( ! ( vm.count ( "input" ) ) ) missingParam ( "--input" );
  std::string filename = vm["input"].as<std::string>();
  if ( ! ( vm.count ( "output" ) ) ) missingParam ( "--output" );
  std::string outputFileName = vm["output"].as<std::string>();

  typedef ImageContainerBySTLVector<Z3i::Domain, unsigned char>  MyImageC;

  trace.beginBlock("Loading..");
  MyImageC  imageL = VolReader< MyImageC >::importVol ( filename );
  trace.endBlock();

  trace.beginBlock("Exporting...");
  ofstream myfile;
  myfile.open (outputFileName, ios::out | ios::binary);
  
  Point size= imageL.domain().upperBound() - imageL.domain().lowerBound();
  DGtal::uint32_t cpt=0;
  for(auto it = imageL.range().begin(), itend = imageL.range().end();
      it!=itend; ++it)
    if (*it != 0)
      cpt++;
  
  
  myfile <<'V'<<'O'<<'X'<<' ';
  write_word(myfile, (DGtal::uint32_t)150);
  myfile <<'M'<<'A'<<'I'<<'N';
  //chunkid ?
  write_word(myfile,DGtal::uint32_t(0));
  write_word(myfile,DGtal::uint32_t(4+4+4+cpt));
  
  myfile <<'S'<<'I'<<'Z'<<'E';
  write_word(myfile,DGtal::uint32_t(size[0]+1));
  write_word(myfile,DGtal::uint32_t(size[1]+1));
  write_word(myfile,DGtal::uint32_t(size[2]+1));
  myfile << 'X'<<'Y'<<'Z'<<'I';
  write_word(myfile, cpt);
  
  for(auto it = imageL.domain().begin(), itend = imageL.domain().end();
      it!=itend; ++it)
    if (imageL(*it) != 0)
    {
      Point p = (*it) - imageL.domain().upperBound();
      myfile.put((DGtal::uint8_t)(p)[0]);
      myfile.put( (DGtal::uint8_t)(p)[1]);
      myfile.put( (DGtal::uint8_t)(p)[2]);
    }
  
  
  trace.endBlock();
  
  return 0;
}
