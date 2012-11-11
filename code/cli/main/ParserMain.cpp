//
// Copyright 2009 Arun Saha
//

#include <cli/main/ParserMain.h>

#include <cli/common/CommonParserNodes.h>
#include <cli/global/GlobalParser.h>
#include <cli/config/ConfigParser.h>
#include <cli/port/PortParser.h>
#include <cli/epu/EpuParser.h>
#include <cli/simul/SimulParser.h>

#include <mol/common/MolVp.h>

///////////////////////////////////////////////////////////

ParserMain * MyParserMain_;

///////////////////////////////////////////////////////////

ParserMain::ParserMain()
 :  molVp_( 0 ),
    cfgState_( 0 ),
    gp_( 0 ),
    cp_( 0 ),
    pp_( 0 ),
    ep_( 0 ),
    sp_( 0 ),
    SavedConfigFileName_( "" ),
    globalPromptSuffix_( "" ),
    configPromptSuffix_( "" ),
    portPromptSuffix_( "" ),
    epuPromptSuffix_( "" ),
    vbPromptSuffix_( "" ),
    simulPromptSuffix_( "" )
{
	molVp_ = 0;

	cfgState_ = new ConfigState;
	assert( cfgState_ );

	gp_ 	= new GlobalParser;
	assert( gp_ );

	cp_		= new ConfigParser;
	assert( cp_ );

	pp_		= new PortParser;
	assert( pp_ );

	ep_		= new EpuParser;
	assert( ep_ );

	sp_		= new SimulParser;
	assert( sp_ );

    SavedConfigFileName_ = "saved_config.txt";

    globalPromptSuffix_     = "#";
    configPromptSuffix_     = "(config)#";
    portPromptSuffix_       = "(config-" + PortNode.tokenString() + ")#";
    epuPromptSuffix_        = "(config-" + PortNode.tokenString() + "-" 
                                + EpuNode.tokenString() + ")#"; 
    vbPromptSuffix_         = "(config-" + VbNode.tokenString() + ")#";
    simulPromptSuffix_      = "(" + SimulationNode.tokenString() + ")#";

	MyParserMain_ = this;
}

ParserMain::~ParserMain()
{
    assert( cfgState_ );
	delete cfgState_;
	cfgState_ = 0;

    assert( gp_ );
	delete gp_;
	gp_ = 0;

    assert( cp_ );
	delete cp_;
	cp_ = 0;

    assert( pp_ );
	delete pp_;
	pp_ = 0;

    assert( ep_ );
	delete ep_;
	ep_ = 0;

    assert( sp_ );
	delete sp_;
	sp_ = 0;

    assert( molVp_ );
	delete molVp_;
	molVp_ = 0;
}

MolVp *
ParserMain::molVp() const
{
	return( molVp_ );
}

void
ParserMain::molVp( MolVp * m )
{
	assert( !molVp_ && m );
	molVp_ = m;
}

ConfigState *
ParserMain::configState() const
{
	assert( MyParserMain_ );
	return( MyParserMain_->cfgState_ );
}

string
ParserMain::globalPrompt() const
{
    return( configState()->hostname() + globalPromptSuffix_ );
}

string
ParserMain::configPrompt() const
{
    return( configState()->hostname() + configPromptSuffix_ );
}

string
ParserMain::portPrompt() const
{
    return( configState()->hostname() + portPromptSuffix_ );
}

string
ParserMain::epuPrompt() const
{
    return( configState()->hostname() + epuPromptSuffix_ );
}

string
ParserMain::vbPrompt() const
{
    return( configState()->hostname() + vbPromptSuffix_ );
}

string
ParserMain::simulPrompt() const
{
    return( configState()->hostname() + simulPromptSuffix_ );
}


void
ParserMain::printPrompt( ParserMode m ) const
{
    cout << endl;

    switch( m )
    {
        case ParserModeGlobal:
            cout << globalPrompt();
            break;

        case ParserModeConfig:
            cout << configPrompt();
            break;

        case ParserModePort:
            cout << portPrompt();
            break;

        case ParserModeEpu:
            cout << epuPrompt();
            break;

        case ParserModeVb:
            cout << vbPrompt();
            break;

        case ParserModeSimul:
            cout << simulPrompt();
            break;
    }
}


void
ParserMain::main()
{
    string      rawInputString, storedString, modifiedInputString;
	bool		helpReq;

	rawInputString.clear();
	storedString.clear();
	modifiedInputString.clear();

    readSavedConfig();

    printPrompt( cfgState_->currParserMode_ );

    while( true )
    {
		getline( cin, rawInputString );

        if( rawInputString.compare( "quit" ) == 0 )
        {
            cout << "Quiting.." << endl;
            break;
        }

        if( rawInputString.empty() && storedString.empty() )
		{
           	printPrompt( cfgState_->currParserMode_ );
           	continue;
        }

		modifiedInputString = storedString + rawInputString;
        
		cfgState_->commandLines().push_back( modifiedInputString );

		helpReq = false;

        switch( cfgState_->currParserMode_ )
        {
            case ParserModeGlobal:
                gp_->globalParser( modifiedInputString, helpReq, cfgState_ );
                break;

            case ParserModeConfig:
                cp_->configParser( modifiedInputString, helpReq, cfgState_ );
                break;

            case ParserModePort:
                pp_->portParser( modifiedInputString, helpReq, cfgState_ );
                break;

            case ParserModeEpu:
                ep_->epuParser( modifiedInputString, helpReq, cfgState_ );
                break;

            case ParserModeVb:
                break;

            case ParserModeSimul:
                sp_->simulParser( modifiedInputString, helpReq, cfgState_ );
                break;
        }

		if( helpReq )
		{
			size_t posQMark = modifiedInputString.find( '?' );
			if( posQMark == 0 )
			{
				modifiedInputString.clear();
				storedString.clear();
			}
			else
			{
				storedString = modifiedInputString.substr( 0, posQMark );
			}
		}
		else
		{
			storedString.clear();
		}

        printPrompt( cfgState_->currParserMode_ );
		cout << storedString;

    }

}

void
ParserMain::readSavedConfig() const
{
    string   line;
    ifstream savedFile;

    cfgState_->parserVerbosity_ = false;

    savedFile.open( SavedConfigFileName_, ios::in );

    if( savedFile.is_open() )
    {
        while( ! savedFile.eof() )
        {
            getline( savedFile, line );

            cout << line << endl;

            if( line.empty() )
            {
                continue;
            }

            if( line.compare( "!" ) == 0 )
            {
	            cfgState_->currParserMode_ = ParserModeConfig;
                continue;
            }

            parseSavedLine( line );
            
            line.clear();
        }

        savedFile.close();

	    cfgState_->currParserMode_ = ParserModeGlobal;
    }

    cfgState_->parserVerbosity_ = true;
}

void
ParserMain::parseSavedLine( string const & inputString ) const
{
    bool helpReq = false;

    switch( cfgState_->currParserMode_ )
    {
        case ParserModeGlobal:
            gp_->globalParser( inputString, helpReq, cfgState_ );
            break;

        case ParserModeConfig:
            cp_->configParser( inputString, helpReq, cfgState_ );
            break;

        case ParserModePort:
    TryParserModePort:                                 // Aha :->         
            pp_->portParser( inputString, helpReq, cfgState_ );
            break;

        case ParserModeEpu:
            if( ! ep_->epuParser( inputString, helpReq, cfgState_ ) )
            {
                goto TryParserModePort;
            }
            break;

        case ParserModeVb:
            break;

        case ParserModeSimul:
            break;
    }
}

///////////////////////////////////////////////////////////

ConfigState::ConfigState() 
 :  hostname_( "SoftSwitch" ),
    commandLines_(),
    currParserMode_( ParserModeGlobal ),
    currPortId_( 0 ),
    currEpuId_( 0 ),
    parserVerbosity_( true )
{
}

ConfigState::~ConfigState() 
{
	clear();
}

void
ConfigState::clear() 
{
    hostname_ 		= string( "SoftSwitch" );
	currParserMode_ = ParserModeGlobal;
	currPortId_		= 0;
	currEpuId_		= 0;
    parserVerbosity_ = true;
	commandLines_.clear();
}

string
ConfigState::hostname() const
{
    return hostname_;
}

void
ConfigState::hostname( string const & s )
{
    hostname_ = s;
}

vector< string > &
ConfigState::commandLines() 
{
	return commandLines_;
}


///////////////////////////////////////////////////////////

void
printHelp( ParseNode const & p )
{
	printf( "  %12s        %s\n", p.token(), p.helpString().c_str() );
}

///////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

/*
// #include <curses.h>
	// int			c;
	// unsigned	currIndex, currSize;

			if( ! possibleInputString.empty() )
			{
				rawInputString = possibleInputString;
				possibleInputString.clear();
			}
			else

		currSize = (unsigned) cfg.commandLines().size();
		currIndex = currSize - 1;

COMMAND_HISTORY:

		// noecho();
		// refresh();
		keypad ( stdscr, TRUE );

		c = getch();
		switch( c )
		{
			case KEY_UP:
			  cout << "Up" << endl;
			  // Find previous command than current
			  if( currIndex - 1 >= 0 )
			  {
			    currIndex = currIndex - 1;
			  	possibleInputString = lines[ currIndex ];
				cout << possibleInputString;
			  }
			  goto COMMAND_HISTORY;

			case KEY_DOWN:
			  cout << "Down" << endl;
			  // Find next command than current
			  if( currIndex + 1 < currSize )
			  {
			    currIndex = currIndex + 1;
			  	possibleInputString = lines[ currIndex ];
				cout << possibleInputString;
			  }
			  goto COMMAND_HISTORY;

			default:
			  // ungetch( c );
			  break;
		}

		// echo();
		keypad( stdscr, FALSE );
*/

