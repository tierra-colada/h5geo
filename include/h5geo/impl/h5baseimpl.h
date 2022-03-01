#ifndef H5BASEIMPL_H
#define H5BASEIMPL_H

#include "h5base.h"

template <typename TBase = H5Base>
class H5BaseImpl : public TBase
{
protected:
  virtual ~H5BaseImpl() = default;

public:
  virtual H5Base* clone() override;
  virtual void Delete() override;

protected:
  virtual std::vector<h5gt::Group>
  getChildGroupList(h5gt::Group& group,
               const h5geo::ObjectType& objType);
  virtual std::vector<std::string>
  getChildNameList(h5gt::Group& group,
               const h5geo::ObjectType& objType);

  static std::optional<h5gt::File> createContainer(
      std::string& fileName,
      const h5geo::ContainerType& containerType,
      h5geo::CreationType createFlag);
  static std::optional<h5gt::File> createContainer(
      h5gt::File h5File,
      const h5geo::ContainerType& containerType,
      h5geo::CreationType createFlag);

  std::optional<h5gt::Group>
  /// \brief createObject Creates new object. If `CREATE_UNDER_NEW_NAME`
  /// then `objName` will be chaged to be unique in its parent file.
  /// \param objName
  /// \param parentGroup
  /// \param objType
  /// \param p
  /// \param createFlag
  createObject(
      std::string& objName,
      h5gt::File parentFile,
      const h5geo::ObjectType& objType,
      void* p,
      h5geo::CreationType createFlag);

  std::optional<h5gt::Group>
  /// \brief createObject Creates new object. If `CREATE_UNDER_NEW_NAME`
  /// then `objName` will be chaged to be unique in its parent group.
  /// \param objName
  /// \param parentGroup
  /// \param objType
  /// \param p
  /// \param createFlag
  createObject(
      std::string& objName,
      h5gt::Group parentGroup,
      const h5geo::ObjectType& objType,
      void* p,
      h5geo::CreationType createFlag);

  std::optional<h5gt::Group>
  createObject(
      h5gt::Group objG,
      const h5geo::ObjectType& objType,
      void* p,
      h5geo::CreationType createFlag);

private:
  static std::optional<h5gt::File>
  createNewContainer(
      h5gt::File &file,
      const h5geo::ContainerType& containerType);
  std::optional<h5gt::Group>
  createNewObject(
      h5gt::Group &group,
      const h5geo::ObjectType& objType,
      void* p);
  std::optional<h5gt::Group>
  createNewPoints(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewMap(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewWell(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewLogCurve(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewDevCurve(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewSeis(h5gt::Group &group, void* p);

  // Seismic
  std::optional<h5gt::DataSet>
  createTextHeader(
      h5gt::Group &seisGroup,
      bool mapSEGY);
  std::optional<h5gt::DataSet>
  createBinHeader(
      h5gt::Group &seisGroup,
      const hsize_t& stdChunk,
      bool mapSEGY);
  std::optional<h5gt::DataSet>
  createTrace(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const size_t& nSamp,
      const hsize_t& trcChunk,
      bool mapSEGY);
  std::optional<h5gt::DataSet>
  createTraceHeader(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const hsize_t& trcChunk,
      bool mapSEGY);
  std::optional<h5gt::DataSet>
  createBoundary(
      h5gt::Group &seisGroup,
      const hsize_t& stdChunk);
  std::optional<h5gt::Group>
  createSort(
      h5gt::Group &seisGroup);

  // Seismic SEGY
  std::optional<h5gt::Group>
  createExternalSEGY(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const size_t& nSamp,
      const hsize_t& trcChunk,
      const hsize_t& stdChunk,
      const std::vector<std::string>& segyFiles,
      h5geo::Endian endian);

// --------------MISCELLANEOUS------------
protected:
  /// \brief isSuccessor checks whether `childG` is successor of `parentG`
  /// \param parentG
  /// \param childG
  /// \return
  bool isSuccessor(h5gt::Group& parentG, h5gt::Group& childG);
  /// \brief isSuccessor works only with absolute path!
  /// \param parent
  /// \param child
  /// \return
  bool isSuccessor(const std::string& parent, const std::string& child);

  //----------- FRIEND CLASSES -----------
  friend H5SeisContainer* h5geo::openSeisContainer(
      h5gt::File h5File);
  friend H5MapContainer* h5geo::openMapContainer(
      h5gt::File h5File);
  friend H5WellContainer* h5geo::openWellContainer(
      h5gt::File h5File);
};

#endif // H5BASEIMPL_H
