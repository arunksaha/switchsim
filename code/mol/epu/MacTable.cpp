//
// Copyright 2009 Arun Saha
//
#include <cli/common/CommonParserNodes.h>
#include <mol/epu/MacTable.h>

MacTable::MacTable() {
  macEntries_.clear();
  assert(macEntries_.empty());
}

MacTable::~MacTable() {
  MacEntrySetItr it = macEntries_.begin();

  while (it != macEntries_.end()) {
    MacEntryKey i = (*it).first;
    MacEntry*   e = (*it).second;

    macEntries_.erase(i);

    if (e) {
      delete e;
      e = 0;
    }

    ++it;
  }

  assert(macEntries_.empty());
}

void MacTable::showMacEntries() const {
  dumpHeader();

  if (macEntries_.empty()) {
    return;
  }

  for (MacEntrySetConstItr it = macEntries_.begin(); it != macEntries_.end();
       it++) {
    MacEntryKey mek = (*it).first;
    MacEntry*   me  = (*it).second;

    assert(mek == me->key_);

    dumpMacEntry(me);
  }
}

void MacTable::learnSa(MacAddr const& mac, FwdDomainId const& fdId,
                       EpuId const& epuId) {
  MacEntryKey mek;
  mek.mac_      = mac;
  mek.domainId_ = fdId;

  MacEntry* meExisting = findMacEntry(mek);

  if (!meExisting) {
    // First appearance of this MAC in this Domain //

    MacEntry* me = createMacEntry(mek);
    assert(me);
    assert(mek == me->key_);

    me->epuId_ = epuId;
  } else {
    // Repeat appearance of this MAC in this Domain //

    // Todo: Refresh timestamp for aging purposes

    if (meExisting->epuId_ != epuId) {
      // Mac Move
      cout << "\t\t\t MAC Move: Mac Addr = " << mac << ", "
           << VbNode.tokenString() << " Id = " << fdId << "; Old "
           << EpuNode.tokenString() << " = " << meExisting->epuId_ << ", New "
           << EpuNode.tokenString() << " = " << epuId << endl;

      meExisting->epuId_ = epuId;
    }
  }
}

MacEntry* MacTable::lookupDa(MacAddr const&     mac,
                             FwdDomainId const& fdId) const {
  MacEntryKey mek;
  mek.mac_      = mac;
  mek.domainId_ = fdId;

  MacEntry* meExisting = findMacEntry(mek);

  return (meExisting);
}

/////////////////////////////////////////////////////////////////////////////////////

MacEntry* MacTable::createMacEntry(MacEntryKey const& mek) {
  // Create object in heap
  MacEntry* e = new MacEntry;
  assert(e);
  e->key_ = mek;

  // Insert object in dictionary
  macEntries_[mek] = e;

  return e;
}

void MacTable::deleteMacEntry(MacEntryKey const& mek) {
  MacEntry* me = findMacEntry(mek);

  if (me) {
    assert(mek == me->key_);

    // Remove object from dictionary
    macEntries_.erase(mek);

    // Destroy the object from heap
    delete me;
  }
}

MacEntry* MacTable::findMacEntry(MacEntryKey const& mek) const {
  // MacEntryKey & eId = const_cast< MacEntryKey & > (mek);
  MacEntrySetConstItr it = macEntries_.find(mek);

  if (it != macEntries_.end()) {
    MacEntry* me = (*it).second;

    assert(mek == me->key_);

    return me;
  } else {
    return 0;
  }
}

void MacTable::dumpHeader() const {
  cout << "----------------------------------------------" << endl;
  cout << "Mac Address \t\t " << VbNode.tokenString() << " Id \t"
       << EpuNode.tokenString() << " Id " << endl;
  cout << "----------------------------------------------" << endl;
}

void MacTable::dumpMacEntry(MacEntry const* me) const {
  cout << (me->key_).mac_ << " \t\t\t ";
  cout << (me->key_).domainId_ << " \t\t ";
  cout << (me->epuId_) << endl;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

MacEntryKey::MacEntryKey() { clear(); }

MacEntryKey::~MacEntryKey() { clear(); }

void MacEntryKey::clear() {
  mac_      = 0;
  domainId_ = 0;
}

bool MacEntryKey::operator==(MacEntryKey const& rhs) const {
  return ((mac_ == rhs.mac_) && (domainId_ == rhs.domainId_));
}

bool MacEntryKey::operator<(MacEntryKey const& rhs) const {
  return ((domainId_ < rhs.domainId_) ||
          ((domainId_ == rhs.domainId_) && (mac_ < rhs.mac_)));
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

MacEntry::MacEntry() { clear(); }

MacEntry::~MacEntry() { clear(); }

void MacEntry::clear() {
  key_.clear();
  epuId_ = 0;
}

bool MacEntry::operator==(MacEntry const& rhs) const {
  return ((key_ == rhs.key_) && (epuId_ == rhs.epuId_));
}
