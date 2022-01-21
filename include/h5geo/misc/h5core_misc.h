#ifndef H5CORE_MISC_H
#define H5CORE_MISC_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include "h5core_enum.h"
#include "h5core_types.h"
#include "h5deviation.h"
#include "h5easyhull.h"
#include "h5sort.h"

#include <type_traits>
#include <string>
#include <vector>
#include <regex>

#include <Eigen/Dense>

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Attribute.hpp>

namespace h5geo
{

// strings are intensionally TAB delimited
inline std::vector<std::string> getRawBinHeaderNames()
{
  return
  {
    {"Job identification number.	JOB	3201	4"},
    {"Line number.  For 3-D poststack data, this will typically contain the in-line number.	LINE	3205	4"},
    {"Reel number.	REEL	3209	4"},
    {"Number of data traces per ensemble.	TRACENUM	3213	2"},
    {"Number of auxiliary traces per ensemble.	AUX	3215	2"},
    {"Sample interval.	SAMP_RATE	3217	2"},
    {"Sample interval of original field recording.	SAMP_FRATE	3219	2"},
    {"Number of samples per data trace.	SAMP_NUM	3221	2"},
    {"Number of samples per data trace for original field recording.	SAMP_FNUM	3223	2"},
    {"Data sample format code.  Mandatory for all data. 1=4-byte IBM float; 2=4-byte, two`s complement integer; 3=2-byte, two`s complement integer; 4=4-byte fixed-point with gain (obsolete); 5=4-byte IEEE float; 6,7=not used; 8=1-byte, two`s complement integer	FORMAT	3225	2"},
    {"CMP fold.	CDP_FOLD	3227	2"},
    {"Trace sorting code. -1=Other (should be explained in user Extended Textual Header); 0=Unknown; 1=As recorded (no sort); 2=CDP; 3=Single fold continuous profile; 4=Horizontally stacked; 5=Common source; 6=Common receiver; 7=Common offset; 8=Common mid-point; 9=Common conversion point	SORT	3229	2"},
    {"Vertical sum code. 1=no sum; 2=two sum; ... N=M-1 sum (M=2 to 32,767)	VERT_SUM	3231	2"},
    {"Sweep frequency at start (Hz).	SWEEP_START	3233	2"},
    {"Sweep frequency at end (Hz).	SWEEP_END	3235	2"},
    {"Sweep length (ms).	SWEEP_LENGTH	3237	2"},
    {"Sweep type code. 1=linear; 2=parabolic; 3=exponential; 4=other	SWEEP_TYPE	3239	2"},
    {"Trace number of sweep channel	SWEEP_CHAN	3241	2"},
    {"Sweep trace taper length at start if tapered	SWEEP_TAPER_START	3243	2"},
    {"Sweep trace taper length at end	SWEEP_TAPER_END	3245	2"},
    {"Taper type. 1=linear; 2=cos^2; 3=other	SWEEP_TAPER_TYPE	3247	2"},
    {"Correlated data traces. 1=no; 2=yes	CORR	3249	2"},
    {"Binary gain recovered. 1=yes; 2=no	BIN_GAIN	3251	2"},
    {"Amplitude recovery method. 1=none; 2=spherical divergence; 3=AGC; 4=other	AMP_REC	3253	2"},
    {"Measurement system. 1=Meters; 2=Feet	LENGTH_SYS	3255	2"},
    {"Impulse signal polarity. 1=Increase in pressure or upward geophone case movement gives negative number on tape; 2=Increase in pressure or upward geophone case movement gives positive number on tape	POLARITY	3257	2"},
    {"Vibratory polarity code: Seismic signal lags pilot signal by: 1=337.5 to 22.5; 2=22.5 to 67.5; 3=67.5 to 112.5; 4=112.5 to 157.5; 5=157.5 to 202.5; 202.5 to 247.5; 7=247.5 to 292.5; 8=292.5 to 337.5	VIB_POL	3259	2"},
    {"SEGY format revision number.	REVISION	3501	2"},
    {"Fixed length trace flag. 0=trace length may vary and the actual length is stored in 115-116 bytes of trace header; 1=all traces have the same length	FIXED_TRLENGTH	3503	2"},
    {"Extended textual header. 0=there are no additional textual header; positive number indicates that there exactly that many Extended Textual Headers	N_EXT_HDRS	3505	2"}
  };
}

// strings are intensionally TAB delimited
inline std::vector<std::string> getRawTraceHeaderNames()
{
  return
  {
    {"* Trace sequence number within line	SEQWL	1	4"},
    {"Trace sequence number within reel	SEQWR	5	4"},
    {"* FFID - Original field record number	FFID	9	4"},
    {"* Trace number within field record	TRCFLD	13	4"},
    {"SP - Energy source point number	SP	17	4"},
    {"CDP ensemble  number	CDP	21	4"},
    {"Trace  number	TRCNUM	25	4"},
    {"* Trace identification code	TRCID	29	2"},
    {"Number of vertically summed traces	NVST	31	2"},
    {"Number of horizontally stacked traces	NHST	33	2"},
    {"Data use ( 1-production, 2-test )	DU	35	2"},
    {"Distance from source point to receiv grp	DSREG	37	4"},
    {"Receiver group elevation	RGE	41	4"},
    {"Surface elevation at source	SES	45	4"},
    {"Source depth below surface	SDBS	49	4"},
    {"Datum elevation at receiver group	DERG	53	4"},
    {"Datum elevation at source	DES	57	4"},
    {"Water depth at source	WDS	61	4"},
    {"Water depth at group	WGD	65	4"},
    {"Scaler to all elevations & depths	SAED	69	2"},
    {"Scaler to all coordinates	SAC	71	2"},
    {"Source X coordinate	SRCX	73	4"},
    {"Source Y coordinate	SRCY	77	4"},
    {"Group  X coordinate	GRPX	81	4"},
    {"Group  Y coordinate	GRPY	85	4"},
    {"Coordinate units (1-lenm/ft 2-secarc)	UNITS	89	2"},
    {"Weathering velocity	WVEL	91	2"},
    {"Subweathering velocity	SVEL	93	2"},
    {"Uphole time at source	UTSRC	95	2"},
    {"Uphole time at group	UTGRP	97	2"},
    {"Source static correction	SECSCOR	99	2"},
    {"Group  static correction	GRPSCOR	101	2"},
    {"Total static applied	TSA	103	2"},
    {"Lag time A	LAGTA	105	2"},
    {"Lag time B	LAGTB	107	2"},
    {"Delay Recording time	DELRECT	109	2"},
    {"Mute time start	MTSTART	111	2"},
    {"Mute time end	MTEND	113	2"},
    {"* Number of samples in this trace	NSMP	115	2"},
    {"* Sample interval in ms for this trace	SI	117	2"},
    {"Gain type of field instruments	GTFI	119	2"},
    {"Instrument gain	IG	121	2"},
    {"Instrument gain constant	IGC	123	2"},
    {"Correlated (1-yes / 2-no)	CORREL	125	2"},
    {"Sweep frequency at start	SFSTART	127	2"},
    {"Sweep frequency at end	SFEND	129	2"},
    {"Sweep lenth in ms	SLEN	131	2"},
    {"Sweep type 1-lin,2-parabol,2-exp,4-ohter	STYP	133	2"},
    {"Sweep trace taper length at start in ms	SSTRLS	135	2"},
    {"Sweep trace taper length at end   in ms	SSTLE	137	2"},
    {"Taper type 1-lin,2-cos2,3-other	TTYP	139	2"},
    {"Alias filter frequency, if used	AFF	141	2"},
    {"Alias filter slope	AFS	143	2"},
    {"Notch filter frequency, if used	NFF	145	2"},
    {"Notch filter slope	NFS	147	2"},
    {"Low cut frequency,  if used	LOCF	149	2"},
    {"High cut frequency, if used	HOCF	151	2"},
    {"Low cut slope	LOCS	153	2"},
    {"High cut slope	HICS	155	2"},
    {"Year data recorded	YEAR	157	2"},
    {"Day of year	DAY	159	2"},
    {"Hour of day	HOUR	161	2"},
    {"Minute of hour	MINUTE	163	2"},
    {"Second of minute	SCE	165	2"},
    {"Time basis code 1-local,2-GMT,3-other	TMBS	167	2"},
    {"Trace weighting factor	TWF	169	2"},
    {"Geophone group number of roll sw pos 1	GGNSW	171	2"},
    {"Geophone group number of trace # 1	GGN1ST	173	2"},
    {"Geophone group number of last trace	GGNLST	175	2"},
    {"Gap size (total # of groups dropped)	GAPSZ	177	2"},
    {"Overtravel assoc w taper of beg/end line	OAWT	179	2"},
    {"+ CDP X	CDP_X	181	4"},
    {"+ CDP Y	CDP_Y	185	4"},
    {"+ Inline Number	INLINE	189	4"},
    {"+ Clossline Number	XLINE	193	4"},
    {"+ Shot Point Number	SPN	197	4"},
    {"+ Shot Point Scalar	SPS	201	2"},
    {"+ Trace value measurement unit	TVMU	203	2"}
  };
}

inline bool compareStrings(
    const std::string& bigger,
    const std::string& smaller,
    const CaseSensitivity& caseSensitivity)
{
  switch (caseSensitivity) {
  case CaseSensitivity::CASE_INSENSITIVE :{
    // 0 - means strings are equal
    int val = bigger.compare(0, smaller.size(), smaller);
    if (val == 0)
      return true;
    else
      return false;
  } case CaseSensitivity::CASE_SENSITIVE : {
    return std::equal(smaller.begin(), smaller.end(), bigger.begin(),
                      [](char a, char b) {return tolower(a) == tolower(b);});
  } default :
    return false;
  }
}

inline void eraseSubStr(
    std::string & mainStr, const std::string & toErase)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

inline void eraseAllSubStr(
    std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

inline char getDelimiter(const Delimiter& delimiter){
  if (delimiter == Delimiter::DOT)
    return '.';
  else if (delimiter == Delimiter::COMMA)
    return ',';
  else if (delimiter == Delimiter::SPACE)
    return ' ';
  else if (delimiter == Delimiter::SEMICOLON)
    return ';';
  else if (delimiter == Delimiter::TABULATION)
    return '\n';

  return ' ';
}

inline std::string generateName(
    const std::vector<std::string> &nameList,
    std::string baseName){
  if (baseName.empty())
    baseName = "no_name";

  if(std::find(nameList.begin(), nameList.end(), baseName) == nameList.end()){
    /* nameList does not contain baseName */
    return baseName;
  }

  /* nameList contains baseName */
  size_t i = 1;
  std::string name = baseName + "_" + std::to_string(i);
  while (std::find(nameList.begin(), nameList.end(), name)
         != nameList.end()){
    name = baseName + "_" + std::to_string(i);
    i++;
  }

  return name;
}

inline std::vector<std::string> splitString(const std::string &s, const std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
    token = s.substr (pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back (token);
  }

  res.push_back (s.substr (pos_start));
  return res;
}

inline void splitHeaderNames(const std::vector<std::string> &headerNamesToSplit,
                      std::vector<std::string> &fullHeaderNames,
                      std::vector<std::string> &shortHeaderNames){
  fullHeaderNames.clear();
  shortHeaderNames.clear();
  for (size_t i = 0; i < headerNamesToSplit.size(); i++){
    std::vector<std::string> v = splitString(headerNamesToSplit[i], "\t");
    fullHeaderNames.push_back(v[0]);
    shortHeaderNames.push_back(v[1]);
  }
}

inline void splitHeaderBytes(
    const std::vector<std::string> &headerNamesToSplit,
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes){
  bytesStart.clear();
  nBytes.clear();
  for (size_t i = 0; i < headerNamesToSplit.size(); i++){
    std::vector<std::string> v = splitString(headerNamesToSplit[i], "\t");
    bytesStart.push_back(std::stoi(v.end()[-2]));
    nBytes.push_back(std::stoi(v.end()[-1]));
  }
}

inline std::vector<std::string> splitPath(
    std::string path)
{
  std::vector<std::string> results;

  size_t cutAt;
  while ((cutAt = path.find_first_of('/')) != path.npos){
    if(cutAt > 0)
      results.push_back(path.substr (0,cutAt));
    path = path.substr(cutAt+1);
  }

  if (path.length () > 0)
    results.push_back(path);

  return results;
}

inline std::vector<std::string> splitPath(
    std::string path, std::string& filteredPath)
{
  std::vector<std::string> results = splitPath(path);
  if (path[0] == '/')
    filteredPath = '/';

  if (!results.empty()){
    filteredPath += results[0];

    for (size_t i = 1; i < results.size(); i++)
      filteredPath += "/" + results[i];
  }

  return results;
}

inline std::string splitPathToParentAndObj(
    const std::string& path, std::string& objName)
{
  if (path.empty())
    return std::string();

  std::vector<std::string> pathVec = splitPath(path);
  if(pathVec.empty()){
    objName.clear();
    return std::string();
  }

  std::string parentPath;
  if (path[0] == '/')
    parentPath = '/';

  for (size_t i = 0; i < pathVec.size()-1; i++){
    if (i < pathVec.size()-2)
      parentPath += pathVec[i] + '/';
    else
      parentPath += pathVec[i];
  }

  objName = pathVec.back();
  return parentPath;
}

inline std::string getRelativePath(
    const std::string& parentPath,
    const std::string& objPath,
    const CaseSensitivity& caseSensitivity)
{
  if (parentPath.empty())
    return objPath;

  if (objPath.empty())
    return std::string();

  if (parentPath.size() >= objPath.size())
    return std::string();

  bool val = compareStrings(objPath, parentPath, caseSensitivity);

  if (!val)
    return std::string();

  if (objPath[parentPath.size()] == '/' &&
      objPath.size() > parentPath.size()+1){
    return objPath.substr(parentPath.size()+1, objPath.size()-parentPath.size()-1);
  } else {
    return objPath.substr(parentPath.size(), objPath.size()-parentPath.size());
  }
}

inline void getTraceHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> traceHeaderNames = getRawTraceHeaderNames();

  splitHeaderNames(traceHeaderNames, fullHeaderNames, shortHeaderNames);
}

inline void getBinHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> binHeaderNames = getRawBinHeaderNames();

  splitHeaderNames(binHeaderNames, fullHeaderNames, shortHeaderNames);
}

inline void getTraceHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes){
  std::vector<std::string> traceHeaderNames = getRawTraceHeaderNames();

  splitHeaderBytes(traceHeaderNames, bytesStart, nBytes);
}

inline void getBinHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes){
  std::vector<std::string> binHeaderNames = getRawBinHeaderNames();

  splitHeaderBytes(binHeaderNames, bytesStart, nBytes);
}

inline size_t getTraceHeaderCount(){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames.size();
}

inline size_t getBinHeaderCount(){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames.size();
}

inline ptrdiff_t getIndexFromAttribute(
    h5gt::DataSet& dataset,
    const std::string& attrName)
{
  // hdf5 throws exception when empty name is passed
  if (attrName.empty() || !dataset.hasAttribute(attrName))
    return -1;

  ptrdiff_t idx;
  dataset.getAttribute(attrName).read(idx);
  return idx;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool deleteAllAttributes(Object& object){
  try {
    std::vector<std::string> attrNameList =
        object.listAttributeNames();
    for (const auto& name : attrNameList)
      object.deleteAttribute(name);
  } catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Parent,
         typename std::enable_if<
           std::is_same<Parent, h5gt::File>::value ||
           std::is_same<Parent, h5gt::Group>::value>::type*>
inline bool unlinkObject(Parent& parent, const std::string& objPath){
  try {
    parent.unlink(objPath);
  }  catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value>::type*>
inline bool unlinkContent(Object& object){
  try {
    std::vector<std::string> objNames =
        object.listObjectNames();
    for (const auto& name : objNames)
      object.unlink(name);
  } catch (h5gt::Exception e) {
    return false;
  }
  return true;
}

template<typename Object, typename T,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value>::type*>
inline bool _overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nH5Rows,
    size_t nH5Cols,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if(datasetPath.empty() ||
     !node.hasObject(datasetPath, h5gt::ObjectType::Dataset))
    return false;

  if (nH5Rows == 0 || nH5Cols == 0)
    return false;

  h5gt::DataSet dset = node.getDataSet(datasetPath);
  if (!dset.getCreateProps().isChunked())
    return false;

  auto dtype = dset.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>())){
    return false;
  }

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nH5Rows*nH5Cols; i++)
      M[i] *= coef;
  }

  try {
    std::vector<size_t> dims = {nH5Rows, nH5Cols};
    dset.resize(dims);
    dset.write_raw(M);
    return true;
  } catch (h5gt::Exception e) {
    return false;
  }
}

template<typename Object, typename D,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteResizableDataset(
      node, datasetPath, M.derived().data(), M.cols(), M.rows(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    std::vector<T>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteResizableDataset(
      node, datasetPath, v.data(), 1, v.size(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteResizableDataset(
    Object& node,
    const std::string& datasetPath,
    T& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteResizableDataset(
      node, datasetPath, &v, 1, 1, unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value>::type*>
inline bool _overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nH5Rows,
    size_t nH5Cols,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (datasetPath.empty())
    return false;

  if (nH5Rows == 0 || nH5Cols == 0)
    return false;

  if(node.hasObject(datasetPath, h5gt::ObjectType::Dataset)){
    h5gt::DataSet dset = node.getDataSet(datasetPath);
    auto dtype = dset.getDataType();
    if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
        dset.getMemSpace().getElementCount() != nH5Rows*nH5Cols){
      node.unlink(datasetPath);
      node.template createDataSet<T>(
          datasetPath, h5gt::DataSpace({nH5Rows, nH5Cols}));
    }
  } else {
    node.template createDataSet<T>(
        datasetPath, h5gt::DataSpace({nH5Rows, nH5Cols}));
  }

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nH5Rows*nH5Cols; i++)
      M[i] *= coef;
  }

  node.getDataSet(datasetPath).write_raw(M);
  return true;
}


template<typename Object, typename D,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteDataset(
      node, datasetPath, M.derived().data(), M.cols(), M.rows(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    std::vector<T>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteDataset(
      node, datasetPath, v.data(), 1, v.size(), unitsFrom, unitsTo);
}

template<typename Object, typename T,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value &&
           std::is_arithmetic<T>::value>::type*>
inline bool overwriteDataset(
    Object& node,
    const std::string& datasetPath,
    T& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteDataset(
      node, datasetPath, &v, 1, 1, unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value>::type*>
inline bool _readDataset(
    Object& node,
    const std::string& datasetPath,
    T* M,
    size_t nElem,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (datasetPath.empty() ||
      !node.hasObject(datasetPath, h5gt::ObjectType::Dataset))
    return false;

  if (nElem == 0)
    return false;

  h5gt::DataSet dset = node.getDataSet(datasetPath);
  auto dtype = dset.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
      dset.getMemSpace().getElementCount() != nElem)
    return false;

  dset.read(M);

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nElem; i++)
      M[i] *= coef;
  }

  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool readDataset(
    Object& node,
    const std::string& datasetPath,
    Eigen::DenseBase<D>& M,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  // we don't want to resize vector if no data to read
  if (datasetPath.empty() ||
      !node.hasObject(datasetPath, h5gt::ObjectType::Dataset))
    return false;

  h5gt::DataSet dset = node.getDataSet(datasetPath);
  auto dtype = dset.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<typename D::Scalar>()))
    return false;

  std::vector<size_t> dims = dset.getDimensions();
  M.derived().resize(dims[1], dims[0]); // resize the derived object
  return _readDataset(node, datasetPath, M.derived().data(), M.size(), unitsFrom, unitsTo);
}

template<typename Object,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value ||
             std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::MatrixXf readFloatEigenMtxDataset(
    Object& node,
    const std::string& datasetPath,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  Eigen::MatrixXf M;
  readDataset(node, datasetPath, M, unitsFrom, unitsTo);
  return M;
}

template<typename Object,
         typename std::enable_if<
             std::is_same<Object, h5gt::File>::value ||
             std::is_same<Object, h5gt::Group>::value ||
             std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::MatrixXd readDoubleEigenMtxDataset(
    Object& node,
    const std::string& datasetPath,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  Eigen::MatrixXd M;
  readDataset(node, datasetPath, M, unitsFrom, unitsTo);
  return M;
}


template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool _readAttribute(
    Object& holder,
    const std::string& attrName,
    T *v,
    size_t nElem,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (attrName.empty() || !holder.hasAttribute(attrName))
    return false;

  if (nElem == 0)
    return false;

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
      attr.getMemSpace().getElementCount() != nElem)
    return false;

  attr.read(v);

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nElem; i++)
      v[i] *= coef;
  }

  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool readAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D> &v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  // we don't want to resize vector if no data to read
  if (attrName.empty() || !holder.hasAttribute(attrName))
    return false;

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<typename D::Scalar>()))
    return false;

  v.derived().resize(holder.getAttribute(attrName).getMemSpace().getElementCount());
  return _readAttribute(
      holder, attrName, v.derived().data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool readAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T> &v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  // we don't want to resize vector if no data to read
  if (attrName.empty() || !holder.hasAttribute(attrName))
    return false;

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()))
    return false;

  v.resize(holder.getAttribute(attrName).getMemSpace().getElementCount());
  return _readAttribute(
      holder, attrName, v.data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool readAttribute(
    Object& holder,
    const std::string& attrName,
    T &v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _readAttribute(
      holder, attrName, &v, 1, unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value||
            std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool _overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T* v,
    size_t nElem,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  if (attrName.empty())
    return false;

  if (nElem == 0)
    return false;

  if (!holder.hasAttribute(attrName))
    holder.template createAttribute<T>(
          attrName, h5gt::DataSpace({nElem}));

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<T>()) ||
      attr.getMemSpace().getElementCount() != nElem){
    try {
      holder.deleteAttribute(attrName);
      attr = holder.template createAttribute<T>(
            attrName, h5gt::DataSpace({nElem}));
    }  catch (h5gt::Exception e) {
      return false;
    }
  }

  if (!unitsFrom.empty() && !unitsTo.empty()){
    double coef = units::convert(
        units::unit_from_string(unitsFrom),
        units::unit_from_string(unitsTo));
    for(size_t i = 0; i < nElem; i++)
      v[i] *= coef;
  }

  attr.write(v);
  return true;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    const std::string& str)
{
  if (attrName.empty())
    return false;

  if (!holder.hasAttribute(attrName))
    holder.template createAttribute<std::string>(
          attrName, h5gt::DataSpace::From(str));

  h5gt::Attribute attr = holder.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<std::string>()) ||
      attr.getMemSpace().getElementCount() != 1)
    return false;

  attr.write(str);
  return true;
}

template<typename Object, typename D,
         typename std::enable_if<
           (std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value) &&
           std::is_arithmetic<typename D::Scalar>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    Eigen::DenseBase<D>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteAttribute(
      holder, attrName, v.derived().data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    std::vector<T>& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteAttribute(
      holder, attrName, v.data(), v.size(), unitsFrom, unitsTo);
}

template <typename Object, typename T,
          typename std::enable_if<
            (std::is_same<Object, h5gt::File>::value ||
            std::is_same<Object, h5gt::Group>::value ||
            std::is_same<Object, h5gt::DataSet>::value) &&
            std::is_arithmetic<T>::value>::type*>
inline bool overwriteAttribute(
    Object& holder,
    const std::string& attrName,
    T& v,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  return _overwriteAttribute(
      holder, attrName, &v, 1, unitsFrom, unitsTo);
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline unsigned readEnumAttribute(Object& object, const std::string& attrName){
  /* as we often use magic_enum to convert enum to string
   * we need to remove `h5geo::` from enum name given
   * from magic_enum () as for example:
   * magic_enum::enum_type_name<h5geo::SurveyType>() */
//  eraseSubStr(attrName, "h5geo::");

  unsigned value = 0;
  readAttribute(object, attrName, value);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline std::string readStringAttribute(Object& object, const std::string& attrName){
  std::string str;
  if (attrName.empty() || !object.hasAttribute(attrName))
    return str;

  h5gt::Attribute attr = object.getAttribute(attrName);
  auto dtype = attr.getDataType();
  if (!dtype.isTypeEqual(h5gt::AtomicType<std::string>()) ||
      attr.getMemSpace().getElementCount() != 1)
      return str;

  attr.read(str);
  return str;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline float readFloatAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  float value = NAN;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline double readDoubleAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo)
{
  double value = NAN;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline std::vector<float> readFloatVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  std::vector<float> value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline std::vector<double> readDoubleVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  std::vector<double> value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::VectorXf readFloatEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  Eigen::VectorXf value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename Object,
         typename std::enable_if<
           std::is_same<Object, h5gt::File>::value ||
           std::is_same<Object, h5gt::Group>::value ||
           std::is_same<Object, h5gt::DataSet>::value>::type*>
inline Eigen::VectorXd readDoubleEigenVecAttribute(
    Object& object, const std::string& attrName,
    const std::string& unitsFrom,
    const std::string& unitsTo){
  Eigen::VectorXd value;
  readAttribute(object, attrName, value, unitsFrom, unitsTo);
  return value;
}

template<typename D>
inline bool writeData2IndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName,
    const Eigen::DenseBase<D>& v,
    bool resize)
{
  if (v.size() == 0)
    return false;

  ptrdiff_t ind = h5geo::getIndexFromAttribute(
        dataset, attrName);

  if (ind < 0)
    return false;

  std::vector dims = dataset.getDimensions();

  if (resize == false &&
      dims[1] < v.size())
    return false;

  if (resize == true  &&
      dims[1] < v.size())
    dataset.resize({dims[0], size_t(v.size())});

  try {
    dataset.select({size_t(ind), 0}, {1, size_t(v.size())}).
        write_raw(v.derived().data());
  } catch (h5gt::Exception e) {
    return false;
  }

  return true;
}

template<typename T>
inline Eigen::VectorX<T> getDataFromIndexedDataset(
    h5gt::DataSet& dataset,
    const std::string& attrName)
{
  ptrdiff_t ind = h5geo::getIndexFromAttribute(
        dataset, attrName);

  if (ind < 0)
    return Eigen::VectorXd();

  std::vector dims = dataset.getDimensions();
  Eigen::VectorX<T> v(dims[1]);

  try {
    dataset.select({size_t(ind), 0}, {1, dims[1]}).
        read(v.derived().data());
  } catch (h5gt::Exception e) {
    return Eigen::VectorX<T>();
  }

  return v;
}

template<typename T>
inline Eigen::VectorX<ptrdiff_t> find_index(
    Eigen::DenseBase<T> const & M){
  Eigen::VectorX<ptrdiff_t> ind(M.size());
  ptrdiff_t ii = 0;
  for (ptrdiff_t i = 0; i < M.size(); i++){
    if (M(i) != 0){
      ind(ii) = i;
      ii++;
    }
  }
  ind.conservativeResize(ii);

  return ind;
}

template<typename D, typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const Eigen::DenseBase<D>&  cols)
{
  ptrdiff_t I = cols.size();
  std::vector<size_t> v(2*I, row);

  for (ptrdiff_t i = 0; i < I; i++)
    v[2*i+1] = cols(i);

  return h5gt::ElementSet(v);
}

template<typename D, typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowsCol2ElementSet(
    const Eigen::DenseBase<D>& rows,
    const T& col)
{
  ptrdiff_t I = rows.size();
  std::vector<size_t> v(2*I, col);

  for (ptrdiff_t i = 0; i < I; i++)
    v[2*i] = rows(i);

  return h5gt::ElementSet(v);
}

template<typename D>
inline h5gt::ElementSet rowsCols2ElementSet(
    const Eigen::DenseBase<D>& rows,
    const Eigen::DenseBase<D>& cols)
{
  ptrdiff_t I = rows.size();
  ptrdiff_t J = cols.size();

  std::vector<size_t> v(2*I*J);

  for (ptrdiff_t i = 0; i < I; i++){
    for (ptrdiff_t j = 0; j < J; j++){
      v[2*j + 2*i*J] = rows(i);
      v[2*j+1 + 2*i*J] = cols(j);
    }
  }

  return h5gt::ElementSet(v);
}

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowCols2ElementSet(
    const T& row,
    const std::vector<T>& cols)
{
  size_t I = cols.size();
  std::vector<size_t> v(2*I, row);

  for (size_t i = 0; i < I; i++)
    v[2*i+1] = cols[i];

  return h5gt::ElementSet(v);
}

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowsCol2ElementSet(
    const std::vector<T>& rows,
    const T& col)
{
  size_t I = rows.size();
  std::vector<size_t> v(2*I, col);

  for (size_t i = 0; i < I; i++)
    v[2*i] = rows[i];

  return h5gt::ElementSet(v);
}

template<typename T,
         typename std::enable_if<
           std::is_arithmetic<T>::value>::type*>
inline h5gt::ElementSet rowsCols2ElementSet(
    const std::vector<T>& rows,
    const std::vector<T>& cols)
{
  size_t I = rows.size();
  size_t J = cols.size();

  std::vector<size_t> v(2*I*J);

  for (size_t i = 0; i < I; i++){
    for (size_t j = 0; j < J; j++){
      v[2*j + 2*i*J] = rows[i];
      v[2*j+1 + 2*i*J] = cols[j];
    }
  }

  return h5gt::ElementSet(v);
}

} // namespace h5geo

#endif
