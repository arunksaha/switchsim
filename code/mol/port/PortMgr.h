//
// Copyright 2009 Arun Saha
//
#ifndef MOL_PORTMGR_H
#define MOL_PORTMGR_H

#include <mol/common/Types.h>
#include <mol/port/Port.h>

class PortMgr {
 public:
  PortMgr();
  ~PortMgr();

  void showRun(ostream &) const;
  void showPorts(PortId const &, bool const &) const;

  unsigned numPorts() const;

  Port *port(PortId const &) const;

 private:
  PortMgr(PortMgr const &);
  PortMgr &operator=(PortMgr const &);

  enum { MaxPorts = 8 };

  void dumpHeader(bool const &) const;
  void dumpPort(Port const *) const;

  unsigned numPorts_;
  Port **  port_;
};

#endif
