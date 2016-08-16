//
// Copyright 2009 Arun Saha
//
#ifndef CLI_EPUPARSERHANDLER_H
#define CLI_EPUPARSERHANDLER_H

#include <cli/main/ParserMain.h>

#include <mol/common/MolVp.h>
#include <mol/epu/EpuMgr.h>

class EpuParserHandler {
 public:
  EpuParserHandler();
  ~EpuParserHandler();

  void epuAdminStateCfg(bool) const;
  void epuFdIdCfg(FwdDomainId const &) const;
  void epuMatchCfg(EpuMatchCriteria const &) const;
  void epuIngressRewriteCfg(EpuRewriteOp const &) const;
  void epuEgressRewriteCfg(EpuRewriteOp const &) const;

  void epuParser(string const &, bool &, ConfigState &);

 private:
  ParserMain *parserMain() const;
};

#endif
