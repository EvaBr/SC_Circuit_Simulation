/*
 * IP_SIMetrixParser.cpp
 *
 *  Created on: Mar 14, 2015
 *      Author: benk
 */

#include "IP_SIMetrixParser.hpp"
#include "IP_XCall.hpp"
#include "IP_Param.hpp"
#include "IP_ScannerBase.hpp"

IP_SIMetrixParser::IP_SIMetrixParser() : IP_ParserBase()
{
  // nothing to do at the moment
}

// =============== OUTPUT matching ============
bool IP_SIMetrixParser::matchOutput(const std::vector<IP_Token> &tokens)
{
  IP_Token &firstToken = tokens_[0];
  // match for output
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".graph"
      // TODO: this is just to swallow this ".simulation" only for SIMetrix
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".simulator"
     )
    return true;
  else
    return false;
}

// =============== X-CALL BUILDING ============
boost::shared_ptr<IP_XCall>               IP_SIMetrixParser::buildXCall()
{
  int n = (int) tokens_.size(), i = 1;
  int nrPinn = 0;
  bool hadSubckt = false;
  std::string subcirName = ":=| :-("; // to signal error
  std::vector< boost::shared_ptr<IP_Param> > xparams(0);

  // test if we should have at least 3 tokens
  if (tokens_.size() < 3)
    reportError("Not enough tokens for X-call", IP_Parser_NOFATAL, tokens_[0]);

  // "pinnames:" delete from this string all the tokens since they are obsolete
   while ( (tokens_[i].getValue() != "pinnames:") && (i < n)) { i++; }
   tokens_.resize(i);
   n = (int) tokens_.size();

   i = 1;
  // now loop through each token and see which are pinns and which are params of the subckt
  while (i < n)
    {
      // skip the : after the subcircuitname
      while (tokens_[i].getValue() == ":") { tokens_.erase(tokens_.begin() + i); n--; }

      std::vector<IP_Token> tokensTmp(0); // required only for parameter declaration
      if (    (i + 1 < n)
           && (tokens_[i + 1].getTokenType() == TK_EQUAL)
         )
        { // here we just call the method to parse the parameter sequence
          xparams = buildParameterSequence( i, (int) tokens_.size(), tokens_);
          // break the loop, since we are finished
          if ( i < 2 )
            reportError("At least one pinn is required for an X-call", IP_Parser_NOFATAL, tokens_[i]);
          else {
            subcirName = tokens_[i-1].getValue();
            hadSubckt = true;
          }
          break;
        }
      else
        { // here we have simple port/pin, and just count the pinn numbers
          i++; nrPinn++;
        }
    }

  // if no parameters are given, then we will end up here, than the last one is the subcircuit name
  if (!hadSubckt)
    subcirName = tokens_[tokens_.size()-1].getValue();
  // debug the name of the called subcircuit name
  SIM_PRINT_L6(verb(), " IP_XCall name=" << subcirName);

  // build the X-call
  return boost::shared_ptr<IP_XCall>(new  IP_XCall(tokens_,
      nrPinn-1, subcirName, xparams));
}
