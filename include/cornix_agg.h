/*
 * filename   :    cornix_agg.h
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 * Description:
 *     Definitions of Cornix 
 *     aggregation operations
 */

#ifndef CORNIX_AGG_H
#define CORNIX_AGG_H

#include "cornix_includes.h"

// READ DATA
// input key
// output value for the key
std::string read_data(rocksdb::DB* db, rocksdb::Status status, std::string key)
{
  std::string value ;
  status = db->rocksdb::DB::Get(ReadOptions(), key, &value);
  if( status.ok() )
    return value ;
  else
    return "" ;
    //return "No data for key: " + key ;
}

// AVG
// Assume one field in key list
double avg(rocksdb::DB* db, rocksdb::Status status, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  std::vector<double> valList ;

  // Get field from keyList
  std::string key = keyList[0] ;
  // Get value at the key
  std::string value = read_data(db, status, key) ;
  // Get tuple List from value
  std::vector<std::string> tupleList = parseList(value) ;

  // Get valid values
  for(unsigned int i = 0 ; i < tupleList.size() ; i++)
  {
    std::string tupleID = getID( tupleList[i] ) ;
    std::string tupleVal = getValue( tupleList[i] ) ;
    bool check = std::find(idList.begin(), idList.end(), tupleID) != idList.end() ;
    if( check == true )
    {
      // Convert value from string to number
      double newValue = std::stod (tupleVal) ;
      // Append to end of valList
      valList.push_back(newValue) ; 
    }
  }

  int numElements = valList.size() ;
  double sumElements = std::accumulate(valList.begin(), valList.end(), 0);

  if(value == "No data for key: "+key)
    return (-999999999) ;
  else
    return sumElements/numElements ;
}

// MIN
// Assume one field in key list
double min(rocksdb::DB* db, rocksdb::Status status, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  std::vector<double> valList ;

  // Get field from keyList
  std::string key = keyList[0] ;
  // Get value at the key
  std::string value = read_data(db, status, key) ;
  // Get tuple List from value
  std::vector<std::string> tupleList = parseList(value) ;

  // Get valid values
  for(unsigned int i = 0 ; i < tupleList.size() ; i++)
  {
    std::string tupleID = getID( tupleList[i] ) ;
    std::string tupleVal = getValue( tupleList[i] ) ;
    bool check = std::find(idList.begin(), idList.end(), tupleID) != idList.end() ;
    if( check == true )
    {
      // Convert value from string to number
      double newValue = std::stod (tupleVal) ;
      // Append to end of valList
      valList.push_back(newValue) ;
    }
  }

  // get min
  if(value == "No data for key: "+key)
    return (-999999999) ;
  else
  {
    std::vector<double>::iterator result = std::min_element(std::begin(valList), std::end(valList)) ;
    double min = *result ; 
    return min ;
  }
}

// MAX
// Assume one field in key list
double max(rocksdb::DB* db, rocksdb::Status status, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  std::vector<double> valList ;

  // Get field from keyList
  std::string key = keyList[0] ;
  // Get value at the key
  std::string value = read_data(db, status, key) ;
  // Get tuple List from value
  std::vector<std::string> tupleList = parseList(value) ;

  // Get valid values
  for(unsigned int i = 0 ; i < tupleList.size() ; i++)
  {
    std::string tupleID = getID( tupleList[i] ) ;
    std::string tupleVal = getValue( tupleList[i] ) ;
    bool check = std::find(idList.begin(), idList.end(), tupleID) != idList.end() ;
    if( check == true )
    {
      // Convert value from string to number
      double newValue = std::stod (tupleVal) ;
      // Append to end of valList
      valList.push_back(newValue) ;
    }
  }

  // get min
  if(value == "No data for key: "+key)
    return (-999999999) ;
  else
  {
    std::vector<double>::iterator result = std::max_element(std::begin(valList), std::end(valList)) ;
    double max = *result ;
    return max ;
  }
}

// SUM
// Assume one field in key list
double sum(rocksdb::DB* db, rocksdb::Status status, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  std::vector<double> valList ;

  // Get field from keyList
  std::string key = keyList[0] ;
  // Get value at the key
  std::string value = read_data(db, status, key) ;
  // Get tuple List from value
  std::vector<std::string> tupleList = parseList(value) ;

  // Get valid values
  for(unsigned int i = 0 ; i < tupleList.size() ; i++)
  {
    std::string tupleID = getID( tupleList[i] ) ;
    std::string tupleVal = getValue( tupleList[i] ) ;    bool check = std::find(idList.begin(), idList.end(), tupleID) != idList.end() ;
    if( check == true )
    {
      // Convert value from string to number
      double newValue = std::stod (tupleVal) ;
      // Append to end of valList
      valList.push_back(newValue) ;
    }
  }
  // get sum
  if(value == "No data for key: "+key)
    return (-999999999) ;
  else
  { 
    double sum = std::accumulate(valList.begin(), valList.end(), 0);
    return sum ;
  }   
}

// COUNT
// Assume one field in key list
double count(rocksdb::DB* db, rocksdb::Status status, std::vector<std::string> keyList, std::vector<std::string> idList)
{
  std::vector<double> valList ;

  // Get field from keyList
  std::string key = keyList[0] ;
  // Get value at the key
  std::string value = read_data(db, status, key) ;
  // Get tuple List from value
  std::vector<std::string> tupleList = parseList(value) ;

  // Get valid values
  for(unsigned int i = 0 ; i < tupleList.size() ; i++)
  {
    std::string tupleID = getID( tupleList[i] ) ;
    std::string tupleVal = getValue( tupleList[i] ) ;
    bool check = std::find(idList.begin(), idList.end(), tupleID) != idList.end() ;
    if( check == true )
    {
      // Convert value from string to number
      double newValue = std::stod (tupleVal) ;
      // Append to end of valList
      valList.push_back(newValue) ;
    }
  }
  // get count
  if(value == "No data for key: "+key)
    return (-999999999) ;
  else
  {
    double count = valList.size() ;
    return count ;
  }
}

#endif
