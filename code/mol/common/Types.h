//
// Copyright 2009 Arun Saha
//
#ifndef MOL_COMMON_TYPES_H
#define MOL_COMMON_TYPES_H

#include <assert.h>
#include <iostream>
#include <map>
#include <vector>

typedef unsigned PortId;
enum { InvalidPortId = PortId(0xFFFF) };

typedef unsigned EpuId;
enum { InvalidEpuId = EpuId(0xFFFF) };

typedef unsigned FwdDomainId;
enum { InvalidFwdDomainId = FwdDomainId(0xFFFF) };

class MolVp;
class PortMgr;
class Port;
class EpuMgr;
class Epu;
class FwdDomainMgr;
class FwdDomain;

#endif
