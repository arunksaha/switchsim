//
// Copyright 2009 Arun Saha
//
#ifndef MOL_EPU_FWDDOMAIN_H
#define MOL_EPU_FWDDOMAIN_H

#include <mol/common/Types.h>
#include <mol/epu/Epu.h>
#include <mol/port/Port.h>  // for EpuList

enum FwdDomainType { FdTypeUnknown, FdTypeP2P, FdTypeMP2MP, FdTypeP2MP };

std::ostream &operator<<(std::ostream &, FwdDomainType const &);

class FwdDomain {
 public:
  FwdDomain();
  ~FwdDomain();

  FwdDomainId fdId() const;

  void addEpu(Epu *);
  void removeEpu(Epu *);

  unsigned currentNumEpus() const;

  FwdDomainType fdType() const;
  void          fdType(FwdDomainType);

  bool learningRequired() const;

  EpuList &      epuList();
  EpuList const &epuList() const;

  Counter inFrames() const;
  void    incrInFrames() const;

  Counter outFrames() const;
  void    incrOutFrames() const;

 private:
  friend class FwdDomainMgr;

  enum { MaxEpusAllowed = 4094 };

  void dump(bool const &) const;
  void clear();
  void updateFdType();

  FwdDomainId   fdId_;
  EpuList       epuList_;
  unsigned      currentNumEpus_;
  FwdDomainType fdType_;

  mutable Counter inFrames_;
  mutable Counter outFrames_;
};

#endif
