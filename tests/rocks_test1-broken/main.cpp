#include <iostream>
#include <assert.h>
//#include <rocksdb/db.h>

int main(int argc, char** argv) 
{
  std::cout << "You're AWESOME 1! =D" << std::endl ;

/*
  rocksdb::DB *db;

  rocksdb::Options options;
  options.create_if_missing = true;
  rocksdb::Status status = rocksdb::DB::Open(options, "/afs/cats.ucsc.edu/users/j/kmdahlgr/temp/testdb", &db);
  //rocksdb::Status status = rocksdb::DB::Open(options, "/tmp/testdb/", &db);
  assert(status.ok());

  delete db ;
*/
  std::cout << "You're AWESOME 2! =D" << std::endl ;

  return 0;
}
