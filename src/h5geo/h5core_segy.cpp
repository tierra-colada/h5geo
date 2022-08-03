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

namespace h5geo {

bool isSEGY(const std::string& segy){
  try {
    auto segySize = std::filesystem::file_size(segy);
    if (segySize < 3600 + 240)
      return false;
  } catch(std::filesystem::filesystem_error& e) {
    return false;
  }

  std::ifstream file(segy);
  if (!file.is_open())
    return false;

  return true;
}

TextEncoding getSEGYTextEncoding(const std::string& segy){
  if (!isSEGY(segy))
    return static_cast<h5geo::TextEncoding>(0);

  std::ifstream file(segy);
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

  std::ifstream file(segy);
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

  std::ifstream file(segy);
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

  std::ifstream file(segy);
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

bool readSEGYBinHeader(
    const std::string& segy,
    double binHdr[30], h5geo::Endian endian)
{
  if (!isSEGY(segy))
    return false;

  std::ifstream file(segy);
  if (!file.is_open())
    return false;

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  int binHdr4[3];
  short binHdr2[27], binHdr2tmp[3];

  file.seekg(3200);
  file.read(bit_cast<char *>(&binHdr4), 12);
  file.read(bit_cast<char *>(&binHdr2), 48);

  file.seekg(3500);
  file.read(bit_cast<char *>(&binHdr2tmp), 6);

  binHdr2[24] = binHdr2tmp[0];
  binHdr2[25] = binHdr2tmp[1];
  binHdr2[26] = binHdr2tmp[2];

  to_native_endian<int>(std::begin(binHdr4), std::end(binHdr4), std::begin(binHdr4), endian);
  to_native_endian<short>(std::begin(binHdr2), std::end(binHdr2), std::begin(binHdr2), endian);

  for (int i = 0; i < 3; i++) {
    binHdr[i] = binHdr4[i];
  }

  for (int i = 3; i < 30; i++) {
    binHdr[i] = binHdr2[i - 3];
  }

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

  if (nTrc < 1)
    nTrc = getSEGYNTrc(segy, nSamp, endian);

  if (nSamp < 1)
    nSamp = getSEGYNSamp(segy, endian);

  if (nSamp < 1 || nTrc < 1)
    return false;

  if (std::string{magic_enum::enum_name(format)}.empty())
    format = getSEGYFormat(segy, endian);

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  if (std::string{magic_enum::enum_name(format)}.empty() ||
      std::string{magic_enum::enum_name(endian)}.empty())
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

}
