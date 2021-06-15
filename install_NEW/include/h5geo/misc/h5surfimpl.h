#ifndef H5SURFIMPL_H
#define H5SURFIMPL_H

#include "../h5surf.h"
#include "h5baseobjectimpl.h"

class H5SurfImpl :
    public virtual H5Surf,
    public virtual H5BaseObjectImpl
{
protected:
  explicit H5SurfImpl(const h5gt::Group &group);
  virtual ~H5SurfImpl() = default;

public:
  virtual bool writeData(
      const Eigen::Ref<const Eigen::MatrixXd>& M) override;

  virtual Eigen::MatrixXd getData() const override;

  virtual H5SurfContainer* getSurfContainer() const override;

  virtual std::optional<h5gt::DataSet> getSurfD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5SurfContainerImpl;
};

#endif // H5SURFIMPL_H
