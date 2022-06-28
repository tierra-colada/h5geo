#ifndef H5HORIZONIMPL_H
#define H5HORIZONIMPL_H

#include "../h5horizon.h"
#include "h5baseobjectimpl.h"

template <typename TBase>
class H5BaseContainerImpl;

class H5HorizonImpl : public H5BaseObjectImpl<H5Horizon>
{
protected:
  explicit H5HorizonImpl(const h5gt::Group &group);
  virtual ~H5HorizonImpl() = default;

public:
  virtual bool setNPoints(size_t n) override;
  virtual bool setNComponents(size_t n) override;
  virtual bool setComponents(const std::map<std::string, size_t>& components) override;
  virtual bool setDomain(const h5geo::Domain& domain) override;

  virtual H5BaseContainer* openContainer() const override;
  virtual size_t getNPoints() override;
  virtual size_t getNComponents() override;
  virtual std::map<std::string, size_t> getComponents() override;
  virtual h5geo::Domain getDomain() override;

  virtual HorizonParam getParam() override;

  virtual std::optional<h5gt::DataSet> getHorizonD() const override;

  //----------- FRIEND CLASSES -----------
  friend class H5BaseObjectImpl<H5Horizon>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend H5Horizon* h5geo::openHorizon(h5gt::Group group);
};

#endif // H5HORIZONIMPL_H
