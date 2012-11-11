//
// Copyright 2009 Arun Saha
//
#include <cli/global/GlobalParser.h>
#include <cli/common/CommonParserNodes.h>

#include <spirit/spiritinclude.h>
using namespace boost::spirit;
using namespace boost::spirit::classic;

static GlobalParserHandler * gph_Copy_; 

extern ParserMain * MyParserMain_;

GlobalParser::GlobalParser()
 :  gph_( 0 )
{
	gph_	= new GlobalParserHandler;
	assert( gph_ );

	gph_Copy_ = gph_;
}

GlobalParser::~GlobalParser()
{
	if( gph_ )
	{
		delete gph_;
		gph_ = 0;

		gph_Copy_ = 0;
	}
}

///////////////////////////////////////////////////////////////////////////

namespace GlobalParserNameSpace
{
	bool            ParserModeChanged;
    ParserMode      NewParserMode;
	bool	        HelpFlag;
    bool            CountersFlag;

    unsigned 	    GlobalParam1; 
    string          Hostname;

	ParseNode ShowNode			( "show",		"show something" );
	ParseNode ConfigNode		( "config",		"configure something" );
	ParseNode RunNode			( "run",		"running configuration" );
    ParseNode SaveNode          ( "save",       "save current configuration" );

	void doGlobalHelp( const char * a, const char * b )
	{
		printHelp( ShowNode );
		printHelp( ConfigNode );
		printHelp( SaveNode );
		printHelp( HostnameNode );
		printHelp( SimulationNode );
	}

	void doGlobalShowHelp( const char * a, const char * b )
	{
		printHelp( RunNode );
		printHelp( PortNode );
		printHelp( VbNode );
		printHelp( EpuNode );
        printHelp( MacTableNode );
	}

	void doGlobalShowRunHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doSaveHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doGlobalShowPortHelp( const char * a, const char * b )
	{
		printHelp( InputNumberNode );
        printHelp( CountersNode );
		printHelp( ReturnNode );
	}

	void doGlobalPortIdHelp( const char * a, const char * b )
	{
        printHelp( CountersNode );
		printHelp( ReturnNode );
	}

	void doGlobalShowVbHelp( const char * a, const char * b )
	{
		printHelp( InputNumberNode );
        printHelp( CountersNode );
		printHelp( ReturnNode );
	}

	void doMacTableVbHelp( const char * a, const char * b )
	{
		printHelp( InputNumberNode );
		printHelp( ReturnNode );
	}

	void doGlobalVbIdHelp( const char * a, const char * b )
	{
        printHelp( CountersNode );
		printHelp( ReturnNode );
	}

	void doMacTableVbIdHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doGlobalShowEpuHelp( const char * a, const char * b )
	{
		printHelp( InputNumberNode );
        printHelp( CountersNode );
		printHelp( ReturnNode );
	}

	void doGlobalEpuIdHelp( const char * a, const char * b )
	{
        printHelp( CountersNode );
		printHelp( ReturnNode);
	}

	void doGlobalPortIdCountersHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode);
	}

	void doGlobalVbIdCountersHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode);
	}

	void doGlobalEpuIdCountersHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode);
	}

	void doGlobalConfigHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode);
	}

	void doGlobalShowMacTableHelp( const char * a, const char * b )
	{
        printHelp( VbNode );
		printHelp( ReturnNode );
	}

    void doGlobalShowMacTableVbHelp( const char * a, const char * b )
    {
		printHelp( InputNumberNode );
    }

	void acceptGlobalHelpEol( const char * a, const char * b )
	{
		HelpFlag = true;
	}

    void acceptGlobalRunEol( const char * a, const char * b )
    {
        gph_Copy_->showRun();
    }

    void acceptSaveEol( const char * a, const char * b )
    {
        gph_Copy_->saveConfig();
    }

	void doHostnameHelp( const char * a, const char * b )
	{
		printf( "  %12s        %s\n", 
			"character string", "name of this device (max 16 alphanumeric characters)" );
	}

    void doHostnameStringHelp( const char * a, const char * b )
    {
        printHelp( ReturnNode );
    }

    void acceptHostnameEol( const char * a, const char * b )
    {
        if( ! Hostname.empty() )
        {
            gph_Copy_->configHostname( Hostname );
        }
    }

    void getGlobalPortId( const unsigned int x )
    {
        GlobalParam1 = x;
    }

    void getGlobalVbId( const unsigned int x )
    {
        GlobalParam1 = x;
    }

    void getGlobalEpuId( const unsigned int x )
    {
        GlobalParam1 = x;
    }

    void acceptCountersFlag( const char * a, const char * b )
    {
        CountersFlag = true;
    }

    void acceptGlobalPortIdEol( const char * a, const char * b )
    {
        gph_Copy_->showPorts( GlobalParam1, CountersFlag );
        GlobalParam1 = 0;
        CountersFlag = false;
    }
    
    void acceptGlobalVbIdEol( const char * a, const char * b )
    {
        gph_Copy_->showFds( GlobalParam1, CountersFlag );
        GlobalParam1 = 0;
        CountersFlag = false;
    }
    
    void acceptGlobalEpuIdEol( const char * a, const char * b )
    {
        gph_Copy_->showEpus( GlobalParam1, CountersFlag );
        GlobalParam1 = 0;
        CountersFlag = false;
    }

    void acceptGlobalMacTableEol( const char * a, const char * b )
    {
        gph_Copy_->showMacTable();
    }
    
    void acceptGlobalConfigEol( const char * a, const char * b )
    {
		gph_Copy_->parserInConfigMode();
		ParserModeChanged   = true;
        NewParserMode       = ParserModeConfig;
    }
    
    void doSimulHelp( const char * a, const char * b )
    {
        printHelp( ReturnNode );
    }

    void acceptSimulEol( const char * a, const char * b )
    {
		gph_Copy_->parserInSimulMode();
		ParserModeChanged   = true;
        NewParserMode       = ParserModeSimul;
    }
}

// Grammar
struct GlobalGrammar : public grammar< GlobalGrammar >
{

    template <typename ScannerT>
    struct definition
    {
        definition( GlobalGrammar const& /*self*/ )
        {
            using namespace GlobalParserNameSpace;

            // Expected tokens
			HelpTok		= ch_p('?') >> *print_p >> end_p[ &acceptGlobalHelpEol ];
			ShowTok		= str_p( ShowNode.token() );
			ConfigTok	= str_p( ConfigNode.token() );
			RunTok		= str_p( RunNode.token() );
			PortTok		= str_p( PortNode.token() );
			VbTok		= str_p( VbNode.token() );
			EpuTok		= str_p( EpuNode.token() );
            SaveTok     = str_p( SaveNode.token() );
            HostnameTok = str_p( HostnameNode.token() );
            SimulTok    = str_p( SimulationNode.token() );
            CountersTok = str_p( CountersNode.token() );
            MacTableTok = str_p( MacTableNode.token() );

            // Production rules
			expression		= GlobalHelp | ShowRuleNode | ConfigRuleNode | SaveRuleNode
                                | HostnameRuleNode | SimulRuleNode;
			GlobalHelp		= HelpTok[ &doGlobalHelp ];

			ShowRuleNode	= ShowTok >> (ShowHelp | ShowRunRuleNode | ShowPortRuleNode | 
								ShowVbRuleNode | ShowEpuRuleNode | ShowMacRuleNode );
			ShowHelp		= HelpTok[ &doGlobalShowHelp ];

			ShowRunRuleNode	= RunTok >> (RunHelp | RunEol);
			RunHelp			= HelpTok[ &doGlobalShowRunHelp ];
			RunEol			= end_p[ &acceptGlobalRunEol ];

			ShowPortRuleNode
							= PortTok >> (PortHelp | PortIdRuleNode | PortIdCountersNode | PortIdEol);
			PortHelp		= HelpTok[ &doGlobalShowPortHelp ];
			PortIdRuleNode	= uint_p[ &getGlobalPortId ] >> (PortIdHelp | PortIdCountersNode | PortIdEol);
			PortIdHelp		= HelpTok[ &doGlobalPortIdHelp ];
            PortIdCountersNode = CountersTok[ &acceptCountersFlag ] >> (PortIdCountersHelp | PortIdEol);
            PortIdCountersHelp = HelpTok[ &doGlobalPortIdCountersHelp ];
			PortIdEol		= end_p[ &acceptGlobalPortIdEol ];

			ShowVbRuleNode 	= VbTok >> (VbHelp | VbIdRuleNode | VbIdCountersNode | VbIdEol);
			VbHelp			= HelpTok[ &doGlobalShowVbHelp ];
			VbIdRuleNode	= uint_p[ &getGlobalVbId ] >> (VbIdHelp | VbIdCountersNode | VbIdEol);
			VbIdHelp		= HelpTok[ &doGlobalVbIdHelp ];
            VbIdCountersNode = CountersTok[ &acceptCountersFlag ] >> (VbIdCountersHelp | VbIdEol);
            VbIdCountersHelp = HelpTok[ &doGlobalVbIdCountersHelp ];
			VbIdEol			= end_p[ &acceptGlobalVbIdEol ];

			ShowEpuRuleNode	= EpuTok >> (EpuHelp | EpuIdRuleNode | EpuIdCountersNode | EpuIdEol);
			EpuHelp			= HelpTok[ &doGlobalShowEpuHelp ];
			EpuIdRuleNode	= uint_p[ &getGlobalEpuId ] >> (EpuIdHelp | EpuIdCountersNode | EpuIdEol);
			EpuIdHelp		= HelpTok[ &doGlobalEpuIdHelp ];
            EpuIdCountersNode = CountersTok[ &acceptCountersFlag ] >> (EpuIdCountersHelp | EpuIdEol);
            EpuIdCountersHelp = HelpTok[ &doGlobalEpuIdCountersHelp ];
			EpuIdEol		= end_p[ &acceptGlobalEpuIdEol ];

            ShowMacRuleNode = MacTableTok >> (MacTableHelp | MacTableVbRuleNode | MacTableEol);
            MacTableHelp    = HelpTok[ &doGlobalShowMacTableHelp ];
            MacTableEol     = end_p[ &acceptGlobalMacTableEol ];
            MacTableVbRuleNode      = VbTok >> (MacTableVbHelp | MacTableVbIdRuleNode);
            MacTableVbHelp          = HelpTok[ &doGlobalShowMacTableVbHelp ];
            MacTableVbIdRuleNode    = uint_p[ &getGlobalVbId ] >> (MacTableVbIdHelp | MacTableEol);
            MacTableVbIdHelp        = HelpTok[ &doMacTableVbIdHelp ];


			ConfigRuleNode	= ConfigTok >> (ConfigHelp | ConfigEol);
			ConfigHelp		= HelpTok[ &doGlobalConfigHelp ];
			ConfigEol		= end_p[ &acceptGlobalConfigEol ];


            SaveRuleNode    = SaveTok >> (SaveHelp | SaveEol);
            SaveHelp        = HelpTok[ &doSaveHelp ];
            SaveEol         = end_p[ &acceptSaveEol ];


            HostnameRuleNode = HostnameTok >> (HostnameHelp | HostnameStringRuleNode);
            HostnameHelp    = (HelpTok)[ &doHostnameHelp ];
            HostnameStringRuleNode
                            = (+alnum_p)[ assign_a( Hostname ) ] >> (HostnameStringHelp | HostnameEol);
            HostnameStringHelp
                            = (HelpTok)[ &doHostnameStringHelp ];
            HostnameEol     = end_p[ &acceptHostnameEol ];


            SimulRuleNode   = SimulTok >> (SimulHelp | SimulEol);
            SimulHelp       = (HelpTok)[ &doSimulHelp ];
            SimulEol        = end_p[ &acceptSimulEol ];

        }

        rule< ScannerT > expression, GlobalHelp, ShowRuleNode, ConfigRuleNode,
							HelpTok, ShowTok, ConfigTok, PortTok, VbTok, EpuTok,
                            CountersTok, MacTableTok,
							ShowHelp, ShowPortRuleNode, ShowVbRuleNode, ShowEpuRuleNode,
							PortHelp, PortIdRuleNode, VbHelp, VbIdRuleNode, EpuHelp, 
							EpuIdRuleNode, RunTok, ShowRunRuleNode, RunHelp, RunEol,
                            PortIdCountersNode, PortIdCountersHelp,
                            VbIdCountersNode, VbIdCountersHelp,
                            EpuIdCountersNode, EpuIdCountersHelp,
							PortIdHelp, PortIdEol, VbIdHelp, VbIdEol, EpuIdHelp, EpuIdEol,
                            ShowMacRuleNode, MacTableHelp, MacTableEol,
                            MacTableVbRuleNode, MacTableVbHelp, MacTableVbIdRuleNode, MacTableVbIdHelp,
							ConfigHelp, ConfigEol,
                            SaveRuleNode, SaveTok, SaveHelp, SaveEol,
                            HostnameRuleNode, HostnameHelp, HostnameStringRuleNode,
                            HostnameStringHelp, HostnameEol,
                            SimulRuleNode, SimulHelp, SimulEol, HostnameTok, SimulTok 
							;

        rule< ScannerT > const& start() const
        {
            return expression;
        }
    };
};

void
GlobalParser::globalParser( 
	string const & inputString, 
	bool & helpRequested,
	ConfigState * c )
{
    using namespace GlobalParserNameSpace;

	ParserModeChanged = false;
    NewParserMode = ParserModeGlobal;
    assert( c );
	c->currParserMode_ = ParserModeGlobal;
	HelpFlag = false;
    CountersFlag = false;

	GlobalParam1 = 0;
    Hostname.clear();


    GlobalGrammar g;
    parse_info<> parseInfo = 
        parse(inputString.c_str(), g, space_p);

    // cout << "  --In globalParser..." << endl;

    if( parseInfo.full )
    {
        // cout << "Parsed successfully" << endl;

        if( ParserModeChanged )
        {
		    assert( ! HelpFlag );

            c->currParserMode_ = NewParserMode;
        }

		if( HelpFlag )
		{
			assert( ! ParserModeChanged );
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
