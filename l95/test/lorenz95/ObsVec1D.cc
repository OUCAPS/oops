/*
 * (C) Copyright 2009-2016 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <string>
#include <vector>

#include <boost/scoped_ptr.hpp>

#include "./TestConfig.h"
#include "eckit/config/LocalConfiguration.h"
#include "eckit/testing/Test.h"
#include "lorenz95/ObsTableView.h"
#include "lorenz95/ObsVec1D.h"
#include "oops/base/Variables.h"
#include "test/TestFixture.h"

using eckit::types::is_approximately_equal;

namespace test {

// -----------------------------------------------------------------------------
class ObsVecTestFixture : TestFixture {
 public:
  ObsVecTestFixture() {
    const eckit::LocalConfiguration conf(TestConfig::config(), "Observations");
    const util::DateTime bgn(conf.getString("window_begin"));
    const util::DateTime end(conf.getString("window_end"));
    const eckit::LocalConfiguration otconf(conf, "Observation");
    obstable_.reset(new lorenz95::ObsTableView(otconf, bgn, end));
    const std::vector<std::string> vv{"zz"};
    vars_.reset(new oops::Variables(vv));
  }
  ~ObsVecTestFixture() {}
  boost::scoped_ptr<lorenz95::ObsTableView> obstable_;
  boost::scoped_ptr<oops::Variables> vars_;
};
// -----------------------------------------------------------------------------
CASE("test_ObsVec1D") {
  ObsVecTestFixture fix;
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_constructor") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    EXPECT(ov.get() != NULL);
    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov)(ii) == 0.0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_nobs") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    EXPECT(ov->nobs() == fix.obstable_->nobs());
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_read") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov->read("ObsValue");
    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov)(ii) != 0.0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_copy_constructor_copy") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov2)(ii) == (*ov1)(ii));
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_copy_constructor_no_copy") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, false));

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov2)(ii) == 0.0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_classname") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    EXPECT(ov->classname() == "lorenz95::ObsVec1D");
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_assignment") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));

    // use the assignment operator to populate the second ObsVec1D object
    *ov2 = *ov1;

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov2)(ii) == (*ov1)(ii));
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_compound_assignment_multiply_double") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    // create a random double value
    double mult = 7.92;
    *ov2 *= mult;

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov2)(ii) == (*ov1)(ii) * mult);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_compound_assignment_add") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    *ov1 += *ov2;

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov1)(ii) == (*ov2)(ii) + (*ov2)(ii));
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_compound_assignment_subtract") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    *ov1 -= *ov2;

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov1)(ii) == 0.0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_compound_assignment_multiply") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    *ov1 *= *ov2;

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov1)(ii) == (*ov2)(ii) * (*ov2)(ii));
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_compound_assignment_divide") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    *ov1 /= *ov2;

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov1)(ii) == 1.0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_zero") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov->read("ObsValue");

    ov->zero();

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov)(ii) == 0.0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_axpy") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    double mult = 2.00;

    ov1->axpy(mult, *ov2);

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov1)(ii) == (*ov2)(ii) + mult * (*ov2)(ii));
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_invert") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    ov1->invert();

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov1)(ii) == 1.0 / (*ov2)(ii));
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_random") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));

    ov->random();

    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      EXPECT((*ov)(ii) != 0);
    }
  }
// -----------------------------------------------------------------------------
  SECTION("test_ObsVec1D_dot_product_with") {
    boost::scoped_ptr<lorenz95::ObsVec1D> ov1(new lorenz95::ObsVec1D(*fix.obstable_, *fix.vars_));
    ov1->read("ObsValue");
    boost::scoped_ptr<lorenz95::ObsVec1D> ov2(new lorenz95::ObsVec1D(*ov1, true));

    double result = ov1->dot_product_with(*ov2);

    double check = 0.0;
    for (unsigned int ii = 0; ii < fix.obstable_->nobs(); ++ii) {
      check += (*ov2)(ii) * (*ov2)(ii);
    }

    EXPECT(is_approximately_equal(result, check, 1.0e-8));
  }
// -----------------------------------------------------------------------------
}  //  CASE
// -----------------------------------------------------------------------------
}  // namespace test
int main(int argc, char **argv)
{
    return eckit::testing::run_tests ( argc, argv );
}
