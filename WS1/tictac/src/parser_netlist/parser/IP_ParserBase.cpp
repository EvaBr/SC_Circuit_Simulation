/*
 * IP_ParserBase.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: benk
 */

#include "IP_ParserBase.hpp"

#include "IP_ConditionedNetlist.hpp"
#include "IP_Element.hpp"
#include "IP_Function.hpp"
#include "IP_Model.hpp"
#include "IP_Param.hpp"
#include "IP_SubcirDef.hpp"
#include "IP_XCall.hpp"
#include "IP_Option.hpp"
#include "IP_Output.hpp"
#include "IP_AnaysisCard.hpp"
#include "IP_GlobalSettings.hpp"
#include "IP_SubcirSettings.hpp"

#include "IP_Measurement.hpp"
#include "IP_Assertion.hpp"

#include "IP_SimpleScanner.hpp"
#include "IP_NetlistStructSequence.hpp"
#include "sim_utils.hpp"

IP_ParserBase::IP_ParserBase() : UT_SIMVerboseClass(std::string("parser"))
{
  // initalization is made when the parsing starts

  subcircuitLevel_ = -1; // such that ++ will give 0 and this means top-level
  //this->setVerb(10);
}


void IP_ParserBase::parseFile(std::string netlistFile)
{
  // initialize parser
  subcircuitLevel_ = -1;
  scanner_ = boost::shared_ptr<IP_ScannerBase>(new IP_SimpleScanner());
  // start reading the netlist
  scanner_->startReadingFile(netlistFile);
  // only if we did not finished reading
  if (scanner_->finishedReading()) {
      reportError("Scanner not open file" + netlistFile, IP_Parser_NOFATAL);
  }
  else {
      isParsingTopLevel_ = true;
      topDefTree_ = buildSubcirDef();
  }
}


void IP_ParserBase::reportError(std::string errMsg, ParserErrorSeverity severity)
{
  SIM_ERROR_NOSTOP(errMsg);
  if (severity == IP_Parser_FATAL) {
      exit(1);
  }
}


void IP_ParserBase::reportError(std::string errMsg, ParserErrorSeverity severity, const IP_Token &token)
{
  std::cout << " At line: " << token.getRowIndex() << " col:" << token.getColumnIndex()
      << " starting with token:\"" << token.getValue() << "\"\n";
  // TODO: open the file and read until this line ... and display this line
  displayParsingErrorAtToken(token);
  SIM_ERROR_NOSTOP(errMsg);
  if (severity == IP_Parser_FATAL) {
      exit(1);
  }
}

void IP_ParserBase::displayParsingErrorAtToken(const IP_Token &token)
{
  const std::string* fileName = token.getFileName();
  int rowInt = token.getRowIndex();
  int colInt = token.getColumnIndex();
  std::ifstream actFileHandle;
  std::string   actLine;

  actFileHandle.open(fileName->c_str(), std::ifstream::in);
  for (int i = 0; i < rowInt; i++ ) {
      std::getline(actFileHandle, actLine);
  }
  std::cout << actLine << "\n";
  for (int i = 0; i < colInt; i++ ) {
      std::cout << ".";
  }
  std::cout << "^\n";
  actFileHandle.close();
}

bool IP_ParserBase::matchOptions(const std::vector<IP_Token> &tokens)
{
  IP_Token &firstToken = tokens_[0];
  // match to options
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".option"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".options" )
    return true;
  else
    return false;
}

bool IP_ParserBase::matchOutput(const std::vector<IP_Token> &tokens)
{
  IP_Token &firstToken = tokens_[0];
  // match for output
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".output"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".probe"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".plot"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".print"
     )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchMeasurement(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match for measurement
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".measure"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".meas" )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchAssertion(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match for assertion
  if ( IP_ScannerBase::toLower(firstToken.getValue()) == ".assert" )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchGlobalSettings(const std::vector<IP_Token> &tokens)
{
  IP_Token &firstToken = tokens_[0];
  // match for global settings
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".global"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".save"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".test"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".hdl"
     )
    return true;
  else
    return false;
}

bool IP_ParserBase::matchAnalysisCard(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match various analysys cards
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".op"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".dc"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".tran"
     )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchConditionedNetlist(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  const IP_Token &thirdToken = (tokens.size() >= 3) ? tokens[2] : tokens[0];
  // match for conditioned netlisting
  if (    IP_ScannerBase::toLower(firstToken.getValue()) == ".if"
       && IP_ScannerBase::toLower(thirdToken.getValue()) == "then"
     )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchFunction(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match for function declaration
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".func" )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchParam(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match for function declaration
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".param"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".params" )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchModel(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match for model declaration
  if (    IP_ScannerBase::toLower(firstToken.getValue()) == ".model"
       && (tokens.size() > 2) // this must have at least a model name and a model type
     )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchSubcirDef(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  // match for subcircuit declaration
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".subckt" )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchSubcirSettings(const std::vector<IP_Token> &tokens)
{
  IP_Token &firstToken = tokens_[0];
  // match for subcircuit settings/options
  if (   IP_ScannerBase::toLower(firstToken.getValue()) == ".connect"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".nodeset"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".ic"
      || IP_ScannerBase::toLower(firstToken.getValue()) == ".comopt"
     )
    return true;
  else
    return false;
}
bool IP_ParserBase::matchXcall(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  std::string potentialInstanceName = IP_ScannerBase::toLower(firstToken.getValue());
  // match for potential X-call, based on the potential instance name
  if ( potentialInstanceName[0] == 'x' )
    return true;
  else
    return false;
}

bool IP_ParserBase::matchElement(const std::vector<IP_Token> &tokens)
{
  const IP_Token &firstToken = tokens[0];
  std::string potentialInstanceName = IP_ScannerBase::toLower(firstToken.getValue());
  // test for all potential instance names
  // TODO: later extend this list with other elements
  if (    potentialInstanceName[0] == 'r'
       || potentialInstanceName[0] == 'c'
       || potentialInstanceName[0] == 'l'
       || potentialInstanceName[0] == 'v'
       || potentialInstanceName[0] == 'd'
       || potentialInstanceName[0] == 'e'
       || potentialInstanceName[0] == 'g'
       || potentialInstanceName[0] == 'm'
       || potentialInstanceName[0] == 'q'
       || potentialInstanceName[0] == 'j'
       || potentialInstanceName[0] == 'k'
       || potentialInstanceName[0] == 'u'
     )
    return true;
  else
    return false;
}

// method to read in the next record (a record should contain at least one token)
void IP_ParserBase::readInNextRecord()
{
  bool needsNextRead = false;

  SIM_PRINT_L6(verb(), " IP_ParserBase::readInNextRecord() ");
  // loop as long we either finish reading, have no error,
  // and have at least one token in the record
  do
    {
      IP_ScannerStatusInfo status;
      scanner_->getNextRecord(status);
      // check scanner for error
      if (status.isError())
        {
          reportError(std::string("ScannerMSG: ") + status.errorMessage(), IP_Parser_NOFATAL);
          needsNextRead = true;
        }

      // if no record is there then we should read further
      if (scanner_->getNrTokensInActualRecord() < 1)
        needsNextRead = true;

      // if we have new tokens than put them into out token lis:
      if ( (needsNextRead == false) )
        {
          SIM_PRINT_L6(verb(), " store tokens nr.tokens=" << scanner_->getNrTokensInActualRecord()
              << " needsNextRead="<< needsNextRead);
          this->tokens_.clear();
          for (int i = 0; i < scanner_->getNrTokensInActualRecord(); i++)
            tokens_.push_back( scanner_->getToken(i) );
        }

      // if reading is finished then exit the whole parser
      if (scanner_->finishedReading() && needsNextRead ) {
          SIM_PRINT_L6(verb(), " finish fl=" << scanner_->finishedReading() );
          reportError(std::string("Scanner: finished reading internal error "), IP_Parser_NOFATAL);
          needsNextRead = false;
      }
      // read as long we have at least one token as a valid recors
      SIM_PRINT_L6(verb(), " readInNextRecord nrTokens="
          << scanner_->getNrTokensInActualRecord() << " needsNextRead=" << needsNextRead
          << " status.isError()=" << status.isError());
    } while (needsNextRead == true);
}



// =============== ELEMENT/DEVICE BUILDING =============
boost::shared_ptr<IP_Element>             IP_ParserBase::buildElement()
{
  IP_Token &firstToken = tokens_[0];
  std::string potentialInstanceName = IP_ScannerBase::toLower(firstToken.getValue());
  IP_ElemPreType elemt = ERR_ELEM;
  // get the type of the element
  if (potentialInstanceName[0] == 'r') elemt = R_ELEM;
  if (potentialInstanceName[0] == 'c') elemt = C_ELEM;
  if (potentialInstanceName[0] == 'l') elemt = L_ELEM;
  if (potentialInstanceName[0] == 'v') elemt = V_ELEM;
  if (potentialInstanceName[0] == 'd') elemt = D_ELEM;

  if (potentialInstanceName[0] == 'e') elemt = E_ELEM;
  if (potentialInstanceName[0] == 'g') elemt = G_ELEM;

  if (potentialInstanceName[0] == 'm') elemt = M_ELEM;
  if (potentialInstanceName[0] == 'q') elemt = Q_ELEM;
  if (potentialInstanceName[0] == 'j') elemt = J_ELEM;

  if (potentialInstanceName[0] == 'k') elemt = K_ELEM;

  if (potentialInstanceName[0] == 'u') elemt = U_ELEM;

  if (elemt == ERR_ELEM) {
      reportError(std::string("No match for element, return dummy Resistor ")
                  + potentialInstanceName, IP_Parser_NOFATAL);
      return boost::shared_ptr<IP_Element>(new  IP_Element(tokens_, R_ELEM));
  }
  else {
    // we generate an element
      SIM_PRINT_L5(verb(), " buildElement no error ");
    return boost::shared_ptr<IP_Element>(new  IP_Element(tokens_, elemt));
  }
}

// =============== FUNCTION BUILDING =============
boost::shared_ptr<IP_Function>            IP_ParserBase::buildFunction()
{
  // just build a function with the actual tokens
  return boost::shared_ptr<IP_Function>(new  IP_Function(tokens_));
}

// =============== MODEL BUILDING =============
boost::shared_ptr<IP_Model>               IP_ParserBase::buildModel()
{
  // get the parameter declarations on the model card
  std::vector< boost::shared_ptr<IP_Param> > params =
      buildParameterSequence( 3, (int)tokens_.size(), tokens_);

  // just build a model with the actual tokens
  return boost::shared_ptr<IP_Model>(new  IP_Model(tokens_, tokens_[1].getValue(),
      tokens_[2].getValue(),  params ));
}


// =============== X-CALL BUILDING ============
boost::shared_ptr<IP_XCall>               IP_ParserBase::buildXCall()
{
  int n = (int) tokens_.size(), i = 1;
  int nrPinn = 0;
  bool hadSubckt = false;
  std::string subcirName = ":=| :-("; // to signal error
  std::vector< boost::shared_ptr<IP_Param> > xparams(0);

  // test if we should have at least 3 tokens
  if (tokens_.size() < 3)
    reportError("Not enough tokens for X-call", IP_Parser_NOFATAL, tokens_[0]);

  // now loop through each token and see which are pinns and which are params of the subckt
  while (i < n)
    {
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

// ============== OUTPUT =============
boost::shared_ptr<IP_Output>             IP_ParserBase::buildOutput()
{
  // just build the probe statements, such that later they could be postprocessed
  return boost::shared_ptr<IP_Output>(new  IP_Output(tokens_));
}

// ============== MEASUREMENT =============
boost::shared_ptr<IP_Measurement>        IP_ParserBase::buildMeasurement()
{
  // just build a measurement
  return boost::shared_ptr<IP_Measurement>(new  IP_Measurement(tokens_));
}

// ============== ASSERTION =============
boost::shared_ptr<IP_Assertion>          IP_ParserBase::buildAssertion()
{
  // just build an assertion
  return boost::shared_ptr<IP_Assertion>(new  IP_Assertion(tokens_));
}

// =============== BUILD PARAMETER declaration
std::vector< boost::shared_ptr<IP_Param> >  IP_ParserBase::buildParam()
{
  // just call a function to build the list of parameter declarations
  // starting from the first token
  return buildParameterSequence(1, (int)tokens_.size(), tokens_);
}

// ================= BUILD OPTIONS ============
std::vector< boost::shared_ptr<IP_Option> >        IP_ParserBase::buildOptions()
{
  //here it could be multiple options .. and an option is either NAME or NAME=VALUE

  // this is the return vector
  std::vector< boost::shared_ptr<IP_Option> > retVect;
  int i = 1, n = (int)tokens_.size();

  // loop for all tokens
  while ( i < n )
    {
      std::vector<IP_Token> tokens(0);
      // the name of the option
      tokens.push_back(tokens_[i]);
      // there must be an "=" sign and then an expression token
      if ( (i+2 < n) && (tokens_[i+1].getTokenType() == TK_EQUAL) ){
          tokens.push_back(tokens_[i+2]);
          tokens[1].setTokenType(TK_EXPRESSION); // this must be an expression
          // we need to pass this Ctor only 2 tokens
          retVect.push_back( boost::shared_ptr<IP_Option>(new IP_Option(tokens) ));
          i = i + 3;
      }
      else {
          // report error
          retVect.push_back( boost::shared_ptr<IP_Option>(new IP_Option(tokens) ));
          i++;
      }
    }
  // just return the vector with options
  return retVect;
}


// ================= BUILD ANALYSIS CARD ============
boost::shared_ptr<IP_AnaysisCard>         IP_ParserBase::buildAnalysisCard()
{
  IP_Token &firstToken = tokens_[0];
  AnalysisCardType analyType = IP_ANALYSIS_ERR;
  // match various analysys cards
  if ( IP_ScannerBase::toLower(firstToken.getValue()) == ".op" ) analyType = IP_ANALYSIS_OP;
  if ( IP_ScannerBase::toLower(firstToken.getValue()) == ".dc" ) analyType = IP_ANALYSIS_DC;
  if ( IP_ScannerBase::toLower(firstToken.getValue()) == ".tran" ) analyType = IP_ANALYSIS_TR;
  if ( IP_ScannerBase::toLower(firstToken.getValue()) == ".ac" ) analyType = IP_ANALYSIS_AC;
  // return the analysis card
  return boost::shared_ptr<IP_AnaysisCard>(new  IP_AnaysisCard(tokens_, analyType));
}

boost::shared_ptr<IP_GlobalSettings>        IP_ParserBase::buildGlobalSettings()
{
  IP_Token &firstToken = tokens_[0];
  std::string fTokenStr = IP_ScannerBase::toLower(firstToken.getValue());
  GlobalSettingType settingType = IP_GSETTING_ERR;
  // get the type of the element
  if (fTokenStr == ".global") settingType = IP_GSETTING_GLOBAL;
  if (fTokenStr == ".save")   settingType = IP_GSETTING_SAVE;
  if (fTokenStr == ".test")   settingType = IP_GSETTING_TEST;
  if (fTokenStr == ".hdl")    settingType = IP_GSETTING_HDL;

  // see if this was an error
  if (settingType == IP_GSETTING_ERR)
        reportError(std::string("No match for global setting ") + fTokenStr, IP_Parser_NOFATAL);

  return boost::shared_ptr<IP_GlobalSettings>(new  IP_GlobalSettings(tokens_, settingType));
}

boost::shared_ptr<IP_SubcirSettings>  IP_ParserBase::buildSubcirSettings()
{
  IP_Token &firstToken = tokens_[0];
  std::string fTokenStr = IP_ScannerBase::toLower(firstToken.getValue());
  SubcktSettingType settingType = IP_SSETTING_ERR;
  // get the type of the element
  if (fTokenStr == ".connect") settingType = IP_SSETTING_CONNECT;
  if (fTokenStr == ".nodeset") settingType = IP_SSETTING_NODESET;
  if (fTokenStr == ".ic") settingType = IP_SSETTING_IC;
  if (fTokenStr == ".comopt") settingType = IP_SSETTING_COMOPT;

  // see if this was an error
  if (settingType == IP_SSETTING_ERR)
        reportError(std::string("No match for subcircuit setting ") + fTokenStr, IP_Parser_NOFATAL);

  return boost::shared_ptr<IP_SubcirSettings>(new  IP_SubcirSettings(tokens_, settingType));
}


// ========== BUILD a sequence of parameters from a token sequence
std::vector< boost::shared_ptr<IP_Param> > IP_ParserBase::buildParameterSequence(
    int startIndex,  int endIndex,
    const std::vector<IP_Token> &tokens)
{
  // this is the general method to try to get a sequence of parameters from a token list
  // where the key concept is to use "=" then the token before is an ID and the other is
  // an expression

  int i = startIndex; // we start with the first element
  int n = ((int)tokens.size() < endIndex) ? ((int)tokens.size()) : endIndex ;
  // the returned vector
  std::vector< boost::shared_ptr<IP_Param> > retVect(0);

  // loop for all tokens
  while ( i < n )
    {
      std::vector<IP_Token> tokens_tmp(0);
      // the name of the param
      tokens_tmp.push_back(tokens[i]);
      // there must be an "=" sign and then an expression token
      if ( (i+2 < n) && (tokens_[i+1].getTokenType() == TK_EQUAL) )
        {
          tokens_tmp.push_back(tokens[i+2]);
          tokens_tmp[1].setTokenType(TK_EXPRESSION); // this must be an expression
          // we need to pass this Ctor only 2 tokens
          retVect.push_back( boost::shared_ptr<IP_Param>(new IP_Param(tokens_tmp) ));
          i = i + 3;
        }
      else
        { // report error
          reportError("Cannot build parameter", IP_Parser_NOFATAL, tokens[i]);
          i++;
        }
    }
  // return the build vector
  return retVect;
}


// ================== CIRCUIT BODY BUILDER ========================
boost::shared_ptr<IP_NetlistStructSequence> IP_ParserBase::buildStuff(std::vector< std::string> &expectedEnd)
{
  // this reads as long we have a match of the body components
  bool hasMatch = false;
  int i = 0;
  boost::shared_ptr<IP_NetlistStructSequence> ret(new IP_NetlistStructSequence( tokens_ ) );
  SIM_PRINT_L6(verb(), " IP_ParserBase::buildStuff() START " << this->subcircuitLevel_);
  do
    {
      SIM_PRINT_L6(verb(), " IP_ParserBase::buildStuff() i=" << i << " isParsingTopLevel_="
          << isParsingTopLevel_ );
      if (tokens_.size() > 0)
        SIM_PRINT_L6(verb(), " first token " << tokens_[0].getValue());
      // TODO: in the mathing one could also consider the flag "this->isParsingTopLevel_"
      // to allow stuff only on the top-level ... or only within subcircuits

      // if file is done than exit
      if (scanner_->finishedReading() && (i > 0) ) break;

      hasMatch = false;
      // read in next record
      readInNextRecord();
      i = i + 1;

      // now for each possible construct we check if there is a match
      // conditioned netlisting
      if (matchConditionedNetlist(tokens_)) {
          SIM_PRINT_L6(verb(), " buildConditionedNetlist() i=" << i);
          ret->addNetlistStruct(buildConditionedNetlist());
          hasMatch = true; continue;
      }
      // function declaration
      if (matchFunction(tokens_))  {
          SIM_PRINT_L6(verb(), " buildFunction() i=" << i);
          ret->addNetlistStruct(buildFunction());
          hasMatch = true; continue;
      }
      // params declarations
      if (matchParam(tokens_))  {
          SIM_PRINT_L6(verb(), " buildParam() i=" << i);
          std::vector< boost::shared_ptr<IP_Param> > tmpVect = buildParam();
          for (int j = 0; j < tmpVect.size(); j++)
            ret->addNetlistStruct(tmpVect[j]);
          hasMatch = true; continue;
      }
      // model declaration
      if (matchModel(tokens_))  {
          SIM_PRINT_L6(verb(), " buildModel() i=" << i);
          ret->addNetlistStruct(buildModel());
          hasMatch = true; continue;
      }
      // subcircuit declaration
      if (matchSubcirDef(tokens_))  {
          SIM_PRINT_L6(verb(), " buildSubcirDef() i=" << i);
          ret->addNetlistStruct(buildSubcirDef());
          hasMatch = true; continue;
      }
      // X-call subcircuit instantiation
      if (matchXcall(tokens_))  {
          SIM_PRINT_L6(verb(), " buildXCall() i=" << i);
          ret->addNetlistStruct(buildXCall());
          hasMatch = true; continue;
      }
      // instantiation of an element (device)
      if (matchElement(tokens_))  {
          SIM_PRINT_L6(verb(), " buildElement() i=" << i);
          ret->addNetlistStruct(buildElement());
          hasMatch = true; continue;
      }
      // match options
      if (matchOptions(tokens_))  {
          SIM_PRINT_L6(verb(), " buildOptions() i=" << i);
          std::vector< boost::shared_ptr<IP_Option> > tmpVect = buildOptions();
          for (int j = 0; j < tmpVect.size(); j++)
            ret->addNetlistStruct(tmpVect[j]);
          hasMatch = true; continue;
      }
      // match output
      if (matchOutput(tokens_))  {
          SIM_PRINT_L6(verb(), " buildOutput() i=" << i);
          ret->addNetlistStruct(buildOutput());
          hasMatch = true; continue;
      }
      // match analysis cards
      if (matchAnalysisCard(tokens_))  {
          SIM_PRINT_L6(verb(), " buildAnalysisCard() i=" << i);
          ret->addNetlistStruct(buildAnalysisCard());
          hasMatch = true; continue;
      }
      // match measurement
      if (matchMeasurement(tokens_))  {
          SIM_PRINT_L6(verb(), " buildMeasurement() i=" << i);
          ret->addNetlistStruct(buildMeasurement());
          hasMatch = true; continue;
      }
      // match assertion
      if (matchAssertion(tokens_))  {
          SIM_PRINT_L6(verb(), " buildAssertion() i=" << i);
          ret->addNetlistStruct(buildAssertion());
          hasMatch = true; continue;
      }
      // match global settings
      if (matchGlobalSettings(tokens_) && isParsingTopLevel_)  {
          SIM_PRINT_L6(verb(), " buildGlobalSettings() i=" << i);
          ret->addNetlistStruct(buildGlobalSettings());
          hasMatch = true; continue;
      }
      // match subckt settings, only when we are
      if (matchSubcirSettings(tokens_))  {
          SIM_PRINT_L6(verb(), " buildSubcirSettings() i=" << i);
          ret->addNetlistStruct(buildSubcirSettings());
          hasMatch = true; continue;
      }
      // IMPORTANT: DO NOT PUT ANY CODE HERE DUE TO THE "contine" in the IFs
      // for the case when there is a metch this will not be executed !!!!

      // test if in case of non-match this s
      for (int j = 0; (j < (int)expectedEnd.size()) && (hasMatch == false); j++)
        {
          SIM_PRINT_L6(verb(), " end str '" << tokens_[0].getValue() << "' exp:'" << expectedEnd[j] << "'");
          if ( IP_ScannerBase::toLower(tokens_[0].getValue()) == expectedEnd[j] )
            hasMatch = true;
        }

      // this means this sequence ended with something unexpected
      if (!hasMatch)
        {
          // this means sequence ended
          std::string errMsg = "Sequence not ended with " + expectedEnd[0];
          for (int j = 1; j < (int)expectedEnd.size(); j++)
            errMsg += " | " + expectedEnd[j];
          errMsg += " ... Ignoring this record";
          reportError( errMsg , IP_Parser_NOFATAL, tokens_[0]);
          //
          hasMatch = !(scanner_->finishedReading());
        }
      else // the else branch means sequence ended OK, and we should exit the loop
        hasMatch = false;

    }
  // loop as long we find matching for our staff
  while (hasMatch);
  SIM_PRINT_L6(verb(), " IP_ParserBase::buildStuff() END " << this->subcircuitLevel_);
  // return the sequence
  return ret;
}


// ============================= CONDITIONED NETLISTING =======================
boost::shared_ptr<IP_ConditionedNetlist>  IP_ParserBase::buildConditionedNetlist()
{
  IP_Token &secondToken = tokens_[1];
  std::vector< std::string > endings2(0);
  std::vector< std::string > endings1(0);

  secondToken.setTokenType(TK_EXPRESSION);
  SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist START l = " << subcircuitLevel_);

  boost::shared_ptr<IP_ConditionedNetlist> ret
    = boost::shared_ptr<IP_ConditionedNetlist>(new IP_ConditionedNetlist(tokens_));

  // prepare the ending strings
  endings2.push_back(".else");
  endings2.push_back(".endif");
  endings2.push_back(".elseif");
  endings1.push_back(".endif");

  // parse the true branch
  boost::shared_ptr<IP_NetlistStructSequence> newThanBr = buildStuff(endings2);
  ret->setThanBranch(newThanBr);

  // store this first token in order to preserv
  std::string keyword = IP_ScannerBase::toLower(tokens_[0].getValue());
  std::string third   =  (tokens_.size() >= 3) ? IP_ScannerBase::toLower(tokens_[2].getValue()) : keyword;

  // at this point we either should have ".else" or ".endif"
  if ( keyword == ".else" )
    {
      SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist START ELSE");
      boost::shared_ptr<IP_NetlistStructSequence> newFalseBr = buildStuff(endings1);
      ret->setFalseBranch( newFalseBr );
      SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist END ELSE");
    }
  else {
      // this is the elseif branch, IMPORTANT: then we do not require ".ENDIF" at the end,
      // since this is a recursive construct !!!
      if (   (keyword == ".elseif")
          && (third   == "then" ) )
        {
          SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist START ELSEIF");
          boost::shared_ptr<IP_NetlistStructSequence> newFalseBr(new IP_NetlistStructSequence(tokens_));
          boost::shared_ptr<IP_ConditionedNetlist>  anotherIf = buildConditionedNetlist();
          newFalseBr->addNetlistStruct(anotherIf);
          ret->setFalseBranch( newFalseBr );
          // no next record reading !!!
          SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist END ELSEIF");
        }
      else {
          SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist START ENDIF");
          // this means we have ".endif" and this should be digested !!!
          // read in next record , since the actual one should be ".endif"
          SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist END ENDIF");
      }
  }

  SIM_PRINT_L6(verb(), " IP_ParserBase::buildConditionedNetlist END l = " << subcircuitLevel_);

  // return the resulting conditioned netlisting
  return ret;
}


//==================================== SUBCKT ==================================
boost::shared_ptr<IP_SubcirDef>           IP_ParserBase::buildSubcirDef()
{
  // signal that we have non top-level after this
  bool isThisTopLevel = isParsingTopLevel_;
  int levelTmp = subcircuitLevel_;
  subcircuitLevel_++;
  // decide if we have top-level or not
  isParsingTopLevel_ = (subcircuitLevel_ > 0)?false:true;
  //
  boost::shared_ptr<IP_SubcirDef> retSubCir;
  boost::shared_ptr<IP_NetlistStructBase> test;
  std::string defName = "TOPLEVEL";
  std::vector < boost::shared_ptr<IP_Param> > subCirParams(0);
  std::vector < IP_Token > subCircPorts(0);
  std::vector< std::string > endings(0);

  SIM_PRINT_L6(verb(), " IP_ParserBase::buildSubcirDef top = " << isParsingTopLevel_ << " subcircuitLevel_ = " << subcircuitLevel_);

  // create the subcircuit
  if (isParsingTopLevel_) // means we are on the toplevel
    { // toplevel
      std::vector<IP_Token> tokens(1);
      std::string tmp = ".TOPLEVEL";
      tokens[0].setValue(tmp);
      retSubCir = boost::shared_ptr<IP_SubcirDef>(
          new IP_SubcirDef(tokens, defName, subCirParams, subCircPorts));
      retSubCir->setTopLevelFlag(true);
      // TOPLEVEL should end with ".end"
      endings.push_back(".end");
    }
  else
    { // non toplevel
      int n = (int) tokens_.size(), i = 2;
      // non TOPLEVEL should end with ".ends"
      endings.push_back(".ends");
      if (tokens_.size() < 3) {
        reportError("Not enough tokens for .SUBCKT declaration (subcircuit without pinns)",
            IP_Parser_NOFATAL, tokens_[0]);
      }

      // the subcircuit name also (as evrything) we store in lower case (if no case sensitive) format
      if (tokens_.size() > 1)
        defName = IP_ScannerBase::toLower(tokens_[1].getValue());

      // now loop through each token and see which are pinns and which are params of the subckt
      while (i < n)
        {

          std::vector<IP_Token> tokensTmp(0); // required only for parameter declaration
          if (   (i + 1 < n)
              && (tokens_[i + 1].getTokenType() == TK_EQUAL)
              )
            { // here we just call the method to parse the parameter sequence
              subCirParams = buildParameterSequence( i, (int) tokens_.size(), tokens_);
              // break the loop, since we are finished
              break;
            }
          else
            { // here we have simple port/pin, and we simply add it to the list
              subCircPorts.push_back(tokens_[i]);
              i++;
            }
        }
      // create subcircuit header
      retSubCir = boost::shared_ptr<IP_SubcirDef>(
                new IP_SubcirDef(tokens_, defName, subCirParams, subCircPorts));
    }

  SIM_PRINT_L6(verb(), " IP_ParserBase::buildSubcirDef START BODY subcircuitLevel_ = " << subcircuitLevel_);

  // get the body of the subcircuit
  boost::shared_ptr<IP_NetlistStructSequence> subcktBody = buildStuff(endings);
  retSubCir->setSubcktBody( subcktBody );

  SIM_PRINT_L6(verb(), " IP_ParserBase::buildSubcirDef END BODY subcircuitLevel_ = " << subcircuitLevel_);

  // read in next record, since the actual one is ".ENDS" or "END"
  //if ( !(scanner_->finishedReading()))
  //  readInNextRecord();

  // reset the falgs
  isParsingTopLevel_ = isThisTopLevel;
  subcircuitLevel_   = levelTmp;
  // return the subcircuit
  return retSubCir;
}
