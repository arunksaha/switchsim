//
// Copyright 2009 Arun Saha
//
#ifndef CLI_CONFIGPARSERHANDLER_H
#define CLI_CONFIGPARSERHANDLER_H

#include <cli/main/ParserMain.h>
#include <mol/common/Types.h>
#include <mol/common/MolVp.h>

class ConfigParserHandler {

  public: 

	ConfigParserHandler();
	~ConfigParserHandler();

	void modifyPortN( PortId ) const;

    bool validPortId( PortId ) const;

  private:

	ParserMain 		* parserMain() const;
};

#endif
