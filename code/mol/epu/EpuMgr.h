//
// Copyright 2009 Arun Saha
//
#ifndef MOL_EPUMGR_H
#define MOL_EPUMGR_H

#include <mol/epu/Epu.h>

typedef std::map<EpuId, Epu *>                 EpuSet;
typedef std::map<EpuId, Epu *>::iterator       EpuSetItr;
typedef std::map<EpuId, Epu *>::const_iterator EpuSetConstItr;

class EpuMgr {
 public:
  EpuMgr();
  ~EpuMgr();

  // if id == 0 then dump all
  void showEpus(EpuId const &, bool const &) const;

  Epu *createEpu(EpuId const &);
  void deleteEpu(EpuId const &);
  Epu *findEpu(EpuId const &);

 private:
  void dumpEpu(Epu const *, bool const &) const;
  void dumpHeader(bool const &) const;

  EpuSet epuSet_;
};

#endif
