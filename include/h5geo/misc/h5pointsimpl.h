#ifndef H5POINTSIMPL_H
#define H5POINTSIMPL_H

#include "../h5points.h"
#include "h5baseobjectimpl.h"

template <typename TBase>
class H5BaseContainerImpl;

class H5PointsImpl : public H5BaseObjectImpl<H5Points>
{
protected:
  explicit H5PointsImpl(const h5gt::Group &group);
  virtual ~H5PointsImpl() = default;

public:
  virtual bool writeData(
      h5geo::PointArray& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;

  virtual h5geo::PointArray getData(
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false) override;

  virtual bool setDomain(const h5geo::Domain& domain) override;

  virtual H5BaseContainer* getContainer() const override;

  virtual h5geo::Domain getDomain() override;

  virtual std::optional<h5gt::DataSet> getPointsD() const override;

protected:
  bool overwritePointsDataset(
      h5geo::PointArray& data,
      const std::string& lengthUnits = "",
      const std::string& temporalUnits = "",
      bool doCoordTransform = false);
  bool transformPoints(
      h5geo::PointArray& data,
      const std::string& lengthUnitsFrom = "",
      const std::string& lengthUnitsTo = "",
      const std::string& temporalUnitsFrom = "",
      const std::string& temporalUnitsTo = "",
      bool doCoordTransform = false,
      OGRCoordinateTransformation* coordTrans = nullptr);

  //----------- FRIEND CLASSES -----------
  friend class H5BaseObjectImpl<H5Points>;
  friend class H5BaseContainerImpl<H5BaseContainer>;
  friend class H5BaseContainerImpl<H5MapContainer>;
  friend class H5BaseContainerImpl<H5SeisContainer>;
  friend class H5BaseContainerImpl<H5WellContainer>;
  friend H5Points* h5geo::openPoints(h5gt::Group group);
};

#endif // H5POINTS_H
