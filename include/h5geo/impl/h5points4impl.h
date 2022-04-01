#ifndef H5POINTS4IMPL_H
#define H5POINTS4IMPL_H

#include "../h5points4.h"
#include "h5basepointsimpl.h"

class H5Points4Impl : public H5BasePointsImpl<H5Points4>
{
protected:
  explicit H5Points4Impl(const h5gt::Group &group);
  virtual ~H5Points4Impl() = default;

public:
  virtual bool writeData(
      h5geo::Point4Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;

  virtual h5geo::Point4Array getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;

protected:
  bool overwritePointsDataset(
      h5geo::Point4Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false);
  /// Do the coordinate transformation and units convertion
  /// \param toRead if true then OGRCT will be created to read data
  bool transformPoints(
      h5geo::Point4Array& data,
      bool toReadData,
      const std::string& lengthUnitsFrom = "",
      const std::string& lengthUnitsTo = "",
      const std::string& temporalUnitsFrom = "",
      const std::string& temporalUnitsTo = "",
      bool doCoordTransform = false);

  //----------- FRIEND CLASSES -----------
  friend class H5BaseObjectImpl<H5Points4>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend H5BasePoints* h5geo::openPoints(h5gt::Group group);
};

#endif // H5POINTS4IMPL_H
