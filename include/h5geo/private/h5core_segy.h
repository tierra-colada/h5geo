#ifndef H5CORE_SEGY_H
#define H5CORE_SEGY_H

// to suppress undef var errors in IDE
#include "h5geo/h5core.h"

#include <cstring>
#include <cmath>
#include <bitset>
#include <fstream>

class H5Seis;

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
  const int constnum1 = 2130706432;      // hex2dec('7f000000') (Matlab)
  const int constnum2 = 16777215;        // hex2dec('00ffffff') (Matlab)
  const float constnum3 = 16777216;      // 2^24

  int sgn = (1 - 2 * bits_from[31]);
  int expon = (from & constnum1) >> 24;
  int mantiss = (from & constnum2);

  to = float(sgn) * float(pow(16, expon - 64)) * float(mantiss) / constnum3;

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

H5GEO_EXPORT bool isSEGY(const std::string& segy);

H5GEO_EXPORT TextEncoding getSEGYTextEncoding(const std::string& segy);

H5GEO_EXPORT Endian getSEGYEndian(const std::string& segy);

H5GEO_EXPORT SegyFormat getSEGYFormat(
    const std::string& segy, h5geo::Endian endian = static_cast<h5geo::Endian>(0));

H5GEO_EXPORT bool readSEGYTextHeader(
    const std::string& segy,
    char txtHdr[40][80], h5geo::TextEncoding encoding = static_cast<h5geo::TextEncoding>(0));

H5GEO_EXPORT bool readSEGYBinHeader(
    const std::string& segy,
    double binHdr[30], h5geo::Endian endian = static_cast<h5geo::Endian>(0));

H5GEO_EXPORT double getSEGYSampRate(
    const std::string& segy, h5geo::Endian endian = static_cast<h5geo::Endian>(0));

H5GEO_EXPORT size_t getSEGYNSamp(
    const std::string& segy, h5geo::Endian endian = static_cast<h5geo::Endian>(0));

H5GEO_EXPORT size_t getSEGYNTrc(
    const std::string& segy, size_t nSamp = 0, h5geo::Endian endian = static_cast<h5geo::Endian>(0));

/// \brief readSEGYTraceHeader read selected header from all the traces (4 byte SEGY supported only)
/// \param segy path to SEGY file
/// \param hdrOffset in range [0, 238]
/// \param hdrSize usually 2 or 4
/// \param fromTrc first trace to read
/// \param toTrc last trace to read
/// \param nSamp number of samples in SEGY (if 0 then try automatically detect)
/// \param nTrc number of traces in SEGY (if 0 then try automatically detect)
/// \param endian Big or Little
/// \param progressCallback callback function of form `void foo(double progress)`
/// \return  
H5GEO_EXPORT Eigen::VectorX<ptrdiff_t> readSEGYTraceHeader(
    const std::string& segy,
    const size_t& hdrOffset,
    const size_t& hdrSize,
    size_t fromTrc = 0,
    size_t toTrc = std::numeric_limits<size_t>::max(),
    size_t nSamp = 0,
    size_t nTrc = 0,
    h5geo::Endian endian = static_cast<h5geo::Endian>(0),
    std::function<void(double)> progressCallback = nullptr);

/// \brief readSEGYTraces read traces
/// \param segy path to SEGY file
/// \param fromSamp first sample to read
/// \param toSamp last sample to read
/// \param fromTrc first trace to read
/// \param toTrc last trace to read
/// \param nSamp number of samples in SEGY (if 0 then try automatically detect)
/// \param nTrc number of traces in SEGY (if 0 then try automatically detect)
/// \param format SEGY format (ibm32, ieee32 or int4)
/// \param endian Big or Little
/// \param progressCallback callback function of form `void foo(double progress)`
/// \return
H5GEO_EXPORT Eigen::MatrixXf readSEGYTraces(
    const std::string& segy,
    size_t fromSamp = 0,
    size_t toSamp = std::numeric_limits<size_t>::max(),
    size_t fromTrc = 0,
    size_t toTrc = std::numeric_limits<size_t>::max(),
    size_t nSamp = 0,
    size_t nTrc = 0,
    h5geo::SegyFormat format = static_cast<h5geo::SegyFormat>(0),
    h5geo::Endian endian = static_cast<h5geo::Endian>(0),
    std::function<void(double)> progressCallback = nullptr);

/// \brief readSEGYTraces read and write SEGY traces and trace headers to
/// H5Seis object using memory mapping technique (OpenMP enabled)
/// \param seis
/// \param segy path to SEGY file
/// \param appendTraces instead of overwriting existing H5Seis traces it simply
/// adds new traces at the end of array
/// \param nSamp number of samples in SEGY (if 0 then try automatically detect)
/// \param nTrc number of traces in SEGY (if 0 then try automatically detect)
/// \param format SEGY format (ibm32, ieee32 or int4)
/// \param endian Big or Little
/// \param trcHdrNames use only those defined in 'getTraceHeaderShortNames',
/// but you can change their order thus fix probably messed up trace header bytes
/// (empty to use defined in 'getTraceHeaderShortNames' func)
/// \param trcBuffer number of traces per thread to read before writing them at once
/// \param nThreads number of threads (to use all threads pass any number `<1`)
/// \param progressCallback callback function of form `void foo(double progress)`
/// \return
H5GEO_EXPORT bool readSEGYTraces(
    H5Seis* seis,
    const std::string& segy,
    bool appendTraces = false,
    size_t nSamp = 0,
    size_t nTrc = 0,
    h5geo::SegyFormat format = static_cast<h5geo::SegyFormat>(0),
    h5geo::Endian endian = static_cast<h5geo::Endian>(0),
    std::vector<std::string> trcHdrNames = std::vector<std::string>(),
    size_t trcBuffer = 10000,
    int nThreads = -1,
    std::function<void(double)> progressCallback = nullptr);


}

#endif // H5CORE_SEGY_H
