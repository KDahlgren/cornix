
#include <algorithm>
#include <assert.h>

#include <cstdio>
#include <cstdlib>
#include <ctime>
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
#include <stdlib.h>
#include <string>

#include <time.h>

#include <vector>


int main( int argc, char** argv )
{
  srand(time(NULL)) ;
  int idNum = 1 ;
  int testNum = 10000 ;
  int numIts = 1 ;
  int test = 10 ;

  // Generate timestamp
  //std::string myTime = currentDateTime() ;
  // Generate file name
  std::string filename = "readRandom_" + std::to_string(numIts) + "_" + std::to_string(testNum) + "_" + std::to_string(test) +  ".txt" ;
  
  // Write contents of map to file in ../data/
  std::string pathName = "./" + filename ;
  std::ofstream dataFile ;
  dataFile.open( pathName.c_str() ) ;
  if( dataFile.is_open() )
  {
    for(int i = 0 ; i < numIts ; i++)
    {
      int id = rand() % testNum + 1 ;
      std::string line = "{ (name, age, country, state, city) : ($id _eq " + std::to_string(id) + ") }" ;

      dataFile << line <<std::endl ;
    }
  }
  dataFile.close();

  return 0 ;
}
