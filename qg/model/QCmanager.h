/*
 * (C) Copyright 2017-2018 UCAR
 * 
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
 */

#ifndef QG_MODEL_QCMANAGER_H_
#define QG_MODEL_QCMANAGER_H_

#include <memory>
#include <ostream>

#include "eckit/config/LocalConfiguration.h"

#include "model/QgTraits.h"

#include "oops/base/Variables.h"
#include "oops/interface/ObsFilterBase.h"
#include "oops/util/Printable.h"

namespace qg {
  class GomQG;
  template <typename DATATYPE> class ObsDataQG;
  class ObsDiagsQG;
  class ObsSpaceQG;
  class ObsVecQG;

class QCmanager : public oops::interface::ObsFilterBase<QgObsTraits> {
 public:
  QCmanager(const ObsSpaceQG &, const eckit::Configuration &,
            std::shared_ptr<ObsDataQG<int> >, std::shared_ptr<ObsDataQG<float> >): novars_() {}
  ~QCmanager() {}

  void preProcess() override {}
  void priorFilter(const GomQG &) override {}
  void postFilter(const ObsVecQG &, const ObsDiagsQG &) override {}

  oops::Variables requiredVars() const override {return novars_;}
  oops::Variables requiredHdiagnostics() const override {return novars_;}

 private:
  void print(std::ostream &) const override {}
  const oops::Variables novars_;
};

}  // namespace qg

#endif  // QG_MODEL_QCMANAGER_H_
