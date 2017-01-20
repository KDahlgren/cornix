/*
 * filename   :    cornix_driver.cpp
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 * Description:
 *     The driver for Cornix. Place all initial
 *     examples and tests here.
 */

#include "cornix_crud.h"
#include "cornix_utils.h"
#include "cornix_ops.h"
#include "cornix_agg.h"
#include "cornix_booleans.h"
#include "styx.h"

int main(int argc, char** argv)
{
  std::cout << "Hi, I'm Cornix!" << std::endl ;
  //crud_message() ;

  // Create DB
  std::string dbname = "/afs/cats.ucsc.edu/users/j/kmdahlgr/temp/cornixdb/" ;
  std::pair<rocksdb::DB*, rocksdb::Status> result = create_db(dbname) ;
  rocksdb::DB* db = result.first ;
  rocksdb::Status status = result.second ;

  // CLEAR DB
  status = clear_db(db, status) ;
  read_db(db, status) ;

  // read
  std::cout << "Test read" << std::endl ;
  status = update_col_atomic(db, status, "name", "{ (101, Dee) }") ;
  assert(status.ok()) ;
  status = update_col_atomic(db, status, "age", "{ (101, 20), (102, 21), (103, 22) }") ;
  status = update_col_atomic(db, status, "country", "{ (101, us), (102, us), (103, canada) }") ;
  status = update_col_atomic(db, status, "state", "{ (101, ca), (102, fl) }") ;
  status = update_col_atomic(db, status, "city", "{ (101, santacruz), (102, miami), (103, toronto) }") ;
  status = update_col_atomic(db, status, "id", "{ (101, 101), (102, 102), (103, 103) }") ;

  std::string query_read = "{ (name, age, country, state, city) : ($id _eq 101), ($id _eq 102), ($id _eq 103) }" ;
  read( db, status, query_read ) ;
  std::cout << "End of test read" << std::endl ;
  status = delete_col(db, status, "country") ;
  status = delete_col(db, status, "age") ;
  status = delete_col(db, status, "name") ;
  status = delete_col(db, status, "state") ;
  status = delete_col(db, status, "city") ;
  status = delete_col(db, status, "id") ;

/*
  // Test styx
  std::cout << "Test styx" << std::endl ;

  std::string q1 = "{ $name: Alice, $age: 23, $country: us, $state: ca, $city: sanfransisco }" ;
  std::string q2 = "{ $name: Bria, $age: 23, $country: us, $state: ca, $city: toronto }" ;
  std::string q3 = "{ $name: Cate, $age: 23, $country: canada, $state: ca, $city: toronto }" ;
  std::string q4 = "{ $name: Dee, $age: 23, $country: canada, $state: alberta, $city: toronto }" ;
  std::string q5 = "{ $name: Enya, $age: 23, $country: canada, $state: ontario, $city: toronto }" ;
  std::string q6 = "{ $id: fa37JncCHr, $country: canada, $city: sanfransisco }" ;

  std::string attUp_styx0 = "{ $id: 1, $name: Dee, $age: 23 }" ;
  std::string attUp_styx1 = q1 ;
  std::string attUp_styx2 = q2 ;
  std::string attUp_styx3 = q3 ;
  std::string attUp_styx4 = q4 ;
  std::string attUp_styx5 = q5 ;
  std::string attUp_styx6 = q6 ; 
 
  bool result_styx0 = styx( attUp_styx0 ) ;
  bool result_styx1 = styx( attUp_styx1 ) ;
  bool result_styx2 = styx( attUp_styx2 ) ;
  bool result_styx3 = styx( attUp_styx3 ) ;
  bool result_styx4 = styx( attUp_styx4 ) ;
  bool result_styx5 = styx( attUp_styx5 ) ;
  bool result_styx6 = styx( attUp_styx6 ) ;

  std::cout << "attempted update: " << attUp_styx0 << std::endl ;
  std::cout << "styx result: " << result_styx0 << std::endl ;
  std::cout << "attempted update: " << attUp_styx1 << std::endl ;
  std::cout << "styx result: " << result_styx1 << std::endl ;
  std::cout << "attempted update: " << attUp_styx2 << std::endl ;
  std::cout << "styx result: " << result_styx2 << std::endl ;
  std::cout << "attempted update: " << attUp_styx3 << std::endl ;
  std::cout << "styx result: " << result_styx3 << std::endl ;
  std::cout << "attempted update: " << attUp_styx4 << std::endl ;
  std::cout << "styx result: " << result_styx4 << std::endl ;
  std::cout << "attempted update: " << attUp_styx5 << std::endl ;
  std::cout << "styx result: " << result_styx5 << std::endl ;
  std::cout << "attempted update: " << attUp_styx6 << std::endl ;
  std::cout << "styx result: " << result_styx6 << std::endl ;
  
  std::cout << "End of test styx" << std::endl ;
*/
/*
  // Test insert and update with styx
  // insert
  std::cout << "Test insert" << std::endl ;

  std::string q1 = "{ $name: Alice, $age: 23, $country: us, $state: ca, $city: sanfransisco }" ;
  std::string q2 = "{ $name: Bria, $age: 23, $country: us, $state: ca, $city: toronto }}" ;
  std::string q3 = "{ $name: Cate, $age: 23, $country: canada, $state: ca, $city: toronto }" ;
  std::string q4 = "{ $name: Dee, $age: 23, $country: canada, $state: alberta, $city: toronto }" ;
  std::string q5 = "{ $name: Enya, $age: 23, $country: canada, $state: ontario, $city: toronto }" ;

  std::string q6 = "{ $id: 1, $name: Frankie, $age: 23, $country: us, $state: ca, $city: sanfransisco }" ;
  std::string q7 = "{ $id: 2, $name: Greta, $age: 23, $country: us, $state: ca, $city: sanfransisco }" ;
  std::string q8 = "{ $id: 3, $name: Hannah, $age: 23, $country: canada, $state: ontario, $city: toronto }" ;
  std::string q9 = "{ $id: 4, $name: Ia, $age: 23, $country: canada, $state: ontario, $city: toronto }" ;
  std::string q10 = "{ $id: 5, $name: Jackie, $age: 23, $country: canada, $state: ontario, $city: toronto }" ;

  status = insert(db, status, q1 ) ;
  status = insert(db, status, q2 ) ;
  status = insert(db, status, q3 ) ;
  status = insert(db, status, q4 ) ;
  status = insert(db, status, q5 ) ;
  status = insert(db, status, q6 ) ;
  status = insert(db, status, q7 ) ;
  status = insert(db, status, q8 ) ;
  status = insert(db, status, q9 ) ;
  status = insert(db, status, q10 ) ;
  read_db(db, status) ;
  std::cout << "End of test insert" << std::endl ;

  // update
  std::cout << "Test update" << std::endl ;
  status = update_col_atomic(db, status, "age", "{(fa37JncCHr, 23),(yDsbzayy4c, 23)}") ;
  status = update_col_atomic(db, status, "city", "{(fa37JncCHr, sanfransisco),(yDsbzayy4c, toronto)}") ;
  status = update_col_atomic(db, status, "country", "{(fa37JncCHr, us),(yDsbzayy4c, canada)}") ;
  status = update_col_atomic(db, status, "name", "{(fa37JncCHr, Alice),(yDsbzayy4c, Enya)}") ;
  status = update_col_atomic(db, status, "state", "{(fa37JncCHr, ca),(yDsbzayy4c, ontario)}") ;

  status = update(db, status, "{ $id: fa37JncCHr, $country: us, $city: miami }") ;
  status = update(db, status, "{ $id: yDsbzayy4c, $country: canada, $city: windsor }") ;
  status = update(db, status, "{ $id: fa37JncCHr, $country: canada, $city: sanfransisco }") ;
  status = update(db, status, "{ $id: yDsbzayy4c, $country: us, $city: toronto }") ;

  read_db(db, status) ;
  std::cout << "End of test update" << std::endl ;
*/
/*
  // Test styx
  std::cout << "Test styx" << std::endl ;
  std::string attUp_styx1 = "{ $id: 1, $name: Dee, $age: 23 }" ;
  std::string attUp_styx2 = "{ $country: us, $state: ca, $city: sanfransisco }" ;
  std::string attUp_styx3 = "{ $country: us, $state: ca, $city: toronto }" ;
  std::string attUp_styx4 = "{ $country: canada, $state: ca, $city: toronto }" ;
  std::string attUp_styx5 = "{ $country: canada, $state: alberta, $city: toronto }" ;
  std::string attUp_styx6 = "{ $country: canada, $state: ontario, $city: toronto }" ;

  bool result_styx1 = styx( attUp_styx1 ) ;
  bool result_styx2 = styx( attUp_styx2 ) ;
  bool result_styx3 = styx( attUp_styx3 ) ;
  bool result_styx4 = styx( attUp_styx4 ) ;
  bool result_styx5 = styx( attUp_styx5 ) ;
  bool result_styx6 = styx( attUp_styx6 ) ;

  std::cout << "attempted update: " << attUp_styx1 << std::endl ;
  std::cout << "styx result: " << result_styx1 << std::endl ;
  std::cout << "attempted update: " << attUp_styx2 << std::endl ;
  std::cout << "styx result: " << result_styx2 << std::endl ;
  std::cout << "attempted update: " << attUp_styx3 << std::endl ;
  std::cout << "styx result: " << result_styx3 << std::endl ;
  std::cout << "attempted update: " << attUp_styx4 << std::endl ;
  std::cout << "styx result: " << result_styx4 << std::endl ;
  std::cout << "attempted update: " << attUp_styx5 << std::endl ;
  std::cout << "styx result: " << result_styx5 << std::endl ;
  std::cout << "attempted update: " << attUp_styx6 << std::endl ;
  std::cout << "styx result: " << result_styx6 << std::endl ;

  std::cout << "End of test styx" << std::endl ;
*/
/*
  // Test getOntoMap
  std::cout << "Test getOntoMap" << std::endl ;
  std::string path_getOntoMap = "./ontology.json" ;
  std::map< std::string, std::vector<std::string> > myMap_getOntoMap = getOntoMap ( path_getOntoMap) ;
  // Iterate through all elements in std::map
  std::map<std::string, std::vector<std::string>>::iterator it = myMap_getOntoMap.begin();
  while(it != myMap_getOntoMap.end())
  {
    //std::cout << it->first << " :: " << it->second << std::endl ;
    std::cout << "key: " << it->first << ", vector size = " << it->second.size() << std::endl ;
    it++ ;
  }
  std::cout << "End of test getOntoMap" << std::endl ;
*/
/*
  // delete on pred
  std::cout << "Test deletOn_pred" << std::endl ;
  status = update_col_atomic(db, status, "name", "{(1, Dee), (2, Kay)}") ;
  status = update_col_atomic(db, status, "age",  "{(1, 22), (2, 19)}") ;
  status = update_col_atomic(db, status, "favNum",  "{(1, 25), (2, 50)}") ;

  //std::string query_deleteOn_pred = "{ () : ($name _gte Dee) }" ;
  //std::string query_deleteOn_pred = "{ () : ($favNum _gte 25) }" ;
  //std::string query_deleteOn_pred = "{ () : ($name _lte Kay) }" ;
  std::string query_deleteOn_pred = "{ () : ($favNum _lte 49) }" ;

  status = deleteOn_pred(db, status, query_deleteOn_pred) ;
  std::cout << "End of test deleteOn_pred" << std::endl ;
  read_db(db, status) ;
  status = delete_col(db, status, "age") ;
  status = delete_col(db, status, "name") ;
  status = delete_col(db, status, "favNum") ;
*/
/*
  // getIDList
  std::cout << "Test getIDList" << std::endl ;
  status = update_col_atomic(db, status, "name", "{(1, Dee), (2, Kay)}") ;
  status = update_col_atomic(db, status, "age",  "{(1, 22), (2, 19)}") ;
 
  std::string query_getIDList2 = "{ () : ($name _lte E) }" ;
  std::cout << "query = " << query_getIDList2 << std::endl ;
 
  getIDList( db, status, query_getIDList2 ) ;
  std::cout << "End of test getIDList" << std::endl ;
  status = delete_col(db, status, "f1") ;
  status = delete_col(db, status, "f2") ;
  status = delete_col(db, status, "f3") ;
*/
/*
  // delete on id
  std::cout << "Test deletOn_id" << std::endl ;
  status = update_col_atomic(db, status, "name", "{(1, Dee), (2, Kay)}") ;
  status = update_col_atomic(db, status, "age",  "{(1, 22), (2, 19)}") ;
  status = deleteOn_id(db, status, "4") ;
  std::cout << "End of test deleteOn_id" << std::endl ;
  read_db(db, status) ;
  status = delete_col(db, status, "age") ;
  status = delete_col(db, status, "name") ;
*/
/*
  // insert
  std::cout << "Test insert" << std::endl ;
  status = update_col_atomic(db, status, "name", "{(1, Dee), (2, Kay)}") ;
  status = update_col_atomic(db, status, "age",  "{(1, 22), (2, 19)}") ;
  status = insert(db, status, "{ $name: Dee, $age: 23 }") ;
  std::cout << "End of test insert" << std::endl ;
  read_db(db, status) ;
  status = delete_col(db, status, "age") ;
  status = delete_col(db, status, "name") ;
*/
/*
  // update
  std::cout << "Test update" << std::endl ;
  status = update_col_atomic(db, status, "name", "{(1, Dee), (2, Kay)}") ;
  status = update_col_atomic(db, status, "age",  "{(1, 22), (2, 19)}") ;
  status = update(db, status, "{ $id: 1, $name: Dee, $age: 23 }") ;
  std::cout << "End of test update" << std::endl ;
*/
/*
  // getArgs
  std::cout << "Test getArgs" << std::endl ;
  std::string row_getArgs = "{ $id: 1, $name: Dee, $age: 22 }" ;
  std::vector<std::string> argsList = getArgs( row_getArgs ) ;
  for(unsigned int i = 0 ; i < argsList.size() ; i++)
  {
    std::cout << argsList[i] << std::endl ;
  }
  std::cout << "End of test getArgs" << std::endl ;
*/
/*
  // read
  std::cout << "Test read" << std::endl ;
  status = update_col_atomic(db, status, "h", "{ (1,5.1), (2,5.2), (3,1.2), (4, 3.0),  (100, 5.0) }") ;
  assert(status.ok()) ;
  status = update_col_atomic(db, status, "age", "{ (1,25), (2,50), (4, 100), (59, 111) }") ;
  status = update_col_atomic(db, status, "name", "{ (1,Kat), (2,Kit), (3,Kay), (4, Kai) }") ;
  status = update_col_atomic(db, status, "f4", "{ (3,a), (4,b), (100,c), (59, e), (112,d) }") ;
  //std::string query_read = "{ (h, age, name) : ($age _gte 25), ($h _lte 5.1) }" ;
  //std::string query_read = "{ (h, age, name) : ($age _gte 25) }" ;
  //std::string query_read = "{ (h, age, name) : ($h _lte 5.0) }" ;
  std::string query_read = "{ (h, age, name, f4) : }" ;
  read( db, status, query_read ) ;
  std::cout << "End of test read" << std::endl ;
  status = delete_col(db, status, "h") ;
  status = delete_col(db, status, "age") ;
  status = delete_col(db, status, "name") ;
  status = delete_col(db, status, "f4") ;
*/
/*
  // isNumber
  std::cout << "Test isNumber" << std::endl ;
  printf((isNumber("100")) ? "true" : "false") ;
  printf("\n") ;
  printf((isNumber("10a")) ? "true" : "false");
  printf("\n") ;
  std::cout << "End of test isNumber" << std::endl ;

  // AND
  std::cout << "Test AND" << std::endl ;
  std::vector<std::string> idList_AND ;
  idList_AND.push_back("1") ;
  idList_AND.push_back("2") ;
  idList_AND.push_back("1") ;
  idList_AND.push_back("3") ;
  std::vector<std::string> results_AND = AND( idList_AND, 2 ) ;
  for(unsigned int i = 0 ; i < results_AND.size() ; i++)
    std::cout << results_AND[i] << std::endl ;
  std::cout << "End of test AND" << std::endl ;
*/
/*
  // getIDList
  std::cout << "Test getIDList" << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,25), (2,50), (3,75), (4, 100), (10, 8) }") ;
  assert(status.ok()) ;
  status = update_col_atomic(db, status, "f2", "{ (1,25), (2,50), (3,75), (4, 100) }") ;
  status = update_col_atomic(db, status, "f3", "{ (1,Kat), (2,Kit), (3,Kay), (4, Kai) }") ;
  //std::string query_getIDList = "{ (f1, f2, f3) : ($f1 _gte 26), ($f2 _lte 50) }" ;
  std::string query_getIDList = "{ (f1, f2, f3) : }" ;

  std::vector<std::string> keyList_getIDList = getKeyList(query_getIDList) ;
  std::cout << "keyList_getIDList :" << std::endl ;
  for(unsigned int i = 0 ; i < keyList_getIDList.size() ; i++)
  {
    std::cout << keyList_getIDList[i] << std::endl ;
  }
  std::vector<std::string> parsedQuery_getIDList = parseRead(query_getIDList) ;
  std::vector<std::string> predList_getIDList ;
  // Get list of predicates
  for(unsigned int i = 1 ; i < parsedQuery_getIDList.size() ; i++)
    predList_getIDList.push_back( parsedQuery_getIDList[i] ) ;
  std::cout << "predList_getIDList :" << std::endl ;
  for(unsigned int i = 0 ; i < predList_getIDList.size() ; i++)
  {
    std::cout << predList_getIDList[i] << std::endl ;
  }

  getIDList( db, status, query_getIDList ) ;
  std::cout << "End of test getIDList" << std::endl ;
  status = delete_col(db, status, "f1") ;
  status = delete_col(db, status, "f2") ;
  status = delete_col(db, status, "f3") ;
*/

  // getData
  std::cout << "Test getData" << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  status = update_col_atomic(db, status, "f2", "{ (1,Kat), (2,Kit), (3,Kay), (4, Kai) }") ;
  assert(status.ok()) ;
  std::vector<std::string> keyList_getData ;
  std::vector<std::string> idList_getData ;
  keyList_getData.push_back("f1") ;
  idList_getData.push_back("2") ;
  idList_getData.push_back("3") ;
  idList_getData.push_back("1") ;
  getData(db, status, "count", keyList_getData, idList_getData) ;

  read(db, status, "{ _sum(f1) }") ;

  keyList_getData.push_back("f2") ;
  getData(db, status, "", keyList_getData, idList_getData) ;
  status = delete_col(db, status, "f1") ;
  status = delete_col(db, status, "f2") ;
  std::cout << "End of test getData" << std::endl ;
/*
  // getData_Agg
  std::cout << "Test getData_Agg" << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  std::vector<std::string> keyList_getData_Agg ;
  std::vector<std::string> idList_getData_Agg ;
  keyList_getData_Agg.push_back("f1") ;
  idList_getData_Agg.push_back("2") ;
  idList_getData_Agg.push_back("3") ;
  idList_getData_Agg.push_back("1") ;
  std::cout << "AVG:" << std::endl ;
  getData_Agg(db, status, "avg", keyList_getData_Agg, idList_getData_Agg) ;
  std::cout << "MIN:" << std::endl ;
  getData_Agg(db, status, "min", keyList_getData_Agg, idList_getData_Agg) ;
  std::cout << "MAX:" << std::endl ;
  getData_Agg(db, status, "max", keyList_getData_Agg, idList_getData_Agg) ;
  std::cout << "SUM:" << std::endl ;
  getData_Agg(db, status, "sum", keyList_getData_Agg, idList_getData_Agg) ;
  std::cout << "COUNT:" << std::endl ;
  getData_Agg(db, status, "count", keyList_getData_Agg, idList_getData_Agg) ;
  status = delete_col(db, status, "f1") ;
  std::cout << "End of test getData_Agg" << std::endl ;

  // getData_noAgg
  std::cout << "Test getData_noAgg" << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  status = update_col_atomic(db, status, "f2", "{ (1,101), (3,103) }") ;
  assert(status.ok()) ;
  std::vector<std::string> keyList_getData_noAgg ;
  std::vector<std::string> idList_getData_noAgg ;
  keyList_getData_noAgg.push_back("f1") ;
  keyList_getData_noAgg.push_back("f2") ;
  idList_getData_noAgg.push_back("2") ;
  idList_getData_noAgg.push_back("3") ;
  idList_getData_noAgg.push_back("1") ;
  getData_noAgg(db, status, keyList_getData_noAgg, idList_getData_noAgg) ;
  status = delete_col(db, status, "f1") ;
  status = delete_col(db, status, "f2") ;
  std::cout << "End of test getData_noAgg" << std::endl ;

  // COUNT
  std::cout << "Test COUNT" << std::endl ;
  std::vector<std::string> keyList5 ;
  std::vector<std::string> idList5 ;
  keyList5.push_back("f1") ;
  idList5.push_back("2") ;
  idList5.push_back("3") ;
  idList5.push_back("4") ;
  double result9 = count(db, status, keyList5, idList5 ) ;
  std::cout << "result9 = " << result9 << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  double result10 = count(db, status, keyList5, idList5) ;
  std::cout << "result10 = " << result10 << std::endl ;
  
  status = delete_col(db, status, "f1") ;
  std::cout << "End of test COUNT" << std::endl ;

  // SUM
  std::cout << "Test SUM" << std::endl ;
  std::vector<std::string> keyList4 ;
  std::vector<std::string> idList4 ;
  keyList4.push_back("f1") ;
  idList4.push_back("2") ;
  idList4.push_back("3") ;
  idList4.push_back("4") ;
  double result7 = sum(db, status, keyList4, idList4 ) ;
  std::cout << "result7 = " << result7 << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  double result8 = sum(db, status, keyList4, idList4) ;
  std::cout << "result8 = " << result8 << std::endl ;
  
  status = delete_col(db, status, "f1") ;
  std::cout << "End of test SUM" << std::endl ;

  // MAX
  std::cout << "Test MAX" << std::endl ;
  std::vector<std::string> keyList3 ;
  std::vector<std::string> idList3 ;
  keyList3.push_back("f1") ;
  idList3.push_back("2") ;
  idList3.push_back("3") ;
  idList3.push_back("4") ;
  double result5 = max(db, status, keyList3, idList3 ) ;
  std::cout << "result5 = " << result5 << std::endl ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  double result6 = max(db, status, keyList3, idList3) ;
  std::cout << "result6 = " << result6 << std::endl ;
  
  status = delete_col(db, status, "f1") ;
  std::cout << "End of test MAX" << std::endl ;

  // MIN
  std::cout << "Test MIN" << std::endl ;
  std::vector<std::string> keyList2 ;
  std::vector<std::string> idList2 ;
  keyList2.push_back("f1") ;
  idList2.push_back("2") ;
  idList2.push_back("3") ;
  idList2.push_back("4") ;
  double result3 = min(db, status, keyList2, idList2 ) ;
  std::cout << "result3 = " << result3 << std::endl ;
  
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  double result4 = min(db, status, keyList2, idList2) ;
  std::cout << "result4 = " << result4 << std::endl ;
  
  status = delete_col(db, status, "f1") ;
  std::cout << "End of test MIN" << std::endl ;

  // AVG
  std::cout << "Test AVG" << std::endl ;
  std::vector<std::string> keyList1 ;
  std::vector<std::string> idList1 ;
  keyList1.push_back("f1") ;
  idList1.push_back("3") ;
  idList1.push_back("4") ;
  double result1 = avg(db, status, keyList1, idList1 ) ;
  std::cout << "result1 = " << result1 << std::endl ;

  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;

  double result2 = avg(db, status, keyList1, idList1) ;  
  std::cout << "result2 = " << result2 << std::endl ;

  status = delete_col(db, status, "f1") ;
  std::cout << "End of test avg" << std::endl ;

  // Test lte
  std::cout << "Test lte" << std::endl ;
  std::string lhs8 = "f1" ;
  std::string rhs8 = "22" ;
  std::vector<std::string> tupleList11 = lte(db, status, lhs8, rhs8) ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  std::vector<std::string> tupleList12= lte(db, status, lhs8, rhs8) ;  std::cout << "tupleList12.size() = " << tupleList12.size() << std::endl ;
  for(unsigned int i = 0 ; i < tupleList12.size() ; ++i)
    std::cout << tupleList12[i] << std::endl ;
  
  status = delete_col(db, status, lhs8) ;
  std::cout << "End of test lte" << std::endl ;

  // Test lt
  std::cout << "Test lt" << std::endl ;
  std::string lhs7 = "f1" ;
  std::string rhs7 = "22" ;
  std::vector<std::string> tupleList9 = lt(db, status, lhs7, rhs7) ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  std::vector<std::string> tupleList10= lt(db, status, lhs7, rhs7) ;  std::cout << "tupleList10.size() = " << tupleList10.size() << std::endl ;
  for(unsigned int i = 0 ; i < tupleList10.size() ; ++i)
    std::cout << tupleList10[i] << std::endl ;
  
  status = delete_col(db, status, lhs7) ;
  std::cout << "End of test lt" << std::endl ;

  // Test gte
  std::cout << "Test gte" << std::endl ;
  std::string lhs6 = "f1" ;
  std::string rhs6 = "22" ;
  std::vector<std::string> tupleList7 = gte(db, status, lhs6, rhs6) ;  
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  std::vector<std::string> tupleList8 = gte(db, status, lhs6, rhs6) ;  std::cout << "tupleList8.size() = " << tupleList8.size() << std::endl ;
  for(unsigned int i = 0 ; i < tupleList8.size() ; ++i)
    std::cout << tupleList8[i] << std::endl ;
  
  status = delete_col(db, status, lhs6) ;
  std::cout << "End of test gte" << std::endl ;

  // Test gt
  std::cout << "Test gt" << std::endl ;
  std::string lhs5 = "f1" ;
  std::string rhs5 = "0" ;
  std::vector<std::string> tupleList5 = gt(db, status, lhs5, rhs5) ;  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  std::vector<std::string> tupleList6 = gt(db, status, lhs5, rhs5) ;  std::cout << "tupleList6.size() = " << tupleList6.size() << std::endl ;
  for(unsigned int i = 0 ; i < tupleList6.size() ; ++i)
  std::cout << tupleList6[i] << std::endl ;
  
  status = delete_col(db, status, lhs5) ;
  std::cout << "End of test gt" << std::endl ;

  // Test ne
  std::cout << "Test ne" << std::endl ;
  std::string lhs4 = "f1" ;
  std::string rhs4 = "21" ;
  std::vector<std::string> tupleList3 = ne(db, status, lhs4, rhs4) ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;
  
  std::vector<std::string> tupleList4 = ne(db, status, lhs4, rhs4) ;
  std::cout << "tupleList4.size() = " << tupleList4.size() << std::endl ;
  for(unsigned int i = 0 ; i < tupleList4.size() ; ++i)
    std::cout << tupleList4[i] << std::endl ;
  
  status = delete_col(db, status, lhs4) ;
  std::cout << "End of test ne" << std::endl ;

  // Test eq
  std::cout << "Test eq" << std::endl ;
  std::string lhs3 = "f1" ;
  std::string rhs3 = "21" ;
  std::vector<std::string> tupleList1 = eq(db, status, lhs3, rhs3) ;
  status = update_col_atomic(db, status, "f1", "{ (1,21), (2,22), (3,21), (4, 50) }") ;
  assert(status.ok()) ;

  std::vector<std::string> tupleList2 = eq(db, status, lhs3, rhs3) ;
  std::cout << "tupleList2.size() = " << tupleList2.size() << std::endl ;
  for(unsigned int i = 0 ; i < tupleList2.size() ; ++i)
    std::cout << tupleList2[i] << std::endl ;

  status = delete_col(db, status, lhs3) ;
  std::cout << "End of test eq" << std::endl ;

  // Test getValue
  std::cout << "Test getValue" << std::endl ;
  std::string myValue = getValue("(1, Kit)") ;
  std::cout << "myValue = " << myValue << std::endl ;
  std::cout << "End of test getValue" << std::endl ;

  // Test getID
  std::cout << "Test getID" << std::endl ;
  std::string myID = getID("(1, Kit)") ;
  std::cout << "myID = " << myID << std::endl ;
  std::cout << "End of test getID" << std::endl ;

  // Test parseList
  std::cout << "Test parseList" << std::endl ;
  std::vector<std::string> tupleList = parseList("{ (1, Kit), (2, Kay), (3, Kat) }") ;
  for(unsigned int i = 0 ; i < tupleList.size() ; ++i)
    std::cout << tupleList[i] << std::endl ;
  std::cout << "End of test parseList" << std::endl ;

  // Test getKeyList
  std::cout << "Test getKeyList" << std::endl ;
  std::string q1 = "{ (f1, f2, f3) : ($f1 _gt 0), ($f2 _lt 100) }" ;
  std::vector<std::string> keyList = getKeyList(q1) ;
  for(unsigned int i = 0 ; i < keyList.size() ; ++i)
    std::cout << keyList[i] << std::endl ;
  std::cout << "End of test getKeyList" << std::endl ;

  // Test parseRead
  std::string query1 = "{ (f1, f2, f3) : ($f1 _gt 0), ($f2 _lt 100) }" ;
  std::vector<std::string> val = parseRead(query1) ;
  for(unsigned int i = 0 ; i < val.size() ; ++i)
    std::cout << val[i] << std::endl ;

  // Test getReturnOp
  std::string query2 = "{ _AVG(f1) : ($f1 _gte 0), ($f2 _lt 100) }" ;
  std::vector<std::string> val2 = parseRead(query2) ;
  std::string aggOp2 = getReturnOp(query2) ;
  std::cout << "aggOp2: " << aggOp2 << std::endl ;

  // Test getLHS
  std::string pred1 = val2[1] ;
  std::string lhs1 = getLHS(pred1) ;
  std::cout << "lhs1: " << lhs1 << std::endl ;
  std::string pred2 = val2[2] ;
  std::string lhs2 = getLHS(pred2) ;
  std::cout << "lhs2: " << lhs2 << std::endl ;

  // Test getRHS
  std::string rhs1 = getRHS(pred1) ;
  std::cout << "rhs1: " << rhs1 << std::endl ;
  std::string rhs2 = getRHS(pred2) ;
  std::cout << "rhs2: " << rhs2 << std::endl ;

  // Test getOp
  std::string op1 = getOp(pred1) ;
  std::cout << "op1: " << op1 << std::endl ;
  std::string op2 = getOp(pred2) ;
  std::cout << "op2: " << op2 << std::endl ;

  // Test getFieldList
  std::cout << "Test getFieldList:" << std::endl ;
  std::vector<std::string> l1 = getFieldList(val[0]) ;
  std::cout << "Size of l1: " << l1.size() << std::endl ;
  for(unsigned int i = 0 ; i < l1.size() ; ++i)
    std::cout << l1[i] << std::endl ;

  // Read DB
  read_db(db, status) ;

  // Update col
  std::string key1 = "name" ;
  std::string value1 = "{ (1, Kat), (2, Kit), (3, Kay) }" ;

  status = update_col_atomic(db, status, key1, value1) ;

  // Read DB
  read_db(db, status) ;

  // Delete col
  status = delete_col(db, status, key1) ;

*/
  // Read DB
  //read_db(db, status) ;

  std::cout << "You're awesome! =D" << std::endl ;

  // Delete DB
  delete_db(db) ;

  return 0 ;
}
