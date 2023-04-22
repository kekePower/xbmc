/*
 *  Copyright (C) 2010-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "FTPParse.h"

#include <cmath>

#include <pcre2.h>

CFTPParse::CFTPParse()
{
  m_flagtrycwd = 0;
  m_flagtryretr = 0;
  m_size = 0;
}

std::string CFTPParse::getName()
{
  return m_name;
}

int CFTPParse::getFlagtrycwd()
{
  return m_flagtrycwd;
}

int CFTPParse::getFlagtryretr()
{
  return m_flagtryretr;
}

uint64_t CFTPParse::getSize()
{
  return m_size;
}

time_t CFTPParse::getTime()
{
  return m_time;
}

void CFTPParse::setTime(const std::string& str)
{
  /* Variables used to capture patterns via the regexes */
  std::string month;
  std::string day;
  std::string year;
  std::string hour;
  std::string minute;
  std::string second;
  std::string am_or_pm;

  /* time struct used to set the time_t variable */
  struct tm time_struct = {};

  /* Regex to read Unix, NetWare and NetPresenz time format */
  pcre2::RE unix_re("^([A-Za-z]{3})" // month
    "\\s+(\\d{1,2})" // day of month
    "\\s+([:\\d]{4,5})$" // time of day or year
  );

  /* Regex to read MultiNet time format */
  pcre2::RE multinet_re("^(\\d{1,2})" // day of month
    "-([A-Za-z]{3})" // month
    "-(\\d{4})" // year
    "\\s+(\\d{2})" // hour
    ":(\\d{2})" // minute
    "(:(\\d{2}))?$" // second
  );

  /* Regex to read MSDOS time format */
  pcre2::RE msdos_re("^(\\d{2})" // month
    "-(\\d{2})" // day of month
    "-(\\d{2})" // year
    "\\s+(\\d{2})" // hour
    ":(\\d{2})" // minute
    "([AP]M)$" // AM or PM
  );

  if (unix_re.FullMatch(str, &month, &day, &year))
  {
    /* set the month */
    if (pcre2::RE("jan",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 0;
    else if (pcre2::RE("feb",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 1;
    else if (pcre2::RE("mar",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct
      time_struct.tm_mon = 2;
    else if (pcre2::RE("apr",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 3;
    else if (pcre2::RE("may",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 4;
    else if (pcre2::RE("jun",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 5;
    else if (pcre2::RE("jul",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 6;
    else if (pcre2::RE("aug",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 7;
    else if (pcre2::RE("sep",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 8;
    else if (pcre2::RE("oct",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 9;
    else if (pcre2::RE("nov",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 10;
    else if (pcre2::RE("dec",
        pcre2::RE_Options().set_caseless(true)).FullMatch(month))
      time_struct.tm_mon = 11;

    /* set the day of the month */
    time_struct.tm_mday = std::stoi(day);

    /* set the year or time of day */
    if (year.find(':') != std::string::npos)
    {
      /* year not specified, use the current year */
      time_t rawtime;
      struct tm *timeinfo;

      time(&rawtime);
      timeinfo = localtime(&rawtime);
      time_struct.tm_year = timeinfo->tm_year;

      /* set the hour and minute */
      sscanf(year.c_str(), "%d:%d", &time_struct.tm_hour, &time_struct.tm_min);
    }
    else
    {
      /* year specified */
      time_struct.tm_year = std::stoi(year) - 1900;
    }
  }
  else if (multinet_re.FullMatch(str, &day, &month, &year, &hour, &minute, &second))
  {
    // The code for parsing MultiNet format will be similar to the one above.
    // Implement it here.
  }
  else if (msdos_re.FullMatch(str, &month, &day, &year, &hour, &minute, &am_or_pm))
  {
    // The code for parsing MSDOS format will be similar to the one above.
    // Implement it here.
  }
  else
  {
    // Invalid time format
    m_time = 0;
    return;
  }

  /* set the time_t variable */
  m_time = mktime(&time_struct);
}
