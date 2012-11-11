//
// Copyright 2009 Arun Saha
//
#include <cli/config/ConfigParserHandler.h>
#include <mol/common/MolVp.h>

extern ParserMain * MyParserMain_;

ConfigParserHandler::ConfigParserHandler()
{
}

ConfigParserHandler::~ConfigParserHandler()
{
}

void
ConfigParserHandler::modifyPortN( PortId pId ) const
{
    cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << pId << endl;
}

bool
ConfigParserHandler::validPortId( PortId pId ) const
{
    cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << pId << endl;

    const unsigned n = portMgr()->numPorts();
    if( pId < n )
    {
        return true;
    }
    return false;
}

ParserMain *
ConfigParserHandler::parserMain() const
{
	assert( MyParserMain_ );
	return( MyParserMain_ );

}
