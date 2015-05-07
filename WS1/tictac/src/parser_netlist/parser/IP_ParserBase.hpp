/*
 * IP_ParserBase.hpp
 *
 *  Created on: Jan 29, 2015
 *      Author: benk
 */

#ifndef IP_PARSERBASE_HPP_
#define IP_PARSERBASE_HPP_

#include "sim_utils.hpp"
#include "IP_Token.hpp"

class IP_ConditionedNetlist;
class IP_GlobalSettings;
class IP_Element;
class IP_Function;
class IP_Model;
class IP_Param;
class IP_SubcirDef;
class IP_SubcirSettings;
class IP_XCall;
class IP_Option;
class IP_Output;
class IP_AnaysisCard;
class IP_Assertion;
class IP_Measurement;

class IP_ScannerBase;
class IP_NetlistStructSequence;

/** enum type for the various error types */
typedef enum{
  IP_Parser_NOFATAL, ///< non-fatal error, parsing could continue
  IP_Parser_FATAL     ///< fatal error parsing should stop
} ParserErrorSeverity;

/** class representing the simple parser implementation */
class IP_ParserBase : public UT_SIMVerboseClass
{
public:

  /** empty Ctor */
  IP_ParserBase();

  /** empty Dtor */
  virtual ~IP_ParserBase() {;}

  /** main entry point */
  virtual void parseFile(std::string netlistFile);

  /* these are the virtual functions which decide where to "turn" with a record */

  // ========== methods to decide for "rough" and "first" match ===========
  virtual bool matchConditionedNetlist(const std::vector<IP_Token> &tokens);
  virtual bool matchGlobalSettings(const std::vector<IP_Token> &tokens);
  virtual bool matchFunction(const std::vector<IP_Token> &tokens);
  virtual bool matchParam(const std::vector<IP_Token> &tokens);
  virtual bool matchModel(const std::vector<IP_Token> &tokens);
  virtual bool matchSubcirDef(const std::vector<IP_Token> &tokens);
  virtual bool matchSubcirSettings(const std::vector<IP_Token> &tokens);
  virtual bool matchXcall(const std::vector<IP_Token> &tokens);
  virtual bool matchElement(const std::vector<IP_Token> &tokens);
  virtual bool matchOptions(const std::vector<IP_Token> &tokens);
  virtual bool matchMeasurement(const std::vector<IP_Token> &tokens);
  virtual bool matchAssertion(const std::vector<IP_Token> &tokens);
  virtual bool matchOutput(const std::vector<IP_Token> &tokens);
  virtual bool matchAnalysisCard(const std::vector<IP_Token> &tokens);

  // ========== these methods build the netlist structs ====
  // these functions also represent the grammar in the netlist structure
  // which is only the first approximation to categorize and transfer a record
  // to a first preliminary structure
  virtual boost::shared_ptr<IP_Element>               buildElement();
  virtual boost::shared_ptr<IP_Function>              buildFunction();
  virtual boost::shared_ptr<IP_Model>                 buildModel();
  virtual std::vector< boost::shared_ptr<IP_Param> >  buildParam();
  virtual boost::shared_ptr<IP_SubcirDef>             buildSubcirDef();
  virtual boost::shared_ptr<IP_SubcirSettings>        buildSubcirSettings();
  virtual boost::shared_ptr<IP_XCall>                 buildXCall();
  virtual boost::shared_ptr<IP_ConditionedNetlist>    buildConditionedNetlist();
  virtual boost::shared_ptr<IP_GlobalSettings>        buildGlobalSettings();
  virtual std::vector< boost::shared_ptr<IP_Option> > buildOptions();
  virtual boost::shared_ptr<IP_Output>                buildOutput();
  virtual boost::shared_ptr<IP_Measurement>           buildMeasurement();
  virtual boost::shared_ptr<IP_Assertion>             buildAssertion();
  virtual boost::shared_ptr<IP_AnaysisCard>           buildAnalysisCard();

  /** this updates the array of tokens with the actual record */
  virtual void readInNextRecord();

  /** internal method to report parser errors */
  virtual void reportError(std::string errMsg, ParserErrorSeverity severity);
  virtual void reportError(std::string errMsg, ParserErrorSeverity severity, const IP_Token &token);

  /** at many places a sequence of "paramName=paramValueExpr" is required, such as by .model or .param
   * or  even at X-calls
   * For such general purpose we use this function which tries to cast this to such
   * parameter sequence starting from the start index, until the given end index */
  virtual std::vector< boost::shared_ptr<IP_Param> > buildParameterSequence(int startIndex,  int endIndex,
      const std::vector<IP_Token> &tokens);

  /** returns the toplevel subcircuit */
  boost::shared_ptr<IP_SubcirDef>& getTopLevel() { return topDefTree_; }
  const boost::shared_ptr<IP_SubcirDef>& getTopLevel() const { return topDefTree_; }

protected:

  /** display an error which occured at a token */
  void displayParsingErrorAtToken(const IP_Token &token);

  /** this is the internal method to build stuff for the body of a subcircuit
   * or for a conditioned netlisting
   * @param expectedEnd the special first token values which would end this sequence
   *                    (in lower case if non case sensitivity) */
  virtual boost::shared_ptr<IP_NetlistStructSequence> buildStuff(std::vector< std::string> &expectedEnd);

  /** here is the definition tree (top level) */
  boost::shared_ptr<IP_SubcirDef> topDefTree_;

  /** this signals if the parsing is on the top level */
  bool isParsingTopLevel_;

  /** the actual tokens read-in by the parser */
  std::vector<IP_Token> tokens_;

  /** scanner */
  boost::shared_ptr<IP_ScannerBase> scanner_;

  /** the level of the subcircuit nesting level=0 is toplevel */
  int subcircuitLevel_;
};

#endif /* IP_PARSERBASE_HPP_ */
