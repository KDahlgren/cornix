/*
 * filename   :    cornix_parser.h
 * author     :    Kathryn Dahlgren
 * date       :    27 Feb 2016
 * Description:
 *     Definitions of Cornix 
 *     Create, Read, Update, and Delete ops.
 */

#ifndef CORNIX_PARSER_H
#define CORNIX_PARSER_H

#include "cornix_includes.h"

// parseRead
// Input query
// Output return vector of () delimited components
// The zeroth argument of the vector is the comma-delimited list
// of fields in the select clause.
std::vector<std::string> parseRead(std::string query)
{
  std::vector<std::string> args ;

  bool skipFlag = true ;
  std::string newTuple = "" ;
  for (unsigned int i = 0 ; i < query.length() ; i++)
  {
    if( query[i] == '(' )
    {
      skipFlag = false ;
      continue ;
    }
    else if( query[i] == ')' )
    {
      skipFlag = true ;
      args.push_back(newTuple) ;
      newTuple = "" ;
    }

    if ( skipFlag == false )
      newTuple += query[i] ;
  }

  return args ;
}

// getReturnOp
// Input select clause of query
// Output the return operator if one exists
// Assume only one aggregate operator per
// select clause.
std::string getReturnOp(std::string query)
{
  bool skipFlag = true ;
  std::string aggOp = "" ;
  for(unsigned int i = 0 ; i < query.length() ; i++)
  {
    if( query[i] == '_' )
    {
      skipFlag = false ;
      continue ;
    }
    else if( query[i] == '(' )
    { 
      skipFlag = true ;
      aggOp += "" ;
    }
    else if( query[i] == ':')
      break ;

    if ( skipFlag == false )
      aggOp += query[i] ;
  }

  return aggOp ;
}

// getLHS
// Assumes 1 field on LHS of operator.
std::string getLHS(std::string pred)
{
  bool skipFlag = true ;
  std::string lhs = "" ;
  for(unsigned int i = 0 ; i < pred.length() ; i++)
  {
    if( pred[i] == '$' )
    {
      skipFlag = false ;
      continue ;
    }
    else if( pred[i] == ' ' )
    {
      skipFlag = true ;
      lhs += "" ;
    }
    else if( pred[i] == ':')
      break ;

    if ( skipFlag == false )
      lhs += pred[i] ;
  }

  return lhs ;
}

//getRHS
// Assumes 1 field on RHS of operator.
std::string getRHS(std::string pred)
{
  bool skipFlag = true ;
  std::string rhs = "" ;
  for(unsigned int i = 0 ; i < pred.length() ; i++)
  {
    if( pred[i] == '_' )
    {
      skipFlag = false ;
      if(pred[i+3] == ' ')
        i += 3 ;
      else
        i += 4 ;
      continue ;
    }

    if ( skipFlag == false )
      rhs += pred[i] ;
  }

  return rhs ;
}

//getOp
std::string getOp(std::string pred)
{
  bool skipFlag = true ;
  std::string op = "" ;
  for(unsigned int i = 0 ; i < pred.length() ; i++)
  {
    if( pred[i] == '_' )
    {
      skipFlag = false ;
      continue ;
    }
    else if(pred[i] == ' ')
      skipFlag = true ;

    if ( skipFlag == false )
      op += pred[i] ;
  }

  return op ;
}

//getFieldList
// output list of fields in the select clause
std::vector<std::string> getFieldList(std::string clause)
{
  std::vector<std::string> list ;
  std::string field = "" ;
  for(unsigned int i = 0 ; i < clause.length() ; i++)
  {
    if( clause[i] != ',' && clause[i] != ' ')
    {
      field += clause[i] ;
    }
    else if(clause[i] == ',')
    {
      list.push_back(field) ;
      field = "" ;
    }
    if (i == (clause.length()-1))
      list.push_back(field) ;
  }
  return list ;
}

// parseList
// Input column value at some key
// output list of tuples in the key value
std::vector<std::string> parseList (std::string value)
{
  std::vector<std::string> tupleList ;
  bool skipFlag = true ;
  std::string tuple = "" ;
  for(unsigned int i = 0 ; i < value.length() ; i++)
  {
    if( value[i] == '(' )
      skipFlag = false ;
    else if( value[i] == ')' )
    {
      skipFlag = true ;
      tuple += value[i] ;
      tupleList.push_back(tuple) ;
      tuple = "" ;
    }
    if( skipFlag == false )
      tuple += value[i] ;
  }
  return tupleList ;
}

// getID
// input tuple
// output first arg of tuple
std::string getID (std::string tuple)
{
  bool skipFlag = true ;
  std::string id = "" ;
  for(unsigned int i = 0 ; i < tuple.length() ; i++)
  {
    if( tuple[i] == '(' )
    {
      skipFlag = false ;
      continue ;
    }
    else if( tuple[i] == ',' )
    {
      break ;
    }
    if( skipFlag == false )
      id += tuple[i] ;
  }
  return id ;
}

// getValue
// input tuple
// output second arg of tuple
std::string getValue (std::string tuple)
{
  bool skipFlag = true ;
  std::string value = "" ;
  for(unsigned int i = 0 ; i < tuple.length() ; i++)
  {
    if( tuple[i] == ',' )
    {
      skipFlag = false ;
      if (tuple[i+1] == ' ')
        i += 1 ;
      continue ;
    }
    else if( tuple[i] == ')' )
    {
      break ;
    }
    if( skipFlag == false )
      value += tuple[i] ;
  }
  return value ;
}

// getArgs
// input update or insert clause
// output list of tuples (field, value)
std::vector<std::string> getArgs ( std::string row )
{
  std::vector<std::string> argList ;
  bool skipFlag = true ;
  std::string base = "(" ;
  std::string arg = base ;
  for(unsigned int i = 0 ; i < row.length() ; i++)
  {
    if( row[i] == '$' )
    {
      skipFlag = false ;
      continue ;
    }
    else if( row[i] == ',' )
    {
      arg += ")" ;
      argList.push_back(arg) ;
      arg = base ;
      skipFlag = true ;
    }
    else if( (row[i] == '}') )
    {
      arg += ")" ;
      argList.push_back(arg) ;
      arg = base ;
      skipFlag = true ;
    }
    else if( row[i] == ':')
    {
      arg += "," ;
      continue ;
    }
    else if( row[i] == ' ')
    {
      continue ;
    }
    else if( row[i] != ':' && row[i] != ' ' && row[i] != '}' && (row[i] != '{') )
    {
      skipFlag = false ;
    }
    if( skipFlag == false )
      arg += row[i] ;
  }

  return argList ;
}

// getOntoMap
// input path to ontology
// output map of ontology contents
std::map< std::string, std::vector<std::string> > getOntoMap ( std::string ontoPath )
{
  std::map< std::string, std::vector<std::string> > ontoMap ;

  // Open file
  // http://www.cplusplus.com/doc/tutorial/files/
  std::string line ;
  std::ifstream ontoFile ( ontoPath ) ;
  if( ontoFile.is_open() )
  {
    while ( std::getline( ontoFile, line ) )
    {
      if( line != "{" && line != "}")
      {
      // key for line
      std::string key = "" ;
      // value for line
      std::vector<std::string> conceptList ;

      bool skipFlag = true ;
      bool isKey = true ;
      std::string newVal = "" ;
      for (unsigned int i = 0 ; i < line.length() ; i++)
      {
        if( line[i] == '"' && line[i-1] == ' ')
        {
            skipFlag = false ;
            continue ;
          }
          else if( line[i] == '"' && line[i+1] == ':' )
          {
            // Just got a key
            isKey = false ;
            skipFlag = true ;
            continue ;
          }
          else if( line[i] == '"' && (line[i+1] == ',' || line[i+1] == ']') )
          {
            // Just got a value
            // Save to conceptList
            conceptList.push_back( newVal ) ;
            // Reset newVal
            newVal = "" ;
            skipFlag = true ;
            continue ;
          }
          else if( line[i] == '"' && line[i-1] == '[')
          {
            skipFlag = false ;
            continue ;
          }
          if( skipFlag == false )
          {
            if( isKey == true )
              key += line[i] ;
            else
              newVal += line[i] ;
          }
        }
        // Save key and conceptList in map
        ontoMap.insert( std::make_pair(key, conceptList) ) ;
      }
    }
    ontoFile.close();
  }
  else
    std::cout << "Unable to open file : " << ontoPath << std::endl ; 

  return ontoMap ;
}

#endif
