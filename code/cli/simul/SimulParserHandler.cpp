//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <cli/simul/SimulParserHandler.h>

#include <mol/common/MolVp.h>
#include <mol/epu/FwdDomainMgr.h>
#include <mol/port/PortMgr.h>

#include <dataplane/DataPlaneMgr.h>

extern ParserMain* MyParserMain_;

SimulParserHandler::SimulParserHandler() {}

SimulParserHandler::~SimulParserHandler() {}

void SimulParserHandler::rxFrame(PortId pId, Frame f) {
  if (pId >= portMgr()->numPorts()) {
    cerr << "Port " << pId
         << "does not exist, please specify a port number in 0-"
         << portMgr()->numPorts() << endl;
    return;
  }

  cout << "\tIngress Frame at " << PortNode.tokenString() << " " << pId << endl;
  cout << "\t\tFrame := " << f << endl << endl;

  dataPlaneMgr()->processFrame(pId, f);
}

ParserMain* SimulParserHandler::parserMain() const {
  assert(MyParserMain_);
  return (MyParserMain_);
}
