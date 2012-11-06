//
// Copyright 2009 Arun Saha
//
#ifndef CLI_GLOBALPARSER_H
#define CLI_GLOBALPARSER_H

#include <cli/main/ParserMain.h>
#include <cli/global/GlobalParserHandler.h>

class GlobalParser {

  public:
	GlobalParser();
	~GlobalParser();

	ParserMain * parserMain() const;

	void globalParser( string const &, bool &, ConfigState * );

  // private:

	GlobalParser( GlobalParser const & );
    GlobalParser & operator=( GlobalParser const & );

	GlobalParserHandler		* gph_;
};

#endif
