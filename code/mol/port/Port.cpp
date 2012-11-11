//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <mol/port/Port.h>
#include <algorithm>

Port::Port()
{
	epuList_.clear();
    inFrames_ = 0;
    outFrames_ = 0;
}

Port::~Port()
{
	epuList_.clear();
    inFrames_ = 0;
    outFrames_ = 0;
}

void
Port::addEpu( Epu * e )
{
	assert( e );
	epuList_.push_back( e );
}

void
Port::removeEpu( Epu * e )
{
	assert( e );

    EpuListItr pos = std::find( epuList_.begin(), epuList_.end(), e );
    assert( pos != epuList_.end() );

    epuList_.erase( pos );
}

void
Port::showRun( ostream & os ) const
{
	EpuListConstItr it;

	os << "!" << endl;
    os << " " << PortNode.tokenString() << " " << id_ << endl;

    // TODO: port level configuration

	for( it = epuList_.begin(); it != epuList_.end(); ++it )
	{
		(*it)->showRun( os );
	}
}

Counter
Port::inFrames() const
{
    return inFrames_;
}

void
Port::incrInFrames()
{
    ++inFrames_;
}

Counter
Port::outFrames() const
{
    return outFrames_;
}

void
Port::incrOutFrames()
{
    ++outFrames_;
}

Epu const *
Port::ingressMatchClassify( Frame const & frame )
{
    Epu const * matchEpu = 0;
    signed bestMatchDepthSoFar = -1;

    signed containmentDepth;

    VlanHeader frameVlanHeader = frame.vlanHeader();

	EpuListConstItr it;

	for( it = epuList_.begin(); it != epuList_.end(); ++it )
	{
        Epu const * candidateEpu = (*it);
        assert( candidateEpu );

        // cout << "\t\t\tTrying " << candidateEpu->epuId() << ": " << candidateEpu->ingressMatch() << " ... ";

        EpuMatchCriteria const & candidateMatchCriteria = 
            candidateEpu->ingressMatch();

        containmentDepth = -1;
        if( candidateMatchCriteria.contains( frameVlanHeader, containmentDepth ) )
        {
            // cout << " YES (depth = " << containmentDepth << ")" << endl;

            if( containmentDepth > bestMatchDepthSoFar )
            {
                matchEpu = candidateEpu;
                bestMatchDepthSoFar = containmentDepth;
            }
        }
        else
        {
            // cout << " NO" << endl;
        }
    }

    return matchEpu;
}

void
Port::dump( bool const & counterFlag ) const
{
    if( epuList_.empty() )
    {
        return;
    }

	EpuListConstItr it;

	cout << id_ << " \t\t ";

	for( it = epuList_.begin(); it != epuList_.end(); ++it )
	{
		EpuId eId = (*it)->epuId();
        cout << eId << ",";
    }

    if( counterFlag )
    {
        cout << "\t\t\t" << inFrames();
        cout << "\t\t\t" << outFrames();
    }

    cout << endl;
}

