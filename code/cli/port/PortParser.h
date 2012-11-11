//
// Copyright 2009 Arun Saha
//
#ifndef CLI_PORTPARSER_H
#define CLI_PORTPARSER_H

#include <cli/main/ParserMain.h>
#include <cli/port/PortParserHandler.h>

class PortParser {

  public:

	PortParser();
	~PortParser();
	void molVp( MolVp * );

	// EpuMgr * epuMgr() const;

	void portParser( string const &, bool &, ConfigState * );

  // private:

	PortParser( PortParser const & );
    PortParser & operator=( PortParser const & );

	PortParserHandler		* pph_;

  	
};

#endif
