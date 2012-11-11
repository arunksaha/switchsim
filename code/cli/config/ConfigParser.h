//
// Copyright 2009 Arun Saha
//
#ifndef CLI_CONFIGPARSER_H
#define CLI_CONFIGPARSER_H

#include <cli/main/ParserMain.h>
#include <cli/config/ConfigParserHandler.h>

class ConfigParser {

  public: 
	ConfigParser();
	~ConfigParser();

	void configParser( string const &, bool &, ConfigState * );

  private: 

	ConfigParser( ConfigParser const & );
    ConfigParser & operator=( ConfigParser const & );

	ConfigParserHandler		* cph_;
};

#endif
