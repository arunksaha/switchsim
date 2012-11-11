//
// Copyright 2009 Arun Saha
//
#ifndef FRAME_H
#define FRAME_H

class MacAddr;
class VlanTag;
class VlanHeader;
class Frame;
class Payload;
class Fcs;

/////////////////////////////////////////////////////////////////////

enum VlanType {
	VlanTypeNull = 0,
	VlanTypeDot1q,
	VlanTypeDot1ad
};

std::string VlanTypeToString( VlanType );

enum EtherType {
    EtherTypeIpv4 = 0x0800,
    EtherTypeDot1q = 0x8100,
    EtherTypeDot1ad = 0x88A8
};

enum FrameType {
    Untagged = 0,
    SingleTagged,
    DoubleTagged,
    FrameTypeUnknown
};

/////////////////////////////////////////////////////////////////////
class MacAddr {

  public:

    MacAddr();
    ~MacAddr();
    MacAddr( unsigned long int );

    bool operator==( MacAddr const & ) const;
    bool operator!=( MacAddr const & ) const;
    bool operator< ( MacAddr const & ) const;

    bool unicastAddr() const;
    bool multicastAddr() const;

    void clear();
    std::ostream & dump( std::ostream & ) const;

  private:

    unsigned long int addr_;
};

std::ostream & operator<<( std::ostream &, MacAddr const & );

/////////////////////////////////////////////////////////////////////
class VlanTag {

  public:

    VlanTag();

    void clear();
    std::ostream & dump( std::ostream & ) const;

    std::string toString() const;

  // private: 

    enum { DefaultCosValue = 0xFFFF, DefaultVlanIdValue = 0xFFFF };

	VlanType		vlanType_;
  	unsigned short	cosValue_;
	unsigned short	vlanidValue_;
};

std::ostream & operator<<( std::ostream &, VlanTag const & );

/////////////////////////////////////////////////////////////////////
class VlanHeader {

  public:

    VlanHeader();

    void clear();
    std::ostream & dump( std::ostream & ) const;

    unsigned    numVlanTags() const;

    bool        contains( VlanHeader const &, signed & );

  // private:

    enum { MaxVlanTags = 2 };

    VlanTag tag_[ 2 ];
};

std::ostream & operator<<( std::ostream &, VlanHeader const & );

/////////////////////////////////////////////////////////////////////
class Payload {

  public:

    Payload() 
     : contents_ ( "DummyPayload" )
    {
    }

    std::ostream & dump( std::ostream & o ) const {
        o << contents_;
        return o;
    }

  private:
    std::string      contents_;
};

std::ostream & operator<<( std::ostream &, Payload const & );

/////////////////////////////////////////////////////////////////////
class Fcs {

  public:

    Fcs() 
     : contents_( "DummyFcs" )
    {
    }

    std::ostream & dump( std::ostream & o ) const {
        o << contents_;
        return o;
    }

  private:
    std::string      contents_;
};

std::ostream & operator<<( std::ostream &, Fcs const & );

/////////////////////////////////////////////////////////////////////
class Frame {

  public:

    Frame();
    ~Frame();

    void clear();
    std::ostream & dump( std::ostream & ) const;

    FrameType       frameType() const;

    MacAddr         dstAddr() const;
    void            dstAddr( MacAddr const & );

    MacAddr         srcAddr() const;
    void            srcAddr( MacAddr const & );

    unsigned        numVlanTags() const;
    VlanHeader &    vlanHeader();
    VlanHeader const &    vlanHeader() const;
    void            vlanHeader( VlanHeader const & );

    Payload         payload() const;
    void            payload( Payload const & );

    Fcs             fcs() const;
    void            fcs( Fcs const & );

  private:

    MacAddr        da_;
    MacAddr        sa_;
    VlanHeader     vlanHeader_;
    EtherType      frameEtherType_;
    Payload        payload_;
    Fcs            fcs_;
};

std::ostream & operator<<( std::ostream &, Frame const & );

#endif
