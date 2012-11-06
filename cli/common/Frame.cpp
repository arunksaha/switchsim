//
// Copyright 2009 Arun Saha
//
#include <cli/common/Types.h>
#include <cli/common/Frame.h>

std::ostream & operator<<( std::ostream & o, MacAddr const & x )
{
    x.dump( o );
    return o;
}

std::ostream & operator<<( std::ostream & o, VlanTag const & x )
{
    x.dump( o );
    return o;
}

std::ostream & operator<<( std::ostream & o, VlanHeader const & x )
{
    x.dump( o );
    return o;
}

std::ostream & operator<<( std::ostream & o, Frame const & x )
{
    x.dump( o );
    return o;
}

std::ostream & operator<<( std::ostream & o, Payload const & x )
{
    x.dump( o );
    return o;
}

std::ostream & operator<<( std::ostream & o, Fcs const & x )
{
    x.dump( o );
    return o;
}

/////////////////////////////////////////////////////////////////////

std::string
VlanTypeToString( VlanType vt )
{
    std::string vlanTypeStr = "ERROR";

	switch( vt )
	{
	  case VlanTypeNull:
	  	vlanTypeStr = "unassigned";
	  	break;

	  case VlanTypeDot1q:
	  	vlanTypeStr = "dot1q";
	  	break;

	  case VlanTypeDot1ad:
	  	vlanTypeStr = "dot1ad";
	  	break;
	}

    return vlanTypeStr;
}

/////////////////////////////////////////////////////////////////////

MacAddr::MacAddr() 
 : addr_( 0 )
{
}

MacAddr::MacAddr( unsigned long int x )
 : addr_( x )
{
}

MacAddr::~MacAddr() 
{
    clear();
}

void
MacAddr::clear()
{
    addr_ = 0;
}

std::ostream &
MacAddr::dump( std::ostream & o ) const
{
    o << addr_;
    return o;
}

bool
MacAddr::operator==( MacAddr const & rhs ) const
{
    return( addr_ == rhs.addr_ );
}

bool
MacAddr::operator!=( MacAddr const & rhs ) const
{
    return( !( *this == rhs ) );
}

bool
MacAddr::operator<( MacAddr const & rhs ) const
{
    return( addr_ < rhs.addr_ );
}

/////////////////////////////////////////////////////////////////////

VlanTag::VlanTag()
 :  vlanType_( VlanTypeNull ), 
    cosValue_( DefaultCosValue ), 
    vlanidValue_( DefaultVlanIdValue )
{
}

void
VlanTag::clear()
{
    vlanType_      = VlanTypeNull;
    cosValue_      = DefaultCosValue;
    vlanidValue_   = DefaultVlanIdValue;
}

std::ostream &
VlanTag::dump( std::ostream & o ) const
{
    if( vlanType_ == VlanTypeNull )
    {
        return o;
    }

    std::string vlanTypeStr = VlanTypeToString( vlanType_ );

	o << vlanTypeStr;
	if( cosValue_ != DefaultCosValue )
	{
	    o << " cos " << cosValue_ << " ";
	}
	o << " (vlanid " << vlanidValue_ << ")";

	return o;
}

/////////////////////////////////////////////////////////////////////

VlanHeader::VlanHeader()
{
    clear();
}

void
VlanHeader::clear()
{
    for( unsigned ui = 0; ui < MaxVlanTags; ++ui )
    {
        tag_[ ui ].clear();
    }
}

std::ostream &
VlanHeader::dump( std::ostream & o ) const
{
    for( unsigned ui = 0; ui < MaxVlanTags; ++ui )
    {
        o << tag_[ ui ];
        o << " ";
    }

	return o;
}

unsigned
VlanHeader::numVlanTags() const
{
    unsigned x = 0;

    if( tag_[ 0 ].vlanType_ != VlanTypeNull )
    {
        x += 1;

        if( tag_[ 1 ].vlanType_ != VlanTypeNull )
        {
            x += 1;
        }
    }

    return x;
}

/////////////////////////////////////////////////////////////////////

Frame::Frame()
 :  da_( 0 ),
    sa_( 0 ),
    vlanHeader_(),
    frameEtherType_( EtherTypeIpv4 ),
    payload_(),
    fcs_()
{
}

Frame::~Frame()
{
    clear();
}

void
Frame::clear()
{
    da_ = 0;
    sa_ = 0;
    vlanHeader_.clear();
    frameEtherType_ = EtherTypeIpv4;
    // payload_.clear();
    // fcs_.clear();
}

std::ostream &
Frame::dump( std::ostream & os ) const
{
    os << "DA = " << da_;
    os << ", SA = " << sa_;
    if( vlanHeader_.numVlanTags() != 0 )
    {
        os << ", VlanHeader = " << vlanHeader_;
    }
    os << " [ ... ]";
    // os << ", EType = " << hex << frameEtherType_ << dec;
    // os << ", Payload = " << payload_;
    // os << ", FCS = " << fcs_;

    return os;
}

FrameType
Frame::frameType() const
{
    FrameType ft = FrameTypeUnknown;

    unsigned x = vlanHeader().numVlanTags();

    if( x == 0 )
    {
        ft = Untagged;
    }
    else if( x == 1 )
    {
        ft = SingleTagged;
    }
    else if( x == 2 )
    {
        ft = DoubleTagged;
    }
    else
    {
        assert( 0 );
    }

    return ft;
}

MacAddr
Frame::dstAddr() const
{
    return da_;
}

void
Frame::dstAddr( MacAddr const & x )
{
    assert( x != 0 );
    da_ = x;
}

MacAddr
Frame::srcAddr() const
{
    return sa_;
}

void
Frame::srcAddr( MacAddr const & x )
{
    assert( x != 0 );
    sa_ = x;
}

unsigned
Frame::numVlanTags() const
{
    return( vlanHeader().numVlanTags() );
}

VlanHeader &
Frame::vlanHeader()
{
    return vlanHeader_;
}

VlanHeader const &
Frame::vlanHeader() const
{
    return vlanHeader_;
}

void
Frame::vlanHeader( VlanHeader const & x )
{
    vlanHeader_ = x;
}

Payload
Frame::payload() const
{
    return payload_;
}

void
Frame::payload( Payload const & x )
{
    payload_ = x;
}

Fcs
Frame::fcs() const
{
    return fcs_;
}

void
Frame::fcs( Fcs const & x )
{
    fcs_ = x;
}

