#ifndef H5CORE_SEGY_H
#define H5CORE_SEGY_H

#include <vector>
#include <cstring>
#include <cmath>
#include <bitset>
#include <fstream>
#include <filesystem>
#include <filesystem>
#include <omp.h>

#include <Eigen/Dense>

#include <mio/mmap.hpp>

// enum string is needed to include magic_enum with predefined macro
#include "h5enum_string.h"
#include "../h5util.h"
#include "../h5seis.h"


namespace h5geo {

template <class To, class From>
typename std::enable_if<(sizeof(To) == sizeof(From)) &&
std::is_trivially_copyable<From>::value &&
std::is_trivial<To>::value,
// this implementation requires that To is
// trivially default constructible
To>::type
// constexpr support needs compiler magic
inline bit_cast(const From &src) noexcept {
  To dst;
  std::memcpy(&dst, &src, sizeof(To));
  return dst;
}

inline float ibm2ieee(const int &from) {
  std::bitset<32> bits_from(from);
  float to;
  const int constnum1 = 2.130706432e+09; // hex2dec('7f000000') (Matlab)
  const int constnum2 = 16777215;        // hex2dec('00ffffff') (Matlab)
  const float constnum3 = 16777216;      // 2^24

  float sgn = (1 - 2 * bits_from[31]);
  float expon = (from & constnum1) >> 24;
  float mantiss = (from & constnum2);

  to = sgn * float(pow(16, expon - 64)) * mantiss / constnum3;

  return to;
}

inline unsigned char ebc_to_ascii_table(unsigned char ascii) {
  unsigned char asc;

  int Tableebc2Asc[256] = {
      32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
      32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
      32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
      32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,
      32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  91,
      46,  60,  40,  43,  33,  38,  32,  32,  32,  32,  32,  32,  32,  32,  32,
      93,  36,  42,  41,  59,  94,  45,  47,  32,  32,  32,  32,  32,  32,  32,
      32,  124, 44,  37,  95,  62,  63,  32,  32,  32,  32,  32,  32,  238, 160,
      161, 96,  58,  35,  64,  39,  61,  34,  230, 97,  98,  99,  100, 101, 102,
      103, 104, 105, 164, 165, 228, 163, 229, 168, 169, 106, 107, 108, 109, 110,
      111, 112, 113, 114, 170, 171, 172, 173, 174, 175, 239, 126, 115, 116, 117,
      118, 119, 120, 121, 122, 224, 225, 226, 227, 166, 162, 236, 235, 167, 232,
      237, 233, 231, 234, 158, 128, 129, 150, 132, 133, 148, 131, 123, 65,  66,
      67,  68,  69,  70,  71,  72,  73,  149, 136, 137, 138, 139, 140, 125, 74,
      75,  76,  77,  78,  79,  80,  81,  82,  141, 142, 143, 159, 144, 145, 92,
      32,  83,  84,  85,  86,  87,  88,  89,  90,  146, 147, 134, 130, 156, 155,
      48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  135, 152, 157, 153, 151,
      32};

  asc = Tableebc2Asc[ascii];
  return (asc);
}


template <typename T>
inline T bswap(T val) {
    T retVal;
    char *pVal = (char*) &val;
    char *pRetVal = (char*)&retVal;
    int size = sizeof(T);
    for(int i=0; i<size; i++) {
        pRetVal[size-1-i] = pVal[i];
    }

    return retVal;
}

/// if vin, vout vectors:
/// bswap<T>(vin.begin(), vin.end(), vout.begin())
/// if b is C array:
/// bswap<T>(std::begin(b), std::end(b), std::begin(b));
template <typename Scalar, typename II, typename OI>
inline void bswap(II begin, II end, OI dest) {
    std::transform(begin, end, dest, bswap<Scalar>);
}

template <typename T>
inline T to_native_endian(T val, h5geo::Endian fromEndian) {
  if (O32_HOST_ORDER == O32_LITTLE_ENDIAN){
    if (fromEndian == h5geo::Endian::Big){
      return bswap(val);
    }
  } else if (O32_HOST_ORDER == O32_BIG_ENDIAN){
    if (fromEndian == h5geo::Endian::Little){
      return bswap(val);
    }
  }
  return val;
}

template <typename Scalar, typename II, typename OI>
inline void to_native_endian(II begin, II end, OI dest, h5geo::Endian fromEndian) {
  if (O32_HOST_ORDER == O32_LITTLE_ENDIAN){
    if (fromEndian == h5geo::Endian::Big){
      bswap<Scalar>(begin, end, dest);
    }
  } else if (O32_HOST_ORDER == O32_BIG_ENDIAN){
    if (fromEndian == h5geo::Endian::Little){
      bswap<Scalar>(begin, end, dest);
    }
  }
}

inline bool isSEGY(
    const std::string& segy)
{
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

inline TextEncoding getSEGYTxtEncoding(
    const std::string& segy)
{
  if (!isSEGY(segy))
    return static_cast<h5geo::TextEncoding>(0);

  std::ifstream file(segy);
  if (!file.is_open())
    return static_cast<h5geo::TextEncoding>(0);

  h5geo::TextEncoding encoding = static_cast<h5geo::TextEncoding>(0);
  char txtHdr[40][80];
  file.read(*txtHdr, 3200);

  int n;
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

inline Endian getSEGYEndian(
    const std::string& segy)
{
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

inline SegyFormat getSEGYFormat(
    const std::string& segy, h5geo::Endian endian = static_cast<Endian>(0))
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

inline bool readSEGYTextHeader(
    const std::string& segy,
    char txtHdr[40][80], h5geo::TextEncoding encoding = static_cast<TextEncoding>(0))
{
  if (!isSEGY(segy))
    return false;

  std::ifstream file(segy);
  if (!file.is_open())
    return false;

  if (std::string{magic_enum::enum_name(encoding)}.empty())
    encoding = getSEGYTxtEncoding(segy);

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

inline bool readSEGYBinHeader(
    const std::string& segy,
    double binHdr[30], h5geo::Endian endian = static_cast<Endian>(0))
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

inline double getSEGYSampRate(
    const std::string& segy, h5geo::Endian endian = static_cast<Endian>(0))
{
  if (!isSEGY(segy))
    return 0;

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  double binHdr[30];
  readSEGYBinHeader(segy, binHdr, endian);

  return binHdr[5];
}

inline size_t getSEGYNSamp(
    const std::string& segy, h5geo::Endian endian = static_cast<Endian>(0))
{
  if (!isSEGY(segy))
    return 0;

  if (std::string{magic_enum::enum_name(endian)}.empty())
    endian = getSEGYEndian(segy);

  double binHdr[30];
  readSEGYBinHeader(segy, binHdr, endian);

  return binHdr[7];
}

inline size_t getSEGYNTrc(
    const std::string& segy, size_t nSamp = 0, h5geo::Endian endian = static_cast<Endian>(0))
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

/// \brief readSEGYTraces read and write SEGY traces and trace headers to
/// H5Seis object using memory mapping technique (OpenMP enabled)
/// \param seis
/// \param segy path to SEGY file
/// \param nSamp number of samples in SEGY (if 0 then try automatically detect)
/// \param nTrc number of traces in SEGY (if 0 then try automatically detect)
/// \param fromTrc the starting trace index in H5Seis to be written (may be useful
/// when reading multiple SEGY in the same H5Seis object)
/// \param trcBuffer number of traces per thread to read before writing them at once
/// \param format SEGY format (ibm32, ieee32 or int4)
/// \param endian Big or Little
/// \param trcHdrNames use only those defined in 'getTraceHeaderShortNames',
/// but you can change their order thus fix probably messed up trace header bytes
/// \return
inline bool readSEGYTraces(
    H5Seis* seis,
    const std::string& segy,
    size_t nSamp = 0,
    size_t nTrc = 0,
    size_t fromTrc = 0,
    size_t trcBuffer = 10000,
    h5geo::SegyFormat format = static_cast<SegyFormat>(0),
    h5geo::Endian endian = static_cast<Endian>(0),
    std::vector<std::string> trcHdrNames = getTraceHeaderShortNames())
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

#ifdef H5GEO_USE_THREADS
#pragma omp parallel for private(HDR, TRACE, J)
#endif
  for (ptrdiff_t n = 0; n <= N; n++) {
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
              to_native_endian(m_int[j * bytesPerTrc / 4 + i + 60], endian);
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

      for (ptrdiff_t i = 0; i < HDR.rows(); i++) {
        for (ptrdiff_t j = 0; j < HDR.cols(); j++) {
          minHDRVec(j) = std::min(minHDRVec(j), HDR(i, j));
          maxHDRVec(j) = std::max(maxHDRVec(j), HDR(i, j));
        }
      }
    }
    rw_mmap.unmap();
  }

  return true;
}


}

#endif // H5CORE_SEGY_H
