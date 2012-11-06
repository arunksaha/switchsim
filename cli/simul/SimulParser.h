//
// Copyright 2009 Arun Saha
//
#ifndef CLI_SIMULPARSER_H
#define CLI_SIMULPARSER_H

#include <cli/main/ParserMain.h>
#include <cli/simul/SimulParserHandler.h>

class SimulParser {

  public:
	SimulParser();
	~SimulParser();
	void molVp( MolVp * );

	void simulParser( string const &, bool &, ConfigState * );

  // private:

	SimulParser( SimulParser const & );
    SimulParser & operator=( SimulParser const & );

	SimulParserHandler		* sph_;
  	
};

#endif
