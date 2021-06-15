#ifndef H5SURF_H
#define H5SURF_H

#include "h5baseobject.h"

#include <Eigen/Dense>

class H5SurfContainer;

class H5Surf : public virtual H5BaseObject
{
protected:
  virtual ~H5Surf() = default;

public:
  virtual bool writeData(
      const Eigen::Ref<const Eigen::MatrixXd>& M) = 0;

  virtual Eigen::MatrixXd getData() const = 0;

  virtual H5SurfContainer* getSurfContainer() const = 0;

  virtual std::optional<h5gt::DataSet> getSurfD() const = 0;
};

using H5Surf_ptr = std::unique_ptr<H5Surf, h5geo::ObjectDeleter>;

#endif // H5SURF_H
