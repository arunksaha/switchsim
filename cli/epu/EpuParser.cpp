//
// Copyright 2009 Arun Saha
//

#include <cli/epu/EpuParser.h>
#include <cli/common/CommonParserNodes.h>
#include <cli/epu/EpuParserHandler.h>

#include <mol/epu/Epu.h>

#include <spirit/spiritinclude.h>
using namespace boost::spirit;
using namespace boost::spirit::classic;

static EpuParserHandler * eph_Copy_; 

extern ParserMain * MyParserMain_;

EpuParser::EpuParser()
{
	eph_	= new EpuParserHandler;
	assert( eph_ );

	eph_Copy_ = eph_;
}

EpuParser::~EpuParser()
{
	if( eph_ )
	{
		delete eph_;
		eph_ = 0;

		eph_Copy_ = 0;
	}
}

///////////////////////////////////////////////////////////////////////////

namespace EpuParserNameSpace
{
	enum ListType {
		OuterCos = 0,
		OuterVlanId,
		InnerCos,
		InnerVlanId,
		ListTypeNull
	};

    enum Direction {
        DirIngress = 0,
        DirEgress,
        DirUnknown
    };

	static short			ParserModeChange;
    static bool             HelpFlag;
	unsigned 				vbId, ptId; 
	bool					untagFlag;
	NumberList				nList;
	ListType				nListDest;
	unsigned short			lower;
	short					nthTag;
	EpuMatchCriteria		epuMatchConfig;

    signed                  whichTag;
    Direction               dir;
    VlanTag                 vlanTag;
    VlanHeader              vlanHeader;
    EpuRewriteOp            epuRewriteOp;


	/////////////////////////////////////////////////////////////////

	ParseNode IngressNode	( "ingress", 	"specify action for incoming frames" );
	ParseNode EgressNode	( "egress", 	"specify action for outgoing frames" );
	ParseNode MatchNode		( "match", 		"specify match criteria: which ingress frames at the container port are mapped to this?" );	
	ParseNode RewriteNode	( "rewrite", 	"specify VLAN tag translation rules" );	

	ParseNode UntagNode		( "untagged",	"match untagged frames" );	
	ParseNode PtagNode		( "ptagged",	"match priority-tagged frames" );	
	// ParseNode PtNode		( "passthrough", "associate this EPU to a passthrough (ELINE)" );
	ParseNode PopNode		( "pop", 		"specify number of VLAN tags to be popped from the top of the frame" );
	ParseNode PushNode		( "push", 		"specify VLAN tags to be pushed at the top of the frame" );
	ParseNode SwapNode		( "swap", 		"specify VLAN tags to be swapped in " );
	ParseNode AnyNode		( "any", 		"match any" );
	ParseNode UntagCommaNode	( ",",		"specify tag matching criteria followed by comma" );
	ParseNode PtagCommaNode		( ",",		"specify VLAN ID(s) followed by comma" );
	ParseNode NumberListNode	( "number-list", "single number, or comma separated list of number(s) or ranges(s), e.g. 10,20-30,40" );

	/////////////////////////////////////////////////////////////////

	void doEpuHelp( const char * a, const char * b )
	{
		printHelp( ExitNode );
		printHelp( EndNode );
        printHelp( ShutdownNode );
		printHelp( IngressNode );
		printHelp( EgressNode );
		printHelp( VbNode );
		// printHelp( PtNode );
	}


	void doEpuExitHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doEpuExitEol( const char * a, const char * b )
	{
		MyParserMain_->configState()->currEpuId_ = 0;
		ParserModeChange = -1;
		// cout << " -> " << __PRETTY_FUNCTION__ << endl;
	}


	void doEpuShutdownHelp( const char * a, const char * b )
	{
		printHelp( ShutdownNode );
	}

	void doEpuShutdownEol( const char * a, const char * b )
	{
		// cout << " -> " << __PRETTY_FUNCTION__ << endl;
		eph_Copy_->epuAdminStateCfg( true );
	}

	void doEpuEndHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void doEpuEndEol( const char * a, const char * b )
	{
		MyParserMain_->configState()->currEpuId_ = 0;
		ParserModeChange = -2;
		// cout << " -> " << __PRETTY_FUNCTION__ << endl;
	}


	/////////////////////////  vbridge start ///////////////////////
	void getEpuVbId( const unsigned x )
	{
		vbId = x;
	}

	void doEpuVbHelp( const char * a, const char * b )
	{
        printVbIdHelp();
	}

	void doEpuVbIdHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void acceptEpuVbIdEol( const char * a, const char * b )
	{
		// cout << " -> " << __PRETTY_FUNCTION__ << endl;
		if( vbId != 0 )
		{
			// cout << " -> call handleVbConfigN(" << vbId << ")" << endl;
			eph_Copy_->epuFdIdCfg( vbId );
		}
	}
	/////////////////////////  vbridge end /////////////////////////


	//////////////////////  passthrough start //////////////////////
	void getEpuPtId( const unsigned x )
	{
		vbId = x;
	}

	void doEpuPtHelp( const char * a, const char * b )
	{
		printf( "  %12s        %s\n", 
			"passthrough-id", "passthrough identifier (globally unique)" );
	}

	void doEpuPtIdHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void acceptEpuPtIdEol( const char * a, const char * b )
	{
		// cout << " -> " << __PRETTY_FUNCTION__ << endl;
		if( vbId != 0 )
		{
			// cout << " -> call handlePtConfigN(" << vbId << ")" << endl;
		}
	}
	//////////////////////  passthrough end /////////////////////

	void putNlist() 
	{
		switch( nListDest )
		{
		  case OuterCos:
			epuMatchConfig.tag_[ 0 ].cosList_ = nList;
			break;

		  case OuterVlanId:
			epuMatchConfig.tag_[ 0 ].vlanidList_ = nList;
			break;

		  case InnerCos:
			epuMatchConfig.tag_[ 1 ].cosList_ = nList;
			break;

		  case InnerVlanId:
			epuMatchConfig.tag_[ 1 ].vlanidList_ = nList;
			break;

		  case ListTypeNull:
			break;
		}
	}


	void doEpuIngressHelp( const char * a, const char * b )
	{
		printHelp( MatchNode );
		printHelp( RewriteNode );
	}

    void acceptIngressDirection( const char * a, const char * b )
    {
        dir = DirIngress;
    }

	void doEpuIngMatchHelp( const char * a, const char * b )
	{
		printHelp( UntagNode );
		printHelp( Dot1adNode );
		printHelp( Dot1qNode );
	}

	void doEpuUntagHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
		printHelp( UntagCommaNode );
	}
	
	void doEpuDot1adHelp( const char * a, const char * b )
	{
		printHelp( CosNode );
		printHelp( OpenBraceNode );
	}
	
	void doEpuDot1qHelp( const char * a, const char * b )
	{
		printHelp( CosNode );
		printHelp( OpenBraceNode );
	}
	
	void doEpuUntagCommaHelp( const char * a, const char * b )
	{
		printHelp( Dot1adNode );
		printHelp( Dot1qNode );
	}
	
	void doListCosHelp( const char * a, const char * b )
	{
		printHelp( NumberListNode );
	}
	
	void doEpuCosListHelp( const char * a, const char * b )
	{
		printHelp( OpenBraceNode );
	}
	
	void doOpenBraceHelp( const char * a, const char * b )
	{
		printHelp( VlanIdNode );
		printHelp( PtagNode );
	}
	
	void doVlanIdHelp( const char * a, const char * b )
	{
		printHelp( NumberListNode );
	}
	
	void doVlanListHelp( const char * a, const char * b )
	{
		printHelp( CloseBraceNode );
	}
	
	void doCloseBraceHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
		printHelp( Dot1adNode );
		printHelp( Dot1qNode );
	}
	
	void doEpuPtagHelp( const char * a, const char * b )
	{
		printHelp( CloseBraceNode );
		printHelp( PtagCommaNode );
	}
	
	void doEpuPtagCommaHelp( const char * a, const char * b )
	{
		printHelp( VlanIdNode );
	}

	void acceptEpuUntag( const char * a, const char * b )
	{
		epuMatchConfig.isUntagged_ = true;
	}

	void acceptEpuUntagEol( const char * a, const char * b )
	{
		epuMatchConfig.isValid_ = true;
		// cout << "  Parsed contents = " << epuMatchConfig << endl;
		// cout << endl << " -> ingress match untagged EOL" << endl;
		eph_Copy_->epuMatchCfg( epuMatchConfig );
	}

	void acceptCloseBraceTokEol( const char * a, const char * b )
	{
		epuMatchConfig.isValid_ = true;
		// cout << "  Parsed contents = " << epuMatchConfig << endl;
		// cout << endl << " -> ingress match ... EOL" << endl;
		eph_Copy_->epuMatchCfg( epuMatchConfig );
	}


	void doEpuIngRwHelp ( const char * a, const char * b )
	{
		printHelp( PopNode );
	}

	void doEpuIngRwPopHelp( const char * a, const char * b )
	{
		printf( "  %12s        %s\n", "1|2", "specify number of tags to pop" );
	}

	void doEpuIngRwPopNumHelp( const char * a, const char * b )
	{
		printHelp( ReturnNode );
	}

	void getEpuNumPopTags( const unsigned x )
	{
        epuRewriteOp.numPopTags_ = x;
	}

	void acceptEpuIngRwPopNumEol( const char * a, const char * b )
	{
        assert( dir == DirIngress );

		if( epuRewriteOp.numPopTags_ != 0 )
		{
			eph_Copy_->epuIngressRewriteCfg( epuRewriteOp );
		}
	}

	void doEpuEgressHelp( const char * a, const char * b )
	{
        printHelp( RewriteNode );
	}

    void acceptEgressDirection( const char * a, const char * b )
    {
        dir = DirEgress;
    }

	void doEpuEgressRwHelp( const char * a, const char * b )
	{
        printHelp( PushNode );
        printHelp( SwapNode );
	}

	void acceptEpuHelpEol( const char * a, const char * b )
	{
		HelpFlag = true;
	}

	void acceptDot1adTok( const char * a, const char * b )
	{
		if( (nthTag == -1) || (nthTag == 0) )
		{
			nthTag++;
			epuMatchConfig.tag_[ nthTag ].vlanType_ = VlanTypeDot1ad; 
		}
	}

	void acceptDot1qTok( const char * a, const char * b )
	{
		if( (nthTag == -1) || (nthTag == 0) )
		{
			nthTag++;
		    epuMatchConfig.tag_[ nthTag ].vlanType_ = VlanTypeDot1q; 
		}
	}

	void acceptCosTok( const char * a, const char * b )
	{
		nListDest = (nthTag == 0) ? OuterCos : InnerCos;
	}

	void acceptOpenBraceTok( const char * a, const char * b )
	{
		putNlist();

		nListDest = ListTypeNull;
		nList.clear();
	}

	void acceptEpuPtag( const char * a, const char * b )
	{
		epuMatchConfig.tag_[ nthTag ].isPtagged_ = true;
	}

	void acceptVlanIdTok( const char * a, const char * b )
	{
		nListDest = (nthTag == 0) ? OuterVlanId : InnerVlanId;
	}

	void acceptCloseBraceTok( const char * a, const char * b )
	{
		putNlist();

		nListDest = ListTypeNull;
		nList.clear();
	}

	void getLowerNumber( const unsigned x )
	{
		lower = x;
	}

	void getHigherNumber( const unsigned x )
	{
		NumberTuple e;

		e.lower_ = lower;
		e.upper_ = x;

		nList.insertEntry( e );
	}

	void getSingleRangeNumber( const unsigned x )
	{
		NumberTuple e;

		e.lower_ = x;
		e.upper_ = x;

		nList.insertEntry( e );
	}

    //////////////////////////////////////////////////////////////////////////

    void acceptPopTok( const char * a, const char * b )
    {
        epuRewriteOp.rewriteType_ = RewriteTypePop;
    }

    void acceptPushTok( const char * a, const char * b )
    {
        epuRewriteOp.rewriteType_ = RewriteTypePush;
    }

    void acceptSwapTok( const char * a, const char * b )
    {
        epuRewriteOp.rewriteType_ = RewriteTypeSwap;
    }

    ///////////////////////////////////////////////////////////
    void doVlanHeaderHelp( const char * a, const char * b )
    {
        printHelp( Dot1adNode );
        printHelp( Dot1qNode );
    }

    void acceptVhDot1adTok( const char * a, const char * b )
    {
        if( whichTag == -1 || whichTag == 0 )
            ++whichTag;
             
        vlanTag.vlanType_ = VlanTypeDot1ad;
    }

    void acceptVhDot1qTok( const char * a, const char * b )
    {
        if( whichTag == -1 || whichTag == 0 )
            ++whichTag;

        vlanTag.vlanType_ = VlanTypeDot1q;
    }

    void doVh1Help( const char * a, const char * b )
    {
        printHelp( CosNode );
        printHelp( OpenBraceNode );
    }

    void acceptVhCosTok( const char * a, const char * b )
    {
    }

    void doVhCosHelp( const char * a, const char * b )
    {
        printHelp( InputNumberNode );
    }

    void getCosVal( const unsigned x )
    {
        vlanTag.cosValue_ = x;
    }

    void doVhCosValueHelp( const char * a, const char * b )
    {
        printHelp( OpenBraceNode );
    }

    void acceptVhOpenBraceTok( const char * a, const char * b )
    {
    }

    void doVhOpenBraceHelp( const char * a, const char * b )
    {
        printHelp( VlanIdNode );
    }

    void acceptVhVlanIdTok( const char * a, const char * b )
    {
    }

    void getVlanIdVal( const unsigned x )
    {
        vlanTag.vlanidValue_ = x;
    }

    void doVhVlanIdHelp( const char * a, const char * b )
    {
        printHelp( InputNumberNode );
    }

    void doVhVlanIdValueHelp( const char * a, const char * b )
    {
        printHelp( CloseBraceNode );
    }

    void acceptVhCloseBraceTok( const char * a, const char * b )
    {
        vlanHeader.tag_[ whichTag ] = vlanTag;
        vlanTag.clear();
    }

    void doVhCloseBraceHelp( const char * a, const char * b )
    {
        printHelp( ReturnNode );
        printHelp( Dot1adNode );
        printHelp( Dot1qNode );
    }
    ///////////////////////////////////////////////////////////

    void acceptVhCloseBraceTokEol( const char * a, const char * b )
    {
        epuRewriteOp.vlanHeader_ = vlanHeader;
        // cout << "Epu Rewrite Op = " << epuRewriteOp << endl;

        assert( dir == DirEgress );
		eph_Copy_->epuEgressRewriteCfg( epuRewriteOp );
    }

    //////////////////////////////////////////////////////////////////////////
}


// Grammar
struct EpuGrammar : public grammar< EpuGrammar >
{
    template <typename ScannerT>
    struct definition
    {
        definition( EpuGrammar const& /*self*/ )
        {
            using namespace EpuParserNameSpace;

            // Expected tokens
			HelpTok			= ch_p('?') >> *print_p >> end_p[ &acceptEpuHelpEol ];
			ExitTok 		= str_p( ExitNode.token() );
            EndTok          = str_p( EndNode.token() );
			ShutdownTok		= str_p( ShutdownNode.token() );
			IngressTok 		= str_p( IngressNode.token() );
			EgressTok		= str_p( EgressNode.token() );
			VbTok			= str_p( VbNode.token() );
			// PtTok			= str_p( PtNode.token() );
			MatchTok		= str_p( MatchNode.token() );
			RewriteTok		= str_p( RewriteNode.token() );
			UntagTok		= str_p( UntagNode.token() );
			PtagTok			= str_p( PtagNode.token() );
			PopTok			= str_p( PopNode.token() );
			PushTok			= str_p( PushNode.token() );
			SwapTok			= str_p( SwapNode.token() );
			Dot1adTok		= str_p( Dot1adNode.token() );
			Dot1qTok		= str_p( Dot1qNode.token() );
			CosTok			= str_p( CosNode.token() );
			VlanIdTok		= str_p( VlanIdNode.token() );
			AnyTok			= str_p( AnyNode.token() );
			CommaTok		= ch_p(',');
			OpenBraceTok	= ch_p('(');
			CloseBraceTok	= ch_p(')');

			
            // Production rules
			expression		= EpuHelp | ExitRuleNode | EndRuleNode | ShutdownRuleNode | VbRuleNode |
								/* PtRuleNode | */ IngressRuleNode | EgressRuleNode; 
			EpuHelp			= (HelpTok)[ &doEpuHelp ];

			ExitRuleNode	= ExitTok >> (ExitHelp | ExitEol);
			ExitHelp		= (HelpTok)[ &doEpuExitHelp ];
			ExitEol			= end_p[ &doEpuExitEol ];

			EndRuleNode	    = EndTok >> (EndHelp | EndEol);
			EndHelp		    = (HelpTok)[ &doEpuEndHelp ];
			EndEol			= end_p[ &doEpuEndEol ];

			ShutdownRuleNode= ShutdownTok >> (ShutdownHelp | ShutdownEol);
			ShutdownHelp	= (HelpTok)[ &doEpuShutdownHelp ];
			ShutdownEol		= end_p[ &doEpuShutdownEol ];

			VbRuleNode		= VbTok >> (VbHelp | VbIdNode);
			VbHelp			= (HelpTok)[ &doEpuVbHelp ];
			VbIdNode		= uint_p[ &getEpuVbId ] >> (VbIdHelp | VbIdEol);
			VbIdHelp		= (HelpTok)[ &doEpuVbIdHelp ];
			VbIdEol			= end_p[ &acceptEpuVbIdEol ];

			/*
			PtRuleNode		= PtTok >> (PtHelp | PtIdNode);
			PtHelp			= (HelpTok)[ &doEpuPtHelp ];
			PtIdNode		= uint_p[ &getEpuPtId ] >> (PtIdHelp | PtIdEol);
			PtIdHelp		= (HelpTok)[ &doEpuPtIdHelp ];
			PtIdEol			= end_p[ &acceptEpuPtIdEol ];
			*/

            //////////////////////////////////////////////////////////////////////////////////////////

			IngressRuleNode = IngressTok[ &acceptIngressDirection ] >> 
								(IngressHelp | IngressRewriteRuleNode | IngressMatchRuleNode); 
			IngressHelp		= (HelpTok)[ &doEpuIngressHelp ];

			IngressRewriteRuleNode	
							= RewriteTok >> (IngRwHelp | IngRwPopNode);
			IngRwHelp		= (HelpTok)[ &doEpuIngRwHelp ];

			IngRwPopNode	= PopTok[ &acceptPopTok ] >> (IngRwPopHelp | IngRwPopNumNode);
			IngRwPopHelp	= (HelpTok)[ &doEpuIngRwPopHelp ];

			IngRwPopNumNode	= uint_p[ &getEpuNumPopTags ] >> (IngRwPopNumHelp | IngRwPopNumEol);
			IngRwPopNumHelp	= (HelpTok)[ &doEpuIngRwPopNumHelp ];
			IngRwPopNumEol	= end_p[ &acceptEpuIngRwPopNumEol ];

			IngressMatchRuleNode	
							= MatchTok >> 
								(IngMatchHelp | UntagRuleNode | Dot1adRuleNode | Dot1qRuleNode);
			IngMatchHelp	= (HelpTok)[ &doEpuIngMatchHelp ];

			UntagRuleNode 	= UntagTok[ &acceptEpuUntag ] >> 
								(UntagHelp | UntagEol | UntagCommaRuleNode);
			UntagHelp		= (HelpTok)[ &doEpuUntagHelp ];
			UntagEol		= end_p[ &acceptEpuUntagEol ];

			Dot1adRuleNode 	= Dot1adTok[ &acceptDot1adTok ] >> 
								(Dot1adHelp | CosRuleNode | OpenBraceNode);
			Dot1adHelp		= (HelpTok)[ &doEpuDot1adHelp ];

			Dot1qRuleNode 	= Dot1qTok[ &acceptDot1qTok ] >> 
								(Dot1qHelp | CosRuleNode | OpenBraceNode);
			Dot1qHelp		= (HelpTok)[ &doEpuDot1qHelp ];

			UntagCommaRuleNode 	
							= CommaTok >> (UntagCommaHelp | Dot1adRuleNode | Dot1qRuleNode);
			UntagCommaHelp	= (HelpTok)[ &doEpuUntagCommaHelp ];

			CosRuleNode 	= CosTok[ &acceptCosTok ] >> (CosHelp | CosListNode);
			CosHelp			= (HelpTok)[ &doListCosHelp ];

			CosListNode 	= NumberList >> (CosListHelp | OpenBraceNode);
			CosListHelp		= (HelpTok)[ &doEpuCosListHelp ];

			OpenBraceNode 	= OpenBraceTok[ &acceptOpenBraceTok ] >> (OpenBraceHelp | VlanIdRuleNode | PtagRuleNode);
			OpenBraceHelp	= (HelpTok)[ &doOpenBraceHelp ];

			VlanIdRuleNode	= VlanIdTok[ &acceptVlanIdTok ] >> (VlanIdHelp | VlanIdListNode);
			VlanIdHelp		= (HelpTok)[ &doVlanIdHelp ];

			VlanIdListNode 	= NumberList >> (VlanListHelp | CloseBraceNode);
			VlanListHelp	= (HelpTok)[ &doVlanListHelp ];

			CloseBraceNode 	= CloseBraceTok[ &acceptCloseBraceTok ] >> 
								(CloseBraceHelp | CloseBraceEol | Dot1adRuleNode | Dot1qRuleNode);
			CloseBraceHelp	= (HelpTok)[ &doCloseBraceHelp ];
			CloseBraceEol	= end_p[ &acceptCloseBraceTokEol ];

			PtagRuleNode 	= PtagTok >> (PtagHelp | CloseBraceNode | PtagCommaRuleNode);
			PtagHelp		= (HelpTok)[ &doEpuPtagHelp ];

			PtagCommaRuleNode 	
							= CommaTok[ &acceptEpuPtag ] >> (PtagCommaHelp | VlanIdRuleNode);
			PtagCommaHelp	= (HelpTok)[ &doEpuPtagCommaHelp ];


            ///////////////////////////////////////////////////////////////////////////////////////////

			EgressRuleNode	    = EgressTok[ &acceptEgressDirection ]  >> (EgressHelp | EgressRwRuleNode);
			EgressHelp		    = (HelpTok)[ &doEpuEgressHelp ];
            EgressRwRuleNode    = RewriteTok >> (EgRwHelp | EgRwPushRuleNode | EgRwSwapRuleNode);
            EgRwHelp            = (HelpTok)[ &doEpuEgressRwHelp ];
            EgRwPushRuleNode    = PushTok[ &acceptPushTok ] >> (VlanHeaderHelp | VhDot1adRuleNode | VhDot1qRuleNode);
            EgRwSwapRuleNode    = SwapTok[ &acceptSwapTok ] >> (VlanHeaderHelp | VhDot1adRuleNode | VhDot1qRuleNode);
            VlanHeaderHelp         = (HelpTok)[ &doVlanHeaderHelp ];

            // Vh = Vlan Header

            VhDot1adRuleNode 	    = Dot1adTok[ &acceptVhDot1adTok ] >> (Vh1Help | VhCosRuleNode | VhOpenBraceRuleNode);
			VhDot1qRuleNode 	    = Dot1qTok[ &acceptVhDot1qTok ] >> (Vh1Help | VhCosRuleNode | VhOpenBraceRuleNode);
            Vh1Help                 = HelpTok[ &doVh1Help ];

			VhCosRuleNode 	        = CosTok[ &acceptVhCosTok ] >> (VhCosHelp | VhCosValueNode);
			VhCosHelp	            = (HelpTok)[ &doVhCosHelp ];
			VhCosValueNode          = uint_p[ &getCosVal ] >> (VhCosValueHelp | VhOpenBraceRuleNode);
			VhCosValueHelp	        = (HelpTok)[ &doVhCosValueHelp ];

			VhOpenBraceRuleNode     = OpenBraceTok[ &acceptVhOpenBraceTok ] >> (VhOpenBraceHelp | VhVlanIdRuleNode );
			VhOpenBraceHelp         = (HelpTok)[ &doVhOpenBraceHelp ];

			VhVlanIdRuleNode        = VlanIdTok[ &acceptVhVlanIdTok ] >> (VhVlanIdHelp | VhVlanIdValueRuleNode);
			VhVlanIdHelp		    = (HelpTok)[ &doVhVlanIdHelp ];
            VhVlanIdValueRuleNode   = uint_p[ &getVlanIdVal ] >> (VhVlanIdValueHelp | VhCloseBraceRuleNode);
            VhVlanIdValueHelp       = (HelpTok)[ &doVhVlanIdValueHelp ];

			VhCloseBraceRuleNode 	= CloseBraceTok[ &acceptVhCloseBraceTok ] >> (VhCloseBraceHelp | VhCloseBraceEol | VhDot1adRuleNode | VhDot1qRuleNode);
            VhCloseBraceHelp        = (HelpTok)[ &doVhCloseBraceHelp ];
			VhCloseBraceEol	        = end_p[ &acceptVhCloseBraceTokEol ];




            /////////////////////////////////////////////////////////////////////////////////////////

			NumberListEntry	= ( uint_p[ &getLowerNumber ] >> ch_p('-') >> uint_p[ &getHigherNumber ] ) 
								| uint_p[ &getSingleRangeNumber ];
			NumberList		= NumberListEntry >> *( ch_p(',') >> NumberListEntry );
        }

        // Declarations
        rule< ScannerT >	HelpTok, ExitTok, IngressTok, EgressTok, VbTok, PtTok,
							MatchTok, UntagTok, PtagTok, PopTok, Dot1adTok, Dot1qTok, 
							CosTok, VlanIdTok, AnyTok, CommaTok, RewriteTok, 
							expression,
							IngressRuleNode, IngressRewriteRuleNode, IngressMatchRuleNode, 
							MatchHelp, UntagHelp, UntagEol, UntagCommaNode, 
							Dot1adHelp, OpenBraceNode, OpenBraceExp, Dot1qHelp, UntagCommaHelp,
							CosHelp, CosListNode, NumberLIst, CosListHelp, OpenBraceTok,
							OpenBraceHelp, VlanIdHelp, VlanIdListNode, VlanListHelp,
							CloseBraceExp, CloseBraceNode, CloseBraceTok, CloseBraceHelp,
							CloseBraceEol, PtagRuleNode, PtagHelp, PtagCommaRuleNode, 
							PtagCommaHelp,
							EgSuffixTree, NumberList, NumberListEntry, IngMatchHelp, 
							UntagRuleNode,
							Dot1adRuleNode, Dot1qRuleNode, CosRuleNode, VlanIdRuleNode, 
							UntagCommaRuleNode, IngRwHelp, IngRwPopNode, IngRwPopHelp,
							IngRwPopNumNode, IngRwPopNumHelp, IngRwPopNumEol,
							EpuHelp, ExitRuleNode, ShutdownRuleNode, VbRuleNode, PtRuleNode,
							EgressRuleNode, ExitHelp, ExitEol, ShutdownTok, ShutdownHelp,
							ShutdownEol, VbHelp, VbIdNode, VbIdHelp, VbIdEol, PtHelp,
							PtIdNode, PtIdEol, PtIdHelp, IngressHelp, EgressHelp,
							EndTok, EndRuleNode, EndHelp, EndEol,
                            VhDot1adRuleNode, VhDot1qRuleNode, Vh1Help, VhCosRuleNode, VhCosHelp, 
                            VhCosValueNode, VhCosValueHelp, VhOpenBraceRuleNode, VhOpenBraceHelp,
                            VhVlanIdRuleNode, VhVlanIdHelp, VhVlanIdValueRuleNode, VhVlanIdValueHelp,
                            VhCloseBraceRuleNode, VhCloseBraceHelp, VhCloseBraceEol,
                            EgressRwRuleNode, EgRwHelp, EgRwPushRuleNode, EgRwSwapRuleNode, VlanHeaderHelp,
                            PushTok, SwapTok
							;


        rule< ScannerT > const& start() const
        {
            return expression;
        }
    };
};

bool
EpuParser::epuParser( 
	string const & inputString, 
	bool & helpRequested,
	ConfigState * c )
{
    using namespace EpuParserNameSpace;

	vbId = 0;
	ptId = 0;
	ParserModeChange = 0;
	untagFlag = false;
	HelpFlag = false;
	nList.clear();
	nListDest = ListTypeNull;
	nthTag = -1;
	epuMatchConfig.clear();

    whichTag = -1;
    dir = DirUnknown;
    vlanTag.clear();
    vlanHeader.clear();
    epuRewriteOp.clear();

    assert( c );
	c->currParserMode_ = ParserModeEpu;

    EpuGrammar g;
    parse_info<> parseInfo = 
        parse(inputString.c_str(), g, space_p);

    // cout << "  --In epuParser..." << endl;

    if( parseInfo.full )
    {
        // cout << "Parsed successfully" << endl;
        
        if( ParserModeChange != 0 )
        {
		    assert( HelpFlag == false );

		    if( ParserModeChange == -1 )
        	    c->currParserMode_ = ParserModePort;
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

        return true;
    }
    else
    {
        if( c->parserVerbosity_ )
        {
            cout << "Possible Syntax ERROR: Input unrecognized by EpuParser." << endl;
        }

        return false;
    }
}
