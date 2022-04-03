#ifndef H5WELLTOPSIMPL_H
#define H5WELLTOPSIMPL_H

#include "../h5welltops.h"
#include "h5points1impl.h"

class H5WellTopsImpl : public H5Points1Impl<H5WellTops>
{
protected:
  explicit H5WellTopsImpl(const h5gt::Group &group);
  virtual ~H5WellTopsImpl() = default;

public:
  virtual bool writeWellTops(
      const std::map<std::string, double>& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") override;

  virtual std::map<std::string, double> getWellTops(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "") override;

  virtual H5Well* openWell() override;

  //----------- FRIEND CLASSES -----------
  friend class H5WellImpl;
  friend class H5BaseObjectImpl<H5WellTops>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend H5WellTops* h5geo::openWellTops(h5gt::Group group);
};

#endif // H5WELLTOPSIMPL_H
