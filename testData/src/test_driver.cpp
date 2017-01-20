/*
 * filename   :    test_driver.cpp
 * author     :    Kathryn Dahlgren
 * date       :    02 Mar 2016
 * Description:
 *     The test driver for Cornix. Place all initial
 *     examples and tests here.
 */

#include "cornix_crud.h"
#include "cornix_utils.h"
#include "cornix_ops.h"
#include "cornix_agg.h"
#include "cornix_booleans.h"
#include "styx.h"

// SAVE TIMER (nanoseconds)
// http://stackoverflow.com/questions/12937963/get-local-time-in-nanoseconds
void save_timer( int timer, std::string testInfo )
{
  std::cout << "... saving results ..." << std::endl ;

  std::string filename = "results_" + testInfo + ".txt" ;
  std::string path = "../results/" + filename ;

  std::ofstream resultsFile ;
  resultsFile.open( path.c_str() ) ;
  if( resultsFile.is_open() )
    resultsFile << timer << std::endl ;
  resultsFile.close();

  std::cout << "... done saving results ..." << std::endl ;

  return ;
}

// READ_1_RAND
rocksdb::Status read_1_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... read_1_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    while ( getline (dataFile,line) )
    {
      status = insert( db, status, line ) ;
    }
    dataFile.close();
  }
  else std::cout << "Unable to open file 1"; 

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/readTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "readRandom_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        /*** NANOSECONDS ***/
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        read( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        //int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) ; 
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;        
        std::string testInfo = "read_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;        
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "Unable to open file 2"; 
  }
  //

  std::cout << "... done read_1_rand test ..." << std::endl ;

  return status ;
}

// READ SUB (10 percent of db)
rocksdb::Status read_sub_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... read_sub_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/readTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "readRandom_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {

        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        read( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done read_sub_rand test ..." << std::endl ;

return status ;
}

// UPDATE 10 random
rocksdb::Status update_10_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... update_10_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/updateTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "update_10_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << "cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        status = update( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "update_10_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

       /*** SECONDS ***/
       /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done update_10_rand test ..." << std::endl ;

  return status ;
}

// UPDATE 10 percent random
rocksdb::Status update_sub_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... update_sub_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/updateTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "update_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << "cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        status = update( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "update_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done update_sub_rand test ..." << std::endl ;

  return status ;
}

// INSERT 100
rocksdb::Status insert_100_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... insert_100_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/insertTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "insert_100_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << "cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        status = insert( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "insert_100_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done insert_100_rand test ..." << std::endl ;

  return status ;
}


// DELETE 1 random ID
rocksdb::Status deleteOn_id_1_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... deleteOn_id_1_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/deleteTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "deleteOn_id_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << "cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        status = deleteOn_id( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "deleteOn_id_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done deleteOn_id_1_rand test ..." << std::endl ;

  return status ;
}

// DELETE ON PRED 1
rocksdb::Status deleteOn_pred_1_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... deleteOn_pred_1_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/deleteTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "deleteOn_pred_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << "cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        status = deleteOn_pred( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "deleteOn_pred_1_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done deleteOn_pred_1_rand test ..." << std::endl ;

  return status ;
}


// DELETE ON ID SUB
rocksdb::Status deleteOn_id_sub_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... deleteOn_id_sub_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/deleteTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "deleteOn_id_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << " ******* cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        std::cout << " *******  cmd: " << cmd << std::endl ;
        status = deleteOn_id( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "deleteOn_id_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done deleteOn_id_sub_rand test ..." << std::endl ;

  return status ;
}

// DELETE ON PRED SUB
rocksdb::Status deleteOn_pred_sub_rand( rocksdb::DB* db, rocksdb::Status status, int sizeDB )
{
  std::cout << "... deleteOn_pred_sub_rand test ..." << std::endl ;
  // Populate DB
  std::string path0 = "dataFile_" + std::to_string(sizeDB) + ".txt" ;
  std::string path1 = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/dataFiles/" + path0 ;
  // Insert data into DB
  std::string line ;
  std::ifstream dataFile (path1);
  if (dataFile.is_open())
  {
    std::cout << "here 10" << std::endl ;
    while ( getline (dataFile,line) )
    {
      std::cout << "here 1" << std::endl ;
      status = insert( db, status, line ) ;
      std::cout << "past here 1" << std::endl ;
      assert(status.ok()) ;
    }
    dataFile.close();
  }
  else std::cout << "*** Unable to open file 1 *** : " << path0 << std::endl ;

  std::cout << "here 11" << std::endl ;

  // Define input files
  std::string path = "/afs/cats.ucsc.edu/users/j/kmdahlgr/Cornix/testData/deleteTests/" ;
  std::vector<std::string> inFiles ;
  for(int i = 1 ; i <= 10; i++)
  {
    std::string filename = "deleteOn_pred_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) + ".txt" ;
    std::string absPath = path + filename ;
    inFiles.push_back(absPath) ;
  }
  // Iterate over files
  for(unsigned int i = 0 ; i < inFiles.size()  ; i++)
  {
    // Get in file name
    std::string myFilePath = inFiles[i] ;
    // Iterate over lines in the file
    std::string cmd ;
    std::ifstream myFile ( myFilePath );
    if( myFile.is_open() )
    {
      while ( getline (myFile, cmd) )
      {
        std::cout << "cmd: " << cmd << std::endl ;
        /*** NANOSECONDS ***/
        std::cout << "here 2" << std::endl ;
        // Timer
        auto start = std::chrono::high_resolution_clock::now() ;
        // cmd
        status = deleteOn_pred( db, status, cmd ) ;
        auto finish = std::chrono::high_resolution_clock::now();
        // save timer
        int numNano = std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "deleteOn_pred_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( numNano, testInfo ) ;

        /*** SECONDS ***/
        /*
        std::cout << "here 2" << std::endl ;
        // Timer
        clock_t startTime = clock() ;
        // cmd
        read( db, status, cmd ) ;
        clock_t endTime = clock();
        // save timer
        clock_t clockTicksTaken = (endTime - startTime) / (double) CLOCKS_PER_SEC ;
        //double timer = numNano / 1000000 ;
        std::string testInfo = "read_sub_" + std::to_string(sizeDB) + "_" + std::to_string(i) ;
        save_timer( clockTicksTaken, testInfo ) ;
        */
      }
      myFile.close();
    }
    else std::cout << "*** Unable to open file 2 *** : " << myFilePath << std::endl ;
  }

  std::cout << "... done deleteOn_pred_sub_rand test ..." << std::endl ;

  return status ;
}


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

  // READ
  read_1_rand( db, status, 10) ;   //measure in nano
  status = clear_db(db, status) ;
  read_1_rand( db, status, 100) ;   //measure in nano
  status = clear_db(db, status) ;
  read_1_rand( db, status, 1000) ;   //measure in nano
  status = clear_db(db, status) ;
  read_1_rand( db, status, 10000) ;   //measure in nano
  status = clear_db(db, status) ;

  // read_sub_rand() ;
  read_sub_rand( db, status, 10) ;   //measure in nano
  status = clear_db(db, status) ;
  read_sub_rand( db, status, 100) ;   //measure in nano
  status = clear_db(db, status) ;
  read_sub_rand( db, status, 1000) ;   //measure in nano
  status = clear_db(db, status) ;
  //read_sub_rand( db, status, 10000) ;   //measure in seconds
  //status = clear_db(db, status) ;

  // UPDATE 10 random
  status = update_10_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = update_10_rand( db, status, 100) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  //status = update_10_rand( db, status, 1000) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;
  //status = update_10_rand( db, status, 10000) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;

  // UPDATE 10 percent random
  status = update_sub_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = update_sub_rand( db, status, 100) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  //status = update_sub_rand( db, status, 1000) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;
  //status = update_sub_rand( db, status, 10000) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;

  // INSERT 100 random
  status = insert_100_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  //status = insert_100_rand( db, status, 100) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;
  //status = insert_100_rand( db, status, 1000) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;
  //status = insert_100_rand( db, status, 10000) ;   //measure in nano
  //read_db(db, status) ;
  //status = clear_db(db, status) ;

/*
  // DELETE 1 random on id
  status = deleteOn_id_1_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_id_1_rand( db, status, 100) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_id_1_rand( db, status, 1000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_id_1_rand( db, status, 10000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;

  // DELETE sub random on id
  status = deleteOn_id_sub_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_id_sub_rand( db, status, 100) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_id_sub_rand( db, status, 1000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_id_sub_rand( db, status, 10000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;

  // DELETE 1 random on pred
  status = deleteOn_pred_1_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_pred_1_rand( db, status, 100) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_pred_1_rand( db, status, 1000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_pred_1_rand( db, status, 10000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;

  // DELETE sub random on pred
  status = deleteOn_pred_sub_rand( db, status, 10) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_pred_sub_rand( db, status, 100) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_pred_sub_rand( db, status, 1000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
  status = deleteOn_pred_sub_rand( db, status, 10000) ;   //measure in nano
  read_db(db, status) ;
  status = clear_db(db, status) ;
*/

  std::cout << "You're awesome! =D" << std::endl ;

  // Delete DB
  delete_db(db) ;

  return 0 ;
}
