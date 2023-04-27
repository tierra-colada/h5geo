#include "../../include/h5geo/h5core.h"
#include "../../include/h5geo/private/h5seisimpl.h"

#define _USE_MATH_DEFINES   // should be before <cmath>, include 'pi' val

#include <math.h>
#include <algorithm>
#include <optional>
#include <filesystem>
namespace fs = std::filesystem;

#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Attribute.hpp>


namespace h5geo {

std::optional<h5gt::File> openFile(
    const std::string& fileName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return std::nullopt;

  try {
    return h5gt::File(
          fileName,
          h5gt::File::ReadWrite);
  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

std::optional<h5gt::Group> openGroup(
    const std::string& fileName,
    const std::string& groupName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return std::nullopt;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(groupName, h5gt::ObjectType::Group))
      return std::nullopt;

    return h5File.getGroup(groupName);
  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

std::optional<h5gt::DataSet> openDataSet(
    const std::string& fileName,
    const std::string& dsetName)
{
  if (!fs::exists(fileName) || H5Fis_hdf5(fileName.c_str()) < 1)
    return std::nullopt;

  try {
    h5gt::File h5File(
          fileName,
          h5gt::File::ReadWrite);
    if (!h5File.hasObject(dsetName, h5gt::ObjectType::Dataset))
      return std::nullopt;

    return h5File.getDataSet(dsetName);
  } catch (h5gt::Exception& err) {
    return std::nullopt;
  }
}

// strings are intensionally TAB delimited
std::vector<std::string> getRawBinHeaderNames()
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
std::vector<std::string> getRawTraceHeaderNames()
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
    {"+ Number	INLINE	189	4"},
    {"+ Clossline Number	XLINE	193	4"},
    {"+ Shot Point3 Number	SPN	197	4"},
    {"+ Shot Point3 Scalar	SPS	201	2"},
    {"+ Trace value measurement unit	TVMU	203	2"}
  };
}

void calcGrid3D(
    const Eigen::Ref<Eigen::VectorXd>& x,
    const Eigen::Ref<Eigen::VectorXd>& y,
    double z,
    Eigen::VectorXd& x_loc,
    Eigen::VectorXd& y_loc,
    Eigen::VectorXd& z_loc)
{
  if (x.size() < 1 || y.size() < 1)
    return;

  ptrdiff_t nx = x.size();
  ptrdiff_t ny = y.size();

  x_loc = Eigen::VectorXd::Zero(nx*ny);
  y_loc = Eigen::VectorXd::Zero(nx*ny);
  z_loc = Eigen::VectorXd::Zero(nx*ny);

  ptrdiff_t idx = 0;
  for (ptrdiff_t k = 0; k < ny; k++){
    for (ptrdiff_t j = 0; j < nx; j++){
      x_loc(idx) = x(j);
      y_loc(idx) = y(k);
      z_loc(idx) = z;
      idx++;
    }
  }
}

bool generatePRESTKGeometry(
      H5Seis* seis,
      double src_x0, double src_dx, size_t src_nx,
      double src_y0, double src_dy, size_t src_ny,
      double src_z,
      double rec_x0, double rec_dx, size_t rec_nx,
      double rec_y0, double rec_dy, size_t rec_ny,
      double rec_z,
      double orientation,
      bool moveRec)
{
  if (!seis)
    return false;

  Eigen::VectorXd src_x = Eigen::VectorXd::LinSpaced(src_nx, src_x0, src_x0+src_dx*(src_nx-1));
  Eigen::VectorXd src_y = Eigen::VectorXd::LinSpaced(src_ny, src_y0, src_y0+src_dy*(src_ny-1));
  Eigen::VectorXd src_xloc, src_yloc, src_zloc;
  h5geo::calcGrid3D(src_x, src_y, src_z, src_xloc, src_yloc, src_zloc);
  if (src_xloc.size() < 1 || src_yloc.size() < 1 || src_zloc.size() < 1)
    return false;

  Eigen::VectorXd rec_x = Eigen::VectorXd::LinSpaced(rec_nx, rec_x0, rec_x0+rec_dx*(rec_nx-1));
  Eigen::VectorXd rec_y = Eigen::VectorXd::LinSpaced(rec_ny, rec_y0, rec_y0+rec_dy*(rec_ny-1));
  Eigen::VectorXd rec_xloc, rec_yloc, rec_zloc;
  h5geo::calcGrid3D(rec_x, rec_y, rec_z, rec_xloc, rec_yloc, rec_zloc);
  if (rec_xloc.size() < 1 || rec_yloc.size() < 1 || rec_zloc.size() < 1)
    return false;

  size_t nShot = src_nx*src_ny;
  size_t nRec = rec_nx*rec_ny;
  size_t nTrc = nShot*nRec;
  if (seis->getNTrc() != nTrc)
    if (!seis->setNTrc(nTrc))
      return false;

  double src_xmin = src_x.minCoeff();
  double src_ymin = src_y.minCoeff();

  double rec_xmin = rec_x.minCoeff();
  double rec_ymin = rec_y.minCoeff();
  if (moveRec){
    rec_xmin += src_xmin - src_x0;
    rec_ymin += src_ymin - src_y0;
  }

  Eigen::VectorXd lind = Eigen::VectorXd::LinSpaced(nRec, (double)1, (double)nRec);
  Eigen::VectorXd ones_arr = Eigen::VectorXd::Ones(nRec);

  rec_x = rec_xloc;
  rec_y = rec_yloc;
  for (size_t i = 0; i < nShot; i++){
    size_t fromTrc = i*nRec;
    lind.array() += (double)nRec*bool(i);
    src_x = ones_arr*src_xloc(i);
    src_y = ones_arr*src_yloc(i);
    if (moveRec){
      double dx = src_xloc(i) - src_x0;
      double dy = src_yloc(i) - src_y0;
      rec_x = rec_xloc.array() + dx;
      rec_y = rec_yloc.array() + dy;
    }
    Eigen::VectorXd cdp_x = (src_x+rec_x)/2;
    Eigen::VectorXd cdp_y = (src_y+rec_y)/2;
    Eigen::VectorXd il, xl;
    if (moveRec){
      if (rec_dy == 0 && src_dy == 0)
        il = ones_arr;
      else if (rec_dy == 0)
        il = ((rec_y.array()-rec_ymin)/abs(src_dy)).rint() + 1;
      else
        il = ((rec_y.array()-rec_ymin)/abs(rec_dy)).rint() + 1;

      if (rec_dx == 0 && src_dx == 0)
        xl = ones_arr;
      else if (rec_dx == 0)
        xl = ((rec_x.array()-rec_xmin)/abs(src_dx)).rint() + 1;
      else
        xl = ((rec_x.array()-rec_xmin)/abs(rec_dx)).rint() + 1;
    } else {
      if (rec_dy == 0)
        il = ones_arr;
      else
        il = ((rec_y.array()-rec_ymin)/abs(rec_dy)).rint() + 1;

      if (rec_dx == 0)
        xl = ones_arr;
      else
        xl = ((rec_x.array()-rec_xmin)/abs(rec_dx)).rint() + 1;
    }

    Eigen::VectorXd dx = rec_x-src_x;
    Eigen::VectorXd dy = rec_y-src_y;
    Eigen::VectorXd offset = (dx.array().square() + dy.array().square()).array().sqrt();
    Eigen::VectorXd sp = ones_arr.array()+i;
    Eigen::VectorXd ffid = sp;
    Eigen::VectorXd src_x = ones_arr*src_xloc(i);
    Eigen::VectorXd src_y = ones_arr*src_yloc(i);
    Eigen::VectorXd ses = ones_arr*src_zloc(i);

    // rotate coordinates
    double x0 = std::min(src_xmin,rec_xmin);
    double y0 = std::min(src_ymin,rec_ymin);
    src_x = ((src_x.array()-x0)*cos(orientation)-
             (src_y.array()-y0)*sin(orientation)).array()+x0;
    src_y = ((src_y.array()-y0)*cos(orientation)+
             (src_x.array()-x0)*sin(orientation)).array()+y0;
    rec_x = ((rec_x.array()-x0)*cos(orientation)-
             (rec_y.array()-y0)*sin(orientation)).array()+x0;
    rec_y = ((rec_y.array()-y0)*cos(orientation)+
             (rec_x.array()-x0)*sin(orientation)).array()+y0;

    seis->writeTraceHeader("SEQWL", lind, fromTrc);
    seis->writeTraceHeader("SEQWR", lind, fromTrc);
    seis->writeTraceHeader("TRCFLD", lind, fromTrc);
    seis->writeTraceHeader("TRCID", ones_arr, fromTrc);
    seis->writeTraceHeader("DU", ones_arr, fromTrc);
    seis->writeTraceHeader("SAED", ones_arr, fromTrc);
    seis->writeTraceHeader("SAC", ones_arr, fromTrc);
    seis->writeTraceHeader("SP", sp, fromTrc);
    seis->writeTraceHeader("FFID", ffid, fromTrc);
    seis->writeTraceHeader("SRCX", src_x, fromTrc);
    seis->writeTraceHeader("SRCY", src_y, fromTrc);
    seis->writeTraceHeader("GRPX", rec_x, fromTrc);
    seis->writeTraceHeader("GRPY", rec_y, fromTrc);
    seis->writeTraceHeader("RGE", rec_zloc, fromTrc);
    seis->writeTraceHeader("SES", ses, fromTrc);
    seis->writeTraceHeader("CDP_X", cdp_x, fromTrc);
    seis->writeTraceHeader("CDP_Y", cdp_y, fromTrc);
    seis->writeTraceHeader("INLINE", il, fromTrc);
    seis->writeTraceHeader("XLINE", xl, fromTrc);
    seis->writeTraceHeader("DSREG", offset, fromTrc);
  }

  // update cdp numbers
  Eigen::MatrixXd cdp_yx = seis->getXYTraceHeaders({"CDP_Y", "CDP_X"});
  Eigen::VectorX<ptrdiff_t> cdp_yx_ind = h5geo::sort_rows(cdp_yx);
  Eigen::VectorXd cdp = Eigen::VectorXd::LinSpaced(cdp_yx_ind.size(), (double)1, (double)cdp_yx_ind.size())(cdp_yx_ind);
  seis->writeTraceHeader("CDP", cdp);
  seis->setDataType(h5geo::SeisDataType::PRESTACK);
  return true;
}

std::map<std::string, Eigen::VectorXd> 
    generateSTKGeometry(
        double x0, double dx, size_t nx,
        double y0, double dy, size_t ny,
        double z,
        double orientation)
{
  Eigen::VectorXd x = Eigen::VectorXd::LinSpaced(nx, x0, x0+dx*(nx-1));
  Eigen::VectorXd y = Eigen::VectorXd::LinSpaced(ny, y0, y0+dy*(ny-1));
  Eigen::VectorXd xloc, yloc, zloc;
  h5geo::calcGrid3D(x, y, z, xloc, yloc, zloc);
  if (xloc.size() < 1 || yloc.size() < 1 || zloc.size() < 1)
    return std::map<std::string, Eigen::VectorXd>();

  Eigen::VectorXd xl = Eigen::VectorXd::LinSpaced(nx, (double)1, (double)nx);
  Eigen::VectorXd il = Eigen::VectorXd::LinSpaced(ny, (double)1, (double)ny);
  Eigen::VectorXd xlloc, illoc, notUsed;
  h5geo::calcGrid3D(xl, il, 0, xlloc, illoc, notUsed);
  if (xlloc.size() < 1 || illoc.size() < 1)
    return std::map<std::string, Eigen::VectorXd>();

  size_t nTrc = xloc.size();

  Eigen::VectorXd lind = Eigen::VectorXd::LinSpaced(nTrc, (double)1, (double)nTrc);
  Eigen::VectorXd ones_arr = Eigen::VectorXd::Ones(nTrc);

  // rotate coordinates
  xloc = ((xloc.array()-x0)*cos(orientation)-
            (yloc.array()-y0)*sin(orientation)).array()+x0;
  yloc = ((yloc.array()-y0)*cos(orientation)+
            (xloc.array()-x0)*sin(orientation)).array()+y0;

  std::map<std::string, Eigen::VectorXd> geom;
  geom["SEQWL"] = lind;
  geom["SEQWR"] = lind;
  geom["TRCFLD"] = lind;
  geom["CDP"] = lind;
  geom["TRCID"] = ones_arr;
  geom["DU"] = ones_arr;
  geom["SAED"] = ones_arr;
  geom["SAC"] = ones_arr;
  geom["RGE"] = zloc;
  geom["SES"] = zloc;
  geom["CDP_X"] = xloc;
  geom["CDP_Y"] = yloc;
  geom["INLINE"] = illoc;
  geom["XLINE"] = xlloc;
  return geom;
}

bool compareStrings(
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

void eraseSubStr(
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

void eraseAllSubStr(
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

char getDelimiter(const Delimiter& delimiter){
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

std::string generateName(
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

std::vector<std::string> splitString(const std::string &s, const std::string delimiter) {
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

void splitHeaderNames(const std::vector<std::string> &headerNamesToSplit,
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

void splitHeaderBytes(
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

std::vector<std::string> splitPath(
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

std::vector<std::string> splitPath(
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

std::string splitPathToParentAndObj(
    const std::string& path, std::string& objName)
{
  if (path.empty())
    return std::string();

  std::vector<std::string> pathVec = splitPath(path);
  if(pathVec.empty()){
    objName.clear();
    return std::string();
  }

  std::string referencePath;
  if (path[0] == '/')
    referencePath = '/';

  for (size_t i = 0; i < pathVec.size()-1; i++){
    if (i < pathVec.size()-2)
      referencePath += pathVec[i] + '/';
    else
      referencePath += pathVec[i];
  }

  objName = pathVec.back();
  return referencePath;
}

std::string getRelativePath(
    const std::string& referencePath,
    const std::string& objPath,
    const CaseSensitivity& caseSensitivity)
{
  if (referencePath.empty())
    return objPath;

  if (objPath.empty())
    return std::string();

  if (referencePath.size() >= objPath.size())
    return std::string();

  bool val = compareStrings(objPath, referencePath, caseSensitivity);

  if (!val)
    return std::string();

  if (objPath[referencePath.size()] == '/' &&
      objPath.size() > referencePath.size()+1){
    return objPath.substr(referencePath.size()+1, objPath.size()-referencePath.size()-1);
  } else {
    return objPath.substr(referencePath.size(), objPath.size()-referencePath.size());
  }
}

void getTraceHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> traceHeaderNames = getRawTraceHeaderNames();

  splitHeaderNames(traceHeaderNames, fullHeaderNames, shortHeaderNames);
}

std::vector<std::string> getTraceHeaderShortNames(){
  std::vector<std::string> fullHeaderNames;
  std::vector<std::string> shortHeaderNames;
  getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames;
}

std::vector<std::string> getTraceHeaderFullNames(){
  std::vector<std::string> fullHeaderNames;
  std::vector<std::string> shortHeaderNames;
  getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  return fullHeaderNames;
}

void getBinHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> binHeaderNames = getRawBinHeaderNames();

  splitHeaderNames(binHeaderNames, fullHeaderNames, shortHeaderNames);
}

std::vector<std::string> getBinHeaderShortNames(){
  std::vector<std::string> fullHeaderNames;
  std::vector<std::string> shortHeaderNames;
  getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames;
}

std::vector<std::string> getBinHeaderFullNames(){
  std::vector<std::string> fullHeaderNames;
  std::vector<std::string> shortHeaderNames;
  getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  return fullHeaderNames;
}

void getTraceHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes){
  std::vector<std::string> traceHeaderNames = getRawTraceHeaderNames();

  splitHeaderBytes(traceHeaderNames, bytesStart, nBytes);
}

void getBinHeaderBytes(
    std::vector<int> &bytesStart,
    std::vector<int> &nBytes){
  std::vector<std::string> binHeaderNames = getRawBinHeaderNames();

  splitHeaderBytes(binHeaderNames, bytesStart, nBytes);
}

size_t getTraceHeaderCount(){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames.size();
}

size_t getBinHeaderCount(){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames.size();
}

ptrdiff_t getIndexFromAttribute(
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

template <typename Scalar>
bool _getSurveyInfoFromSortedData(
    const Eigen::Ref<const Eigen::VectorX<Scalar>>& il,
    const Eigen::Ref<const Eigen::VectorX<Scalar>>& xl,
    const Eigen::Ref<const Eigen::VectorX<Scalar>>& x,
    const Eigen::Ref<const Eigen::VectorX<Scalar>>& y,
    double &origin_x,
    double &origin_y,
    double &orientation,
    double &ilSpacing,
    double &xlSpacing,
    bool &isILReversed,
    bool &isXLReversed,
    bool &isPlanReversed)
{
  if (il.size() < 2 ||
      il.size() != xl.size() ||
      xl.size() != x.size() ||
      x.size() != y.size() ||
      y.size() != il.size())
    return false;

  // IL is sorted
  size_t nxl = 0;
  for (ptrdiff_t i = 0; i < il.size(); i++){
    nxl += 1;
    if (il(i) != il(i+1))
      break;
  }

  // nTrc must be equal to nxl*nil
  auto dv = std::div(il.size(), nxl);
  if (dv.rem != 0)
    return false;

  size_t nil = dv.quot;
  ptrdiff_t last_ind = nil*nxl-1;

  Eigen::Vector2d o_XL_IL, o_XLr_IL, o_XL_ILr, o_XLr_ILr;
  o_XL_IL(0) = x(0);
  o_XL_IL(1) = y(0);
  o_XLr_IL(0) = x(nxl-1);
  o_XLr_IL(1) = y(nxl-1);
  o_XL_ILr(0) = x(last_ind-(nxl-1));
  o_XL_ILr(1) = y(last_ind-(nxl-1));
  o_XLr_ILr(0) = x(last_ind);
  o_XLr_ILr(1) = y(last_ind);

  // if nxl == 1 then p1 == origin
  Eigen::Vector2d p1_XL_IL(o_XL_IL), p1_XLr_IL(o_XLr_IL), p1_XL_ILr(o_XL_ILr), p1_XLr_ILr(o_XLr_ILr);
  if (nxl > 1){
    p1_XL_IL(0) = x(nxl-1);
    p1_XL_IL(1) = y(nxl-1);
    p1_XLr_IL(0) = x(0);
    p1_XLr_IL(1) = y(0);
    p1_XL_ILr(0) = x(last_ind);
    p1_XL_ILr(1) = y(last_ind);
    p1_XLr_ILr(0) = x(last_ind-(nxl-1));
    p1_XLr_ILr(1) = y(last_ind-(nxl-1));
  }

  // if nil == 1 then p2 == origin
  Eigen::Vector2d p2_XL_IL(o_XL_IL), p2_XLr_IL(o_XLr_IL), p2_XL_ILr(o_XL_ILr), p2_XLr_ILr(o_XLr_ILr);
  if (nil > 1){
    p2_XL_IL(0) = x(last_ind-(nxl-1));
    p2_XL_IL(1) = y(last_ind-(nxl-1));
    p2_XLr_IL(0) = x(last_ind);
    p2_XLr_IL(1) = y(last_ind);
    p2_XL_ILr(0) = x(0);
    p2_XL_ILr(1) = y(0);
    p2_XLr_ILr(0) = x(nxl-1);
    p2_XLr_ILr(1) = y(nxl-1);
  }

  // angle must be in range [-180,180] degrees (don't make it [0,360] - it will require to reorder octants)
  auto getOctantFromOrientation = [](double a)->int{
    if (a >= -180 && a < -90)
      return 3;
    if (a >= -90 && a < 0)
      return 4;
    if (a >= 0 && a < 90)
      return 1;
    if (a >= 90 && a < 180)
      return 2;
    return -1;
  };

  // orientation to p1 and p2 respectively in range [-180,180] degrees (don't make it [0,360] - it will require to reorder octants)
  double orientation_XL_IL_p1 = 180*std::atan2(p1_XL_IL(1)-o_XL_IL(1), p1_XL_IL(0)-o_XL_IL(0))/M_PI;
  double orientation_XL_IL_p2 = 180*std::atan2(p2_XL_IL(1)-o_XL_IL(1), p2_XL_IL(0)-o_XL_IL(0))/M_PI;

  // to correctly define plan reversed and orientation we need to make 3D plan even if it is 2D
  if (nxl == 1){
    orientation_XL_IL_p1 = orientation_XL_IL_p2 + 90;
    if (orientation_XL_IL_p1 > 180)
      orientation_XL_IL_p1 -= 360;
  } else if (nil == 1){
    orientation_XL_IL_p2 = orientation_XL_IL_p1 + 90;
    if (orientation_XL_IL_p2 > 180)
      orientation_XL_IL_p2 -= 360;
  }

  int octant_XL_IL_p1 = getOctantFromOrientation(orientation_XL_IL_p1);
  int octant_XL_IL_p2 = getOctantFromOrientation(orientation_XL_IL_p2);

  // Plan reversed when p1 is ahead of p2
  isPlanReversed = false;
  if (octant_XL_IL_p1 >= octant_XL_IL_p2 &&
      orientation_XL_IL_p2 - orientation_XL_IL_p1 < 0 &&
      nil > 1 && nxl > 1)
    isPlanReversed = true;

  // XL reversed when XL_min corresponds to X_max
  if (isPlanReversed && o_XL_IL(1) - o_XLr_IL(1) > 0){
    isXLReversed = true;
  } else if (!isPlanReversed && o_XL_IL(0) - o_XLr_IL(0) > 0) {
    isXLReversed = true;
  } else {
    isXLReversed = false;
  }

  // IL reversed when IL_min corresponds to Y_max
  if (isPlanReversed && o_XL_IL(0) - o_XL_ILr(0) > 0){
    isILReversed = true;
  } else if (!isPlanReversed && o_XL_IL(1) - o_XL_ILr(1) > 0) {
    isILReversed = true;
  } else {
    isILReversed = false;
  }

  // p1/p2 - corner points (see picture in header file)
  Eigen::Vector2d p1, p2;
  if (!isXLReversed && !isILReversed){
    origin_x = o_XL_IL(0);
    origin_y = o_XL_IL(1);
    p1 = p1_XL_IL;
    p2 = p2_XL_IL;
  } else if (isXLReversed && !isILReversed){
    origin_x = o_XLr_IL(0);
    origin_y = o_XLr_IL(1);
    p1 = p1_XLr_IL;
    p2 = p2_XLr_IL;
  } else if (!isXLReversed && isILReversed){
    origin_x = o_XL_ILr(0);
    origin_y = o_XL_ILr(1);
    p1 = p1_XL_ILr;
    p2 = p2_XL_ILr;
  } else if (isXLReversed && isILReversed){
    origin_x = o_XLr_ILr(0);
    origin_y = o_XLr_ILr(1);
    p1 = p1_XLr_ILr;
    p2 = p2_XLr_ILr;
  }

  double p1_dx = p1(0)-origin_x;
  double p1_dy = p1(1)-origin_y;
  double p2_dx = p2(0)-origin_x;
  double p2_dy = p2(1)-origin_y;

  // hypotenuse
  ilSpacing = std::hypot(p1_dx, p1_dy);
  xlSpacing = std::hypot(p2_dx, p2_dy);
  if (nxl > 1)
    ilSpacing /= double(nxl-1);
  if (nil > 1)
    xlSpacing /= double(nil-1);

  // this is necessary as spacing can't be 0
  if (ilSpacing == 0)
    ilSpacing = 1;
  if (xlSpacing == 0)
    xlSpacing = 1;

  // orientation to p1 and p2 respectively
  double orientation1 = 180*std::atan2(p1_dy, p1_dx)/M_PI;
  double orientation2 = 180*std::atan2(p2_dy, p2_dx)/M_PI;

  if (isPlanReversed){
    orientation = orientation2;
  } else {
    orientation = orientation1;
  }

  return true;
}

template <typename Scalar>
bool _getSurveyInfoFromUnsortedData(
    Eigen::Ref<Eigen::MatrixX<Scalar>> il_xl,
    Eigen::Ref<Eigen::VectorX<Scalar>> x,
    Eigen::Ref<Eigen::VectorX<Scalar>> y,
    double &origin_x,
    double &origin_y,
    double &orientation,
    double &ilSpacing,
    double &xlSpacing,
    bool &isILReversed,
    bool &isXLReversed,
    bool &isPlanReversed)
{
  if (il_xl.size() < 2 ||
      il_xl.rows() != x.size() ||
      x.size() != y.size() ||
      y.size() != il_xl.rows())
    return false;

  Eigen::MatrixX2<ptrdiff_t> uil_from_size, uxl_from_size;
  Eigen::VectorXd uil, uxl;

  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows(il_xl);
  auto uil_ind = h5geo::sort_unique(il_xl.col(0), uil, uil_from_size);
  auto uxl_ind = h5geo::sort_unique(il_xl.col(1), uxl, uxl_from_size);
  if (ind.size() != il_xl.rows())
    return false;

  il_xl = il_xl(ind, Eigen::all).eval();
  x = x(ind).eval();
  y = y(ind).eval();

  return _getSurveyInfoFromSortedData<Scalar>(
        il_xl.col(0),
        il_xl.col(1),
        x,
        y,
        origin_x,
        origin_y,
        orientation,
        ilSpacing,
        xlSpacing,
        isILReversed,
        isXLReversed,
        isPlanReversed);
}

bool getSurveyInfoFromSortedData(
    const Eigen::Ref<const Eigen::VectorXf>& il,
    const Eigen::Ref<const Eigen::VectorXf>& xl,
    const Eigen::Ref<const Eigen::VectorXf>& x,
    const Eigen::Ref<const Eigen::VectorXf>& y,
    double &origin_x,
    double &origin_y,
    double &orientation,
    double &ilSpacing,
    double &xlSpacing,
    bool &isILReversed,
    bool &isXLReversed,
    bool &isPlanReversed)
{
  return _getSurveyInfoFromSortedData(
        il,
        xl,
        x,
        y,
        origin_x,
        origin_y,
        orientation,
        ilSpacing,
        xlSpacing,
        isILReversed,
        isXLReversed,
        isPlanReversed);
}

bool getSurveyInfoFromSortedData(
    const Eigen::Ref<const Eigen::VectorXd>& il,
    const Eigen::Ref<const Eigen::VectorXd>& xl,
    const Eigen::Ref<const Eigen::VectorXd>& x,
    const Eigen::Ref<const Eigen::VectorXd>& y,
    double &origin_x,
    double &origin_y,
    double &orientation,
    double &ilSpacing,
    double &xlSpacing,
    bool &isILReversed,
    bool &isXLReversed,
    bool &isPlanReversed)
{
  return _getSurveyInfoFromSortedData(
        il,
        xl,
        x,
        y,
        origin_x,
        origin_y,
        orientation,
        ilSpacing,
        xlSpacing,
        isILReversed,
        isXLReversed,
        isPlanReversed);
}

bool getSurveyInfoFromUnsortedData(
    Eigen::Ref<Eigen::MatrixXf> il_xl,
    Eigen::Ref<Eigen::VectorXf> x,
    Eigen::Ref<Eigen::VectorXf> y,
    double &origin_x,
    double &origin_y,
    double &orientation,
    double &ilSpacing,
    double &xlSpacing,
    bool &isILReversed,
    bool &isXLReversed,
    bool &isPlanReversed)
{
  return _getSurveyInfoFromUnsortedData(
        il_xl,
        x,
        y,
        origin_x,
        origin_y,
        orientation,
        ilSpacing,
        xlSpacing,
        isILReversed,
        isXLReversed,
        isPlanReversed);
}

bool getSurveyInfoFromUnsortedData(
    Eigen::Ref<Eigen::MatrixXd> il_xl,
    Eigen::Ref<Eigen::VectorXd> x,
    Eigen::Ref<Eigen::VectorXd> y,
    double &origin_x,
    double &origin_y,
    double &orientation,
    double &ilSpacing,
    double &xlSpacing,
    bool &isILReversed,
    bool &isXLReversed,
    bool &isPlanReversed)
{
  return _getSurveyInfoFromUnsortedData(
        il_xl,
        x,
        y,
        origin_x,
        origin_y,
        orientation,
        ilSpacing,
        xlSpacing,
        isILReversed,
        isXLReversed,
        isPlanReversed);
}

template <typename Scalar>
bool _isStraightLine(
    const Eigen::Ref<const Eigen::VectorX<Scalar>>& x,
    const Eigen::Ref<const Eigen::VectorX<Scalar>>& y,
    Scalar eps)
{
  if (x.size() < 3 ||
      y.size() < 3 ||
      x.size() != y.size())
    return true;

  Scalar dx = x(0) - x(1);
  Scalar dy = y(0) - y(1);
  ptrdiff_t npts = x.size();
  for (ptrdiff_t i = 2; i < npts; i++) {
    Scalar dx1 = x(0) - x(i);
    Scalar dy1 = y(0) - y(i);
    if (std::abs<Scalar>(dx1*dy - dy1*dx) > eps)
      return false;
  }

  return true;
}

bool isStraightLine(
    const Eigen::Ref<const Eigen::VectorXf>& x,
    const Eigen::Ref<const Eigen::VectorXf>& y,
    float eps)
{
  return _isStraightLine(x,y,eps);
}

bool isStraightLine(
    const Eigen::Ref<const Eigen::VectorXd>& x,
    const Eigen::Ref<const Eigen::VectorXd>& y,
    double eps)
{
  return _isStraightLine(x,y,eps);
}


} // h5geo
