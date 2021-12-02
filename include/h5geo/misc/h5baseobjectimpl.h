#ifndef H5BASEOBJECTIMPL_H
#define H5BASEOBJECTIMPL_H

#include "../h5baseobject.h"
#include "h5baseimpl.h"
#include "h5gt/H5Group.hpp"

template <typename TBase = H5BaseObject>
class H5BaseObjectImpl : public H5BaseImpl<TBase>
{
protected:
  explicit H5BaseObjectImpl(const h5gt::Group &group);
  virtual ~H5BaseObjectImpl() = default;

public:
#ifdef H5GEO_USE_GDAL
  ///
  /// Assumes that 'spatial_reference' and 'length_units' attributes
  /// exist and they are used to create transfromation
  virtual OGRCoordinateTransformation* createCoordinateTransformationToReadData(
      const std::string& unitsTo) override;
  ///
  /// Assumes that 'spatial_reference' and 'length_units' attributes
  /// exist and they are used to create transfromation
  virtual OGRCoordinateTransformation* createCoordinateTransformationToWriteData(
      const std::string& unitsFrom) override;
#endif

  virtual bool setSpatialReference(const std::string& str) override;
  virtual bool setSpatialReference(
      const std::string& authName, const std::string& code) override;
  virtual bool setLengthUnits(const std::string& str) override;
  virtual bool setTemporalUnits(const std::string& str) override;
  virtual bool setAngularUnits(const std::string& str) override;
  virtual bool setDataUnits(const std::string& str) override;

  virtual std::string getSpatialReference() override;
  virtual std::string getLengthUnits() override;
  virtual std::string getTemporalUnits() override;
  virtual std::string getAngularUnits() override;
  virtual std::string getDataUnits() override;

  virtual h5gt::File getH5File() const override;
  virtual h5gt::Group getObjG() const override;

  virtual std::optional<h5gt::Group>
  getGroupOpt(
      h5gt::Group& parent,
      const std::string& groupName) const override;

  virtual std::optional<h5gt::DataSet>
  getDatasetOpt(
      const h5gt::Group& parent,
      const std::string& datasetName) const override;

  virtual std::string getName() const override;
  virtual std::string getFullName() const override;

  virtual bool operator == (H5BaseObject& other) const override;
  virtual bool operator != (H5BaseObject& other) const override;

protected:
  /*!
   * \brief getParent Search for parent of type `ObjectType`
   *  for the `objG`
   * \param objType
   * \return
   */
  std::optional<h5gt::Group>
  getParentG(const h5geo::ObjectType& objType);

  //----------- FRIEND CLASSES -----------
  friend H5BaseObject* h5geo::openBaseObject(h5gt::Group group);

protected:
  h5gt::Group objG;
};

#endif // H5BASEOBJECTIMPL_H
