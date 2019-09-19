/*
 * (C) Copyright 2017-2019 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef TEST_INTERFACE_LOCALOBSERVATIONSPACE_H_
#define TEST_INTERFACE_LOCALOBSERVATIONSPACE_H_

#include <string>
#include <vector>

#define ECKIT_TESTING_SELF_REGISTER_CASES 0

#include "eckit/config/LocalConfiguration.h"
#include "eckit/geometry/Point2.h"
#include "eckit/testing/Test.h"
#include "oops/interface/ObservationSpace.h"
#include "oops/runs/Test.h"
#include "test/interface/ObsVector.h"
#include "test/TestEnvironment.h"

namespace test {

// -----------------------------------------------------------------------------

template <typename MODEL> void testLocalObsSpace() {
  typedef ObsVecFixture<MODEL> Test_;
  typedef oops::ObservationSpace<MODEL>       LocalObsSpace_;
  typedef oops::ObsVector<MODEL>              ObsVector_;

  const eckit::LocalConfiguration localconf(TestEnvironment::config(), "LocalObservationSpace");

  for (std::size_t jj = 0; jj < Test_::obspace().size(); ++jj) {
    // get center (for localization) from yaml
    eckit::LocalConfiguration geolocconf(localconf, "GeoLocation");
    double lon = geolocconf.getDouble("lon");
    double lat = geolocconf.getDouble("lat");
    const eckit::geometry::Point2 center(lon, lat);

    // get distance from yaml
    eckit::LocalConfiguration distconf(localconf, "GeoDistance");
    const double dist = distconf.getDouble("distance");

    // initialize local observation space
    LocalObsSpace_ localobs(*Test_::obspace()[jj], center, dist, -1);
    oops::Log::info() << "Local obs within " << dist << " from " << center <<
                         ": " << localobs << std::endl;

    const int ref_nobs = localconf.getInt("reference nobs");

    // test that local nobs is equal to the reference value
    ObsVector_ localvec(localobs);
    const int nobs = localvec.nobs();
    EXPECT(nobs == ref_nobs);
  }
}

// -----------------------------------------------------------------------------

template <typename MODEL> void testLocalObsVector() {
  typedef ObsVecFixture<MODEL> Test_;
  typedef oops::ObservationSpace<MODEL>       LocalObsSpace_;
  typedef oops::ObsVector<MODEL>              ObsVector_;

  const eckit::LocalConfiguration localconf(TestEnvironment::config(), "LocalObservationSpace");

  for (std::size_t jj = 0; jj < Test_::obspace().size(); ++jj) {
    // get center (for localization) from yaml
    eckit::LocalConfiguration geolocconf(localconf, "GeoLocation");
    double lon = geolocconf.getDouble("lon");
    double lat = geolocconf.getDouble("lat");
    const eckit::geometry::Point2 center(lon, lat);

    // get distance from yaml
    eckit::LocalConfiguration distconf(localconf, "GeoDistance");
    const double dist = distconf.getDouble("distance");

    const std::string varname = localconf.getString("varname");

    // initialize full ObsVector for a specified variable
    ObsVector_ fullvec(*Test_::obspace()[jj], varname);
    oops::Log::info() << "Full Obsvector: " << fullvec << std::endl;

    // initialize local observation space
    LocalObsSpace_ localobs(*Test_::obspace()[jj], center, dist, -1);
    oops::Log::info() << "Local obs within " << dist << " from " << center <<
                         ": " << localobs << std::endl;

    // intialize local obsvector by reading specified variable from local obsspace
    ObsVector_ localvec1(localobs, varname);
    oops::Log::info() << "Local Obsvector from Local Obsspace: " << localvec1 << std::endl;
    // initialize local obsvector from full obsvector using local obsspace
    ObsVector_ localvec2(localobs, fullvec);
    oops::Log::info() << "Local ObsVector from full ObsVector: " << localvec2 << std::endl;
    // check that the two are equal
    EXPECT(localvec1.nobs() == localvec2.nobs());
    localvec2 -= localvec1;
    const double rms = dot_product(localvec2, localvec2);
    EXPECT(rms == 0);
  }
}

// -----------------------------------------------------------------------------

template <typename MODEL> class LocalObservationSpace : public oops::Test {
 public:
  LocalObservationSpace() {}
  virtual ~LocalObservationSpace() {}
 private:
  std::string testid() const {return "test::LocalObservationSpace<" + MODEL::name() + ">";}

  void register_tests() const {
    std::vector<eckit::testing::Test>& ts = eckit::testing::specification();
    ts.emplace_back(CASE("interface/LocalObservationSpace/testLocalObsSpace")
      { testLocalObsSpace<MODEL>(); });
    ts.emplace_back(CASE("interface/LocalObservationSpace/testLocalObsVector")
      { testLocalObsVector<MODEL>(); });
  }
};

// =============================================================================

}  // namespace test

#endif  // TEST_INTERFACE_LOCALOBSERVATIONSPACE_H_
