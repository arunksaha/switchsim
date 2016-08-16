//
// Copyright 2009 Arun Saha
//
#include <mol/epu/FwdDomain.h>
#include <algorithm>

FwdDomain::FwdDomain() { clear(); }

FwdDomain::~FwdDomain() { clear(); }

void FwdDomain::clear() {
  fdId_ = 0;
  epuList_.clear();
  currentNumEpus_ = 0;
  fdType_         = FdTypeUnknown;
  inFrames_       = 0;
  outFrames_      = 0;
}

FwdDomainId FwdDomain::fdId() const { return fdId_; }

void FwdDomain::addEpu(Epu* e) {
  assert(e);
  epuList_.push_back(e);

  ++currentNumEpus_;

  updateFdType();
}

void FwdDomain::removeEpu(Epu* e) {
  assert(e);

  EpuListItr pos = std::find(epuList_.begin(), epuList_.end(), e);
  assert(pos != epuList_.end());

  epuList_.erase(pos);

  --currentNumEpus_;
  updateFdType();
}

unsigned FwdDomain::currentNumEpus() const { return currentNumEpus_; }

FwdDomainType FwdDomain::fdType() const { return fdType_; }

void FwdDomain::fdType(FwdDomainType f) {
  // ToDo: Enforce the constraints here
  fdType_ = f;
}

bool FwdDomain::learningRequired() const {
  if (fdType() == FdTypeMP2MP) {
    return true;
  } else {
    return false;
  }
}

EpuList const& FwdDomain::epuList() const { return epuList_; }

EpuList& FwdDomain::epuList() { return epuList_; }

Counter FwdDomain::inFrames() const { return inFrames_; }

void FwdDomain::incrInFrames() const { ++inFrames_; }

Counter FwdDomain::outFrames() const { return outFrames_; }

void FwdDomain::incrOutFrames() const { ++outFrames_; }

/////////////////////////////////////////////////////////////////////////////////

void FwdDomain::dump(bool const& counterFlag) const {
  if (epuList_.empty()) {
    return;
  }

  std::cout << fdId_ << "\t\t ";
  std::cout << fdType_ << "\t\t ";

  EpuListConstItr it;

  for (it = epuList_.begin(); it != epuList_.end(); ++it) {
    EpuId eId = (*it)->epuId();

    std::cout << eId << ", ";
  }

  if (counterFlag) {
    std::cout << "\t\t" << inFrames();
    std::cout << "\t\t\t" << outFrames();
  }

  std::cout << std::endl;
}

std::ostream& operator<<(std::ostream& os, FwdDomainType const& fdt) {
  switch (fdt) {
    case FdTypeP2P:
      os << "P2P  ";
      break;

    case FdTypeMP2MP:
      os << "MP2MP";
      break;

    case FdTypeP2MP:
      os << "P2MP ";
      break;

    case FdTypeUnknown:
      os << "-    ";
      break;
  }

  return os;
}

void FwdDomain::updateFdType() {
  if (currentNumEpus_ < 2) {
    fdType_ = FdTypeUnknown;

  } else if (currentNumEpus_ == 2) {
    fdType_ = FdTypeP2P;
  } else {
    fdType_ = FdTypeMP2MP;
    // Todo: Consider E-TREE later
  }
}
