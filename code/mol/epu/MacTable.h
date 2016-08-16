//
// Copyright 2009 Arun Saha
//
#ifndef MOL_MACTABLE_H
#define MOL_MACTABLE_H

#include <cli/common/Frame.h>
#include <mol/epu/Epu.h>

class MacEntryKey {
 public:
  MacEntryKey();
  ~MacEntryKey();
  void clear();

  bool operator==(MacEntryKey const &) const;
  bool operator<(MacEntryKey const &) const;

  MacAddr     mac_;
  FwdDomainId domainId_;
};

class MacEntry {
 public:
  MacEntry();
  ~MacEntry();
  void clear();

  bool operator==(MacEntry const &) const;

  MacEntryKey key_;
  EpuId       epuId_;
};

typedef std::map<MacEntryKey, MacEntry *>                 MacEntrySet;
typedef std::map<MacEntryKey, MacEntry *>::iterator       MacEntrySetItr;
typedef std::map<MacEntryKey, MacEntry *>::const_iterator MacEntrySetConstItr;

class MacTable {
 public:
  MacTable();
  ~MacTable();

  void showMacEntries() const;

  void      learnSa(MacAddr const &, FwdDomainId const &, EpuId const &);
  MacEntry *lookupDa(MacAddr const &, FwdDomainId const &) const;

 private:
  MacEntry *createMacEntry(MacEntryKey const &);
  void      deleteMacEntry(MacEntryKey const &);
  MacEntry *findMacEntry(MacEntryKey const &) const;

  void dumpHeader() const;
  void dumpMacEntry(MacEntry const *) const;

  MacEntrySet macEntries_;
};

#endif
