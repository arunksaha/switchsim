//
// Copyright 2009 Arun Saha
//
#ifndef CLI_COMMON_TYPES_H
#define CLI_COMMON_TYPES_H

#include <assert.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////
typedef unsigned Counter;

///////////////////////////////////////////////////////////////////
class NumberTuple {

  public:

    NumberTuple();
    std::ostream & dump( std::ostream & ) const;

  // private:

    unsigned short lower_;
    unsigned short upper_;
};

typedef std::vector< NumberTuple > NumberTupleVector;
typedef std::vector< NumberTuple >::const_iterator
    NumberTupleVectorConstItr;

std::ostream & operator<<( std::ostream &, NumberTuple const & );

///////////////////////////////////////////////////////////////////
class NumberList {

  public:
 
    NumberList();
    NumberList & operator=( NumberList const & );

    void clear();
    std::ostream & dump( std::ostream & ) const;

    void insertEntry( NumberTuple const & );
    bool empty() const;
    bool contains( unsigned const ) const;

  // private:

    NumberTupleVector numberTupleVector_;
};

std::ostream & operator<<( std::ostream &, NumberList const & );

#endif
