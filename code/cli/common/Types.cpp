//
// Copyright 2009 Arun Saha
//
#include <cli/common/Types.h>

std::ostream& operator<<(std::ostream& o, NumberTuple const& x) {
  x.dump(o);
  return (o);
}

std::ostream& operator<<(std::ostream& o, NumberList const& x) {
  x.dump(o);
  return (o);
}

/////////////////////////////////////////////////////////

NumberTuple::NumberTuple() : lower_(0xFFFF), upper_(0xFFFF) {}

std::ostream& NumberTuple::dump(std::ostream& o) const {
  if (lower_ == upper_) {
    o << lower_;
  } else {
    o << lower_ << "-" << upper_;
  }
  return o;
}

/////////////////////////////////////////////////////////

NumberList::NumberList() : numberTupleVector_() {}

NumberList& NumberList::operator=(NumberList const& rhs) {
  numberTupleVector_ = rhs.numberTupleVector_;
  return *this;
}

void NumberList::insertEntry(NumberTuple const& e) {
  numberTupleVector_.push_back(e);
}

void NumberList::clear() { numberTupleVector_.clear(); }

std::ostream& NumberList::dump(std::ostream& o) const {
  NumberTupleVectorConstItr it = numberTupleVector_.begin();

  size_t vsize  = numberTupleVector_.size();
  size_t vcount = 0;

  while (it != numberTupleVector_.end()) {
    o << (*it);

    ++it;
    ++vcount;

    if (vcount != vsize) {
      o << ",";
    }
  }

  return o;
}

bool NumberList::empty() const { return (numberTupleVector_.empty()); }

bool NumberList::contains(unsigned const x) const {
  NumberTuple               numberTuple;
  NumberTupleVectorConstItr it = numberTupleVector_.begin();

  while (it != numberTupleVector_.end()) {
    numberTuple = (*it);

    if ((numberTuple.lower_ <= x) && (x <= numberTuple.upper_)) {
      return true;
    }

    if (numberTuple.lower_ > x) {
      break;
    }

    ++it;
  }

  return false;
}
