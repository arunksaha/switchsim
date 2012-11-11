//
// Copyright 2009 Arun Saha
//
#include <cli/config/ConfigParser.h>
#include <cli/common/CommonParserNodes.h>

#include <spirit/spiritinclude.h>
using namespace boost::spirit;
using namespace boost::spirit::classic;

static ConfigParserHandler * cph_Copy_;

extern ParserMain * MyParserMain_;

ConfigParser::ConfigParser()
{
	cph_	= new ConfigParserHandler;
	assert( cph_ );

	cph_Copy_ = cph_;
}

ConfigParser::~ConfigParser()
{
	if( cph_ )
	{
		delete cph_;
		cph_ = 0;

		cph_Copy_ = 0;
	}
}

///////////////////////////////////////////////////////////////////////////

static short	ParserModeChange;
static bool		HelpFlag;
unsigned 	    ConfigParam1; 


void acceptConfigHelpEol( const char * a, const char * b )
{
	HelpFlag = true;
}

void doConfigHelp( const char * a, const char * b )
{
	printHelp( ExitNode );
	printHelp( EndNode );
	printHelp( PortNode );
}

void doConfigPortHelp( const char * a, const char * b )
{
    printPortIdHelp();
}

void getConfigPortId( const unsigned int x )
{
    ConfigParam1 = x;
}

void doConfigPortIdHelp( const char * a, const char * b )
{
	printHelp( ReturnNode );
}

void acceptConfigPortIdEol( const char * a, const char * b )
{
    const bool acceptablePortId = cph_Copy_->validPortId( ConfigParam1 );
    if( !acceptablePortId )
    {
        cout << "ERROR: Invalid port number" << endl;
        return;
    }

	MyParserMain_->configState()->currPortId_ = ConfigParam1;
    cph_Copy_->modifyPortN( ConfigParam1 );
    ConfigParam1 = 0;

	ParserModeChange = +1;
}

void doConfigExitHelp( const char * a, const char * b )
{
	printHelp( ReturnNode );
}

void doConfigExitEol( const char * a, const char * b )
{
	ParserModeChange = -1;
	cout << " -> " << __PRETTY_FUNCTION__ << endl;
}

void doConfigEndHelp( const char * a, const char * b )
{
	printHelp( ReturnNode );
}

void doConfigEndEol( const char * a, const char * b )
{
	MyParserMain_->configState()->currPortId_ = 0;
	ParserModeChange = -2;
	cout << " -> " << __PRETTY_FUNCTION__ << endl;
}


// Grammar
struct ConfigGrammar : public grammar< ConfigGrammar >
{
    template <typename ScannerT>
    struct definition
    {
        definition( ConfigGrammar const& /*self*/ )
        {
            // Expected tokens
			HelpTok		= ch_p('?') >> *print_p >> end_p[ &acceptConfigHelpEol ];
            PortTok     = str_p( PortNode.token() );
			ExitTok     = str_p( ExitNode.token() );
            EndTok      = str_p( EndNode.token() );

            // Production rules
            expression 		= ConfigHelp | PortRuleNode | ExitRuleNode | EndRuleNode;
			ConfigHelp		= HelpTok[ &doConfigHelp ];

			PortRuleNode 	= PortTok >> (PortHelp | PortIdRuleNode);
			PortHelp		= HelpTok[ &doConfigPortHelp ];
			PortIdRuleNode	= uint_p[ &getConfigPortId ] >> (PortIdHelp | PortIdEol);
			PortIdHelp		= HelpTok[ &doConfigPortIdHelp ];
			PortIdEol		= end_p[ &acceptConfigPortIdEol ];

			ExitRuleNode	= ExitTok >> (ExitHelp | ExitEol);
			ExitHelp		= (HelpTok)[ &doConfigExitHelp ];
			ExitEol			= end_p[ &doConfigExitEol ];

			EndRuleNode	    = EndTok >> (EndHelp | EndEol);
			EndHelp		    = (HelpTok)[ &doConfigEndHelp ];
			EndEol			= end_p[ &doConfigEndEol ];
        }

        rule< ScannerT > HelpTok, PortTok, ExitTok, expression,
							ConfigHelp, PortRuleNode, PortHelp,
							PortIdRuleNode, PortIdHelp, PortIdEol,
							ExitRuleNode, ExitHelp, ExitEol,
							EndTok, EndRuleNode, EndHelp, EndEol
							;

        rule< ScannerT > const& start() const
        {
            return expression;
        }
    };
};

void
ConfigParser::configParser( 
	string const & inputString, 
	bool & helpRequested,
	ConfigState * c )
{
	ConfigParam1 = 0;
	ParserModeChange = 0;
	HelpFlag = false;

    assert( c );
	c->currParserMode_ = ParserModeConfig;

    ConfigGrammar g;
    parse_info<> parseInfo = 
        parse(inputString.c_str(), g, space_p);

    // cout << "  --In configParser..." << endl;

    if( parseInfo.full )
    {
        // cout << "Parsed successfully" << endl;

        if( ParserModeChange != 0 )
        {
		    assert( HelpFlag == false );

		    if( ParserModeChange == +1 )
        	    c->currParserMode_ = ParserModePort;
		    else if( ParserModeChange == -1 )
        	    c->currParserMode_ = ParserModeGlobal;
		    else if( ParserModeChange == -2 )
        	    c->currParserMode_ = ParserModeGlobal;
            else
                assert( 0 );
        }

		if( HelpFlag )
		{
			assert( ParserModeChange == 0 );
			helpRequested = true;
		}
    }
    else
    {
        if( c->parserVerbosity_ )
        {
            cout << "Syntax ERROR: Input unrecognized." << endl;
        }
    }

    return;
}
