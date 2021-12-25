#ifndef H5CORE_SR_SETTINGS_H
#define H5CORE_SR_SETTINGS_H

#include "h5geo_export.h"
#include "h5core_enum.h"

#include <string>

class OGRSpatialReference;

namespace h5geo
{


namespace sr {


H5GEO_EXPORT void setSpatialReference(OGRSpatialReference sr);
H5GEO_EXPORT void setSpatialReferenceFromUserInput(
    const std::string& name);
H5GEO_EXPORT void setSpatialReferenceFromUserInput(
    const std::string& authName, const std::string& code);
H5GEO_EXPORT OGRSpatialReference getSpatialReference();

H5GEO_EXPORT void setLengthUnits(const std::string& units);
H5GEO_EXPORT void setAngularUnits(const std::string& units);
H5GEO_EXPORT void setTemporalUnits(const std::string& units);

H5GEO_EXPORT std::string getLengthUnits();
H5GEO_EXPORT std::string getAngularUnits();
H5GEO_EXPORT std::string getTemporalUnits();

H5GEO_EXPORT void setDomain(const std::string& domain);
H5GEO_EXPORT void setDomain(const h5geo::Domain& domain);

H5GEO_EXPORT std::string getDomain();
H5GEO_EXPORT h5geo::Domain getDomainEnum();


} // sr


} // h5geo


#endif // H5CORE_SR_SETTINGS_H
