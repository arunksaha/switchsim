//
// Copyright 2009 Arun Saha
//
#include <cli/port/PortParserHandler.h>
#include <mol/common/MolVp.h>
#include <mol/epu/Epu.h>

extern ParserMain * MyParserMain_;

PortParserHandler::PortParserHandler()
{
}

PortParserHandler::~PortParserHandler()
{
}

ParserMain *
PortParserHandler::parserMain() const
{
	assert( MyParserMain_ );
	return( MyParserMain_ );

}

bool
PortParserHandler::modifyEpuN ( unsigned epuNumber )
{
    cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << epuNumber << endl;

	bool accepted = false;

	Epu * eFind = epuMgr()->findEpu( epuNumber );
	if( eFind )
	{
		if( eFind->portId() == MyParserMain_->configState()->currPortId_ )
		{
			MyParserMain_->configState()->currEpuId_ = eFind->epuId();
			accepted = true;
		}
		else
		{
			accepted = false;
		}
	}
	else
	{
		Epu * eNew = epuMgr()->createEpu( epuNumber );
		assert( eNew );

		eNew->epuId( epuNumber );
		eNew->portId( MyParserMain_->configState()->currPortId_ );
		
		MyParserMain_->configState()->currEpuId_ = eNew->epuId();
		accepted = true;
	}

	return accepted;
}

void
PortParserHandler::deleteEpuN ( unsigned epuNumber )
{
    cout << " -> " << __PRETTY_FUNCTION__ << ", arg = " << epuNumber << endl;

	Epu * e = epuMgr()->findEpu( epuNumber );

	if( e )
	{
		e->portId( InvalidPortId );
        e->fdId( InvalidFwdDomainId );
		epuMgr()->deleteEpu( e->epuId() );
	}
}

