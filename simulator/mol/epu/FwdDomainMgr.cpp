//
// Copyright 2009 Arun Saha
//
// #include <iostream>
#include <cli/common/CommonParserNodes.h>
#include <mol/epu/FwdDomainMgr.h>

FwdDomainMgr::FwdDomainMgr()
{
    macTable_ = new MacTable;
    assert( macTable_ );

	fwdDomainSet_.clear();
	assert( fwdDomainSet_.empty() );
}

FwdDomainMgr::~FwdDomainMgr()
{
    if( macTable_ )
    {
        delete macTable_;
        macTable_ = 0;
    }

	FwdDomainSetItr it = fwdDomainSet_.begin();

	while( it != fwdDomainSet_.end() )
	{
		FwdDomainId i	= (*it).first;
		FwdDomain * e = (*it).second;

		fwdDomainSet_.erase( i );

		if( e )
		{
			delete e;
		}

		++it;
	}

	assert( fwdDomainSet_.empty() );
}

MacTable *
FwdDomainMgr::macTable() const
{
    assert( macTable_ );
    return macTable_;
}

void
FwdDomainMgr::showMacTable() const
{
    macTable()->showMacEntries();
}

FwdDomain *
FwdDomainMgr::createFwdDomain( FwdDomainId const & fwdDomainId )
{
	FwdDomain * e = new FwdDomain;
	assert( e );
	e->fdId_ = fwdDomainId; 

	fwdDomainSet_[ fwdDomainId ] = e;

	return e;
}

void
FwdDomainMgr::deleteFwdDomain( FwdDomainId const & fwdDomainId )
{
	FwdDomain * e = findFwdDomain( fwdDomainId );

	if( e )
	{
		assert( e->fdId_ == fwdDomainId );

		fwdDomainSet_.erase( fwdDomainId );
		delete e;
	}
}

FwdDomain *
FwdDomainMgr::findFwdDomain( FwdDomainId const & fwdDomainId ) 
{
	FwdDomainId & eId = const_cast< FwdDomainId & > (fwdDomainId);
	FwdDomainSetItr it = fwdDomainSet_.find( eId );

	if( it != fwdDomainSet_.end() )
	{
		FwdDomain * fwdDomain = (*it).second;

		assert( fwdDomainId == fwdDomain->fdId_ );

		return fwdDomain;
	}
	else
	{
		return 0;
	}
}

void
FwdDomainMgr::showFds( FwdDomainId const & fdId, bool const & counterFlag ) const
{
	FwdDomainSetConstItr 	it;
	FwdDomainId			    eid;
	FwdDomain			  	*e;

	dumpHeader( counterFlag );

	if( fdId == 0 )
	{
		if( fwdDomainSet_.empty() )
		{
			return;
		}

		for( it = fwdDomainSet_.begin(); 
            it != fwdDomainSet_.end(); it++ )
		{
	
			eid 	= (*it).first;
			e 		= (*it).second;
	
			assert( eid == e->fdId_ );
	
            e->dump( counterFlag );
		}
	}
	else
	{
		it = fwdDomainSet_.find( fdId );

		if( it != fwdDomainSet_.end() )
		{
			e 		= (*it).second;

			assert( fdId == e->fdId_ );

            e->dump( counterFlag );
		}
    }
}

void
FwdDomainMgr::dumpHeader( bool const & counterFlag ) const
{
	cout << "-------------------------------------------------------------------------" << endl;
    cout << 
        VbNode.tokenString() << " Id \t " << 
        "Type \t\t " <<
        EpuNode.tokenString() << " Id(s)";
    if( counterFlag )
    {
        cout << "\t\tIn Packets\t\tOut Packets"; 
    }
    cout << endl;
	cout << "-------------------------------------------------------------------------" << endl;
}

