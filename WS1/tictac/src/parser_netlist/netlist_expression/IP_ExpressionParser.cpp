/*
 * IP_ExpressionParser.cpp
 *
 *  Created on: Feb 24, 2015
 *      Author: benk
 */

#include "IP_ExpressionParser.hpp"
#include "IP_ScannerBase.hpp"
#include "CS_Number.hpp"
#include "CS_String.hpp"
#include "CS_ParameterReference.hpp"
#include "CS_AnalogConcatenation.hpp"
#include "CS_UnaryOperator.hpp"

std::vector< boost::shared_ptr<CS_ExpressionBasis> > IP_ExpressionParser::garbigeCollection_;

IP_ExpressionParser::IP_ExpressionParser( bool ignoreActualLevel )
{
  hasError_ = false;
  errorMessage_ = "";
  ignoreActualLevel_ = ignoreActualLevel;
  //this->setVerb(7);
}

boost::shared_ptr<CS_ExpressionBasis> IP_ExpressionParser::getExprForConstantValue(const ConstExpressionValue& constVal)
{
  // TODO: for error reporting this is nor optimal
  switch (constVal.valueType_){
  case REAL: {
    boost::shared_ptr<CS_ExpressionBasis> tmp = boost::shared_ptr<CS_ExpressionBasis>(new CS_Number( constVal.dval_,0, 0, 0));
    garbigeCollection_.push_back(tmp);
    return tmp;
    break;
  }
  case REAL_ARRAY: {
    CS_AnalogConcatenation* ret = new CS_AnalogConcatenation(0,0,0);
    for (MYINT i = 0; i < (MYINT)constVal.aval_.size(); i++){
        // TODO: memoty LEAK
        CS_ExpressionBasis* tmp = new CS_Number( constVal.aval_[i],0, 0, 0);
        boost::shared_ptr<CS_ExpressionBasis> tmps = boost::shared_ptr<CS_ExpressionBasis>(tmp);
        garbigeCollection_.push_back(tmps);
        ret->addExpr( tmp );
    }
    boost::shared_ptr<CS_ExpressionBasis> tmp = boost::shared_ptr<CS_ExpressionBasis>(ret);
    garbigeCollection_.push_back(tmp);
    return tmp;
    break;
  }
  case STRING: {
    std::string val = constVal.sval_;
    boost::shared_ptr<CS_ExpressionBasis> tmp = boost::shared_ptr<CS_ExpressionBasis>(new CS_String( val,0, 0, 0));
    garbigeCollection_.push_back(tmp);
    return tmp;
    break;
  }
  case STRING_ARRAY: {
    CS_AnalogConcatenation* ret = new CS_AnalogConcatenation(0,0,0);
    for (MYINT i = 0; i < (MYINT)constVal.aval_.size(); i++){
        std::string val =  constVal.saval_[i];
        CS_ExpressionBasis* tmp = new CS_String( val,0, 0, 0);
        boost::shared_ptr<CS_ExpressionBasis> tmps = boost::shared_ptr<CS_ExpressionBasis>(tmp);
        garbigeCollection_.push_back(tmps);
        ret->addExpr( tmp );
    }
    boost::shared_ptr<CS_ExpressionBasis> tmp = boost::shared_ptr<CS_ExpressionBasis>(ret);
    garbigeCollection_.push_back(tmp);
    return tmp;
    break;
  }
  default:
    //TODO: no error? reportError(true, "IP_ExpressionParser::getExprForConstantValue error");
    return boost::shared_ptr<CS_ExpressionBasis>(NULL);
  }

}

boost::shared_ptr<CS_ExpressionBasis> IP_ExpressionParser::parseExpression(
    const IP_Token *token, const ExpansionStack* stack)
{
  hasError_ = false;
  errorMessage_ = "";
  MYREAL signFactor = 1.0;

  // test if this token is an expression token
  //reportError((token->getTokenType() != TK_EXPRESSION),
  //    "IP_ExpressionParser::parseExpression was called with a non-expression token");

  // get the string from the token
  std::string exprStr = token->getValue();

  // test the '-' in the front of the ID or number
  if (exprStr.front() == '-') {
      exprStr.erase(0,1);
      signFactor = -1.0;
  }

  if ( isdigit ( exprStr.front() ) ) {
      // this must be a number
      MYREAL factor = 1.0, value; // the faktor for
      if ( !isdigit ( exprStr.back() ))
        {
          factor = getScalingFactor( exprStr.back() );
          exprStr.pop_back(); // delete last character
        }
      // compute the value
      value = ((MYREAL)atof(exprStr.c_str())) * factor * signFactor;
      boost::shared_ptr<CS_ExpressionBasis> tmp = boost::shared_ptr<CS_ExpressionBasis>(
                new  CS_Number(value, NULL, token->getRowIndex(), token->getColumnIndex()) );
      garbigeCollection_.push_back(tmp);
      return tmp;
  }
  else
    {
      // this might also be a string , the it must start with " and end with "
      if (exprStr.front() == '"')
        {
          exprStr.pop_back(); // delete the trailing ", since this must be there for STRING !!!
          boost::shared_ptr<CS_ExpressionBasis> tmp = boost::shared_ptr<CS_ExpressionBasis>(
              new  CS_String(exprStr, NULL, token->getRowIndex(), token->getColumnIndex()) );
          garbigeCollection_.push_back(tmp);
          return tmp;
        }
      // we use case insensitive IDs here
      std::string id = IP_ScannerBase::toLower( exprStr );
      bool hasInitialValue = false;
      bool paramFound = stack->hasParameter( id, hasInitialValue, ignoreActualLevel_);

      // if this parameter not found than throw error
      reportError( !paramFound, "Parameter (IP_ExpressionParser::parseExpression) not found " + id);

      // if we had error than reutrn NULL
      if (this->hasError_)
        return boost::shared_ptr<CS_ExpressionBasis>(NULL);

      // just either return the initial Value, if found or if not than return the parameter reference
      boost::shared_ptr<CS_ExpressionBasis> minExpr;
      if (hasInitialValue) {
          minExpr = stack->getParameterInitialValue( id , ignoreActualLevel_);
      }
      else {
          minExpr = boost::shared_ptr<CS_ExpressionBasis>(
              new CS_ParameterReference( (stack->getParameterReference(id, ignoreActualLevel_)).get(), NULL,
                  (const CS_ModuleDeclaration*)token->getFileNameDummyMod(), token->getRowIndex(), token->getColumnIndex()) );
          garbigeCollection_.push_back(minExpr);
      }
      if (signFactor < 0.0) {
        minExpr = boost::shared_ptr<CS_ExpressionBasis>(new CS_UnaryOperator( VERILOG_UNAR_MIN, minExpr.get(),
            (const CS_ModuleDeclaration*)token->getFileNameDummyMod(), token->getRowIndex(), token->getColumnIndex()) );
        garbigeCollection_.push_back(minExpr);
      }
      return minExpr;
  }
  // here at the moment we only accept either numbers or IDs
}

// TODO: LATER THIS SHOULD CONTAIN An EXPRESSION PARSER !!!!
// TODO: right now this is only capable of referencing other parameters or constant values

MYREAL IP_ExpressionParser::getScalingFactor(char unit)
{
  MYREAL fact = 1.0;
  switch (unit) {
  case 'T': { fact = 1.0E+12; break;}
  case 'G': { fact = 1.0E+09; break;}
  case 'M': { fact = 1.0E+06; break;}
  case 'K': { fact = 1.0E+03; break;}
  case 'm': { fact = 1.0E-03; break;}
  case 'u': { fact = 1.0E-06; break;}
  case 'n': { fact = 1.0E-09; break;}
  case 'p': { fact = 1.0E-12; break;}
  case 'f': { fact = 1.0E-16; break;}
  // default no scaling factor
  default : { fact = 1.0E+00; break;}
  }
  return fact;
}
