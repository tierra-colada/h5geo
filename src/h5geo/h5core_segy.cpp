#include "../../include/h5geo/h5core.h"

#include <vector>
#include <filesystem>
#ifdef H5GEO_USE_THREADS
#include <omp.h>
#endif

#include <Eigen/Dense>

// 'windows.h' contains min/max macros and thus std::min causes build fail
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <mio/mmap.hpp>

// enum string is needed to include magic_enum with predefined macro
#include "../../include/h5geo/private/h5enum_string.h"
#include "../../include/h5geo/h5seis.h"
#include "../../include/h5geo/h5vol.h"

namespace h5geo {

struct BinHeader {
  int b0[3] = { 0 };
  short b1[24] = { 0 };
  short gap0[120] = { 0 };
  short b2[3] = { 0 };
  short gap1[46] = { 0 };
};

struct TraceHeader {
  int b0[7] = { 0 };
  short b1[4] = { 0 };
  int b2[8] = { 0 };
  short b3[2] = { 0 };
  int b4[4] = { 0 };
  short b5[46] = { 0 };
  int b6[5] = { 0 };
  short b7[2] = { 0 };
  short gap0[18] = { 0 };
};

bool isSEGY(const std::string& segy){
  try {
    auto segySize = std::filesystem::file_size(segy);
    if (segySize < 3600 + 240)
      return false;
  } catch(std::filesystem::filesystem_error& e) {
    return false;
  }

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return false;

  return true;
}

TextEncoding getSEGYTextEncoding(const std::string& segy){
  if (!isSEGY(segy))
    return static_cast<h5geo::TextEncoding>(0);

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return static_cast<h5geo::TextEncoding>(0);

  h5geo::TextEncoding encoding = static_cast<h5geo::TextEncoding>(0);
  char txtHdr[40][80];
  file.read(*txtHdr, 3200);

  int neb = 0;
  int nas = 0;
  for (int i = 0; i < 40; i++) {
    for (int j = 0; j < 80; j++) {
      if (txtHdr[i][j] == ' ')
        nas++;

      if (txtHdr[i][j] == 0x40)
        neb++;
    }
  }

  if (neb > nas)
    encoding = h5geo::TextEncoding::EBCDIC;
  else
    encoding = h5geo::TextEncoding::ASCII;

  return encoding;
}

Endian getSEGYEndian(const std::string& segy){
  if (!isSEGY(segy))
    return static_cast<h5geo::Endian>(0);

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return static_cast<h5geo::Endian>(0);

  h5geo::Endian endian = static_cast<h5geo::Endian>(0);
  short dataFormatCode, dataFormatCodeSE;
  file.seekg(3224);
  file.read(bit_cast<char *>(&dataFormatCode), 2);
  dataFormatCodeSE = bswap(dataFormatCode);
  if (O32_HOST_ORDER == O32_LITTLE_ENDIAN){
    if (dataFormatCode > 0 && dataFormatCode <= 8) {
      endian = h5geo::Endian::Little;
    } else if (dataFormatCodeSE > 0 && dataFormatCodeSE <= 8) {
      endian = h5geo::Endian::Big;
    }
  } else if (O32_HOST_ORDER == O32_BIG_ENDIAN){
    if (dataFormatCode > 0 && dataFormatCode <= 8) {
      endian = h5geo::Endian::Big;
    } else if (dataFormatCodeSE > 0 && dataFormatCodeSE <= 8) {
      endian = h5geo::Endian::Little;
    }
  }
  return endian;
}

SegyFormat getSEGYFormat(const std::string& segy, h5geo::Endian endian)
{
  if (!isSEGY(segy))
    return static_cast<h5geo::SegyFormat>(0);

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return static_cast<h5geo::SegyFormat>(0);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  short dataFormatCode;
  file.seekg(3224);
  file.read(bit_cast<char *>(&dataFormatCode), 2);
  dataFormatCode = to_native_endian(dataFormatCode, endian);

  h5geo::SegyFormat format = static_cast<h5geo::SegyFormat>(0);
  if (dataFormatCode == 1) {
    format = h5geo::SegyFormat::FourByte_IBM;
  } else if (dataFormatCode == 2) {
    format = h5geo::SegyFormat::FourByte_integer;
  } else if (dataFormatCode == 5) {
    format = h5geo::SegyFormat::FourByte_IEEE;
  }
  return format;
}

bool readSEGYTextHeader(
    const std::string& segy,
    char txtHdr[40][80], h5geo::TextEncoding encoding)
{
  if (!isSEGY(segy))
    return false;

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return false;

  if (std::string{magic_enum::enum_name(encoding)}.empty())
    encoding = getSEGYTextEncoding(segy);

  file.read(*txtHdr, 3200);
  if (encoding == h5geo::TextEncoding::EBCDIC) {
    for (int i = 0; i < 40; i++) {
      for (int j = 0; j < 80; j++) {
        txtHdr[i][j] = ebc_to_ascii_table(txtHdr[i][j]);
      }
    }
  }

  return true;
}

bool writeSEGYTextHeader(
    const std::string& segy,
    char txtHdr[40][80], bool truncate)
{
  // open file and overwrite
  int openFlag = std::ios::out | std::ios::binary;
  // but if this is a SEGY file then don't overwrite it
  // the file won't be created if `std::ios::in` is set
  if (isSEGY(segy))
    openFlag |= std::ios::in;
  
  if (truncate)
    openFlag |= std::ios::trunc;

  std::ofstream file(segy, std::ios_base::openmode(openFlag));
  if (!file.is_open())
    return false;

  file.write(bit_cast<char *>(&txtHdr[0][0]), 3200);
  return true;
}

bool readSEGYBinHeader(
    const std::string& segy,
    double binHdr[30], h5geo::Endian endian)
{
  if (!isSEGY(segy))
    return false;

  std::ifstream file(segy, std::ios::in | std::ios::binary);
  if (!file.is_open())
    return false;

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  // int binHdr4[3];
  // short binHdr2[27], binHdr2tmp[3];

  file.seekg(3200);
  BinHeader hdr;
  file.read(bit_cast<char *>(&hdr), sizeof(hdr));
  // file.read(bit_cast<char *>(&binHdr4), 12);
  // file.read(bit_cast<char *>(&binHdr2), 48);

  // file.seekg(3500);
  // file.read(bit_cast<char *>(&binHdr2tmp), 6);

  // binHdr2[24] = binHdr2tmp[0];
  // binHdr2[25] = binHdr2tmp[1];
  // binHdr2[26] = binHdr2tmp[2];

  to_native_endian<int>(std::begin(hdr.b0), std::end(hdr.b0), std::begin(hdr.b0), endian);
  to_native_endian<short>(std::begin(hdr.b1), std::end(hdr.b1), std::begin(hdr.b1), endian);
  to_native_endian<int>(std::begin(hdr.b2), std::end(hdr.b2), std::begin(hdr.b2), endian);
  // to_native_endian<int>(std::begin(binHdr4), std::end(binHdr4), std::begin(binHdr4), endian);
  // to_native_endian<short>(std::begin(binHdr2), std::end(binHdr2), std::begin(binHdr2), endian);

  int ii = 0;
  for (int i = 0; i < (sizeof(hdr.b0)/sizeof(*hdr.b0)); i++) {
    binHdr[ii] = hdr.b0[i];
    ii++;
  }
  for (int i = 0; i < (sizeof(hdr.b1)/sizeof(*hdr.b1)); i++) {
    binHdr[ii] = hdr.b1[i];
    ii++;
  }
  for (int i = 0; i < (sizeof(hdr.b2)/sizeof(*hdr.b2)); i++) {
    binHdr[ii] = hdr.b2[i];
    ii++;
  }

  // for (int i = 0; i < 3; i++) {
  //   binHdr[i] = binHdr4[i];
  // }

  // for (int i = 3; i < 30; i++) {
  //   binHdr[i] = binHdr2[i - 3];
  // }

  return true;
}

bool writeSEGYBinHeader(
    const std::string& segy,
    double binHdr[30], bool truncate)
{
    // open file and overwrite
  int openFlag = std::ios::out | std::ios::binary;
  // but if this is a SEGY file then don't overwrite it
  // the file won't be created if `std::ios::in` is set
  if (isSEGY(segy))
    openFlag |= std::ios::in;

  if (truncate)
    openFlag |= std::ios::trunc;

  std::ofstream file(segy, std::ios_base::openmode(openFlag));
  if (!file.is_open())
    return false;

  file.seekp(3200);
  BinHeader hdr;
  int ii = 0;
  for (int i = 0; i < (sizeof(hdr.b0)/sizeof(*hdr.b0)); i++){
    hdr.b0[i] = (int)binHdr[ii];
    ii++;
  }
  for (int i = 0; i < (sizeof(hdr.b1)/sizeof(*hdr.b1)); i++){
    hdr.b1[i] = (short)binHdr[ii];
    ii++;
  }
  for (int i = 0; i < (sizeof(hdr.b2)/sizeof(*hdr.b2)); i++){
    hdr.b2[i] = (short)binHdr[ii];
    ii++;
  }

  file.write(bit_cast<char *>(&hdr), sizeof(hdr));
  return true;
}

double getSEGYSampRate(
    const std::string& segy, h5geo::Endian endian)
{
  if (!isSEGY(segy))
    return 0;

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  double binHdr[30];
  readSEGYBinHeader(segy, binHdr, endian);

  return binHdr[5];
}

size_t getSEGYNSamp(
    const std::string& segy, h5geo::Endian endian)
{
  if (!isSEGY(segy))
    return 0;

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  double binHdr[30];
  readSEGYBinHeader(segy, binHdr, endian);

  return (size_t)binHdr[7];
}

size_t getSEGYNTrc(
    const std::string& segy, size_t nSamp, h5geo::Endian endian)
{
  if (!isSEGY(segy))
    return 0;

  size_t fsize = 0;
  try {
    fsize = std::filesystem::file_size(segy);
  } catch(std::filesystem::filesystem_error& e) {
    return 0;
  }

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1)
    return 0;

  return (fsize - 3600) / (nSamp * 4 + 240);
}

Eigen::VectorX<ptrdiff_t> readSEGYTraceHeader(
    const std::string& segy,
    const size_t& hdrOffset,
    const size_t& hdrSize,
    size_t fromTrc,
    size_t toTrc,
    size_t nSamp,
    size_t nTrc,
    h5geo::Endian endian,
    std::function<void(double)> progressCallback)
{
  if (hdrOffset+hdrSize > 240)
    return Eigen::VectorX<ptrdiff_t>();

  if (hdrSize != 1 &&
      hdrSize != 2 &&
      hdrSize != 4 &&
      hdrSize != 8)
    return Eigen::VectorX<ptrdiff_t>();

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    return Eigen::VectorX<ptrdiff_t>();

  if (nTrc < 1)
    nTrc = getSEGYNTrc(segy, nSamp, endian);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1 || nTrc < 1)
    return Eigen::VectorX<ptrdiff_t>();

  if (fromTrc >= nTrc)
    return Eigen::VectorX<ptrdiff_t>();

  if (toTrc >= nTrc)
    toTrc = nTrc-1;

  // must do the check as within OMP I cannot return 'false', only 'continue'
  if (!isSEGY(segy))
    return Eigen::VectorX<ptrdiff_t>();

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return Eigen::VectorX<ptrdiff_t>();

  size_t nTrcFact = toTrc-fromTrc+1;
  ptrdiff_t n_passed = 0;
  double progressOld = 0;
  double progressNew = 0;

  auto cbk = [&](){
    progressNew = n_passed / (double)nTrcFact;
    // update callback only if the difference >= 1% than the previous value
    if (progressNew - progressOld >= 0.01){
      progressCallback( progressNew );
      progressOld = progressNew;
    }
    n_passed++;
  };

  size_t skipBytesPerTrc = 4 * nSamp + 240 - hdrSize;
  Eigen::VectorX<ptrdiff_t> hdr(nTrcFact);
  file.seekg(3600+hdrOffset);
  for (size_t i = fromTrc; i <= toTrc; i++){
    if (progressCallback)
      cbk();
    if (i != fromTrc)
      file.seekg(skipBytesPerTrc, std::ios_base::cur);
    file.read(bit_cast<char *>(&hdr(i-fromTrc)), hdrSize);
  }

  if (hdrSize == 2){
    for (size_t i = 0; i < hdr.size(); i++)
      hdr(i) = to_native_endian<short>(hdr(i), endian);
  } else if (hdrSize == 4){
    for (size_t i = 0; i < hdr.size(); i++)
      hdr(i) = to_native_endian<int>(hdr(i), endian);
  } else if (hdrSize == 8){
    for (size_t i = 0; i < hdr.size(); i++)
      hdr(i) = to_native_endian<ptrdiff_t>(hdr(i), endian);
  }

  if (progressCallback)
    progressCallback( double(1) );

  return hdr;
}

void readSEGYTrace(
    std::ifstream& file,
    size_t trcInd,
    h5geo::SegyFormat format,
    h5geo::Endian endian,
    Eigen::Ref<Eigen::VectorXf> trace)
{
  file.seekg(3600+(240+trace.size()*4)*trcInd+240, std::ios_base::beg);
  if (format == h5geo::SegyFormat::FourByte_IBM) {
    Eigen::VectorX<int> tmp(trace.size());
    file.read(bit_cast<char *>(tmp.data()), tmp.size()*4);
    for (size_t i = 0; i < trace.size(); i++){
      trace(i) = ibm2ieee(to_native_endian(tmp(i), endian));
    }
  } else if (format == h5geo::SegyFormat::FourByte_integer) {
    Eigen::VectorX<int> tmp(trace.size());
    file.read(bit_cast<char *>(tmp.data()), tmp.size()*4);
    for (size_t i = 0; i < trace.size(); i++){
      trace(i) = (float)to_native_endian(tmp(i), endian);
    }
  } else if (format == h5geo::SegyFormat::FourByte_IEEE) {
    file.read(bit_cast<char *>(trace.data()), trace.size()*4);
    for (size_t i = 0; i < trace.size(); i++){
      trace(i) = (float)to_native_endian(trace(i), endian);
    }
  }
}

bool writeSEGYTraces(
    const std::string& segy,
    Eigen::Ref<Eigen::MatrixXd> HDR,
    Eigen::Ref<Eigen::MatrixXf> TRACE)
{
  // to open file without truncating it I have to pass both `std::ios::in | std::ios::out`
  std::ofstream file(segy, std::ios::in | std::ios::out | std::ios::binary | std::ios::ate);
  if (!file.is_open())
    return false;

  if (TRACE.cols() != HDR.cols())
    return false;

  TraceHeader hdr;
  for (size_t j = 0; j < TRACE.cols(); j++){
    int ii = 0;
    for (int i = 0; i < (sizeof(hdr.b0)/sizeof(*hdr.b0)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b0[i] = (int)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b1)/sizeof(*hdr.b1)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b1[i] = (short)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b2)/sizeof(*hdr.b2)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b2[i] = (int)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b3)/sizeof(*hdr.b3)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b3[i] = (short)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b4)/sizeof(*hdr.b4)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b4[i] = (int)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b5)/sizeof(*hdr.b5)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b5[i] = (short)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b6)/sizeof(*hdr.b6)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b6[i] = (int)HDR(ii,j);
      ii++;
    }
    for (int i = 0; i < (sizeof(hdr.b7)/sizeof(*hdr.b7)); i++){
      if (ii >= HDR.rows())
        break;
      hdr.b7[i] = (short)HDR(ii,j);
      ii++;
    }

    file.write(bit_cast<char *>(&hdr), sizeof(hdr));
    file.write(bit_cast<char *>(TRACE.col(j).data()), TRACE.rows()*4);
  }
  return true;
}

Eigen::MatrixXf readSEGYTraces(
    const std::string& segy,
    size_t fromSamp,
    size_t toSamp,
    size_t fromTrc,
    size_t toTrc,
    size_t nSamp,
    size_t nTrc,
    h5geo::SegyFormat format,
    h5geo::Endian endian,
    std::function<void(double)> progressCallback)
{
  if (std::string{magic_enum::enum_name(format)}.empty())
    format = getSEGYFormat(segy, endian);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (std::string{magic_enum::enum_name(format)}.empty() ||
      std::string{magic_enum::enum_name(endian)}.empty())
    return Eigen::MatrixXf();

  if (nTrc < 1)
    nTrc = getSEGYNTrc(segy, nSamp, endian);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1 || nTrc < 1)
    return Eigen::MatrixXf();

  if (fromSamp >= nSamp)
    return Eigen::MatrixXf();

  if (toSamp >= nSamp)
    toSamp = nSamp-1;

  if (fromTrc >= nTrc)
    return Eigen::MatrixXf();

  if (toTrc >= nTrc)
    toTrc = nTrc-1;

  // must do the check as within OMP I cannot return 'false', only 'continue'
  if (!isSEGY(segy))
    return Eigen::MatrixXf();

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return Eigen::MatrixXf();

  size_t nSampFact = toSamp-fromSamp+1;
  size_t nTrcFact = toTrc-fromTrc+1;
  ptrdiff_t n_passed = 0;
  double progressOld = 0;
  double progressNew = 0;

  auto cbk = [&](){
    progressNew = n_passed / (double)nTrcFact;
    // update callback only if the difference >= 1% than the previous value
    if (progressNew - progressOld >= 0.01){
      progressCallback( progressNew );
      progressOld = progressNew;
    }
    n_passed++;
  };

  size_t skipBytesPerTrc = 4 * nSamp + 240 - nSampFact*4;
  Eigen::MatrixXf TRACE(nSampFact, nTrcFact);
  file.seekg(3600+(240+nSamp*4)*fromTrc+240+fromSamp*4, std::ios_base::beg);
  if (format == h5geo::SegyFormat::FourByte_IBM) {
    Eigen::VectorX<int> trace(nSampFact);
    for (size_t i = fromTrc; i <= toTrc; i++){
      if (progressCallback)
        cbk();
      if (i != fromTrc)
        file.seekg(skipBytesPerTrc, std::ios_base::cur);
      file.read(bit_cast<char *>(trace.data()), nSampFact*4);
      for (size_t ii = 0; ii < nSampFact; ii++){
        TRACE(ii,i-fromTrc) = ibm2ieee(to_native_endian(trace(ii), endian));
      }
    }
  } else if (format == h5geo::SegyFormat::FourByte_integer) {
    Eigen::VectorX<int> trace(nSampFact);
    for (size_t i = fromTrc; i <= toTrc; i++){
      if (progressCallback)
        cbk();
      if (i != fromTrc)
        file.seekg(skipBytesPerTrc, std::ios_base::cur);
      file.read(bit_cast<char *>(trace.data()), nSampFact*4);
      for (size_t ii = 0; ii < nSampFact; ii++){
        TRACE(ii,i-fromTrc) = (float)to_native_endian(trace(ii), endian);
      }
    }
  } else if (format == h5geo::SegyFormat::FourByte_IEEE) {
    Eigen::VectorX<float> trace(nSampFact);
    for (size_t i = fromTrc; i <= toTrc; i++){
      if (progressCallback)
        cbk();
      if (i != fromTrc)
        file.seekg(skipBytesPerTrc, std::ios_base::cur);
      file.read(bit_cast<char *>(trace.data()), nSampFact*4);
      for (size_t ii = 0; ii < nSampFact; ii++){
        TRACE(ii,i-fromTrc) = to_native_endian(trace(ii), endian);
      }
    }
  }

  if (progressCallback)
    progressCallback( double(1) );
  
  return TRACE;
}

bool readSEGYTraces(
    H5Seis* seis,
    const std::string& segy,
    bool appendTraces,
    size_t nSamp,
    size_t nTrc,
    h5geo::SegyFormat format,
    h5geo::Endian endian,
    std::vector<std::string> trcHdrNames,
    size_t trcBuffer,
    int nThreads,
    std::function<void(double)> progressCallback)
{
  if (!seis || trcBuffer < 1)
    return false;

  if (std::string{magic_enum::enum_name(format)}.empty())
    format = getSEGYFormat(segy, endian);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (std::string{magic_enum::enum_name(format)}.empty() ||
      std::string{magic_enum::enum_name(endian)}.empty())
    return false;

  if (nTrc < 1)
    nTrc = getSEGYNTrc(segy, nSamp, endian);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1 || nTrc < 1)
    return false;

  // must do the check as within OMP I cannot return 'false', only 'continue'
  if (!isSEGY(segy))
    return false;

  if (trcHdrNames.empty())
    trcHdrNames = h5geo::getTraceHeaderShortNames();

  std::vector<std::string> trcHdrNames_original = getTraceHeaderShortNames();
  if (trcHdrNames.size() != trcHdrNames_original.size())
    return false;

  std::vector<size_t> mapHdr2origin(trcHdrNames.size());
  for (size_t i = 0; i < trcHdrNames.size(); i++){
    ptrdiff_t ind = find(trcHdrNames_original.begin(),
                   trcHdrNames_original.end(),
                   trcHdrNames[i]) - trcHdrNames_original.begin();
    if (ind >= trcHdrNames.size()) {
      return false;
    }

    mapHdr2origin[i] = ind;
  }

  size_t fromTrc = 0;
  if (appendTraces)
    fromTrc = seis->getNTrc();

  seis->setNTrc(fromTrc+nTrc);
  seis->setNSamp(nSamp);

  Eigen::MatrixXd HDR;
  Eigen::MatrixXf TRACE;
  Eigen::VectorXd minHDRVec(78), maxHDRVec(78);
  minHDRVec.fill(std::numeric_limits<double>::infinity());
  maxHDRVec.fill(-std::numeric_limits<double>::infinity());

  size_t bytesPerTrc = 4 * nSamp + 240;
  size_t J = trcBuffer;
  size_t N = nTrc / trcBuffer;
  ptrdiff_t n_passed = 0;
  double progressOld = 0;
  double progressNew = 0;

#ifdef H5GEO_USE_THREADS
  if (nThreads < 1 || nThreads > omp_get_max_threads())
    nThreads = omp_get_max_threads();
#pragma omp parallel for num_threads(nThreads) private(HDR, TRACE, J)
#endif
  for (ptrdiff_t n = 0; n <= N; n++) {
    if (progressCallback){
      progressNew = n_passed / (double)N;
      // update callback only if the difference >= 1% than the previous value
      if (progressNew - progressOld >= 0.01){
        progressCallback( progressNew );
        progressOld = progressNew;
      }
    }

    if (n < N) {
      J = trcBuffer;
    } else {
      J = nTrc - N * trcBuffer;
    }

    if (J == 0)
      continue;

    HDR.resize(J, 78);
    TRACE.resize(nSamp, J);

    size_t memoryOffset = 3600 + n * trcBuffer * bytesPerTrc; // trcBuffer !!! NOT J
    size_t memorySize = J * bytesPerTrc;

    std::error_code err;
    mio::mmap_sink rw_mmap = mio::make_mmap_sink(
          segy, memoryOffset, memorySize, err);
    if (err)
      continue;

    short* m_short = h5geo::bit_cast<short *>(rw_mmap.data());
    int* m_int = h5geo::bit_cast<int *>(rw_mmap.data());
    float* m_float = h5geo::bit_cast<float *>(rw_mmap.data());

    for (size_t j = 0; j < J; j++) {
      for (size_t i = 0; i < 7; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_int[j * bytesPerTrc / 4 + i], endian);
      }
      for (size_t i = 7; i < 11; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_short[j * bytesPerTrc / 2 + (i - 7) + 14], endian);
      }
      for (size_t i = 11; i < 19; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_int[j * bytesPerTrc / 4 + (i - 11) + 9], endian);
      }
      for (size_t i = 19; i < 21; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_short[j * bytesPerTrc / 2 + (i - 19) + 34], endian);
      }
      for (size_t i = 21; i < 25; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_int[j * bytesPerTrc / 4 + (i - 21) + 18], endian);
      }
      for (size_t i = 25; i < 71; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_short[j * bytesPerTrc / 2 + (i - 25) + 44], endian);
      }
      for (size_t i = 71; i < 76; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_int[j * bytesPerTrc / 4 + (i - 71) + 45], endian);
      }
      for (size_t i = 76; i < 78; i++) {
        HDR(j, mapHdr2origin[i]) = to_native_endian(m_short[j * bytesPerTrc / 2 + (i - 76) + 100], endian);
      }

      if (format == h5geo::SegyFormat::FourByte_IBM) {
        for (size_t i = 0; i < nSamp; i++) {
          TRACE(i, j) = ibm2ieee(
                to_native_endian(m_int[j * bytesPerTrc / 4 + i + 60], endian));
        }
      } else if (format == h5geo::SegyFormat::FourByte_integer) {
        for (size_t i = 0; i < nSamp; i++) {
          TRACE(i, j) =
              (float)to_native_endian(m_int[j * bytesPerTrc / 4 + i + 60], endian);
        }
      } else if (format == h5geo::SegyFormat::FourByte_IEEE) {
        for (size_t i = 0; i < nSamp; i++) {
          TRACE(i, j) =
              to_native_endian(m_float[j * bytesPerTrc / 4 + i + 60], endian);
        }
      }
    }

#ifdef H5GEO_USE_THREADS
#pragma omp critical
#endif
    {
      seis->writeTraceHeader(HDR, fromTrc);
      seis->writeTrace(TRACE, fromTrc);
      fromTrc = fromTrc + J;
      n_passed++;

      for (ptrdiff_t i = 0; i < HDR.rows(); i++) {
        for (ptrdiff_t j = 0; j < HDR.cols(); j++) {
          minHDRVec(j) = std::min(minHDRVec(j), HDR(i, j));
          maxHDRVec(j) = std::max(maxHDRVec(j), HDR(i, j));
        }
      }
    }
    rw_mmap.unmap();
  }

  if (progressCallback)
    progressCallback( double(1) );

  return true;
}

bool readSEGY(
    H5Seis* seis,
    const std::string& segy,
    bool appendTraces,
    size_t nSamp,
    size_t nTrc,
    h5geo::SegyFormat format,
    h5geo::Endian endian,
    std::vector<std::string> trcHdrNames,
    size_t trcBuffer,
    std::function<void(double)> progressCallback)
{
  if (!seis || trcBuffer < 1)
    return false;

  if (std::string{magic_enum::enum_name(format)}.empty())
    format = getSEGYFormat(segy, endian);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (std::string{magic_enum::enum_name(format)}.empty() ||
      std::string{magic_enum::enum_name(endian)}.empty())
    return false;

  if (nTrc < 1)
    nTrc = getSEGYNTrc(segy, nSamp, endian);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1 || nTrc < 1)
    return false;

  // must do the check as within OMP I cannot return 'false', only 'continue'
  if (!isSEGY(segy))
    return false;

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return false;

  if (trcHdrNames.empty())
    trcHdrNames = h5geo::getTraceHeaderShortNames();

  std::vector<std::string> trcHdrNames_original = getTraceHeaderShortNames();
  if (trcHdrNames.size() != trcHdrNames_original.size())
    return false;

  std::vector<size_t> mapHdr2origin(trcHdrNames.size());
  for (size_t i = 0; i < trcHdrNames.size(); i++){
    ptrdiff_t ind = find(trcHdrNames_original.begin(),
                   trcHdrNames_original.end(),
                   trcHdrNames[i]) - trcHdrNames_original.begin();
    if (ind >= trcHdrNames.size()) {
      return false;
    }

    mapHdr2origin[i] = ind;
  }

  size_t fromTrc = 0;
  if (appendTraces)
    fromTrc = seis->getNTrc();

  seis->setNTrc(fromTrc+nTrc);
  seis->setNSamp(nSamp);

  Eigen::MatrixXd HDR;
  Eigen::MatrixXf TRACE;
  Eigen::VectorXd minHDRVec(78), maxHDRVec(78);
  minHDRVec.fill(std::numeric_limits<double>::infinity());
  maxHDRVec.fill(-std::numeric_limits<double>::infinity());

  size_t bytesPerTrc = 4 * nSamp + 240;
  size_t J = trcBuffer;
  size_t N = nTrc / trcBuffer;
  ptrdiff_t n_passed = 0;
  double progressOld = 0;
  double progressNew = 0;

  TraceHeader hdr;
  size_t skipBytesPerTrc = 4 * nSamp + 240 - nSamp*4;
  file.seekg(3600, std::ios_base::beg);
  for (ptrdiff_t n = 0; n <= N; n++) {
    if (progressCallback){
      progressNew = n_passed / (double)N;
      // update callback only if the difference >= 1% than the previous value
      if (progressNew - progressOld >= 0.01){
        progressCallback( progressNew );
        progressOld = progressNew;
      }
    }

    if (n < N) {
      J = trcBuffer;
    } else {
      J = nTrc - N * trcBuffer;
    }

    if (J == 0)
      continue;

    HDR.resize(J, 78);
    TRACE.resize(nSamp, J);
    for (size_t j = 0; j < J; j++) {
      file.read(bit_cast<char *>(&hdr), sizeof(hdr));
      int ii = 0;
      for (int i = 0; i < (sizeof(hdr.b0)/sizeof(*hdr.b0)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b0[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b1)/sizeof(*hdr.b1)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b1[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b2)/sizeof(*hdr.b2)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b2[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b3)/sizeof(*hdr.b3)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b3[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b4)/sizeof(*hdr.b4)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b4[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b5)/sizeof(*hdr.b5)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b5[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b6)/sizeof(*hdr.b6)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b6[i];
        ii++;
      }
      for (int i = 0; i < (sizeof(hdr.b7)/sizeof(*hdr.b7)); i++){
        if (ii >= HDR.cols())
          break;
        HDR(j,mapHdr2origin[ii]) = (double)hdr.b7[i];
        ii++;
      }

      if (format == h5geo::SegyFormat::FourByte_IBM) {
        Eigen::VectorX<int> trace(TRACE.rows());
        file.read(bit_cast<char *>(trace.data()), trace.size()*4);
        for (size_t i = 0; i < TRACE.rows(); i++){
          TRACE(i,j) = ibm2ieee(to_native_endian(trace(i), endian));
        }
      } else if (format == h5geo::SegyFormat::FourByte_integer) {
        Eigen::VectorX<int> trace(TRACE.rows());
        file.read(bit_cast<char *>(trace.data()), trace.size()*4);
        for (size_t i = 0; i < TRACE.rows(); i++){
          TRACE(i,j) = (float)to_native_endian(trace(i), endian);
        }
      } else if (format == h5geo::SegyFormat::FourByte_IEEE) {
        file.read(bit_cast<char *>(TRACE.col(j).data()), TRACE.rows()*4);
        for (size_t i = 0; i < TRACE.rows(); i++){
          TRACE(i,j) = (float)to_native_endian(TRACE(i,j), endian);
        }
      }
    }

    seis->writeTraceHeader(HDR, fromTrc);
    seis->writeTrace(TRACE, fromTrc);
    fromTrc = fromTrc + J;
    n_passed++;

    for (ptrdiff_t i = 0; i < HDR.rows(); i++) {
      for (ptrdiff_t j = 0; j < HDR.cols(); j++) {
        minHDRVec(j) = std::min(minHDRVec(j), HDR(i, j));
        maxHDRVec(j) = std::max(maxHDRVec(j), HDR(i, j));
      }
    }
  }

  if (progressCallback)
    progressCallback( double(1) );

  return true;
}

bool readSEGYSTACK(
    H5Vol* vol,
    const std::string& segy,
    const size_t& ilHdrOffset,
    const size_t& ilHdrSize,
    const size_t& xlHdrOffset,
    const size_t& xlHdrSize,
    const size_t& xHdrOffset,
    const size_t& xHdrSize,
    const size_t& yHdrOffset,
    const size_t& yHdrSize,
    double sampRate,
    size_t nSamp,
    size_t nTrc,
    h5geo::SegyFormat format,
    h5geo::Endian endian,
    std::function<void(double)> progressCallback)
{
  if (!vol)
    return false;

  // these checks are necessary as `readSEGYTrace()` won't do that for you
  if (std::string{magic_enum::enum_name(format)}.empty())
    format = getSEGYFormat(segy, endian);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (std::string{magic_enum::enum_name(format)}.empty() ||
      std::string{magic_enum::enum_name(endian)}.empty())
    return false;

  if (nTrc < 1)
    nTrc = getSEGYNTrc(segy, nSamp, endian);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1 || nTrc < 1)
    return false;

  Eigen::MatrixXd HDR(nTrc, 4);
  HDR.col(0) = readSEGYTraceHeader(
      segy, ilHdrOffset, ilHdrSize, 
      0, std::numeric_limits<size_t>::max(),
      nSamp, nTrc, endian).cast<double>();
  HDR.col(1) = readSEGYTraceHeader(
      segy, xlHdrOffset, xlHdrSize, 
      0, std::numeric_limits<size_t>::max(),
      nSamp, nTrc, endian).cast<double>();
  HDR.col(2) = readSEGYTraceHeader(
      segy, xHdrOffset, xHdrSize, 
      0, std::numeric_limits<size_t>::max(),
      nSamp, nTrc, endian).cast<double>();
  HDR.col(3) = readSEGYTraceHeader(
      segy, yHdrOffset, yHdrSize, 
      0, std::numeric_limits<size_t>::max(),
      nSamp, nTrc, endian).cast<double>();

  Eigen::VectorX<ptrdiff_t> ind = h5geo::sort_rows(HDR);
  Eigen::MatrixXd HDR_sorted = HDR(ind, Eigen::all);

  double origin_x;
  double origin_y;
  double orientation;
  double ilSpacing;
  double xlSpacing;
  bool isILReversed;
  bool isXLReversed;
  bool isPlanReversed;
  bool status = h5geo::getSurveyInfoFromSortedData(
        HDR_sorted.col(0),
        HDR_sorted.col(1),
        HDR_sorted.col(2),
        HDR_sorted.col(3),
        origin_x,
        origin_y,
        orientation,
        ilSpacing,
        xlSpacing,
        isILReversed,
        isXLReversed,
        isPlanReversed);

  if (!status)
    return false;

  // make plan normal by exchanging INLINE and XLINE headers
  if (isPlanReversed){
    HDR(Eigen::all, Eigen::seq(0,1)) = HDR(Eigen::all, Eigen::seq(1,0,Eigen::fix<-1>)).eval();
    ind = h5geo::sort_rows(HDR);
    HDR_sorted = HDR(ind, Eigen::all);
  }

  // IL is sorted
  size_t nxl = 0;
  for (ptrdiff_t i = 0; i < HDR_sorted.rows(); i++){
    nxl += 1;
    if (HDR_sorted(i,0) != HDR_sorted(i+1,0))
      break;
  }

  auto dv = std::div(HDR_sorted.rows(), nxl);
  if (dv.rem != 0)
    return false;

  size_t nil = dv.quot;
  if (!vol->resize(nxl, nil, nSamp))
    return false;

  // N - number of slices written at once (usually 
  // should be equal to Y-chunkSize to acieve best IO speed)
  size_t N = 64;
  H5VolParam vp = vol->getParam();
  if (vp.yChunkSize > 0 &&
      vp.nY > 0 &&
      vp.yChunkSize < vp.nY){
    N = vp.yChunkSize;
  } else if (vp.yChunkSize > 0 &&
      vp.nY > 0 &&
      vp.yChunkSize > vp.nY &&
      vp.nY < N){
    N = vp.nY;
  }

  double progressOld = 0;
  double progressNew = 0;

  auto cbk = [&](const size_t& i, const size_t& I){
    progressNew = i / (double)I;
    // update callback only if the difference >= 1% than the previous value
    if (progressNew - progressOld >= 0.01){
      progressCallback( progressNew );
      progressOld = progressNew;
    }
  };

  std::ifstream file(segy, std::ifstream::binary | std::ifstream::in);
  if (!file.is_open())
    return false;

  if (isXLReversed && isILReversed){
    for (size_t i = 0; i < nil; i+=N){
      if (progressCallback)
        cbk(i, nil);
      size_t i0 = i*nxl;
      size_t i1 = i0+nxl*N-1;
      if (i1 >= ind.size())
        i1 = ind.size()-1;
      size_t n_fact = (i1-i0+1)/nxl;
      Eigen::VectorX<ptrdiff_t> ind_il(i1-i0+1);
      size_t ii1 = ind_il.size()-1;
      for (size_t n = 0; n < N; n++){
        size_t i1 = i0+nxl-1;
        if (i1 >= ind.size())
          i1 = ind.size()-1;
        size_t ii0 = ii1-nxl+1;
        ind_il(Eigen::seq(ii0,ii1)) = ind(Eigen::seq(i1,i0,Eigen::fix<-1>));
        ii1 = ii0-1;
      }
      Eigen::MatrixXf IL(nSamp, ind_il.size());
      for (size_t j = 0; j < ind_il.size(); j++){
        h5geo::readSEGYTrace(file, ind_il(j), format, endian, IL.col(j));
      }
      IL.transposeInPlace();          // switch X and Z axes
      if (sampRate < 0)
        IL.rowwise().reverseInPlace();  // horizontal flip (Z axis flip)
      vol->writeData(IL, 0, nil-(i+1), 0, nxl, n_fact, nSamp);
    }
  } else if (isXLReversed){
    for (size_t i = 0; i < nil; i+=N){
      if (progressCallback)
        cbk(i, nil);
      size_t i0 = i*nxl;
      size_t i1 = i0+nxl*N-1;
      if (i1 >= ind.size())
        i1 = ind.size()-1;
      size_t n_fact = (i1-i0+1)/nxl;
      Eigen::VectorX<ptrdiff_t> ind_il(i1-i0+1);
      size_t ii1 = ind_il.size()-1;
      for (size_t n = 0; n < N; n++){
        size_t i1 = i0+nxl-1;
        if (i1 >= ind.size())
          i1 = ind.size()-1;
        size_t ii0 = ii1-nxl+1;
        ind_il(Eigen::seq(ii0,ii1)) = ind(Eigen::seq(i0,i1));
        ii1 = ii0-1;
      }
      Eigen::MatrixXf IL(nSamp, ind_il.size());
      for (size_t j = 0; j < ind_il.size(); j++){
        h5geo::readSEGYTrace(file, ind_il(j), format, endian, IL.col(j));
      }
      IL.transposeInPlace();          // switch X and Z axes
      if (sampRate < 0)
        IL.rowwise().reverseInPlace();  // horizontal flip (Z axis flip)
      vol->writeData(IL, 0, nil-(i+1), 0, nxl, n_fact, nSamp);
    }
  } else if (isILReversed){
    for (size_t i = 0; i < nil; i+=N){
      if (progressCallback)
        cbk(i, nil);
      size_t i0 = i*nxl;
      size_t i1 = i0+nxl*N-1;
      if (i1 >= ind.size())
        i1 = ind.size()-1;
      size_t n_fact = (i1-i0+1)/nxl;
      Eigen::VectorX<ptrdiff_t> ind_il = ind(Eigen::seq(i1,i0,Eigen::fix<-1>));
      Eigen::MatrixXf IL(nSamp, ind_il.size());
      for (size_t j = 0; j < ind_il.size(); j++){
        h5geo::readSEGYTrace(file, ind_il(j), format, endian, IL.col(j));
      }
      IL.transposeInPlace();          // switch X and Z axes
      if (sampRate < 0)
        IL.rowwise().reverseInPlace();  // horizontal flip (Z axis flip)
      vol->writeData(IL, 0, i, 0, nxl, n_fact, nSamp);
    }
  } else {
    for (size_t i = 0; i < nil; i+=N){
      if (progressCallback)
        cbk(i, nil);
      size_t i0 = i*nxl;
      size_t i1 = i0+nxl*N-1;
      if (i1 >= ind.size())
        i1 = ind.size()-1;
      size_t n_fact = (i1-i0+1)/nxl;
      Eigen::VectorX<ptrdiff_t> ind_il = ind(Eigen::seq(i0,i1));
      Eigen::MatrixXf IL(nSamp, ind_il.size());
      for (size_t j = 0; j < ind_il.size(); j++){
        h5geo::readSEGYTrace(file, ind_il(j), format, endian, IL.col(j));
      }
      IL.transposeInPlace();          // switch X and Z axes
      if (sampRate < 0)
        IL.rowwise().reverseInPlace();  // horizontal flip (Z axis flip)
      vol->writeData(IL, 0, i, 0, nxl, n_fact, nSamp);
    }
  }

  Eigen::Vector3d origin;
  origin(0) = origin_x;
  origin(1) = origin_y;
  if (sampRate < 0)
    origin(2) = sampRate*(nSamp-1);
  else
    origin(2) = 0;
  vol->setOrigin(origin);

  Eigen::Vector3d spacings;
  // even though we made plan normal `ilSpacing` and `xlSpacing` 
  // are defined relatively to original plan (it may be reversed)
  if (isPlanReversed){
    spacings(0) = xlSpacing;
    spacings(1) = ilSpacing;
  } else {
    spacings(0) = ilSpacing;
    spacings(1) = xlSpacing;
  }
  spacings(2) = std::fabs(sampRate);
  vol->setSpacings(spacings);

  vol->setOrientation(orientation);
  vol->setAngularUnits("degree");

  if (progressCallback)
    progressCallback( double(1) );

  return true;
}

}
