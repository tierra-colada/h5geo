#ifndef H5POINTS3IMPL_H
#define H5POINTS3IMPL_H

#include "../h5points3.h"
#include "h5basepointsimpl.h"

class H5Points3Impl : public H5BasePointsImpl<H5Points3>
{
protected:
  explicit H5Points3Impl(const h5gt::Group &group);
  virtual ~H5Points3Impl() = default;

public:
  virtual bool writeData(
      h5geo::Point3Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;

  virtual h5geo::Point3Array getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;

//  virtual bool setNPoints(size_t n) override;
//  virtual bool setDomain(const h5geo::Domain& domain) override;

//  virtual H5BaseContainer* openContainer() const override;
//  virtual size_t getNPoints() override;
//  virtual h5geo::Domain getDomain() override;

//  virtual std::optional<h5gt::DataSet> getPointsD() const override;

protected:
  bool overwritePointsDataset(
      h5geo::Point3Array& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false);
  /// Do the coordinate transformation and units convertion
  /// \param toRead if true then OGRCT will be created to read data
  bool transformPoints(
      h5geo::Point3Array& data,
      bool toReadData,
      const std::string& lengthUnitsFrom = "",
      const std::string& lengthUnitsTo = "",
      const std::string& temporalUnitsFrom = "",
      const std::string& temporalUnitsTo = "",
      bool doCoordTransform = false);

  //----------- FRIEND CLASSES -----------
  friend class H5BaseObjectImpl<H5Points3>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend H5BasePoints* h5geo::openPoints(h5gt::Group group);
};

#endif // H5POINTS3IMPL_H
