//
// Copyright 2009 Arun Saha
//

#ifndef MOL_EPU_EPU_H
#define MOL_EPU_EPU_H

#include <cli/common/Types.h>   // for NumberList
#include <cli/common/Frame.h>   // for VlanHeader
#include <mol/common/Types.h>

class EpuMatchTag;
class EpuMatchCriteria;
class EpuRewriteOp;
class Epu;

class EpuMatchTag {

  public:

  	EpuMatchTag();

	void clear();
    std::ostream & dump( std::ostream & ) const;

    bool        contains( VlanTag const & ) const;

  // private:

	VlanType		    vlanType_;
  	NumberList		    cosList_;
	bool			    isPtagged_;
	NumberList		    vlanidList_;
};

class EpuMatchCriteria {

  public:

    EpuMatchCriteria();

	void clear();
    std::ostream &   dump( std::ostream & ) const;
    void        print() const;

    // return values:
    //      -1 : this criteria is not valid (e.g. uninitialized)
    //       0 : untagged
    //       1 : [untagged] + single tag
    //       2 : [untagged] + double tag
    signed          numMatchTags() const;

    bool            isUntagged() const;
    bool            isValid() const;
    bool            contains( VlanHeader const &, signed & ) const;

  // private:

	bool			isValid_;
  	bool			isUntagged_;
	EpuMatchTag		tag_[ 2 ];		// outer tag and inner tag

};

enum RewriteType {
    RewriteTypeUnknown,
    RewriteTypePop,
    RewriteTypePush,
    RewriteTypeSwap
};

class EpuRewriteOp {

  public:

  	EpuRewriteOp();

	void clear();
    std::ostream & dump( std::ostream & ) const;
    bool valid() const;

  // private:

    enum { DefaultNumPopTags = 0xFFFF };

    RewriteType     rewriteType_;
    unsigned short  numPopTags_;
    VlanHeader      vlanHeader_;
};

class Epu {

  public:

	Epu();
    ~Epu();

	void                    clear();
	void                    showRun( std::ostream & ) const;
    std::ostream &          dump( std::ostream & ) const;

    PortId                  portId() const;
	void                    portId( PortId const & );

    FwdDomainId             fdId() const;
    void                    fdId( FwdDomainId const & );

    EpuId                   epuId() const;
    void                    epuId( EpuId const & );

    EpuMatchCriteria &      ingressMatch();
    EpuMatchCriteria const &  ingressMatch() const;
    void                    ingressMatch( EpuMatchCriteria const & );

    EpuRewriteOp &          ingressRewrite();
    EpuRewriteOp const &    ingressRewrite() const;
    void                    ingressRewrite( EpuRewriteOp const & );

    EpuRewriteOp &          egressRewrite();
    EpuRewriteOp const &    egressRewrite() const;
    void                    egressRewrite( EpuRewriteOp const & );

    bool                    adminState() const;
    void                    adminState( bool );

    Counter                 inFrames() const;
    void                    incrInFrames() const;

    Counter                 outFrames() const;
    void                    incrOutFrames() const;

                            // bool = 'false' => ingress, bool = 'true' => egress
    void                    rewriteFrame( bool const, Frame & );

  private:

    FwdDomain *             myFd() const;

	EpuId				    epuId_;
	PortId				    portId_;

	FwdDomainId			    fdId_;
	EpuMatchCriteria	    ingressMatch_;
	EpuRewriteOp		    ingressRewrite_;
    EpuRewriteOp            egressRewrite_;
  	bool				    adminState_;

    mutable Counter         inFrames_;
    mutable Counter         outFrames_;
};

std::ostream & operator<<( std::ostream &, EpuMatchTag const & );
std::ostream & operator<<( std::ostream &, EpuMatchCriteria const & );
std::ostream & operator<<( std::ostream &, EpuRewriteOp const & );
std::ostream & operator<<( std::ostream &, Epu const & );

#endif
