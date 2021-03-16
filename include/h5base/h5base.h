#ifndef H5BASE_H
#define H5BASE_H

#include "../h5core/h5core.h"

#include <optional>

struct SurfParam{
  size_t nX, nY;
  double X0, Y0, dX, dY;
};

struct WellParam{
  double headX, headY, kb;
  std::string uwi;
};

struct LogCurveParam{
  hsize_t chunkSize = 1000;
};

struct DevCurveParam{
  bool setActive = false;
  hsize_t chunkSize = 1000;
};

struct SeisParam{
  h5geo::Domain domain;
  h5geo::SeisDataType dataType;
  h5geo::SurveyType surveyType;
  size_t nTrc;
  size_t nSamp;
  double srd = 0;
  hsize_t trcChunk = 20000;
  hsize_t stdChunk = 1000;
  h5geo::CreationType seisCreateType =
      h5geo::CreationType::CREATE_OR_OVERWRITE;
};

class H5GEO_EXPORT H5Base
{
public:
  virtual ~H5Base();

protected:
  explicit H5Base();

  std::vector<h5gt::Group>
  getChildList(h5gt::Group& group,
               const h5geo::ObjectType& objType);

  static bool isObject(
      h5gt::Group &group,
      const h5geo::ObjectType& objType);

  static bool isSurf(h5gt::Group &group);
  static bool isWell(h5gt::Group &group);
  static bool isLogCurve(h5gt::Group &group);
  static bool isDevCurve(h5gt::Group &group);
  static bool isSeis(h5gt::Group &group);

  static std::optional<h5gt::Group>
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
      const h5geo::CreationType createFlag =
      h5geo::CreationType::OPEN_OR_CREATE);

  static std::optional<h5gt::Group>
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
      const h5geo::CreationType createFlag =
      h5geo::CreationType::OPEN_OR_CREATE);

  static std::optional<h5gt::Group>
  createObject(
      h5gt::Group& group,
      const h5geo::ObjectType& objType,
      void* p,
      const h5geo::CreationType createFlag =
      h5geo::CreationType::OPEN_OR_CREATE);

private:
  static std::optional<h5gt::Group>
  createNewObject(
      h5gt::Group &group,
      const h5geo::ObjectType& objType,
      void* p);
  static std::optional<h5gt::Group>
  createNewSurf(h5gt::Group &group, void* p);
  static std::optional<h5gt::Group>
  createNewWell(h5gt::Group &group, void* p);
  static std::optional<h5gt::Group>
  createNewLogCurve(h5gt::Group &group, void* p);
  static std::optional<h5gt::Group>
  createNewDevCurve(h5gt::Group &group, void* p);
  static std::optional<h5gt::Group>
  createNewSeis(h5gt::Group &group, void* p);

  static std::optional<h5gt::DataSet>
  createTextHeader(
      h5gt::Group &seisGroup);
  static std::optional<h5gt::DataSet>
  createBinHeader(
      h5gt::Group &seisGroup,
      const hsize_t& stdChunk);
  static std::optional<h5gt::DataSet>
  createTrace(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const size_t& nSamp,
      const hsize_t& trcChunk);
  static std::optional<h5gt::DataSet>
  createTraceHeader(
      h5gt::Group &seisGroup,
      const size_t& nTrc,
      const hsize_t& trcChunk);
  static std::optional<h5gt::DataSet>
  createBoundary(
      h5gt::Group &seisGroup,
      const hsize_t& stdChunk);
  static std::optional<h5gt::Group>
  createSort(
      h5gt::Group &seisGroup);
};

#endif // H5BASE_H
