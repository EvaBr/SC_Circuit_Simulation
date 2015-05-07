/*
 * TicTacComputer.hpp
 *
 *  Created on: Sep 30, 2013
 *      Author: benk
 */

#ifndef TICTACCOMPUTER_HPP_
#define TICTACCOMPUTER_HPP_

#include "AN_Analysis.hpp"
#include "Netlist.hpp"
#include "IN_SimOptions.hpp"

/** main class of the execution, this holds all top level information
 * and steers the whole execution of the programm */
class TicTacComputer: public UT_SIMVerboseClass
{
public:

  /** Ctor
   * @param IPNetlist the netlist read from the IP */
  TicTacComputer(boost::shared_ptr<Netlist> IPNetlist,
                 boost::shared_ptr<IN_SimOptions> globalOptions);

  /** Dtor that deletes all the allocated objects */
  virtual ~TicTacComputer();

  /** function to be called to execute all the required simulation s*/
  void executeRequiredAnalyses();

  /** method used by the external setup rountines to add analysis to the
   * simulation */
  void addAnlysis(boost::shared_ptr<AN_Analysis> newAnalysis);

private:

  /** list of consecutive analysis */
  std::vector< boost::shared_ptr<AN_Analysis> > analysis_;

  /** number of required analysis*/
  MYINT nr_analysis_;

  /** the global netlist read from the input processor */
  boost::shared_ptr<Netlist> IPNetlist_;

  /** global simulation options */
  boost::shared_ptr<IN_SimOptions> globalOptions_;
};

#endif /* TICTACCOMPUTER_HPP_ */
