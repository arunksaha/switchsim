//
// Copyright 2009 Arun Saha
//
#ifndef CLI_SIMULPARSERHANDLER_H
#define CLI_SIMULPARSERHANDLER_H

#include <cli/main/ParserMain.h>
#include <cli/common/Frame.h>
#include <mol/common/Types.h>

class SimulParserHandler {

  public: 
	
	SimulParserHandler();
	~SimulParserHandler();

    void rxFrame( PortId, Frame );

  private:

	ParserMain 		* parserMain() const;
};

#endif
