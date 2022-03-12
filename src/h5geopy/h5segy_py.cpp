#include "../../include/h5geopy/h5segy_py.h"

#include <h5geo/misc/h5segy.h>

namespace h5geopy {

namespace ext {

std::vector<std::string> readSEGYTextHeader(
    const std::string& segy, h5geo::TextEncoding encoding)
{
  char txt[40][80];
  h5geo::readSEGYTextHeader(segy, txt, encoding);
  std::vector<std::string> v(40);
  for (int i = 0; i < 40; i++)
    v[i] = std::string(txt[i], 80);
  return v;
}

std::vector<double> readSEGYBinHeader(
    const std::string& segy, h5geo::Endian endian)
{
  double binHdr[30];
  h5geo::readSEGYBinHeader(segy, binHdr, endian);

  std::vector<double> v;
  v.insert(v.end(), &binHdr[0], &binHdr[30]);
  return v;
}

} // ext

void defineSEGYFunctions(py::module_& m){
  m.def("isSEGY", &h5geo::isSEGY,
        py::arg("segy"));

  m.def("getSEGYTextEncoding", &h5geo::getSEGYTextEncoding,
        py::arg("segy"));
  m.def("getSEGYEndian", &h5geo::getSEGYEndian,
        py::arg("segy"));
  m.def("getSEGYFormat", &h5geo::getSEGYFormat,
        py::arg("segy"), py::arg("endian"));

  m.def("readSEGYTextHeader", &ext::readSEGYTextHeader,
        py::arg("segy"), py::arg("encoding"));
  m.def("readSEGYBinHeader", &ext::readSEGYBinHeader,
        py::arg("segy"), py::arg("endian"));

  m.def("getSEGYSampRate", &h5geo::getSEGYSampRate,
        py::arg("segy"), py::arg("endian"));
  m.def("getSEGYNSamp", &h5geo::getSEGYNSamp,
        py::arg("segy"), py::arg("endian"));
  m.def("getSEGYNTrc", &h5geo::getSEGYNTrc,
        py::arg("segy"), py::arg("nSamp"), py::arg("endian"));

  m.def("readSEGYTraces", &h5geo::readSEGYTraces,
        py::arg("seis"),
        py::arg("segy"),
        py::arg("nSamp"), py::arg("nTrc"),
        py::arg("fromTrc"), py::arg("trcBuffer"),
        py::arg("segyFormat"),
        py::arg("endian"),
        py::arg("trcHdrNames"));
}


} // h5geopy
