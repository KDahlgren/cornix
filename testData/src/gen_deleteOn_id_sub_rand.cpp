
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
  int testNum = 10000 ;              // test 10^1, 10^2, 10^3, 10^4
  int numIts = testNum * 0.10 ;               // number of updates in test 
  int test = 10 ;                  // trial number 1,2,3,4,5,6,7,8,9,10

  // Generate timestamp
  //std::string myTime = currentDateTime() ;
  // Generate file name
  std::string filename = "deleteOn_id_sub_" + std::to_string(testNum) + "_" + std::to_string(test) + ".txt" ;
  
  // Write contents of map to file in ../data/
  std::string pathName = "./" + filename ;
  std::ofstream dataFile ;
  dataFile.open( pathName.c_str() ) ;
  if( dataFile.is_open() )
  {
    //std::string line = "{ (name, age, country, state, city) : " ;  //READ TESTS
    for(int i = 0 ; i < numIts ; i++)
    {
      std::string line = "" ;
      // get random id
      int id = rand() % testNum + 1 ;
      line += std::to_string(id) ;
      dataFile << line <<std::endl ;
    }
  }
  dataFile.close();

  return 0 ;
}
