//
// Copyright 2009 Arun Saha
//
#ifndef CLI_EPUPARSER_H
#define CLI_EPUPARSER_H

#include <cli/epu/EpuParserHandler.h>
#include <cli/main/ParserMain.h>

class EpuParser {
 public:
  EpuParser();
  ~EpuParser();
  void molVp(MolVp *);

  bool epuParser(string const &, bool &, ConfigState *);

  // private:

  EpuParser(EpuParser const &);
  EpuParser &operator=(EpuParser const &);

  EpuParserHandler *eph_;
};

#endif
