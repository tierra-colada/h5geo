#include "../include/h5geo/h5core.h"
#include "../include/h5geo/misc/h5segy_headers.h"

#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <bitset>

namespace h5geo
{

namespace details
{
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

std::string getRelativePath(
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

void getTraceHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> traceHeaderNames = getRawTraceHeaderNames();

  details::splitHeaderNames(traceHeaderNames, fullHeaderNames, shortHeaderNames);
}

void getBinHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> binHeaderNames = getRawBinHeaderNames();

  details::splitHeaderNames(binHeaderNames, fullHeaderNames, shortHeaderNames);
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
    const std::string& attributeName)
{
  if (!dataset.hasAttribute(attributeName))
    return -1;

  ptrdiff_t idx;
  dataset.getAttribute(attributeName).read(idx);
  return idx;
}

} // namspace details

} // namespace h5geo
