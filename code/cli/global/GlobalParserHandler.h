//
// Copyright 2009 Arun Saha
//
#ifndef CLI_GLOBALPARSERHANDLER_H
#define CLI_GLOBALPARSERHANDLER_H

#include <cli/main/ParserMain.h>
#include <mol/common/Types.h>

class GlobalParserHandler {
 public:
  GlobalParserHandler();
  ~GlobalParserHandler();

  void showRun();
  void showPorts(const unsigned, const bool);
  void showFds(const unsigned, const bool);
  void showEpus(const unsigned, const bool);
  void showMacTable();

  void saveConfig() const;

  void debugPorts(const unsigned);

  void parserInConfigMode();
  void parserInSimulMode();

  void configHostname(string const&);

 private:
  ParserMain* parserMain() const;
};

#endif
