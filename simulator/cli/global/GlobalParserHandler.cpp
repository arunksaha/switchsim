//
// Copyright 2009 Arun Saha
//
#include <cli/global/GlobalParserHandler.h>

#include <mol/common/MolVp.h>
#include <mol/port/PortMgr.h>
#include <mol/epu/FwdDomainMgr.h>

extern ParserMain * MyParserMain_;

GlobalParserHandler::GlobalParserHandler()
{
}

GlobalParserHandler::~GlobalParserHandler()
{
}

void
GlobalParserHandler::showRun()
{
    // cout << " -> " << __PRETTY_FUNCTION__ << endl;
	molVp()->showRun( std::cout );
}

void
GlobalParserHandler::showPorts( const unsigned x, const bool b )
{
    cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << x << ", " << b << endl;
    portMgr()->showPorts( x, b );
}

void
GlobalParserHandler::showFds( const unsigned x, const bool b)
{
    // cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << x << endl;
    fdMgr()->showFds( x, b  );
}

void
GlobalParserHandler::showEpus( const unsigned x, const bool b )
{
    // cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << x << endl;
    epuMgr()->showEpus( x, b );
}

void
GlobalParserHandler::showMacTable()
{
    fdMgr()->showMacTable();
}

void
GlobalParserHandler::saveConfig() const
{
    ofstream targetFile;

    targetFile.open( parserMain()->SavedConfigFileName_, ios::out );

    if( targetFile.is_open() )
    {
        molVp()->showRun( targetFile );

        targetFile.close();
    }
}

void
GlobalParserHandler::debugPorts( const unsigned x )
{
    // cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << x << endl;
}

void
GlobalParserHandler::parserInConfigMode()
{
    // cout << " -> " << __PRETTY_FUNCTION__ << endl;
}

void
GlobalParserHandler::parserInSimulMode()
{
    // cout << " -> " << __PRETTY_FUNCTION__ << endl;
}

void
GlobalParserHandler::configHostname( string const & s )
{
    if( s.length() > 16 )
    {
        return;
    }

    parserMain()->configState()->hostname( s );
    molVp()->hostname( s );
}

ParserMain *
GlobalParserHandler::parserMain() const
{
	assert( MyParserMain_ );
	return( MyParserMain_ );
}

