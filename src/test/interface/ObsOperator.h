/*
 * (C) Copyright 2017-2018 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef TEST_INTERFACE_OBSOPERATOR_H_
#define TEST_INTERFACE_OBSOPERATOR_H_

#include <memory>
#include <string>
#include <vector>

#define ECKIT_TESTING_SELF_REGISTER_CASES 0

#include <boost/noncopyable.hpp>

#include "eckit/config/LocalConfiguration.h"
#include "eckit/testing/Test.h"
#include "oops/interface/GeoVaLs.h"
#include "oops/interface/ObsAuxControl.h"
#include "oops/interface/ObsOperator.h"
#include "oops/interface/ObsVector.h"
#include "oops/runs/Test.h"
#include "test/interface/ObsTestsFixture.h"
#include "test/TestEnvironment.h"

namespace test {

// -----------------------------------------------------------------------------

template <typename MODEL> void testConstructor() {
  typedef ObsTestsFixture<MODEL> Test_;
  typedef oops::ObsOperator<MODEL>       ObsOperator_;

  const eckit::LocalConfiguration obsconf(TestEnvironment::config(), "Observations");
  std::vector<eckit::LocalConfiguration> conf;
  obsconf.get("ObsTypes", conf);

  for (std::size_t jj = 0; jj < Test_::obspace().size(); ++jj) {
    eckit::LocalConfiguration obsopconf(conf[jj], "ObsOperator");
    std::unique_ptr<ObsOperator_> hop(new ObsOperator_(Test_::obspace()[jj], obsopconf));
    EXPECT(hop.get());

    hop.reset();
    EXPECT(!hop.get());
  }
}

// -----------------------------------------------------------------------------

template <typename MODEL> void testSimulateObs() {
  typedef ObsTestsFixture<MODEL> Test_;
  typedef oops::GeoVaLs<MODEL>           GeoVaLs_;
  typedef oops::ObsAuxControl<MODEL>     ObsAuxCtrl_;
  typedef oops::ObsOperator<MODEL>       ObsOperator_;
  typedef oops::ObsVector<MODEL>         ObsVector_;

  const eckit::LocalConfiguration obsconf(TestEnvironment::config(), "Observations");
  std::vector<eckit::LocalConfiguration> conf;
  obsconf.get("ObsTypes", conf);

  for (std::size_t jj = 0; jj < Test_::obspace().size(); ++jj) {
    // initialize observation operator (set variables requested from the model,
    // variables simulated by the observation operator, other init)
    eckit::LocalConfiguration obsopconf(conf[jj], "ObsOperator");
    ObsOperator_ hop(Test_::obspace()[jj], obsopconf);

    // read geovals from the file
    eckit::LocalConfiguration gconf(conf[jj], "GeoVaLs");
    const GeoVaLs_ gval(gconf, Test_::obspace()[jj], hop.variables());

    // initialize bias correction
    const ObsAuxCtrl_ ybias(conf[jj]);

    // create obsvector to hold H(x)
    ObsVector_ ovec(Test_::obspace()[jj]);

    // call H(x), save result in the output file as @hofx
    hop.simulateObs(gval, ovec, ybias);
    ovec.save("hofx");

    const double tol = conf[jj].getDouble("tolerance");
    if (conf[jj].has("vecequiv")) {
      // if reference h(x) is saved in file as a vector, read from file
      // and compare the norm of difference to zero
      ObsVector_ ovec_ref(Test_::obspace()[jj], conf[jj].getString("vecequiv"));
      ovec_ref -= ovec;
      const double zz = ovec_ref.rms();
      oops::Log::info() << "Vector difference between reference and computed: " <<
                           ovec_ref;
      EXPECT(zz < 100*tol);  //  change tol from percent to actual value.
                             //  tol used in is_close is relative
    } else {
      // else compare h(x) norm to the norm from the config
      const double zz = ovec.rms();
      const double xx = conf[jj].getDouble("rmsequiv");

      oops::Log::debug() << "zz: " << std::fixed << std::setprecision(8) << zz << std::endl;
      oops::Log::debug() << "xx: " << std::fixed << std::setprecision(8) << xx << std::endl;

      EXPECT(oops::is_close(xx, zz, tol));
    }
  }
}

// -----------------------------------------------------------------------------

template <typename MODEL>
class ObsOperator : public oops::Test {
 public:
  ObsOperator() {}
  virtual ~ObsOperator() {}
 private:
  std::string testid() const {return "test::ObsOperator<" + MODEL::name() + ">";}

  void register_tests() const {
    std::vector<eckit::testing::Test>& ts = eckit::testing::specification();

    ts.emplace_back(CASE("interface/ObsOperator/testConstructor")
      { testConstructor<MODEL>(); });
    ts.emplace_back(CASE("interface/ObsOperator/testSimulateObs")
      { testSimulateObs<MODEL>(); });
  }
};

// =============================================================================

}  // namespace test

#endif  // TEST_INTERFACE_OBSOPERATOR_H_
