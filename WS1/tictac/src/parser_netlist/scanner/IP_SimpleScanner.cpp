/*
 * IP_SimpleScanner.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: benk
 */

#include "IP_SimpleScanner.hpp"
#include "sim_utils.hpp"


IP_SimpleScanner::IP_SimpleScanner(): IP_ScannerBase(std::string("IP_SimpleScanner"))
{
  boost::shared_ptr<std::ifstream> tmp_handl(new std::ifstream);

  startFile_ = "";
  actualFileParsing_ = "";
  actError_ = "";

  // here we set up the delimiter tokens
  delimiterStartExpressions_.push_back('\''); // classic
  delimiterEndExpressions_.push_back('\'');
  delimiterStartExpressions_.push_back('\"'); // classic for strings
  delimiterEndExpressions_.push_back('\"');
  //delimiterStartExpressions_.push_back('('); // simple brackets
  //delimiterEndExpressions_.push_back(')');
  delimiterStartExpressions_.push_back('{'); // curly brackets
  delimiterEndExpressions_.push_back('}');

  delimiterForComments_.push_back('*');      // classic delimiter for comments
  softDelimiterForComments_.push_back('$');  // only when a token starts with this

  delimiterForRecordContinueEnd_.push_back("//"); // if this is at the end means next line conies this record
  delimiterForRecordContinueStart_.push_back('+');

  delimiterForTokens_.push_back(' ');
  delimiterForTokens_.push_back(',');

  alwaysSeparateTokens_.push_back('=');
  tokenType_.push_back(TK_EQUAL);
  alwaysSeparateTokens_.push_back('(');
  tokenType_.push_back(TK_OPEN_BR);
  alwaysSeparateTokens_.push_back(')');
  tokenType_.push_back(TK_CLOSE_BR);

  includeTokens_.push_back(".include");
  includeTokens_.push_back(".inc");
  includeTokens_.push_back(".lib");

  cRowInd_ = -1;
  cColInd_ = -1;
  hadError_ = false;
  isFinished_ = false;
  // actual file handle
  actFileHandle_ = tmp_handl;

  // set verbosity
  //this->setVerb(10);
}


IP_SimpleScanner::~IP_SimpleScanner()
{
  // nothing to do
}

/** with this method one starts to read a ntlist file (including runset)*/
void IP_SimpleScanner::startReadingFile(std::string fileName)
{
  // initialization
  cRowInd_ = 0;
  cColInd_ = 1;
  hadError_ = false;
  isFinished_ = false;
  startFile_ = fileName;
  actualFileParsing_ = fileName;
  actError_ = "";
  // cleanup
  fileHandleStack.clear();
  allFiles_.clear();
  allFiles_.push_back(fileName);
  allFileDummyModules_.push_back(IP_DummyModule(fileName));
  includeFileStack_.clear();
  tokens_.clear();
  includeFileStack_.push_back(fileName);
  rowIndStack.clear();
  columnIndStack.clear();

  // open file top-level file
  actFileHandle_->open(fileName.c_str(), std::ifstream::in);
  if ( !actFileHandle_->good() ) {
      hadError_ = true;
      isFinished_ = true;
      reportError( std::string("IP_SimpleScanner: Could not open file:") + fileName);
      return;
  }

  // read in the first line
  // this is important since the next lines could also start with "+" and therefore
  // we must always read in the next line
  getNextLineFromActualFile();
}

IP_Token& IP_SimpleScanner::getToken(int index)
{
  return tokens_[index];
}

int IP_SimpleScanner::getNrTokensInActualRecord()
{
  return (int)(tokens_.size());
}

bool IP_SimpleScanner::finishedReading()
{
  return isFinished_;
}

void IP_SimpleScanner::reportError(std::string errMsg)
{
  actError_ += errMsg + "\n";
  SIM_ERROR_NOSTOP(errMsg);
  hadError_ = true;
}

std::string& IP_SimpleScanner::getActualFileName() {  return actualFileParsing_; }

void IP_SimpleScanner::getNextLineFromActualFile()
{
  if ( actFileHandle_->eof() )
    actLine_ = "";
  else
    std::getline(*actFileHandle_, actLine_);
  // TODO: delete trailing and leading spaces ?
  // right trim the string
  actLine_.erase(actLine_.find_last_not_of(" \n\r\t")+1);
  actLine_.erase(0,actLine_.find_first_not_of(" \n\r\t"));
  SIM_PRINT_L5(verb(), " getNextLineFromActualFile line=\"" << actLine_ << "\"");
  cRowInd_++;
  cColInd_ = 1;
}

void IP_SimpleScanner::pushToken(IP_Token &actToken, std::string &value, int column)
{
  // early exit then there is no real token
  if (value.size() < 1) return;

  SIM_PRINT_L6(verb(), " IP_SimpleScanner::pushToken value=\"" << value << "\"" << " t=" << actToken.getTokenType());

  // add this token
  actToken.setColumnIndex(column-1);
  // see if this number looks like a pure number, then do not tolower, since 'M' and 'm' would be confused
  bool isNumber = ( isdigit( value.front() )
      || ( (value.front() == '-') && (value.size() > 1) && isdigit(value[2]) ) );
  // based on the token type we make "tolower"
  // IMPORTANT: that we make this here, such that later we have no trouble comparing strings
  // TODO: is this condition enough ? or too much ??
  if (   (actToken.getTokenType() != TK_FILENAME) && (actToken.getTokenType() != TK_EXPRESSION)
      && !isNumber )
    {
      std::string tmpstr = toLower(value);
      actToken.setValue(tmpstr);
      //actToken.setValue(value);
    }
  else
    actToken.setValue(value);

  tokens_.push_back(actToken);
  // the next token is dy default always "notype"
  actToken.setTokenType(TK_NOTYPE);
  // reset the "value"
  value = "";
}

bool IP_SimpleScanner::popInclude()
{
  if (rowIndStack.size() <= 0 ) return false;

  actualFileParsing_ = includeFileStack_.back();
  actFileHandle_ = fileHandleStack.back();
  cRowInd_ = rowIndStack.back();
  cColInd_ = columnIndStack.back();
  fileHandleStack.pop_back();
  includeFileStack_.pop_back();
  rowIndStack.pop_back();
  columnIndStack.pop_back();

  return true;
}

bool IP_SimpleScanner::processActualLine()
{
  bool needNextLine = false;
  int tmp;
  std::string buildingToken = "";
  IP_Token actTokenTmp(buildingToken , TK_NOTYPE, &(allFiles_.back()),
                        &(allFileDummyModules_.back()), cColInd_, cRowInd_);
  char actChar, prevChar;
  bool somemathch = false;
  bool withinExpression = false;
  bool withinComment = false;
  bool nextLineIsContinuation = false;
  int  exprStackDepth = 0;
  char endExprChar = ' ';
  char startExprChar = ' ';
  // here we assume that "actLine_" contains the actual line

  // we have one line already
  // process one line into tokens (push back on the tokens)

  SIM_PRINT_L5(verb(), " IP_SimpleScanner::processActualLine ");
  // initialize the actual token
  actToken_ = actTokenTmp;

  do
    {
      nextLineIsContinuation = false;

      do
        {
          // set the flag to false
          needNextLine = false;

          if (actLine_.size() > 1)
            actChar = prevChar = actLine_[0];
          else
            actChar = prevChar = '0';

          // loop over each character of the read line
          for (int i = 0; i < actLine_.size(); i++)
            {
              // initialize this character read
              actChar = actLine_[i];
              somemathch = false;

              // "+" charcters just ignore at the beginning
              if ((i == 0) && (matchCharToken(actChar,
                  delimiterForRecordContinueStart_, tmp)))
                continue;

              SIM_PRINT_L6(verb(), " i=" << i << " c=" << actChar);

              // we are not within expressions and we found a delimiter token
              if ( (!withinExpression) && (!withinComment)
                  && matchCharToken(actChar, delimiterForTokens_, tmp))
                {
                  somemathch = true;
                  pushToken(actToken_, buildingToken, cColInd_);
                }

              // test one char tokens such as "=" which are always, outside of expressions, a separate tokens
              if ( (!withinExpression) && (!withinComment)
                   && matchCharToken(actChar, alwaysSeparateTokens_, tmp) )
                {
                  pushToken(actToken_, buildingToken, cColInd_);
                  actToken_.setTokenType(tokenType_[tmp]);
                  // add the equal as token:
                  buildingToken = actChar;
                  pushToken(actToken_, buildingToken, cColInd_);
                  somemathch = true;
                }

              // ========== EXPRESSION ============
              // if we are in the expression
              if (withinExpression && (actChar == endExprChar) && (exprStackDepth <= 1))
                {
                  pushToken(actToken_, buildingToken, cColInd_);
                  actToken_.setTokenType(TK_NOTYPE);
                  withinExpression = false;
                  somemathch = true;
                  SIM_PRINT_L6(verb(), " End expr ");
                }

              if (withinExpression && (actChar == endExprChar) && (exprStackDepth > 1))
                exprStackDepth--;
              if (withinExpression && (actChar == startExprChar) && (exprStackDepth >= 1))
                exprStackDepth++;

              // if we start an expression then actToken.setTokenType
              if ((!withinExpression) && (!withinComment) && !somemathch
                  && matchCharToken(actChar, delimiterStartExpressions_, tmp))
                {
                  somemathch = true;                 // means we have a new token
                  withinExpression = true;           // we are within new expression token
                  exprStackDepth = 1;
                  endExprChar   = delimiterEndExpressions_[tmp];
                  startExprChar = delimiterStartExpressions_[tmp];
                  // it there was an other token in building, then push this token
                  pushToken(actToken_, buildingToken, cColInd_);
                  actToken_.setTokenType(TK_EXPRESSION);
                  SIM_PRINT_L7(verb(), " Start expr ");
                }
              // ======== END EXPRESION ===========

              // test if we are entering a comment
              if (   (!withinExpression)
                  && matchCharToken(actChar, delimiterForComments_, tmp))
                {  withinComment = true;  }

              // soft delimiter for comments, when the token would start with this soft delimiter
              if ( (!withinExpression) && (buildingToken.size() == 0) &&
                   matchCharToken(actChar, softDelimiterForComments_, tmp)
                 )
                {  withinComment = true;  }


              // ========== BUILDING TOKEN ==============
              if (!somemathch && !withinComment)
                buildingToken += actChar;

              // ==== test END characters of the line and the actual token
              if ( (!withinComment) &&
                  matchEndStringToken( buildingToken, delimiterForRecordContinueEnd_, tmp))
                { // read line until end
                  SIM_PRINT_L6(verb(), " matchEndStringToken " );
                  needNextLine = true;
                  //read-in new line
                  getNextLineFromActualFile();
                  break; // exit this for loop
                }

              // if we are within includes then turn off automatic conversion to lower case (if no case sensitive)
              if ((tokens_.size() > 0) && matchStringToken(tokens_[0].getValue(), includeTokens_, tmp)  )
                actToken_.setTokenType(TK_FILENAME);

              // store the previous character (might be needed)
              prevChar = actLine_[i];
              // increment the column counter
              cColInd_ = i + 1;
            }

          // =========== INCLUDE =======
          if ((tokens_.size() > 0) && matchStringToken(tokens_[0].getValue(), includeTokens_, tmp)  )
            {
              // push the actual token
              pushToken(actToken_, buildingToken, cColInd_);
              //  .include or .inc handle here ... jump into a new file
              if (tokens_.size() < 2)
                {
                  reportError( std::string("IP_SimpleScanner: include directives should specify a file name."));
                  return false;
                }
              // the filename must be
              std::string fileName = tokens_[1].getValue();
              // delete all tokens
              tokens_.clear();

              // try to jump into this new file
              actualFileParsing_ = fileName;
              fileHandleStack.push_back(actFileHandle_);
              allFiles_.push_back(fileName);
              allFileDummyModules_.push_back(IP_DummyModule(fileName));
              includeFileStack_.push_back(fileName);
              rowIndStack.push_back(cRowInd_);
              columnIndStack.push_back(cColInd_);

              // cearte new file handle
              actFileHandle_ = boost::shared_ptr<std::ifstream>( new std::ifstream());
              // open file top-level file
              actFileHandle_->open(fileName.c_str(), std::ifstream::in);
              if ( !actFileHandle_->good() )
                {
                  hadError_ = true;
                  reportError( std::string("IP_SimpleScanner: Could not open file:") + fileName);
                  popInclude();
                  return false;
                }
              needNextLine = false;
            }
          // ========== END include ===========
          SIM_PRINT_L5(verb(), " needNextLine=" << needNextLine );

        // ======= LOOP AS LONG WE HAVE uncompleted line, due to "//"
        } while ( (needNextLine == true) && (!actFileHandle_->eof()) );

      // read new line
      getNextLineFromActualFile();
      // initializations:
      withinComment = false;

      // check if this new line is a continuation of the next one
      nextLineIsContinuation = this->matchCharToken(actLine_[0],
                        delimiterForRecordContinueStart_, tmp);

      // if this is true than if we are not in expression than this token should be pushed
      if (nextLineIsContinuation && (actToken_.getTokenType() != TK_EXPRESSION))
        pushToken(actToken_, buildingToken, cColInd_);

      // if this is e.g. "*sjdhksjdf ..." then apparently we should jump over this
      // IMPORTANT: first character must be this !!!
      if (   (actToken_.getTokenType() != TK_EXPRESSION)
          && matchCharToken(actLine_[0], delimiterForComments_, tmp))
        nextLineIsContinuation = true;

      // if the next line is completely empty than we should read the next line
      if ((actLine_.size() < 1) && (!actFileHandle_->eof())) {
          nextLineIsContinuation = true;
          SIM_PRINT_L6(verb(), " actLine_.size()=" << actLine_.size() << " eof=" << actFileHandle_->eof() );
      }

      // if next is line is not a continuation
      // AND we are not in an expression
      if (  (!nextLineIsContinuation) || actFileHandle_->eof() )
        {
          if (withinExpression)
            reportError("Expression NOT ended, considered as ended now");
          pushToken(actToken_, buildingToken, cColInd_);
      }

    } while ( nextLineIsContinuation );

  SIM_PRINT_L5(verb(), " RET needNextLine=" << needNextLine << " nr. token=" << tokens_.size()
        << " buildingToken=" << buildingToken);
  return needNextLine;
}

void IP_SimpleScanner::getNextRecord(IP_ScannerStatusInfo &status)
{

  bool needNextLine = true;

  SIM_PRINT_L6(verb(), " IP_SimpleScanner::getNextRecord " );

  if (isFinished_ && (actLine_.size() < 1))
    {
      status.isError_ = hadError_;
      status.errorMessage_ = actError_;
      SIM_PRINT_L6(verb(), " IP_SimpleScanner::getNextRecord isFinished_=" << isFinished_ );
      // clear the tokens
      tokens_.clear();
      return;
    }

  // clear the tokens
  tokens_.clear();
  // set the flags to default
  isFinished_ = false;

  // we loop as long we find at least one token ...
  do
    {
      // ========== READ RECORD =========
      // read a record, which can be over many rows/lines
      do
        {
          // if processActualLine returns "true" then we MUST read next line in
          // order to complete the record (however this will usually return false)
          needNextLine = this->processActualLine();
          // if we do not have any tokens then try to read further in the current file
          // a record must have at least one token
          if (tokens_.size() < 1)
            needNextLine = true;
          SIM_PRINT_L6(verb(), " needNextLine = " << needNextLine << " tokens_.size()=" << tokens_.size());
        }
      // end of reading one record, or if the file is out
      while (needNextLine && ( (!actFileHandle_->eof()) || (actLine_.size() > 0) ) );
      // ======== END RECORD ============

      // after we read in one record we should check if we reached EOF
      // and in the same time we should also read-in the next line
      while ( (isFinished_ == false) && (actFileHandle_->eof()) )
        {
          if ( actFileHandle_->eof() )
            {
              if (fileHandleStack.size() >= 1){
                  // this means me need to get back from the stack
                  actFileHandle_ = fileHandleStack.back();
                  actualFileParsing_ = includeFileStack_.back();
                  cRowInd_ = rowIndStack.back();
                  cColInd_ = columnIndStack.back();
                  fileHandleStack.pop_back();
                  includeFileStack_.pop_back();
                  rowIndStack.pop_back();
                  columnIndStack.pop_back();
                  isFinished_ = false;
                  // read in next line
                  getNextLineFromActualFile();
              }
              else {
                  SIM_PRINT_L6(verb(), " (no pop-up)isFinished_ = " << isFinished_);
                  isFinished_ = true;
              }
            }
        }
    }
  // if we do not have any tokens yet then (only commnet or similar)
  // we keep going, or if the file is over (EOF)
  while ( (isFinished_ == false) && (tokens_.size() < 1));

}

