/*
 * (C) Copyright 2018 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef LORENZ95_ITERATOR_H_
#define LORENZ95_ITERATOR_H_

#include <iterator>
#include <string>
#include <vector>

#include "lorenz95/Resolution.h"

#include "oops/base/GeoLocation.h"
#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"

namespace oops {
  class GeoLocation;
}

namespace lorenz95 {

class Resolution;

// -----------------------------------------------------------------------------
class Iterator: public std::iterator<std::forward_iterator_tag,
                                     oops::GeoLocation>,
                public util::Printable,
                private util::ObjectCounter<Iterator> {
 public:
  static const std::string classname() {return "lorenz95::Iterator";}

  explicit Iterator(const Resolution & res, const int & index = 0);
  ~Iterator();

  bool operator==(const Iterator &) const;
  bool operator!=(const Iterator &) const;
  oops::GeoLocation operator*() const;
  Iterator& operator++();

  int index() const {return index_;}

 private:
  void print(std::ostream & os) const {os << index_;}
  const int res_;
  int index_;
};

}  // namespace lorenz95

#endif  // LORENZ95_ITERATOR_H_
