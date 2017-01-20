/*
 * filename   :    cornix_crud.h
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 * Description:
 *     Definitions of Cornix 
 *     Create, Read, Update, and Delete ops.
 */

/*
 * FUTURE WORK:
 *
 * Edit insert method to not include id as a column
 * recorded in the database in the case of adding data
 * with custom, rather than random, ids.
 *
 * Example:
 * Inserting { $id: 1, $name: Frankie, $age: 23 }
 * creates an entry in the DB "id: {(1,1)}"
 *
 */

#ifndef CORNIX_CRUD_H
#define CORNIX_CRUD_H

#include "cornix_includes.h"
#include "cornix_parser.h"
#include "cornix_utils.h"
#include "cornix_agg.h"
#include "styx.h"

//CREATE_DB
std::pair<rocksdb::DB*, rocksdb::Status> create_db(std::string abspath)
{
  std::cout << "... Creating db ..." << std::endl ;

  std::string kDBPath = abspath ;
  rocksdb::DB *db;
  rocksdb::Options options;

  //Create the DB if it's not already present
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, kDBPath, &db);
  assert(status.ok());

  std::cout << "... done ..." << std::endl ;

  return std::make_pair(db, status) ;
}

// CLEAR DB
rocksdb::Status clear_db(rocksdb::DB* db, rocksdb::Status status)
{
  std::cout << "... Clearing db ..." << std::endl ;

  rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next())
  {
    status = db->rocksdb::DB::Delete(WriteOptions(), it->key() ) ;
    assert(status.ok());
  }
  assert(it->status().ok()); // Check for any errors found during the scan
  delete it ;

  std::cout << "... done clearing db ..." << std::endl ;
  return status ;
}


//READ
void read(rocksdb::DB* db, rocksdb::Status status, std::string query)
{
  std::cout << "... Reading ..." << std::endl ;

  std::vector<std::string> keyList = getKeyList( query ) ;
  //for(unsigned int i = 0 ; i < keyList.size() ; i++)
  //  std::cout << keyList[i] << std::endl ;
  std::string aggOp = getReturnOp( query ) ;
  //std::cout << "aggOp = " << aggOp << std::endl ;
  std::vector<std::string> idList = getIDList( db, status, query ) ;
  getData( db, status, aggOp, keyList, idList ) ;

  std::cout << "... done ..." << std::endl ;
  return ;
}

//READ DB
void read_db(rocksdb::DB* db, rocksdb::Status status)
{
  std::cout << "... Reading db ..." << std::endl ;

  rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next())
  {
    std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
  }
  assert(it->status().ok()); // Check for any errors found during the scan
  delete it;

  std::cout << "... done reading db ..." << std::endl ;
  return ;
}

//UPDATE COL
rocksdb::Status update_col_atomic(rocksdb::DB *db, rocksdb::Status status, std::string key1, std::string myValue)
{
  std::cout << "... Updating col ..." << std::endl ;

  // atomically apply a set of updates
  WriteBatch batch;
  batch.Put(key1, myValue);
  status = db->Write(WriteOptions(), &batch);

  // non-atomically apply a set of updates
  //status = db->rocksdb::DB::Put(WriteOptions(), key1, myValue);
  assert(status.ok());

  std::cout << "... done updating col ..." << std::endl ;

  return status;
}

//UPDATE 
rocksdb::Status update(rocksdb::DB *db, rocksdb::Status status, std::string row)
{
  if( styx(row) )
  {
    std::vector<std::string> dataList = getArgs( row ) ;
    std::string updateID = getValue( dataList[0] ) ;

    // Iterate over fields in row
    for(unsigned int i = 1 ; i < dataList.size() ; i++)
    {
      // Get first (field, newValue) pair
      std::string t = dataList[i] ;
      std::string tField = getID(t) ;
      std::string tVal = getValue(t) ;
      std::string newTup = "(" + updateID + ", " + tVal + ")" ;

      // Get existing value for key
      std::string currCol = read_data( db, status, tField ) ;
      // Parse value into a vector of tuples
      std::vector<std::string> tupleList = parseList( currCol ) ;
      for(unsigned int j = 0 ; j < tupleList.size() ; j++)
      {
        std::string currTup = tupleList[j] ;
        std::string tupleID = getID( currTup ) ;
        if( updateID == tupleID )
        {
          tupleList.erase( tupleList.begin()+j ) ;
          tupleList.push_back( newTup ) ;
        }
      }

      // Convert vector to string
      // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
      std::ostringstream newVal ;
      newVal << "{" ;
      if( !tupleList.empty() )
      {
        // Convert all but the last element to avoid a trailing ","
        std::copy(tupleList.begin(), tupleList.end()-1, std::ostream_iterator<std::string>( newVal, "," ));
        // Now add the last element with no delimiter
        newVal << tupleList.back() ;
        newVal << "}" ;
      }

      status = update_col_atomic( db, status, tField, newVal.str() ) ;
    }
  }
  return status ;
}

// INSERT 
rocksdb::Status insert(rocksdb::DB *db, rocksdb::Status status, std::string row)
{
  if( styx(row) )
  {
    std::cout << "Passed styx" << std::endl ;
    std::vector<std::string> dataList = getArgs( row ) ;

    std::string updateID ;
    if( getID( dataList[0] ) == "id" )
      updateID = getValue( dataList[0] ) ;
    else
      updateID = random_string( 10 ) ;

    // Iterate over fields in row
    for(unsigned int i = 0 ; i < dataList.size() ; i++)
    {
      // Get first (field, newValue) pair
      std::string t = dataList[i] ;
      std::string tField = getID(t) ;
      std::string tVal = getValue(t) ;
      std::string newTup = "(" + updateID + ", " + tVal + ")" ;

      std::string currCol = read_data(db, status, tField) ;


      // STRING PARSING APPROACH //
      // Add new data tuple
      if( currCol.length() > 0)
        currCol.pop_back() ;
      else
        currCol += "{" ;
      currCol += newTup ;
      currCol += "}" ;

/*
      // VECTOR PARSING APPROACH //
      std::vector<std::string> tupleList = parseList( currCol ) ;
      tupleList.push_back( newTup ) ;

      // Convert vector to string
      // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
      std::ostringstream newVal ;
      newVal << "{" ;
      if( !tupleList.empty() )
      {
        // Convert all but the last element to avoid a trailing ","
        std::copy(tupleList.begin(), tupleList.end()-1, std::ostream_iterator<std::string>( newVal, "," ));
        // Now add the last element with no delimiter
        newVal << tupleList.back() ;
        newVal << "}" ;
      }
      status = update_col_atomic( db, status, tField, newVal.str() ) ;
*/
      status = update_col_atomic( db, status, tField, currCol ) ;
    }
  }
  return status ;
}

// DELETE COL
rocksdb::Status delete_col(rocksdb::DB* db, rocksdb::Status status, std::string key1)
{
  std::cout << "... Deleting col ..." << std::endl ;

  status = db->rocksdb::DB::Delete(WriteOptions(), key1);
  assert(status.ok());

  std::cout << "... done deleting col ..." << std::endl ;
  return status ;
}

// DELETE on id
rocksdb::Status deleteOn_id(rocksdb::DB *db, rocksdb::Status status, std::string delID)
{
  // Get all keys in db
  std::vector<std::string> keyList = getAllKeys( db, status ) ;

  for(unsigned int i = 0 ; i < keyList.size() ; i++)
  {
    std::string currKey = keyList[i] ;
    std::string value = read_data( db, status, currKey ) ;
    std::vector<std::string> tupleList = parseList( value ) ;
    for(unsigned int j = 0 ; j < tupleList.size() ; j++)
    {
      std::string t = tupleList[j] ;
      std::string tID = getID(t) ;
      std::string tVal = getValue(t) ;
      if( delID == tID )
        tupleList.erase( tupleList.begin()+j ) ;

    }

    // Convert vector to string
    // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
    std::ostringstream newVal ;
    newVal << "{" ;
    if( !tupleList.empty() )
    {
      // Convert all but the last element to avoid a trailing ","
      std::copy(tupleList.begin(), tupleList.end()-1, std::ostream_iterator<std::string>( newVal, "," ));
      // Now add the last element with no delimiter
      newVal << tupleList.back() ;
      newVal << "}" ;
    }

    // Update db
    // If id does not exist in db, then request is ignored.
    status = update_col_atomic( db, status, currKey, newVal.str() ) ;
  }

  return status ;
}

// DELETE on predicate
rocksdb::Status deleteOn_pred(rocksdb::DB *db, rocksdb::Status status, std::string query)
{
  // Get all ids satisfying the query
  std::vector<std::string> idList = getIDList( db, status, query ) ;
  // Get all keys in DB
  std::vector<std::string> keyList = getAllKeys( db, status ) ;

  // Iterate over keys
  for(unsigned int i = 0 ; i < keyList.size() ; i++)
  {
    std::string currKey = keyList[i] ;
    std::string value = read_data( db, status, currKey ) ;
    std::vector<std::string> tupleList = parseList( value ) ;

    // Iterate over tuples in key value
    int numTuples = tupleList.size() ;
    std::vector<std::string> cpTupleList = tupleList ;
    for(int j = 0 ; j < numTuples ; j++)
    {
      std::string t = tupleList[j] ;
      std::string tID = getID(t) ;
      std::string tVal = getValue(t) ;

      if( inIDList( tID, idList ) )
        cpTupleList.erase( cpTupleList.begin()+j ) ;
    }
    // Convert vector to string
    // http://stackoverflow.com/questions/8581832/converting-a-vector-to-string
    std::ostringstream newVal ;
    newVal << "{" ;
    if( !cpTupleList.empty() )
    {
    // Convert all but the last element to avoid a trailing ","
    std::copy(cpTupleList.begin(), cpTupleList.end()-1, std::ostream_iterator<std::string>( newVal, "," ));
    // Now add the last element with no delimiter
    newVal << cpTupleList.back() ;
    newVal << "}" ;
    }
    
    // Update db
    // If no valid ids, then request is ignored.
    status = update_col_atomic( db, status, currKey, newVal.str() ) ; 
  }
  
  return status ;
}

//DELETE DB
void delete_db(rocksdb::DB* db)
{
  delete db ;
  return ;
}

//CRUD OPS MESSAGE
void crud_message()
{
  std::cout << "Hi, I'm Cornix's CRUD OPS!" << std::endl ;
  std::cout << "You're awesome! =D" << std::endl ;
  return ;
}

#endif
