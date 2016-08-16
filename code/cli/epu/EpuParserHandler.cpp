//
// Copyright 2009 Arun Saha
//
#include <cli/epu/EpuParserHandler.h>

#include <mol/epu/Epu.h>

extern ParserMain* MyParserMain_;

EpuParserHandler::EpuParserHandler() {}

EpuParserHandler::~EpuParserHandler() {}

ParserMain* EpuParserHandler::parserMain() const {
  assert(MyParserMain_);
  return (MyParserMain_);
}

void EpuParserHandler::epuAdminStateCfg(bool x) const {
  EpuId eid = MyParserMain_->configState()->currEpuId_;
  // cout << " -> " << __PRETTY_FUNCTION__ << ", Epu Id = " << eid << ", param =
  // " << x << endl;

  Epu* e = epuMgr()->findEpu(eid);
  assert(e);

  e->adminState(x);
}

void EpuParserHandler::epuFdIdCfg(FwdDomainId const& x) const {
  EpuId eid = MyParserMain_->configState()->currEpuId_;
  // cout << " -> " << __PRETTY_FUNCTION__ << ", Epu Id = " << eid << ", param =
  // " << x << endl;

  Epu* e = epuMgr()->findEpu(eid);
  assert(e);

  e->fdId(x);
}

void EpuParserHandler::epuMatchCfg(EpuMatchCriteria const& x) const {
  EpuId eid = MyParserMain_->configState()->currEpuId_;
  // cout << " -> " << __PRETTY_FUNCTION__ << ", Epu Id = " << eid << ", param =
  // " << x << endl;

  Epu* e = epuMgr()->findEpu(eid);
  assert(e);

  e->ingressMatch(x);
}

void EpuParserHandler::epuIngressRewriteCfg(EpuRewriteOp const& x) const {
  EpuId eid = MyParserMain_->configState()->currEpuId_;
  // cout << " -> " << __PRETTY_FUNCTION__ << ", Epu Id = " << eid << ", param =
  // " << x << endl;

  Epu* e = epuMgr()->findEpu(eid);
  assert(e);

  e->ingressRewrite(x);
}

void EpuParserHandler::epuEgressRewriteCfg(EpuRewriteOp const& x) const {
  EpuId eid = MyParserMain_->configState()->currEpuId_;
  // cout << " -> " << __PRETTY_FUNCTION__ << ", Epu Id = " << eid << ", param =
  // " << x << endl;

  Epu* e = epuMgr()->findEpu(eid);
  assert(e);

  e->egressRewrite(x);
}
