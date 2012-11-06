//
// Copyright 2009 Arun Saha
//
#include <cli/common/Types.h>   // for NumberList
#include <cli/common/CommonParserNodes.h>
#include <cli/common/Frame.h>

#include <mol/common/MolVp.h>
#include <mol/epu/Epu.h>
#include <mol/port/Port.h>

extern MolVp * MyMolVp_;

std::ostream & 
operator<<( std::ostream & o, EpuMatchTag const & x )
{
	x.dump( o );
	return o;
}

std::ostream & 
operator<<( std::ostream & o, EpuMatchCriteria const & x )
{
	x.dump( o );
	return o;
}

std::ostream & 
operator<<( std::ostream & o, EpuRewriteOp const & x )
{
	x.dump( o );
	return o;
}

std::ostream & 
operator<<( std::ostream & o, Epu const & x )
{
	x.dump( o );
	return o;
}

////////////////////////////////////////////////////

Epu::Epu()
{
	clear();
}

Epu::~Epu()
{
    clear();
}

void
Epu::clear()
{
	epuId_			= 0;
	portId_			= 0;
	adminState_		= true;
	fdId_			= 0;
	ingressMatch_.clear();
	ingressRewrite_.clear();
    inFrames_       = 0;
    outFrames_      = 0;
}

void
Epu::showRun( std::ostream & os ) const
{
    os << "  " << EpuNode.tokenString() << " " << epuId_ << endl;

	if( ingressMatch_.isValid() )
	{
		os << "   ingress match " << ingressMatch_ << endl;
	}

	if( ingressRewrite_.rewriteType_ != RewriteTypeUnknown )
	{
		os << "   ingress rewrite " << ingressRewrite_ << endl;
	}

	if( egressRewrite_.rewriteType_ != RewriteTypeUnknown )
	{
		os << "   egress rewrite " << egressRewrite_ << endl;
	}

	if( fdId_ )
	{
        os << "   " << VbNode.tokenString() << " " << fdId_ << endl;
	}
	
	if( !adminState_ )
	{
		os << "  shutdown" << endl;
	}
}

std::ostream &
Epu::dump( std::ostream & o ) const
{
	o << "EPU Id = " << epuId_ << endl;
	o << "Port Id = " << portId_ << endl;
	o << "Admin State = " << adminState_ << endl;
	o << "FwdDomain Id = " << fdId_ << endl;
	o << "Ingress Match Criteria = {" << ingressMatch_ << "}";
	o << "Ingress Rewrite Action = {" << ingressRewrite_ << "}";
	o << "Egress Rewrite Action = {" << egressRewrite_ << "}";
	o << endl;

	return o;
}

PortId
Epu::portId() const
{
    return portId_;
}

void
Epu::portId( PortId const & pId )
{
	Port * p = 0;

	if( pId != InvalidPortId )              // New EPU getting added
	{
		p = portMgr()->port( pId );
		assert( p );

		portId_ = pId;

		p->addEpu( this );
	}
	else                                    // Existing EPU getting removed
	{
		p = portMgr()->port( portId_ );
		assert( p );

		portId_ = 0;

		p->removeEpu( this );
	}
}

FwdDomainId
Epu::fdId() const
{
    return fdId_;
}

void
Epu::fdId( FwdDomainId const & fId )
{
	FwdDomain * f = 0;

	if( fId != InvalidFwdDomainId )
	{
        // Am I already part of some FD?
		f = fdMgr()->findFwdDomain( fdId_ );
		if( f )
        {
            // then, let me get out of it
		    f->removeEpu( this );
            f = 0;
        }

        // Now, where should I belong to?
		f = fdMgr()->findFwdDomain( fId );
        if( ! f )
        {
            // My container FwdDomain does not exist, so let me create it
            f = fdMgr()->createFwdDomain( fId );
        }
        assert( f );

		fdId_ = fId;

		f->addEpu( this );
	}
	else
	{
		f = fdMgr()->findFwdDomain( fdId_ );
		assert( f );

		fdId_ = 0;

		f->removeEpu( this );
	}
}

EpuId
Epu::epuId() const
{
    return epuId_;
}

void
Epu::epuId( EpuId const & rhs )
{
    epuId_ = rhs;
}

EpuMatchCriteria &
Epu::ingressMatch()
{
    return ingressMatch_;
}

EpuMatchCriteria const &
Epu::ingressMatch() const
{
    return ingressMatch_;
}

void
Epu::ingressMatch( EpuMatchCriteria const & rhs )
{
    ingressMatch_ = rhs;
}
    
EpuRewriteOp &
Epu::ingressRewrite()
{
    return ingressRewrite_;
}

EpuRewriteOp const &
Epu::ingressRewrite() const
{
    return ingressRewrite_;
}

void
Epu::ingressRewrite( EpuRewriteOp const & rhs )
{
    assert( rhs.valid() );

    ingressRewrite_ = rhs;
}

EpuRewriteOp &
Epu::egressRewrite()
{
    return egressRewrite_;
}

EpuRewriteOp const &
Epu::egressRewrite() const
{
    return egressRewrite_;
}

void
Epu::egressRewrite( EpuRewriteOp const & rhs )
{
    assert( rhs.valid() );

    egressRewrite_ = rhs;
}

bool
Epu::adminState() const
{
    return adminState_;
}

void
Epu::adminState( bool rhs )
{
    adminState_ = rhs;
}

Counter
Epu::inFrames() const
{
    return inFrames_;
}

void
Epu::incrInFrames() const
{
    ++inFrames_;

    FwdDomain * f = myFd();
    if( f )
    {
        f->incrInFrames();
    }
}

Counter
Epu::outFrames() const
{
    return outFrames_;
}

void
Epu::incrOutFrames() const
{
    ++outFrames_;

    FwdDomain * f = myFd();
    if( f )
    {
        f->incrOutFrames();
    }
}

void
Epu::rewriteFrame( bool const dir, Frame & frame )
{
    EpuRewriteOp & rewriteConfig = ingressRewrite();

    if( dir )
    {
        rewriteConfig = egressRewrite();
    }

    VlanHeader & vh = frame.vlanHeader();

    switch( rewriteConfig.rewriteType_ )
    {
      case RewriteTypeUnknown:
        break;

      case RewriteTypePop:
      {
        const unsigned short n = rewriteConfig.numPopTags_;
        assert( (n == 1) || (n == 2) );

        if( n == 1 )
        {
            vh.tag_[ 0 ] = vh.tag_[ 1 ];
            vh.tag_[ 1 ].clear();
        }

        if( n == 2 )
        {
            vh.tag_[ 0 ].clear();
            vh.tag_[ 1 ].clear();
        }

        break;
      }

      case RewriteTypePush:
      {
        const unsigned x = vh.numVlanTags();
        assert( x <= 2 );

        switch( x )
        {
          case 0:
            vh = rewriteConfig.vlanHeader_;
            break;

          case 1:
            vh.tag_[ 1 ] = vh.tag_[ 0 ];
            vh.tag_[ 0 ] = rewriteConfig.vlanHeader_.tag_[ 0 ];
            break;

          case 2:
            std::cout << "Frame already has 2 tags, skipping 'push'...."
                << std::endl;
            break;
        }

        break;
      }

      case RewriteTypeSwap:
        std::cout << "Swap is not implemented yet :-(" << std::endl;
        break;
    }

}

FwdDomain *
Epu::myFd() const
{
    return( fdMgr()->findFwdDomain( fdId_ ) );
}
////////////////////////////////////////////////////

EpuMatchCriteria::EpuMatchCriteria()
{
	clear();
}

void
EpuMatchCriteria::clear()
{
	isValid_	= false;
	isUntagged_ = false;
	tag_[ 0 ].clear();
	tag_[ 1 ].clear();
}

std::ostream &
EpuMatchCriteria::dump( std::ostream & o ) const
{
	std::string untagStr = "";

	if( isUntagged_ )
	{
		untagStr = "untagged ";

        if( tag_[ 0 ].vlanType_ != VlanTypeNull )
        {
            untagStr += ", ";
        }
	}

	o << untagStr;
	o << tag_[ 0 ] << " ";
	o << tag_[ 1 ] << " ";

	return o;
}

void
EpuMatchCriteria::print() const
{
    std::cout << (*this);
}

signed
EpuMatchCriteria::numMatchTags() const
{
    signed x = -1;

    if( isValid() )
    {
        if( isUntagged() )
        {
            x = 0;
        }

        if( tag_[ 0 ].vlanType_ != VlanTypeNull )
        {
            x = 1;

            if( tag_[ 1 ].vlanType_ != VlanTypeNull )
            {
                x = 2;
            }
        }
    }

    return x;
}

bool
EpuMatchCriteria::isValid() const
{
    return isValid_;
}

bool
EpuMatchCriteria::isUntagged() const
{
    return isUntagged_;
}

bool
EpuMatchCriteria::contains( VlanHeader const & vh, signed & depth ) const
{
    signed myNumMatchTags  = numMatchTags();
    unsigned vhNumVlanTags = vh.numVlanTags();

    assert( (myNumMatchTags >= -1 ) && (myNumMatchTags <= 2) );

    if( myNumMatchTags == -1 )
    {
        // I am not valid
        assert( ! isValid() );

        return false;
    }

    if( myNumMatchTags == 2 )
    {
        // I have double tagged rule, both tags of frame must match
        if( tag_[ 0 ].contains( vh.tag_[ 0 ] ) && 
            tag_[ 1 ].contains( vh.tag_[ 1 ] ) )
        {
            depth = 2;
            return true;
        }
    }

    if( myNumMatchTags == 1 )
    {
        // I have single tagged rule, I am happy if only the outer 
        // tag of the frame matches
        if( tag_[ 0 ].contains( vh.tag_[ 0 ] ) )
        {
            depth = 1;
            return true;
        }
    }

    if( myNumMatchTags == 0 )
    {
        // Since I am valid, I must be untagged
        assert( isUntagged() );

        // Hence, I will match only untagged frames
        if( vhNumVlanTags == 0 )
        {
            depth = 0;
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////

EpuMatchTag::EpuMatchTag()
{
	clear();
}

void
EpuMatchTag::clear()
{
	vlanType_ = VlanTypeNull;
	cosList_.clear();
	isPtagged_ = false;
	vlanidList_.clear();
}


std::ostream &
EpuMatchTag::dump( std::ostream & o ) const
{
    if( vlanType_ == VlanTypeNull )
    {
        return o;
    }

	std::string vlanTypeStr = VlanTypeToString( vlanType_ );

	std::string ptagStr = "";
	if( isPtagged_ )
	{
		ptagStr = "ptagged ,";
	}

	o << vlanTypeStr;
	if( ! cosList_.empty() )
	{
		o << " cos " << cosList_ << " ";
	}
	o << " (";
	o << ptagStr << "vlanid " << vlanidList_;
	o << ")";

	return o;
}

bool
EpuMatchTag::contains( VlanTag const & vt ) const
{
    bool contains = true;

    if( vlanType_ != vt.vlanType_ )
    {
        contains = false;
    }
    else if( (! cosList_.empty()) && (! cosList_.contains( vt.cosValue_ )) )
    {
        contains = false;
    }
    else if( vt.vlanidValue_ == 0 )
    {
        if( ! isPtagged_ )
        {
            contains = false;
        }
    }
    else
    {
        assert( ! vlanidList_.empty() );

        if( ! vlanidList_.contains( vt.vlanidValue_ ) )
        {
            contains = false;
        }
    }

    return contains;
}

////////////////////////////////////////////////////

EpuRewriteOp::EpuRewriteOp()
{
	clear();
}

void
EpuRewriteOp::clear()
{
    rewriteType_ = RewriteTypeUnknown;
    numPopTags_  = DefaultNumPopTags;
    vlanHeader_.clear();
}

std::ostream &
EpuRewriteOp::dump( std::ostream & o ) const
{
    switch( rewriteType_ )
    {
      case RewriteTypeUnknown:
        break;

      case RewriteTypePop:
        assert( (numPopTags_ == 1) || (numPopTags_ == 2) );
        o << "pop " << numPopTags_;
        break;

      case RewriteTypePush:
        assert( numPopTags_ == DefaultNumPopTags );
        o << "push " << vlanHeader_;
        break;

      case RewriteTypeSwap:
        assert( numPopTags_ == DefaultNumPopTags );
        o << "swap " << vlanHeader_;
        break;
    }

	return o;
}

bool
EpuRewriteOp::valid() const
{
    switch( rewriteType_ )
    {
      case RewriteTypeUnknown:

        // Not meaningful, but valid
        break;

      case RewriteTypePop:

        assert( (numPopTags_ == 1) || (numPopTags_ == 2) );

        if( (numPopTags_ == 1) || (numPopTags_ == 2) )
            return true;

        break;

      case RewriteTypePush:

        assert( numPopTags_ == DefaultNumPopTags );

        if( numPopTags_ == DefaultNumPopTags )
            return true;

        break;

      case RewriteTypeSwap:

        assert( numPopTags_ == DefaultNumPopTags );

        if( numPopTags_ == DefaultNumPopTags )
            return true;

        break;
    }

    return false;
}

