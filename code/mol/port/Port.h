//
// Copyright 2009 Arun Saha
//
#ifndef MOL_PORT_PORT_H
#define MOL_PORT_PORT_H

#include <mol/common/Types.h>
#include <mol/epu/Epu.h>

typedef std::vector< Epu * > EpuList;
typedef std::vector< Epu * >::iterator EpuListItr;
typedef std::vector< Epu * >::const_iterator EpuListConstItr;

class Port {

  public:
  	
	Port();
	~Port();

	void        addEpu( Epu * );
	void        removeEpu( Epu * );

	void        showRun( std::ostream & os ) const;

    Counter     inFrames() const;
    void        incrInFrames();

    Counter     outFrames() const;
    void        incrOutFrames();

    Epu const * ingressMatchClassify( Frame const & );

  private:

	friend class PortMgr;

    void dump( bool const & ) const;

	PortId      id_;
	EpuList     epuList_;

    Counter     inFrames_;
    Counter     outFrames_;
};

#endif
