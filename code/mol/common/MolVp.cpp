//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <mol/common/MolVp.h>

MolVp* MyMolVp_;

MolVp* molVp() {
  assert(MyMolVp_);
  return (MyMolVp_);
}

PortMgr* portMgr() {
  assert(MyMolVp_);
  assert(MyMolVp_->portMgr_);
  return (MyMolVp_->portMgr_);
}

EpuMgr* epuMgr() {
  assert(MyMolVp_);
  assert(MyMolVp_->epuMgr_);
  return (MyMolVp_->epuMgr_);
}

FwdDomainMgr* fdMgr() {
  assert(MyMolVp_);
  assert(MyMolVp_->fdMgr_);
  return (MyMolVp_->fdMgr_);
}

/////////////////////////////////////////////////

MolVp::MolVp() : portMgr_(0), epuMgr_(0), fdMgr_(0), hostname_("SoftSwitch") {
  hostname_ = "SoftSwitch";

  portMgr_ = new PortMgr;
  assert(portMgr_);

  epuMgr_ = new EpuMgr;
  assert(epuMgr_);

  fdMgr_ = new FwdDomainMgr;
  assert(fdMgr_);

  MyMolVp_ = this;
}

MolVp::~MolVp() {
  if (portMgr_) {
    delete portMgr_;
    portMgr_ = 0;
  }

  if (epuMgr_) {
    delete epuMgr_;
    epuMgr_ = 0;
  }

  if (fdMgr_) {
    delete fdMgr_;
    fdMgr_ = 0;
  }
}

string MolVp::hostname() const { return hostname_; }

void MolVp::hostname(string const& s) { hostname_ = s; }

void MolVp::showRun(ostream& os) const {
  os << HostnameNode.tokenString() << " " << hostname() << endl;
  portMgr()->showRun(os);
}
