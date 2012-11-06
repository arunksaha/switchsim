//
// Copyright 2009 Arun Saha
//
#ifndef CLI_PORTPARSERHANDLER_H
#define CLI_PORTPARSERHANDLER_H

#include <cli/main/ParserMain.h>

#include <mol/common/MolVp.h>
#include <mol/epu/EpuMgr.h>
#include <mol/epu/Epu.h>

class PortParserHandler {

  public:
	PortParserHandler(); 
	~PortParserHandler(); 

	bool modifyEpuN( unsigned );

	void deleteEpuN( unsigned );

  private:

	ParserMain 		* parserMain() const;
};

#endif

