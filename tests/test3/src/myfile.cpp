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
  std::string kDBPath = "/afs/cats.ucsc.edu/users/j/kmdahlgr/temp/testdb2" ;

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

  // Put key-value
  status = db->rocksdb::DB::Put(WriteOptions(), "key1", "value");
  assert(status.ok());
  std::string value;
  // get value
  status = db->rocksdb::DB::Get(ReadOptions(), "key1", &value);
  assert(status.ok());
  if (value == "value1")
    std::cout << "Yep." << std::endl ;
  else
    std::cout << "Nope." << std::endl ;

  // atomically apply a set of updates
  {
    WriteBatch batch;
    batch.Delete("key1");
    batch.Put("key2", value);
    status = db->Write(WriteOptions(), &batch);
  }

  status = db->Get(ReadOptions(), "key1", &value);
  assert(status.IsNotFound());

  db->Get(ReadOptions(), "key2", &value);
  assert(value == "value");

  delete db;

  std::cout << "You're AWESOME 2! =D" << std::endl ;
  
  return(0);
}
