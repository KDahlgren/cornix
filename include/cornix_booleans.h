/*
 * filename   :    cornix_booleans.h
 * author     :    Kathryn Dahlgren
 * date       :    28 Feb 2016
 */

#ifndef CORNIX_BOOLEANS_H
#define CORNIX_BOOLEANS_H

// AND
std::vector<std::string> AND( std::vector<std::string> idList, int numPreds )
{
  std::vector<std::string> newIDList ;

  for(unsigned int i = 0 ; i < idList.size() ; i++)
  {
    // Get current id
    std::string currID = idList[i] ;
    // Check if ID occurs multiple times
    if( count (idList.begin(), idList.end(), currID ) >= numPreds )
      newIDList.push_back( currID ) ;
  }

  // Remove duplicates
  sort( newIDList.begin(), newIDList.end() );
  newIDList.erase( unique( newIDList.begin(), newIDList.end() ), newIDList.end() );

  return newIDList ;
}

// OR
std::vector<std::string> OR( std::vector<std::string> idList )
{
  // Remove duplicates
  sort( idList.begin(), idList.end() );
  idList.erase( unique( idList.begin(), idList.end() ), idList.end() );

  return idList ;
}

#endif
