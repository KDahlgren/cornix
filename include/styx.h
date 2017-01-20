/*
 * filename   :    styx.h 
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 * Description:
 *     Definitions of Cornix 
 *     Create, Read, Update, and Delete ops.
 */

/*
 *
 * FUTURE WORK:
 *
 * Verify data within a concept.
 *
 * { $country: canada, $city: ontario } passes.
 *
 * Need another loop to check if ontario is actually a city.
 *
 */

#ifndef STYX_H
#define STYX_H

#include "cornix_includes.h"
#include "cornix_utils.h"

// STYX
// input a row of data to insert into db
// output bool T=no ontology violation,
//             F=yes ontology violation
bool styx( std::string row)
{
  bool flag = true ;

  // Get list of stuff to insert
  std::vector<std::string> dataList = getArgs( row ) ;

  // Get ontology from json file.
  std::string ontoPath = "./ontology.json" ;
  //std::string ontoPath = "./ontology_empty.json" ;
  
  // Get ontology
  // Convert ontology into std::map
  // Each concept corresponds to a key.
  // Each concept subsumes a set of concepts.
  std::map< std::string, std::vector<std::string> > ontoMap = getOntoMap ( ontoPath ) ;

  // Iterate over tuples in dataList
  for(unsigned int i = 0 ; i < dataList.size() ; i++)
  {
    // *** TO DO:   TEST getMAP *** //
    // Test on populated json file
    // Test on empty json file

    std::string currData = dataList[i] ; 
    // Grab field for the tuple
    std::string tField = getID( currData ) ;
    // Grab value for the tuple
    std::string tVal = getValue( currData ) ;
    // Grab the map value for the field in the ontology map, tFieldMap
    std::vector<std::string> tFieldMap = getMap( tField, ontoMap ) ;
    // Grab the map value for the value in the ontology map, tValueMap
    std::vector<std::string> tValMap = getMap( tVal, ontoMap ) ;

    // NOTE: Each data inserted into the db is a concept in the ontology.

    /*
    for(unsigned int k = 0 ; k < dataList.size() ; k++)
      std::cout << "dataList[k] = " << dataList[k] << std::endl ;
    */

    // Iterate over the tuples in dataList
    for(unsigned int j = 0 ; j < dataList.size() ; j++)
    {
      std::string nextData = dataList[j] ;
      // Grab the field, kField
      std::string kField = getID( nextData ) ;
      // Grab the value, kVal
      std::string kVal = getValue( nextData ) ;

      /*
      std::cout << "*** nextData: " << nextData << std::endl ;
      std::cout << "*** kField  : " << kField << std::endl ;
      std::cout << "*** kVal    : " << kVal << std::endl ;
      std::cout << "*** tField  : " << tField << std::endl ;
      std::cout << "*** tVal    : " << tVal << std::endl ;
      */

      // if kField in tFieldMap:
      if( inMap(kField, tFieldMap) )
      {
        /*
        std::cout << "kField is in tFieldMap: " << kField << std::endl ;  
        for(unsigned int k = 0 ; k < tFieldMap.size() ; k++)
          std::cout << "tFieldMap[k] = " << tFieldMap[k] << std::endl ;

        std::cout << "kVal is in tValMap: " << kVal << std::endl ;
        for(unsigned int k = 0 ; k < tValMap.size() ; k++)
          std::cout << "tValMap[k] = " << tValMap[k] << std::endl ;
        */

        // if kVal not in tValMap:
        if( !inMap(kVal, tValMap) )
        {
          /*
          std::cout << "kVal: " << kVal << std::endl ;
          for(unsigned int k = 0 ; k < tValMap.size() ; k++)
            std::cout << "tValMap[k] = " << tValMap[k] << std::endl ;
          */
          // Error! Attempted update violates ontology
          std::cout << "** Error! Attempted update violates ontology" << std::endl ;
          std::cout << "** " << tVal << " does not subsume " << kVal << std::endl ;
          return false ;
        }
      }
    }
  }
  return flag ;
}

void message()
{
  std::cout << "Hi, I'm Styx!" << std::endl ;
  std::cout << "You're awesome! =D" << std::endl ;

  return ;
}

#endif
