//
// Copyright 2009 Arun Saha
//
#ifndef MOL_MOLVP_H
#define MOL_MOLVP_H

#include <mol/common/Types.h>
#include <mol/epu/EpuMgr.h>
#include <mol/epu/FwdDomainMgr.h>
#include <mol/port/PortMgr.h>

MolVp*        molVp();
PortMgr*      portMgr();
EpuMgr*       epuMgr();
FwdDomainMgr* fdMgr();

class MolVp {
 public:
  MolVp();
  ~MolVp();

  string hostname() const;
  void   hostname(string const&);

  void showRun(ostream&) const;

  // private:

  MolVp(MolVp const&);
  MolVp& operator=(MolVp const&);

  PortMgr*      portMgr_;
  EpuMgr*       epuMgr_;
  FwdDomainMgr* fdMgr_;
  string        hostname_;
};

#endif
