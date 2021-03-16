#ifndef H5SURF_H
#define H5SURF_H

#include "../h5base/h5baseobject.h"
#include "../h5core/h5core.h"

#include <Eigen/Dense>
#include <optional>

class H5SurfContainer;

class H5GEO_EXPORT H5Surf : public H5BaseObject
{
private:
  explicit H5Surf(const h5gt::Group &group);

public:
  virtual ~H5Surf() override;

  static std::optional<H5Surf>
  create(h5gt::Group &group);

  bool writeData(
      const Eigen::Ref<const Eigen::MatrixXd>& M);

  Eigen::MatrixXd getData();

  std::optional<H5SurfContainer> getSurfContainer();

  std::optional<h5gt::DataSet> getSurfD();
};

#endif // H5SURF_H
