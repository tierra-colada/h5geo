#ifndef H5POINTS2IMPL_H
#define H5POINTS2IMPL_H

#include "../h5points2.h"
#include "h5basepointsimpl.h"

template <typename TBase = H5Points2>
class H5Points2Impl : public H5BasePointsImpl<TBase>
{
protected:
  explicit H5Points2Impl(const h5gt::Group &group);
  virtual ~H5Points2Impl() = default;

public:
  virtual bool writeData(
      h5geo::Point2Array& data,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

  virtual h5geo::Point2Array getData(
      const std::string& lengthUnits = "",
      bool doCoordTransform = false) override;

protected:
  bool overwritePointsDataset(
      h5geo::Point2Array& data,
      const std::string& lengthUnits = "",
      bool doCoordTransform = false);
  /// Do the coordinate transformation and units convertion
  /// \param toRead if true then OGRCT will be created to read data
  bool transformPoints(
      h5geo::Point2Array& data,
      bool toReadData,
      const std::string& lengthUnitsFrom = "",
      const std::string& lengthUnitsTo = "",
      bool doCoordTransform = false);

  //----------- FRIEND CLASSES -----------
  friend class H5BaseObjectImpl<H5Points2>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend class H5BaseObjectImpl<H5BaseObject>;
  friend class H5BaseObjectImpl<H5BasePoints>;
  friend class H5BaseObjectImpl<H5Points1>;
  friend class H5BaseObjectImpl<H5Points3>;
  friend class H5BaseObjectImpl<H5Points4>;
  friend class H5BaseObjectImpl<H5WellTops>;
  friend class H5BaseObjectImpl<H5Horizon>;
  friend class H5BaseObjectImpl<H5Map>;
  friend class H5BaseObjectImpl<H5Seis>;
  friend class H5BaseObjectImpl<H5Well>;
  friend class H5BaseObjectImpl<H5DevCurve>;
  friend class H5BaseObjectImpl<H5LogCurve>;
  friend H5BasePoints* h5geo::openPoints(h5gt::Group group);
  friend H5Points2* h5geo::openPoints2(h5gt::Group group);
};

#endif // H5POINTS2IMPL_H
