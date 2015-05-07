/*
 * IP_Model.hpp
 *
 *  Created on: Jan 19, 2015
 *      Author: benk
 */

#ifndef IP_MODEL_HPP_
#define IP_MODEL_HPP_

#include "IP_NetlistStructBase.hpp"
#include "IP_Param.hpp"

/** this class models an arbitrary model declaration
 * in the netlist */
class IP_Model : public IP_NetlistStructBase
{
public:

  /** Ctor: the array of tokens contain the whole record
   * @param name the name of the model
   * @param type the type of the mode
   * @param modelParams the model parameters */
  IP_Model(std::vector<IP_Token> &tokens,
      std::string &name, std::string &type,
      std::vector < boost::shared_ptr<IP_Param> > &modelParams);

  virtual ~IP_Model() {;}

  /** method for the data structure traversal*/
  virtual void Accept(IP_NetlistVisitBase &visitor) { visitor.Visit( (IP_Model &)(*this)); }

  /** get the number of model parameters */
  int getNrModelParams() const { (int)modelParams_.size(); }

  boost::shared_ptr<IP_Param>& getModelParam(int i) { return modelParams_[i]; }
  const boost::shared_ptr<IP_Param>& getModelParam(int i) const { return modelParams_[i]; }

  /** returns the model in lower case stored (if no case sensitive)*/
  const std::string& getName() const { return name_; }

  /** returns the type of the the model stored as string (lower case (if no case sensitive))*/
  const std::string& getType() const { return  type_; }

private:

  /** the name of model in lower case stored (if no case sensitive)*/
  std::string name_;

  /** the type of the model stored as string (lower case (if no case sensitive))*/
  std::string type_;

  /** all the specified parameters with their (default) values */
  std::vector < boost::shared_ptr<IP_Param> > modelParams_;
};

#endif /* IP_MODEL_HPP_ */
