//
// Copyright 2009 Arun Saha
//
#ifndef CLI_MAIN_PARSERMAIN_H
#define CLI_MAIN_PARSERMAIN_H

#include <cli/common/Types.h>

using namespace std;

enum ParserMode {
    ParserModeGlobal = 0,
    ParserModeConfig,
    ParserModePort,
    ParserModeEpu,
    ParserModeVb,
    ParserModeSimul
};

//////////////////////////////////////////////////////////////////////////////
class ParseNode
{
  public:

	ParseNode( string const & t, string const & h )
     : token_( t ), helpString_( h )
	{
	}

	ParseNode()
     : token_( "ERROR" ), helpString_( "ERROR" )
	{
	}

	~ParseNode()
	{
		token_ 		= "ERROR";
		helpString_	= "ERROR";	
	}

    ParseNode & operator=( ParseNode const & rhs )
    {
        // ToDo
        // if( this != rhs )
        {
            token_          = rhs.token_;
            helpString_     = rhs.helpString_;
        }

        return (*this);
    }

	const char * token() const
	{
		return token_.c_str();
	}

	string tokenString() const
	{
		return token_;
	}

	string helpString() const
	{
		return helpString_;
	}

  private:

	string	token_;
	string	helpString_;
};

void printHelp( ParseNode const & );

//////////////////////////////////////////////////////////////////////////////
class ConfigState {

  public:

  	ConfigState();
	~ConfigState();
	void clear();

    string hostname() const;
    void hostname( string const & );

    vector< string > & commandLines();

  // private:

    string 				hostname_;
    vector< string > 	commandLines_;
  	ParserMode			currParserMode_;
	unsigned			currPortId_;
	unsigned			currEpuId_;
    bool                parserVerbosity_;
};

//////////////////////////////////////////////////////////////////////////////
class GlobalParser;
class ConfigParser;
class PortParser;
class EpuParser;
class SimulParser;

class MolVp;

class ParserMain {

  public:

  	ParserMain();
	~ParserMain();

	void main();
	void printPrompt( ParserMode m ) const;
    void readSavedConfig() const;
    void parseSavedLine( string const & ) const;

	MolVp 					* molVp() const;
	void					molVp( MolVp * );

	ConfigState				* configState() const;

    string                  globalPrompt() const;
    string                  configPrompt() const;
    string                  portPrompt() const;
    string                  epuPrompt() const;
    string                  vbPrompt() const;
    string                  simulPrompt() const;

  // private:

    ParserMain( ParserMain const & );
    ParserMain & operator=( ParserMain const & );

	MolVp					* molVp_;

	ConfigState   			* cfgState_;

	GlobalParser			* gp_;
	ConfigParser			* cp_;
	PortParser				* pp_;
	EpuParser				* ep_;
    SimulParser             * sp_;

    const char *            SavedConfigFileName_;     

    string                  globalPromptSuffix_;
    string                  configPromptSuffix_;
    string                  portPromptSuffix_;
    string                  epuPromptSuffix_;
    string                  vbPromptSuffix_;
    string                  simulPromptSuffix_;
};

#endif
