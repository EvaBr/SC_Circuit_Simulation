/*
 * IP_Token.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: benk
 */

#include "IP_Token.hpp"
#include "IP_DummyModule.hpp"

IP_Token::IP_Token(std::string &value, IP_TokenType type,
    std::string *filename, IP_DummyModule *dummyFileMode, int colInd, int rowInd):
value_(value), type_(type), filename_(filename), dummyFileMode_(dummyFileMode),
colInd_(colInd), rowInd_(rowInd)
{
  // all fields are set
}

IP_Token::IP_Token():
    value_(std::string("DUMMY")), type_(TK_NOTYPE), filename_(NULL),
    dummyFileMode_(NULL), colInd_(-1), rowInd_(-1)
{

}


IP_Token::IP_Token(const IP_Token &copy)
{
  // copy all fields
  value_ = copy.value_;
  type_ = copy.type_;
  filename_ = copy.filename_;
  dummyFileMode_ = copy.dummyFileMode_;
  colInd_ = copy.colInd_;
  rowInd_ = copy.rowInd_;
}
