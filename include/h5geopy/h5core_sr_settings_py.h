#ifndef H5CORE_SR_SETTINGS_PY_H
#define H5CORE_SR_SETTINGS_PY_H

#include "h5geo_py.h"

namespace h5geopy {

///
/// embed these functions to a submodule 'm_sr':
/// auto m_sr = m.def_submodule("sr");
void defineSRSettingsFunctions(py::module_& m_sr);

} // h5geopy

#endif // H5CORE_SR_SETTINGS_PY_H
