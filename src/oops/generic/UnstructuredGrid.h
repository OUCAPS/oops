/*
 * (C) Copyright 2017 UCAR
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef OOPS_GENERIC_UNSTRUCTUREDGRID_H_
#define OOPS_GENERIC_UNSTRUCTUREDGRID_H_

#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

#include "eckit/config/Configuration.h"
#include "oops/util/ObjectCounter.h"
#include "oops/util/Printable.h"

namespace oops {

// -----------------------------------------------------------------------------

class UnstructuredGrid : public util::Printable,
                         private boost::noncopyable,
                         private util::ObjectCounter<UnstructuredGrid> {
 public:
  static const std::string classname() {return "oops::UnstructuredGrid";}

  explicit UnstructuredGrid(const int &, const int & = 1);
  ~UnstructuredGrid();

// Will be useful for tests
  void zero();
  void random();
  double dot_product_with(const UnstructuredGrid &) const;

// Will be useful for tests
  int & toFortran() {return keyUGrid_;}
  const int & toFortran() const {return keyUGrid_;}

 private:
  void print(std::ostream &) const;

  int keyUGrid_;
  int colocated_;
  int nts_;
};

// -----------------------------------------------------------------------------

}  // namespace oops

#endif  // OOPS_GENERIC_UNSTRUCTUREDGRID_H_
