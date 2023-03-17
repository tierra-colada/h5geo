#ifndef H5BASEPOINTSIMPL_H
#define H5BASEPOINTSIMPL_H

#include "../h5basepoints.h"
#include "h5baseobjectimpl.h"

template <typename TBase>
class H5BaseContainerImpl;

template <typename TBase = H5BasePoints>
class H5BasePointsImpl : public H5BaseObjectImpl<TBase>
{
protected:
  explicit H5BasePointsImpl(const h5gt::Group &group);
  virtual ~H5BasePointsImpl() = default;

public:
  virtual bool setNPoints(size_t n) override;
  virtual bool setDomain(const h5geo::Domain& domain) override;

  virtual H5BaseContainer* openContainer() const override;
  virtual size_t getNPoints() override;
  virtual h5geo::Domain getDomain() override;

  virtual H5PointsParam getParam() override;

  virtual std::optional<h5gt::DataSet> getPointsD() const override;

protected:

  //----------- FRIEND CLASSES -----------
  friend class H5BaseObjectImpl<H5BasePoints>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend class H5BaseObjectImpl<H5BaseObject>;
  friend class H5BaseObjectImpl<H5Points1>;
  friend class H5BaseObjectImpl<H5Points2>;
  friend class H5BaseObjectImpl<H5Points3>;
  friend class H5BaseObjectImpl<H5Points4>;
  friend class H5BaseObjectImpl<H5Horizon>;
  friend class H5BaseObjectImpl<H5WellTops>;
  friend class H5BaseObjectImpl<H5Map>;
  friend class H5BaseObjectImpl<H5Seis>;
  friend class H5BaseObjectImpl<H5Well>;
  friend class H5BaseObjectImpl<H5DevCurve>;
  friend class H5BaseObjectImpl<H5LogCurve>;
  friend H5BasePoints* openPoints(h5gt::Group group);
};

#endif // H5BASEPOINTSIMPL_H
