/*
 * filename   :    cornix_utils.h
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 */

#ifndef CORNIX_UTILS_H
#define CORNIX_UTILS_H

#include "cornix_includes.h"
#include "cornix_parser.h"
#include "cornix_time.h"
#include "cornix_agg.h"
#include "cornix_ops.h"
#include "cornix_booleans.h"

// getData_noAgg
void getData_noAgg(rocksdb::DB* db, rocksdb::Status status, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  std::map<std::string, std::string> resultsMap ;

  // Iterate over keys
  for(unsigned int i = 0 ; i < keyList.size() ; i++)
  {
    // Get field from keyList
    std::string key = keyList[i] ;
    // Get value at key
    std::string value = read_data(db, status, key) ;
    // Get tuple List from value
    std::vector<std::string> tupleList = parseList(value) ;

    // Get valid values
    for(unsigned int j = 0 ; j < tupleList.size() ; j++)
    {
      // Get current tuple
      std::string currtup = tupleList[j] ;

      // Get tuple id
      std::string tupleID = getID( tupleList[j] ) ;
      // Get tuple value
      std::string tupleVal = getValue( tupleList[j] ) ;
      // Check if tuple id in idlist
      bool check = std::find(idList.begin(), idList.end(), tupleID) != idList.end() ;
      if( check == true )
      {
        // Check if the map already contains an entry for tupleID
        if( resultsMap.find(tupleID) != resultsMap.end())
        {
          // Get curr value of tupleID from map
          std::string mapValue = resultsMap[tupleID] ;
          // Concatenate new value to old value
          // Result will be comma-separated values
          std::string newValue = mapValue + ", " + key + ": " + tupleVal ;
          // Add modified back into map
          resultsMap[tupleID] = newValue ;
        }
        else
        {
          std::string entry = "id: " + tupleID + ", " + key + ": " + tupleVal ;
          resultsMap.insert( std::make_pair(tupleID, entry) );
        }
      }
    }
  }

  // Generate timestamp
  std::string myTime = currentDateTime() ;
  // Generate file name
  std::string filename = "readData_" + myTime + ".txt" ;

  // Create new data/ dir in ../ if it does not exist
  // http://www.dreamincode.net/forums/topic/59943-accessing-directories-in-cc-part-i/
  DIR* pdir = NULL ;
  pdir = opendir( "../data/" ) ; 
  if( pdir == NULL )
  {
    std::cout << "Creating data dir at ../data/" << std::endl ;
    const int err = system( "mkdir ../data/" );
    if( err == (-1) )
    {
      std::cout << "Error: Could not create data/ directory" << std::endl ;
      exit(1) ;
    }
  }
  closedir( pdir ) ;

  // Write contents of map to file in ../data/
  std::string pathName = "../data/" + filename ;
  std::ofstream dataFile ;
  dataFile.open( pathName, std::ios::app ) ;
  if( dataFile.is_open() )
  {
    // Iterate through all elements in std::map
    std::map<std::string, std::string>::iterator it = resultsMap.begin();
    while(it != resultsMap.end())
    {
      //dataFile << it->first << ": " << it->second <<std::endl ;
      dataFile << it->second <<std::endl ;
      it++ ;
    }
  }
  dataFile.close();

  return ;
}

// getData_Agg
void getData_Agg(rocksdb::DB* db, rocksdb::Status status, std::string agg, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  double aggResult = 0.0 ;

  // Eval on Agg op
  if( agg == "avg")
    aggResult = avg(db, status, keyList, idList) ;
  else if( agg == "min") 
    aggResult = min(db, status, keyList, idList) ;
  else if( agg == "max")
    aggResult = max(db, status, keyList, idList) ;
  else if( agg == "sum")
    aggResult = sum(db, status, keyList, idList) ;
  else if( agg == "count")
    aggResult = count(db, status, keyList, idList) ;
 
  // Generate timestamp
  std::string myTime = currentDateTime() ;
  // Generate file name
  std::string filename = "readData_" + myTime + ".txt" ;
  
  // Create new data/ dir in ../ if it does not exist
  // http://www.dreamincode.net/forums/topic/59943-accessing-directories-in-cc-part-i/
  DIR* pdir = NULL ;
  pdir = opendir( "../data/" ) ;
  if( pdir == NULL )
  {
    std::cout << "Creating data dir at ../data/" << std::endl ;
    const int err = system( "mkdir ../data/" );
    if( err == (-1) )
    {
      std::cout << "Error: Could not create data/ directory" << std::endl ;
      exit(1) ;
    }
  }
  closedir( pdir ) ;

  std::cout << "aggResult = " << aggResult << std::endl ;
  
  // Write contents of map to file in ../data/
  std::string pathName = "../data/" + filename ;
  std::ofstream dataFile ;
  dataFile.open( pathName, std::ios::app ) ;
  if( dataFile.is_open() )
  {
    dataFile << aggResult <<std::endl ;
  }
  dataFile.close();
  
  return ;
}

// getData
void getData(rocksdb::DB* db, rocksdb::Status status, std::string agg, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  if( agg != "" )
    getData_Agg( db, status, agg, keyList, idList ) ;
  else
    getData_noAgg( db, status, keyList, idList ) ;

  return ;
}

// getKeyList
std::vector<std::string> getKeyList(std::string query)
{
  std::vector<std::string> parsedQuery = parseRead(query) ;
  std::vector<std::string> keyList = getFieldList(parsedQuery[0]) ;
  return keyList ;
}

// getIDList
std::vector<std::string> getIDList(rocksdb::DB* db, rocksdb::Status status, std::string query)
{
  std::vector<std::string> idList ;
  std::vector<std::string> parsedQuery = parseRead(query) ;
  std::vector<std::string> predList ;

  // Get list of predicates
  for(unsigned int i = 1 ; i < parsedQuery.size() ; i++)
    predList.push_back( parsedQuery[i] ) ;

  if( predList.size() != 0)
  {
    // DEBUG
    for(unsigned int i = 0 ; i < predList.size() ; i++)
    {
      std::cout << "*** predList[i] = " << predList[i] << std::endl ;
    }

    // Iterate over predicates
    for(unsigned int i = 0 ; i < predList.size() ; i++)
    {
      std::string currPred = predList[i] ;
      std::string lhs = getLHS( currPred ) ;
      std::string op = getOp( currPred ) ;
      std::string rhs = getRHS( currPred ) ;

      std::vector<std::string> list ;    

      // Eval on op
      if( op == "eq")
        list = eq(db, status, lhs, rhs) ;
      else if( op == "ne")
        list = ne(db, status, lhs, rhs) ;
      else if( op == "gt")
        list = gt(db, status, lhs, rhs) ;
      else if( op == "gte")
        list = gte(db, status, lhs, rhs) ;
      else if( op == "lt")
        list = lt(db, status, lhs, rhs) ;
      else if( op == "lte")
        list = lte(db, status, lhs, rhs) ;

      for(unsigned int k = 0 ; k < list.size() ; k++)
        std::cout << "list[k] = " << list[k] << std::endl ;

      // Append new list of valid ids to idList
      idList.insert( idList.end(), list.begin(), list.end() ) ;

      for(unsigned int k = 0 ; k < idList.size() ; k++)
        std::cout << "idList[k] = " << idList[k] << std::endl ;
    }

    // AND
    //int numPreds = predList.size() ;
    //idList = AND( idList, numPreds ) ;

    // OR
    idList = OR( idList ) ;

  }
  else // no predicates
  {
    // Get all ids in all key fields
    // for each key:
    //   Get value
    //   Convert value into vector of tuplesi
    //   for each tuple:
    //     Get id
    //     Place id in idSet

    std::set<std::string> idSet ;
    std::vector<std::string> keyList = getKeyList( query ) ;

    for(unsigned int i = 0 ; i < keyList.size() ; i++)
    {
      std::string currKey = keyList[i] ;
      // Get value at key
      std::string value = read_data(db, status, currKey) ;
      // Get tuple List from value
      std::vector<std::string> tupleList = parseList(value) ;
      for(unsigned int i = 0 ; i < tupleList.size() ; i++)
      {
        std::string tupleID = getID( tupleList[i] ) ;
        idSet.insert( tupleID ) ;
      }
    }

    std::vector<std::string> listVector( idSet.begin(), idSet.end() );
    idList = listVector ;
  }

  // DEBUG PRINT
  std::cout << "Valid ids for: " << query << std::endl ;
  for(unsigned int i = 0 ; i < idList.size() ; i++)
  {
    std::cout << "i =" << i << ": " << idList[i] << std::endl ;
  }
  std::cout << "     ... exitting getIDList" << std::endl ;

  return idList ;
}

// http://stackoverflow.com/questions/440133/how-do-i-create-a-random-alpha-numeric-string-in-c
std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}

// getAllKeys
// input db and status
// output vector of keys - all keys in entire db
std::vector<std::string> getAllKeys ( rocksdb::DB* db, rocksdb::Status status )
{
  std::vector<std::string> allKeys ;

  // https://github.com/facebook/rocksdb/wiki/Basic-Operations
  rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions()) ;
  for( it->SeekToFirst() ; it->Valid() ; it->Next() )
    allKeys.push_back( it->key().ToString() ) ;
  delete it;

  return allKeys ;
}

// inIDList
// input id, idList
// output bool
bool inIDList ( std::string id, std::vector<std::string> idList )
{
  bool flag = false ;

  if( std::find( idList.begin(), idList.end(), id ) != idList.end() )
    flag = true ;

  return flag ;
}

// getMap
// input map + concept in ontology map
// output vector of concepts subsumed by input concept
std::vector<std::string> getMap ( std::string concept, std::map< std::string, std::vector<std::string> > ontoMap )
{
  std::vector<std::string> mapForConcept = ontoMap[ concept ] ;
  return mapForConcept ;
}

// inMap
// input ontology concept, map value for a concept
// output bool
bool inMap ( std::string concept, std::vector<std::string> mapValue )
{
  bool flag = false ;

  if( std::find( mapValue.begin(), mapValue.end(), concept ) != mapValue.end() )
    flag = true ;

  return flag ;
}

#endif
