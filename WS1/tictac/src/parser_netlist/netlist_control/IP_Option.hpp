/*
 * IP_Option.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#ifndef IP_OPTION_HPP_
#define IP_OPTION_HPP_

#include "IP_ControlBase.hpp"

/** this class represents all the control statements which starts with ".option" or ".options" */
class IP_Option : public IP_ControlBase
{
public:

  /** Ctor with the token
   * ATTENTION: this Ctor should be called only with two tokens in the vector
   * The first one is the name of the option, and the second one is the optional
   * value of the option */
  IP_Option(std::vector<IP_Token> &tokens);

  /** empty Dtor */
  virtual ~IP_Option() {;}

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_Option &)(*this)); }

  /** returns the option's name */
  const std::string& getOptionName() const { return optionName_; }

private:

  /** the name of the option */
  std::string optionName_;

  /** if the option has a value than this token should contain the value
   * (as expression or as string or ... ) */
  IP_Token optionalValue_;

};

#endif /* IP_OPTION_HPP_ */
