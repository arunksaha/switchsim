//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <mol/epu/EpuMgr.h>

EpuMgr::EpuMgr() {
  epuSet_.clear();
  assert(epuSet_.empty());
}

EpuMgr::~EpuMgr() {
  EpuSetItr it = epuSet_.begin();

  while (it != epuSet_.end()) {
    EpuId i = (*it).first;
    Epu * e = (*it).second;

    epuSet_.erase(i);

    if (e) {
      e->clear();
      delete e;
      e = 0;
    }

    ++it;
  }

  assert(epuSet_.empty());
}

void EpuMgr::showEpus(EpuId const &epuId, bool const &counterFlag) const {
  EpuSetConstItr it;
  EpuId          eid;
  Epu *          e;

  dumpHeader(counterFlag);

  if (epuId == 0) {
    if (epuSet_.empty()) {
      return;
    }

    for (it = epuSet_.begin(); it != epuSet_.end(); it++) {
      eid = (*it).first;
      e   = (*it).second;

      assert(eid == e->epuId());

      dumpEpu(e, counterFlag);
    }
  } else {
    it = epuSet_.find(epuId);

    if (it != epuSet_.end()) {
      e = (*it).second;

      assert(epuId == e->epuId());

      dumpEpu(e, counterFlag);
    }
  }
}

void EpuMgr::dumpHeader(bool const &counterFlag) const {
  cout << "--------------------------------------------------------------------"
          "----------"
       << endl;
  cout << EpuNode.tokenString() << " Id \t\t " << PortNode.tokenString()
       << " Id \t " << VbNode.tokenString() << " Id";
  if (counterFlag) {
    cout << "\t\tIn Packets\t\tOut Packets";
  }
  cout << endl;
  cout << "--------------------------------------------------------------------"
          "----------"
       << endl;
}

void EpuMgr::dumpEpu(Epu const *e, bool const &counterFlag) const {
  assert(e);

  cout << e->epuId() << " \t\t ";
  cout << e->portId() << " \t\t ";

  if (e->fdId()) {
    cout << e->fdId();
  } else {
    cout << "-";
  }

  if (counterFlag) {
    cout << "\t\t" << e->inFrames();
    cout << "\t\t\t" << e->outFrames();
  }

  cout << endl;
}

Epu *EpuMgr::createEpu(EpuId const &epuId) {
  Epu *e = new Epu;
  assert(e);
  e->epuId(epuId);

  epuSet_[epuId] = e;

  return e;
}

void EpuMgr::deleteEpu(EpuId const &epuId) {
  Epu *e = findEpu(epuId);

  if (e) {
    assert(e->epuId() == epuId);

    epuSet_.erase(epuId);
    delete e;
  }
}

Epu *EpuMgr::findEpu(EpuId const &epuId) {
  EpuId &   eId = const_cast<EpuId &>(epuId);
  EpuSetItr it  = epuSet_.find(eId);

  if (it != epuSet_.end()) {
    Epu *epu = (*it).second;

    assert(epuId == epu->epuId());

    return epu;
  } else {
    return 0;
  }
}
