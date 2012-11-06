//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <mol/common/MolVp.h>
#include <string>

ParseNode NoNode          ( "no",    "delete something" );
ParseNode ExitNode        ( "exit",  "exit the current configuration mode" );
ParseNode EndNode         ( "end",   "exit the configuration mode completely" );
ParseNode ReturnNode      ( "RETURN", "press <return>" );
ParseNode InputNumberNode ( "integer",  "enter an integer" );
ParseNode ShutdownNode    ( "shutdown",
                 "shutdown current configuration object" );
ParseNode SimulationNode  ( "simulation",
                 "simulate frame ingress egress behavior" );
ParseNode HostnameNode    ( "hostname",   "specify hostname of this device" );
ParseNode CountersNode    ( "counters",   "display various counter values" );
ParseNode MacTableNode    ( "mac-table",
                          "display mac-address-table i.e. filtering database" );

ParseNode PortNode      ( "port",    "port/interface" );
ParseNode VbNode        ( "domain",
                 "specify forwarding domain of the ingress frames" );
ParseNode EpuNode       ( "epu",      "EVC per UNI" );

ParseNode Dot1adNode    ( "dot1ad",  "match frames with TPID=0x88a8" );
ParseNode Dot1qNode     ( "dot1q",   "match frames with TPID=0x8100" );
ParseNode CosNode       ( "cos",     "specify cos value(s)" );
ParseNode VlanIdNode    ( "vlanid",  "specify vlan id(s)" );
ParseNode OpenBraceNode ( "(",
                 "specify 'vlanid' followed by values after open parenthesis" );
ParseNode CloseBraceNode    ( ")",
                 "end 'vlanid' value list with close parenthesis" );

void printPortIdHelp()
{
    const std::string port = PortNode.tokenString();
    const unsigned n = portMgr()->numPorts();
    cout << "\t" << port << "-id\t\t" <<
        "choose a " << port << " among 0 to " << n - 1 << endl;
}

void printEpuIdHelp()
{
    std::string epu  = EpuNode.tokenString();
    cout << "\t" << epu << "-id\t\t" << "specify a " << epu <<
        " id to create/configure/delete" << endl;
}

void printVbIdHelp()
{
    std::string vb   = VbNode.tokenString();
    cout << "\t" << vb << "-id\t\t" << "specify a " << vb <<
        " id for forwarding behavior association" << endl;
}

    /*
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
    */

