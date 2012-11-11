//
// Copyright 2009 Arun Saha
//
#include <cli/port/PortParser.h>
#include <cli/common/CommonParserNodes.h>

#include <spirit/spiritinclude.h>
using namespace boost::spirit;
using namespace boost::spirit::classic;

static PortParserHandler * pph_Copy_;

extern ParserMain * MyParserMain_;

PortParser::PortParser()
{
	pph_	= new PortParserHandler;
	assert( pph_ );

	pph_Copy_ = pph_;
}

PortParser::~PortParser()
{
	if( pph_ )
	{
		delete pph_;
		pph_ = 0;

		pph_Copy_ = 0;
	}
}

///////////////////////////////////////////////////////////////////////////

	static short	ParserModeChange;
	static bool		HelpFlag;
    unsigned 	    PortParam1; 
	bool		    noFlag;


	void acceptPortHelpEol( const char * a, const char * b )
	{
		HelpFlag = true;
	}

	void doPortHelp( const char * a, const char * b )
	{
		printHelp( ExitNode );
		printHelp( EndNode );
		printHelp( NoNode );
		printHelp( EpuNode );
	}

	void doPortNoHelp( const char * a, const char * b )
	{
		printHelp( EpuNode );
	}

	void rememberNo( const char * a, const char * b )
	{
		noFlag = true;
	}

	void doPortEpuHelp( const char * a, const char * b )
	{
        printEpuIdHelp();
	}

    void getPortEpuId( const unsigned int x )
    {
        PortParam1 = x;
    }

	void doPortEpuIdHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

    void acceptPortEpuIdEol( const char * a, const char * b )
    {

		if( noFlag )
		{
			pph_Copy_->deleteEpuN( PortParam1 );
		}
		else
		{
        	bool success = pph_Copy_->modifyEpuN( PortParam1 );

			if( success ) 
			{
				ParserModeChange = +1;
			}
			else
			{
				cout << "Command REJECTED: Probably this EPU exists in some other Port." << endl;
			}
		}

        PortParam1 = 0;

    }

	void doPortExitHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doPortExitEol( const char * a, const char * b )
	{
		MyParserMain_->configState()->currPortId_ = 0;
		ParserModeChange = -1;
		cout << " -> " << __PRETTY_FUNCTION__ << endl;
	}

	void doPortEndHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doPortEndEol( const char * a, const char * b )
	{
		MyParserMain_->configState()->currPortId_ = 0;
		ParserModeChange = -2;
		cout << " -> " << __PRETTY_FUNCTION__ << endl;
	}

// Grammar
struct PortGrammar : public grammar< PortGrammar >
{
    template <typename ScannerT>
    struct definition
    {
        definition( PortGrammar const& /*self*/ )
        {
            // Expected tokens
			HelpTok		= ch_p('?') >> *print_p >> end_p[ &acceptPortHelpEol ];
			NoTok		= str_p( NoNode.token() );
            EpuTok 		= str_p( EpuNode.token() );
			ExitTok 	= str_p( ExitNode.token() );
            EndTok      = str_p( EndNode.token() );

            // Production rules
            expression 		= PortHelp | NoRuleNode | EpuRuleNode 
								| ExitRuleNode | EndRuleNode;
			PortHelp		= HelpTok[ &doPortHelp ];

			NoRuleNode		= NoTok[ &rememberNo ] >> (NoHelp | EpuRuleNode); 
			NoHelp			= HelpTok[ &doPortNoHelp ];

			EpuRuleNode 	= EpuTok >> (EpuHelp | EpuIdRuleNode);
			EpuHelp			= HelpTok[ &doPortEpuHelp ];
			EpuIdRuleNode	= uint_p[ &getPortEpuId ] >> (EpuIdHelp | EpuIdEol);
			EpuIdHelp		= HelpTok[ &doPortEpuIdHelp ];
			EpuIdEol		= end_p[ &acceptPortEpuIdEol ];

			ExitRuleNode	= ExitTok >> (ExitHelp | ExitEol);
			ExitHelp		= (HelpTok)[ &doPortExitHelp ];
			ExitEol			= end_p[ &doPortExitEol ];

			EndRuleNode	    = EndTok >> (EndHelp | EndEol);
			EndHelp		    = (HelpTok)[ &doPortEndHelp ];
			EndEol			= end_p[ &doPortEndEol ];

        }

        rule< ScannerT > HelpTok, EpuTok, expression,
							PortHelp, EpuRuleNode, EpuHelp,
							NoTok, NoRuleNode, NoHelp,
							EpuIdRuleNode, EpuIdHelp, EpuIdEol,
							ExitTok, ExitRuleNode, ExitHelp, ExitEol,
							EndTok, EndRuleNode, EndHelp, EndEol
							;

        rule< ScannerT > const& start() const
        {
            return expression;
        }
    };
};

void
PortParser::portParser( 
	string const & inputString, 
	bool & helpRequested,
	ConfigState * c )
{
    PortParam1 = 0;
	ParserModeChange = 0;
	HelpFlag = false;
	noFlag = false;

    assert( c );
	c->currParserMode_ = ParserModePort;

    PortGrammar g;
    parse_info<> parseInfo = 
        parse(inputString.c_str(), g, space_p);

    // cout << "  --In portParser..." << endl;

    if( parseInfo.full )
    {
        // cout << "Parsed successfully" << endl;

        if( ParserModeChange != 0 )
        {
		    assert( HelpFlag == false );

		    if( ParserModeChange == +1 )
        	    c->currParserMode_ = ParserModeEpu;
		    else if( ParserModeChange == -1 )
        	    c->currParserMode_ = ParserModeConfig;
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
            cout << "Syntax ERROR: Input unrecognized. (PortParser)" << endl;
        }
    }

    return;
}
