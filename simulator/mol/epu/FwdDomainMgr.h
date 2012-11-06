//
// Copyright 2009 Arun Saha
//
#ifndef MOL_FWDDOMAINMGR_H
#define MOL_FWDDOMAINMGR_H

#include <mol/epu/FwdDomain.h>
#include <mol/epu/MacTable.h>

typedef	std::map< FwdDomainId, FwdDomain * > FwdDomainSet;
typedef	std::map< FwdDomainId, FwdDomain * >::iterator FwdDomainSetItr;
typedef	std::map< FwdDomainId, FwdDomain * >::const_iterator FwdDomainSetConstItr;

class FwdDomainMgr {
  
  public:

  	FwdDomainMgr();
	~FwdDomainMgr();

    MacTable *  macTable() const;
    void        showMacTable() const;

	FwdDomain * createFwdDomain( FwdDomainId const & );
	void        deleteFwdDomain( FwdDomainId const & );
	FwdDomain * findFwdDomain( FwdDomainId const & );

    void        showFds( FwdDomainId const &, bool const & ) const;

  private:

  	FwdDomainMgr( FwdDomainMgr const & );
    FwdDomainMgr & operator=( FwdDomainMgr const & );

  	void        dumpFwdDomain( FwdDomain const *, bool const & ) const;
	void        dumpHeader( bool const &) const;

    MacTable *          macTable_;
  	FwdDomainSet		fwdDomainSet_;

};

#endif
