#ifndef H5VOLIMPL_H
#define H5VOLIMPL_H

#include "../h5vol.h"
#include "h5baseobjectimpl.h"

class H5VolImpl : public H5BaseObjectImpl<H5Vol>
{
protected:
  explicit H5VolImpl(const h5gt::Group &group);
  virtual ~H5VolImpl() = default;

public:

  virtual VolParam getParam() override;

  virtual H5VolContainer* openVolContainer() const override;

  virtual std::optional<h5gt::DataSet> getVolD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5VolContainerImpl;
  friend class H5BaseObjectImpl<H5Vol>;
  friend H5Vol* h5geo::openVol(h5gt::Group group);
};

#endif // H5VOLIMPL_H
