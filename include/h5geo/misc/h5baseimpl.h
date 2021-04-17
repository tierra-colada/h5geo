#ifndef H5BASEIMPL_H
#define H5BASEIMPL_H

#include "h5base.h"

class H5BaseImpl : public virtual H5Base
{
protected:
  virtual ~H5BaseImpl() = default;

public:
  virtual void Delete() override;
  virtual std::vector<h5gt::Group>
  getChildList(h5gt::Group& group,
               const h5geo::ObjectType& objType) override;

protected:
  static bool isContainer(
      h5gt::File &file,
      const h5geo::ContainerType& cntType);

  static bool isSurfContainer(h5gt::File &file);
  static bool isWellContainer(h5gt::File &file);
  static bool isSeisContainer(h5gt::File &file);

  bool isObject(
      h5gt::Group &group,
      const h5geo::ObjectType& objType);

  bool isSurf(h5gt::Group &group);
  bool isWell(h5gt::Group &group);
  bool isLogCurve(h5gt::Group &group);
  bool isDevCurve(h5gt::Group &group);
  bool isSeis(h5gt::Group &group);

  static std::optional<h5gt::File> createContainer(
      std::string& fileName,
      const h5geo::ContainerType& containerType,
      h5geo::CreationType createFlag);
  static std::optional<h5gt::File> createContainer(
      h5gt::File &h5File,
      const h5geo::ContainerType& containerType,
      h5geo::CreationType createFlag);

  std::optional<h5gt::Group>
  /*!
   * \brief createObject Creates new object. If `CREATE_UNDER_NEW_NAME`
   * then `objName` will be chaged to be unique in its parent file.
   * \param objName
   * \param parentGroup
   * \param objType
   * \param p
   * \param createFlag
   */
  createObject(
      std::string& objName,
      h5gt::File& parentFile,
      const h5geo::ObjectType& objType,
      void* p,
      h5geo::CreationType createFlag);

  std::optional<h5gt::Group>
  /*!
   * \brief createObject Creates new object. If `CREATE_UNDER_NEW_NAME`
   * then `objName` will be chaged to be unique in its parent group.
   * \param objName
   * \param parentGroup
   * \param objType
   * \param p
   * \param createFlag
   */
  createObject(
      std::string& objName,
      h5gt::Group& parentGroup,
      const h5geo::ObjectType& objType,
      void* p,
      h5geo::CreationType createFlag);

  std::optional<h5gt::Group>
  createObject(
      h5gt::Group& objG,
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
  createNewSurf(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewWell(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewLogCurve(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewDevCurve(h5gt::Group &group, void* p);
  std::optional<h5gt::Group>
  createNewSeis(h5gt::Group &group, void* p);

  std::optional<h5gt::DataSet>
  createTextHeader(
      h5gt::Group &seisGroup);
  std::optional<h5gt::DataSet>
  createBinHeader(
      h5gt::Group &seisGroup,
      const hsize_t& stdChunk);
  std::optional<h5gt::DataSet>
  createTrace(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const size_t& nSamp,
      const hsize_t& trcChunk);
  std::optional<h5gt::DataSet>
  createTraceHeader(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const hsize_t& trcChunk);
  std::optional<h5gt::DataSet>
  createBoundary(
      h5gt::Group &seisGroup,
      const hsize_t& stdChunk);
  std::optional<h5gt::Group>
  createSort(
      h5gt::Group &seisGroup);

// --------------MISCELLANEOUS------------
protected:
  /*!
   * \brief isSuccessor checks whether `childG` is successor of `parentG`
   * \param parentG
   * \param childG
   * \return
   */
  bool isSuccessor(h5gt::Group& parentG, h5gt::Group& childG);
  /*!
   * \brief isSuccessor works only with absolute path!
   * \param parent
   * \param child
   * \return
   */
  bool isSuccessor(const std::string& parent, const std::string& child);

};

#endif // H5BASEIMPL_H
