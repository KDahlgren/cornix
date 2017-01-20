// https://bienuit.wordpress.com/2013/12/04/how-to-install-rocksdb-on-ubuntu-12-04-lts/
#include <cstdio>
#include <string>
#include <iostream>
#include <assert.h>
#include <rocksdb/db.h>
#include <rocksdb/slice.h>
#include <rocksdb/options.h>

using namespace rocksdb;

int main(int argc, char** argv) 
{
  std::string kDBPath = "/afs/cats.ucsc.edu/users/j/kmdahlgr/temp/testdb4" ;

  // call a function in another file
  std::cout << "Print stuff" << std::endl ;

  rocksdb::DB *db;
  rocksdb::Options options;

  //Optimizations do not work.
  // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
  //options.IncreaseParallelism();
  //options.OptimizeLevelStyleCompaction();

  // create the DB if it's not already present
  options.create_if_missing = true;

  // open DB
  rocksdb::Status status = rocksdb::DB::Open(options, "/afs/cats.ucsc.edu/users/j/kmdahlgr/temp/testdb/", &db);
  assert(status.ok());

  //int myValue = 117 ;
  //int foo [5] = { 16, 2, 77, 40, 12071 }; 
  //std::string myValue ;
  //for(int i = 0 ; i < sizeof(foo) ; ++i)
  //   myValue.push_back('0' + foo[i]);

  std::string myValue = "{ 16, 2, 77, 40, 12071 }" ;

  std::cout << "Insert " << std::endl ;

  // Put key-value
  status = db->rocksdb::DB::Put(WriteOptions(), "key1", myValue);
  assert(status.ok());
  std::string value;
  // get value
  status = db->rocksdb::DB::Get(ReadOptions(), "key1", &value);
  assert(status.ok());
  if (value == myValue)
    std::cout << "Yep." << std::endl ;
  else
    std::cout << "Nope." << std::endl ;

  //Print contents 1
  std::cout << "Print contents 1:" << std::endl ;
  rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
  for (it->SeekToFirst(); it->Valid(); it->Next()) 
  {
    std::cout << it->key().ToString() << ": " << it->value().ToString() << std::endl;
  }
  assert(it->status().ok()); // Check for any errors found during the scan
  delete it;

  // atomically apply a set of updates
  {
    WriteBatch batch;
    batch.Delete("key1");
    batch.Put("key2", value);
    status = db->Write(WriteOptions(), &batch);
  }

  //Print contents
  std::cout << "Print contents 2:" << std::endl ;
  rocksdb::Iterator* it1 = db->NewIterator(rocksdb::ReadOptions());
  for (it1->SeekToFirst(); it1->Valid(); it1->Next())
  {
    std::cout << it1->key().ToString() << ": " << it1->value().ToString() << std::endl;
  }
  assert(it1->status().ok()); // Check for any errors found during the scan
  delete it1 ;

  status = db->Get(ReadOptions(), "key1", &value);
  assert(status.IsNotFound());

  db->Get(ReadOptions(), "key2", &value);
  //assert(value == "value");

  //Print contents
  std::cout << "Print contents 2:" << std::endl ;
  rocksdb::Iterator* it2 = db->NewIterator(rocksdb::ReadOptions());
  for (it2->SeekToFirst(); it2->Valid(); it2->Next()) 
  {
    std::cout << it2->key().ToString() << ": " << it2->value().ToString() << std::endl;
  }
  assert(it2->status().ok()); // Check for any errors found during the scan
  delete it2 ;

  delete db;

  std::cout << "You're AWESOME! =D" << std::endl ;
  
  return(0);
}
