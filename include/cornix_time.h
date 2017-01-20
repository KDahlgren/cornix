/*
 * filename   :    cornix_time.h
 * author     :    Kathryn Dahlgren
 * date       :    28 Feb 2016
 * Description:
 *     Get timestamp for output filenames
 *
 * Based on: http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
 */

#ifndef CORNIX_TIME_H
#define CORNIX_TIME_H

const std::string currentDateTime()
{
  time_t curr = time(0);
  struct tm  tstruct;
  char buffer[80];
  tstruct = *localtime( &curr ) ;

  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format

  strftime(buffer, sizeof(buffer), "%d%b%Y_%H.%M.%S", &tstruct);

  return buffer ;
}

#endif
