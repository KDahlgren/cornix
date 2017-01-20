
#include <algorithm>
#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <ctype.h>

#include <dirent.h>

#include <fstream>

#include <iostream>
#include <iterator>

#include <map>

#include <numeric>

#include <set>
#include <sstream>
#include <stdio.h>
#include <string>

#include <time.h>

#include <vector>


int main( int argc, char** argv )
{
  int idNum = 1 ;
  int numData = 10000 ;

  // Generate timestamp
  //std::string myTime = currentDateTime() ;
  // Generate file name
  std::string filename = "dataFile_" + std::to_string(numData) + ".txt" ;
  
  // Write contents of map to file in ../data/
  std::string pathName = "./" + filename ;
  std::ofstream dataFile ;
  dataFile.open( pathName.c_str() ) ;
  if( dataFile.is_open() )
  {
    for(unsigned int i = 0 ; i < numData ; i++)
    {
      // Convert idNum to string
      std::string id = std::to_string( idNum ) ;

    std::string line = "{ $id: " + id + ", $name: Kay, $age: 23, $country: us, $state: ca, $city: santacruz }" ;

      dataFile << line <<std::endl ;
      idNum++ ;
    }
  }
  dataFile.close();

  return 0 ;
}
