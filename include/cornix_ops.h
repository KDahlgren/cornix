/*
 * filename   :    cornix_ops.h
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 * Description:
 *     Definitions of predicate operations
 */

#ifndef CORNIX_OPS_H
#define CORNIX_OPS_H

#include "cornix_includes.h"
#include "cornix_parser.h"
#include "cornix_booleans.h"

// from http://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
bool isNumber( std::string str )
{
  bool flag = true ;
  for(std::string::const_iterator k = str.begin(); k != str.end(); k++)
    flag &= std::isdigit( *k ) ;

  return flag ;
}

// eq
std::vector<std::string> eq(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  std::cout << " ... eq .. " << std::endl ;
  // lhs == column key
  // 1. Get the value for the column key
  // 2. Convert into vector of tuples
  // 3. For each tuple
  // 4. If value in tuple eqs rhs
  //    then add to idList 

  std::vector<std::string> idList ;

  // Get value for lhs column key
  std::string value ;
  status = db->Get(ReadOptions(), lhs, &value);
  if ( status.IsNotFound() )
    std::cout << "No data for: " << lhs << std::endl ;
  else
  {
    // Convert into vector of tuples    
    std::vector<std::string> tupleList = parseList(value) ;
    for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    {
      // If value in tuple eqs rhs      
      // then add id to idList.
      // getID      
      std::string id = getID( tupleList[i] ) ;
      // getValue
      std::string value = getValue( tupleList[i] ) ;
      if ( isNumber(value) && isNumber(rhs) )
      {
        std::cout << "Yes number" << std::endl ;
        std::cout << "value = " << value << std::endl ;
        std::cout << "rhs = " << rhs << std::endl ;
        double v = std::stod( value ) ;
        double r = std::stod( rhs ) ;
        if ( v == r )
        {
          std::cout << "Pushing id: " << id << std::endl ;
          idList.push_back( id ) ;
        }
      }
      else if ( value.compare(rhs) == 0 )
        idList.push_back( id ) ;
    }
  }

  std::cout << " ... eq done ... " << std::endl ;
  return idList ;
}

// ne
std::vector<std::string> ne(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  // lhs == column key
  // 1. Get the value for the column key
  // 2. Convert into vector of tuples
  // 3. For each tuple
  // 4. If value in tuple eqs rhs
  //    then add to idList 

  std::vector<std::string> idList ;

  // Get value for lhs column key
  std::string value ;
  status = db->Get(ReadOptions(), lhs, &value);
  if ( status.IsNotFound() )
  std::cout << "No data for: " << lhs << std::endl ;
  else
  {
    // Convert into vector of tuples    
    std::vector<std::string> tupleList = parseList(value) ;
    for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    {
      // If value in tuple eqs rhs      
      // then add id to idList.
      // getID      
      std::string id = getID( tupleList[i] ) ;
      // getValue
      std::string value = getValue( tupleList[i] ) ;
      if ( isNumber(value) && isNumber(rhs) )
      {
        double v = std::stod( value ) ;
        double r = std::stod( rhs ) ;
        if ( v != r )
          idList.push_back( id ) ;
      }
      else if ( value.compare(rhs) != 0 )
        idList.push_back( id ) ;
    }
  }

  return idList ;
}

// gt
std::vector<std::string> gt(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  // lhs == column key
  // 1. Get the value for the column key
  // 2. Convert into vector of tuples
  // 3. For each tuple
  // 4. If value in tuple eqs rhs
  //    then add to idList 
 
 std::vector<std::string> idList ;
  
  // Get value for lhs column key
  std::string value ;
  status = db->Get(ReadOptions(), lhs, &value);
  if ( status.IsNotFound() )
    std::cout << "No data for: " << lhs << std::endl ;
  else
  {
    // Convert into vector of tuples    
    std::vector<std::string> tupleList = parseList(value) ;
    for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    {
      // If value in tuple eqs rhs      
      // then add id to idList.
      // getID      
      std::string id = getID( tupleList[i] ) ;
      // getValue
      std::string value = getValue( tupleList[i] ) ;
      if ( isNumber(value) && isNumber(rhs) )
      {
        double v = std::stod( value ) ;
        double r = std::stod( rhs ) ;
        if ( v > r )
          idList.push_back( id ) ;
      }
      else if ( value.compare(rhs) > 0 )
        idList.push_back( id ) ;
    }
  }
  
  return idList ;
}

//gte
std::vector<std::string> gte(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  std::vector<std::string> idList1 = eq(db, status, lhs, rhs) ;
  std::vector<std::string> idList2 = gt(db, status, lhs, rhs) ;;

  // Append new list of valid ids to idList
  idList1.insert( idList1.end(), idList2.begin(), idList2.end() ) ;

  return idList1 ;
}
/*
// gte
std::vector<std::string> gte(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  // lhs == column key
  // 1. Get the value for the column key
  // 2. Convert into vector of tuples
  // 3. For each tuple
  // 4. If value in tuple eqs rhs
  //    then add to idList 

  std::vector<std::string> idList ;

  // Get value for lhs column key
  std::string value ;
  status = db->Get(ReadOptions(), lhs, &value);
  if ( status.IsNotFound() )
    std::cout << "No data for: " << lhs << std::endl ;
  else
  {
    // Convert into vector of tuples
    std::vector<std::string> tupleList = parseList(value) ;    
    for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    {
      // If value in tuple eqs rhs
      // then add id to idList.
      // getID
      std::string id = getID( tupleList[i] ) ;
      // getValue
      std::string value = getValue( tupleList[i] ) ;
      if ( isNumber(value) && isNumber(rhs) )
      {
        double v = std::stod( value ) ;
        double r = std::stod( rhs ) ;
        if ( v >= r )
          idList.push_back( id ) ;
      }
      else
      {
        // Convert into vector of tuples    
        std::vector<std::string> tupleList = parseList(value) ;
        for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
        {
          // If value in tuple eqs rhs      
          // then add id to idList.
          // getID      
          std::string id = getID( tupleList[i] ) ;
          // getValue
          std::string value = getValue( tupleList[i] ) ;
          if ( isNumber(value) && isNumber(rhs) )
          {
            double v = std::stod( value ) ;
            double r = std::stod( rhs ) ;
            if ( v >= r )
              idList.push_back( id ) ;
          }
          else if ( value.compare(rhs) >= 0 )
            idList.push_back( id ) ;
        }
      }
    }
  }

  return idList ;
}
*/
// lt
std::vector<std::string> lt(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  // lhs == column key
  // 1. Get the value for the column key
  // 2. Convert into vector of tuples
  // 3. For each tuple
  // 4. If value in tuple eqs rhs
  //    then add to idList 
  
  std::vector<std::string> idList ;
  
  // Get value for lhs column key
  std::string value ;
  status = db->Get(ReadOptions(), lhs, &value);
  if ( status.IsNotFound() )
    std::cout << "No data for: " << lhs << std::endl ;
  else
  {
    // Convert into vector of tuples
    std::vector<std::string> tupleList = parseList(value) ;
    for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    {
      std::cout << "tuple = " << tupleList[i] << ", i = " << i <<  std::endl ;
      // If value in tuple eqs rhs
      // then add id to idList.
      // getID
      std::string id = getID( tupleList[i] ) ;
      // getValue
      std::string value = getValue( tupleList[i] ) ;
      if ( isNumber(value) && isNumber(rhs) )
      {
        double v = std::stod( value ) ;
        double r = std::stod( rhs ) ;
        if ( v < r )
          idList.push_back( id ) ;
      }
      else if ( value.compare(rhs) < 0 )
        idList.push_back( id ) ;
    }
  }
  
  return idList ;
}

//lte
std::vector<std::string> lte(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  std::vector<std::string> idList1 = eq(db, status, lhs, rhs) ;
  std::vector<std::string> idList2 = lt(db, status, lhs, rhs) ;;

  // Append new list of valid ids to idList
  idList1.insert( idList1.end(), idList2.begin(), idList2.end() ) ;

  return idList1 ;
}
/*
// lte
std::vector<std::string> lte(rocksdb::DB* db, rocksdb::Status status, std::string lhs, std::string rhs)
{
  // lhs == column key
  // 1. Get the value for the column key
  // 2. Convert into vector of tuples
  // 3. For each tuple
  // 4. If value in tuple eqs rhs
  //    then add to idList 
  
  std::vector<std::string> idList ;
  
  // Get value for lhs column key
  std::string value ;
  status = db->Get(ReadOptions(), lhs, &value);
  if ( status.IsNotFound() )
  std::cout << "No data for: " << lhs << std::endl ;
  else
  {
    // Convert into vector of tuples    
    std::vector<std::string> tupleList = parseList(value) ;    
    for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    {
      // If value in tuple eqs rhs      
      // then add id to idList.
      // getID      
      std::string id = getID( tupleList[i] ) ;      
      // getValue
      std::string value = getValue( tupleList[i] ) ;
      if ( isNumber(value) && isNumber(rhs) )
      {
        std::cout << "Yes number" << std::endl ;
        std::cout << "value = " << value << std::endl ;
        std::cout << "rhs = " << rhs << std::endl ;
        double v = std::stod( value ) ;
        double r = std::stod( rhs ) ;
        if ( v <= r )
        {
          std::cout << "Pushing id: " << id << std::endl ;
          idList.push_back( id ) ;
        }
      }
      else if ( value.compare(rhs) <= 0 )
        idList.push_back( id ) ;
     }  
   }
    
  return idList ;
} 
*/

#endif
