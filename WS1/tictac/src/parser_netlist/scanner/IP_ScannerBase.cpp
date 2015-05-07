/*
 * IP_ScannerBase.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: benk
 */

#include "IP_ScannerBase.hpp"
#include "sim_utils.hpp"

bool IP_ScannerBase::matchStringToken(std::string &tokenString,
                       std::vector<std::string> &delims,
                       int &ind)
{
  // transfor the token string to lower case (if no case sensitive)
  // and try to match to the array strings
  bool ret = false;
  ind = -1;
  std::string tmpLower = toLower(tokenString);
  for (int i = 0; i < (int)delims.size(); i++)
    {
      if (tmpLower == delims[i]) {
        ind = i;
        ret = true;
      }
    }
  return ret;
}


bool IP_ScannerBase::matchEndStringToken(std::string &tokenString,
                       std::vector<std::string> &delims,
                       int &ind)
{
  // transfor the token string to lower case (if no case sensitive)
  // and try to match to the array strings
  bool ret = false;
  ind = -1;
  std::string tmpLower = toLower(tokenString);
  for (int i = 0; i < (int)delims.size(); i++)
    {
      if (tmpLower.size() >= delims.size())
        {
          std::string ending = tmpLower.substr(tmpLower.size()-delims.size());
          if (tmpLower == delims[i]) {
            ind = i;
            ret = true;
          }
        }
    }
  return ret;
}


bool IP_ScannerBase::matchCharToken(char tokenChar,
                       std::vector<char> &delims,
                       int &ind)
{
  // math the charcters to all the possible characters
  bool ret = false;
  ind = -1;
  for (int i = 0; i < (int)delims.size(); i++)
    {
      if (tokenChar == delims[i]) {
        ind = i;
        ret = true;
      }
    }
  return ret;
}

std::string IP_ScannerBase::toLower(const std::string data)
{
  // an internal method to transform to everything to lower case (if no case sensitive)
  std::string tmp = data;
  std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
  return std::string(tmp);
}
