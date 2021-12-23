/*
 * (C) Copyright 2021 UCAR.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 */

#ifndef QG_MODEL_OBSLOCQG_H_
#define QG_MODEL_OBSLOCQG_H_

#include <ostream>

#include "oops/base/ObsLocalizationBase.h"
#include "oops/util/parameters/RequiredParameter.h"

#include "oops/qg/QgTraits.h"

namespace qg {
  class GeometryQGIterator;
  class ObsSpaceQG;
  class ObsVecQG;

/// \brief Parameters controlling obs-space localization.
class ObsLocParameters : public oops::ObsLocalizationParametersBase {
  OOPS_CONCRETE_PARAMETERS(ObsLocParameters, oops::ObsLocalizationParametersBase)

 public:
  oops::RequiredParameter<double> lengthscale{"lengthscale",
        "Localization distance (distance where localization goes to zero)", this};
};

/// \brief Observation-space localization for QG model (Heaviside function
/// with prescribed lengthscale).
class ObsLocQG : public oops::ObsLocalizationBase<QgTraits, QgObsTraits> {
 public:
  typedef ObsLocParameters Parameters_;

  ObsLocQG(const Parameters_ &, const ObsSpaceQG &);

  /// compute localization and update localization values in \p locfactor
  /// (missing value is for obs outside of localization)
  void computeLocalization(const GeometryQGIterator &, ObsVecQG &) const override;

 private:
  void print(std::ostream &) const override;
  const double lengthscale_;
  const ObsSpaceQG & obsdb_;
};

}  // namespace qg

#endif  // QG_MODEL_OBSLOCQG_H_
