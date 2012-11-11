//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <mol/port/PortMgr.h>

PortMgr::PortMgr()
{
	numPorts_ = MaxPorts;

	port_ = new Port * [ numPorts_ ];
	assert( port_ );

	for( unsigned ui = 0; ui < numPorts_; ++ui )
	{
		port_[ ui ] = new Port;
		assert( port_[ ui ] );

		port_[ ui ]->id_ = ui;
	}

}

PortMgr::~PortMgr()
{
	for( unsigned ui = 0; ui < numPorts_; ++ui )
	{
		delete port_[ ui ];
		port_[ ui ] = 0;
	}

	delete port_;
	port_ = 0;

	numPorts_ = 0;
}

Port *
PortMgr::port( const PortId & pId ) const
{
	assert( pId < numPorts_ );

	return( port_[ pId ] );
}

void
PortMgr::showRun( ostream & os ) const
{
	for( unsigned ui = 0; ui < numPorts_; ++ui )
	{
		assert( port_[ ui ] );

		port_[ ui ]->showRun( os );
	}

}

void
PortMgr::showPorts( PortId const & pId, bool const & counterFlag ) const
{
    if( pId >= numPorts_ )
    {
        cout << "Command REJECTED: port number out of range." << endl;
        return;
    }

    dumpHeader( counterFlag );

    if( pId ) {
        port_[ pId ]->dump( counterFlag );
    }
    else 
    {
	    for( unsigned ui = 0; ui < numPorts_; ++ui )
	    {
            port_[ ui ]->dump( counterFlag );
        }
    }
}

unsigned
PortMgr::numPorts() const
{
    return numPorts_;
}

void
PortMgr::dumpHeader( bool const & counterFlag ) const
{
	cout << "-------------------------------------------------------------------------" << endl;
    cout << PortNode.tokenString() << " Id \t " << EpuNode.tokenString() << " member Id(s)";
    if( counterFlag )
    {
        cout << "\tIn Packets\t\tOut Packets"; 
    }
    cout << endl;
	cout << "-------------------------------------------------------------------------" << endl;
}

