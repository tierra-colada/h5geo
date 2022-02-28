#ifndef H5CORE_SEGY_H
#define H5CORE_SEGY_H

#include <vector>
#include <cstring>
#include <cmath>
#include <bitset>
#include <fstream>
#include <filesystem>

#include "h5core_enum.h"

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



inline TxtEncoding getSEGYTxtEncoding(
    const std::string& segy)
{
  std::ifstream file(segy);
  if (!file.is_open())
    return static_cast<h5geo::TxtEncoding>(0);

  h5geo::TxtEncoding encoding = static_cast<h5geo::TxtEncoding>(0);
  char txtHdr[40][80];
  file.read(*txtHdr, 3200);
  if (((txtHdr[0][0] == 'C') & (txtHdr[1][0] == 'C') & (txtHdr[2][0] == 'C')) |
      ((txtHdr[0][0] == ' ') & (txtHdr[1][0] == ' ') & (txtHdr[2][0] == ' '))) {
    encoding = h5geo::TxtEncoding::ASCII;
  } else if (((ebc_to_ascii_table(txtHdr[0][0]) == 'C') &
              (ebc_to_ascii_table(txtHdr[1][0]) == 'C') &
              (ebc_to_ascii_table(txtHdr[2][0]) == 'C')) |
             ((ebc_to_ascii_table(txtHdr[0][0]) == ' ') &
              (ebc_to_ascii_table(txtHdr[1][0]) == ' ') &
              (ebc_to_ascii_table(txtHdr[2][0]) == ' '))) {
    for (int i = 0; i < 40; i++) {
      for (int j = 0; j < 80; j++) {
        txtHdr[i][j] = ebc_to_ascii_table(txtHdr[i][j]);
      }
    }
    encoding = h5geo::TxtEncoding::EBCDIC;
  }
  return encoding;
}

inline Endian getSEGYEndian(
    const std::string& segy)
{
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
    const std::string& segy, h5geo::Endian endian)
{
  std::ifstream file(segy);
  if (!file.is_open())
    return static_cast<h5geo::SegyFormat>(0);

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

inline void readSEGYTxtHdr(
    const std::string& segy,
    char txtHdr[40][80], h5geo::TxtEncoding encoding)
{
  std::ifstream file(segy);
  if (!file.is_open())
    return;

  file.read(*txtHdr, 3200);
  if (encoding == h5geo::TxtEncoding::EBCDIC) {
    for (int i = 0; i < 40; i++) {
      for (int j = 0; j < 80; j++) {
        txtHdr[i][j] = ebc_to_ascii_table(txtHdr[i][j]);
      }
    }
  }
}

inline void readSEGYBinHdr(
    const std::string& segy,
    double binHdr[30], h5geo::Endian endian)
{
  std::ifstream file(segy);
  if (!file.is_open())
    return;

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
}

inline double getSEGYSampRate(
    const std::string& segy, h5geo::Endian endian)
{
  double binHdr[30];
  readSEGYBinHdr(segy, binHdr, endian);

  return binHdr[5];
}

inline size_t getSEGYNSamp(
    const std::string& segy, h5geo::Endian endian)
{
  double binHdr[30];
  readSEGYBinHdr(segy, binHdr, endian);

  return binHdr[7];
}

inline size_t getSEGYNTrc(
    const std::string& segy, h5geo::Endian endian)
{
  size_t fsize = 0;
  try {
    fsize = std::filesystem::file_size(segy);
  } catch(std::filesystem::filesystem_error& e) {
    return 0;
  }

  size_t nSamp = getSEGYNSamp(segy, endian);
  if (nSamp < 1)
    return 0;

  return (fsize - 3600) / (nSamp * 4 + 240);
}


}

#endif // H5CORE_SEGY_H
