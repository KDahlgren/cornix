#include "cornix_crud.h"
#include "cornix_utils.h"
#include "cornix_ops.h"
#include "cornix_agg.h"
#include "cornix_booleans.h"
#include "styx.h"

int main(int argc, char** argv)
{
  std::cout << "Hi, I'm Cornix!" << std::endl ;
  crud_message() ;
  
  // Create DB
  std::string dbname = "/afs/cats.ucsc.edu/users/j/kmdahlgr/temp/cornixdb/" ;
  std::pair<rocksdb::DB*, rocksdb::Status> result = create_db(dbname) ;
  rocksdb::DB* db = result.first ;
  rocksdb::Status status = result.second ;
  
  // CLEAR DB
  status = clear_db(db, status) ;
  read_db(db, status) ;

  //status = update_col_atomic(db, status, 101, "{ (101, Dee) }") ;
  status = update_col_atomic(db, status, "101", "{ (101, Dee) }") ;
  assert(status.ok()) ;
 
  read_db(db, status) ;
 
  return 0 ;
}
