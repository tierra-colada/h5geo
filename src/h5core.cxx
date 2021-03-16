#include "../include/h5core/h5core.h"
#include "../include/h5core/h5segy_headers.h"

#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <bitset>

using namespace h5geo;

std::regex h5geo::fileNameRegExp(){
  const std::regex re(R"RX(\A(?!(?:COM[0-9]|CON|LPT[0-9]|NUL|PRN|AUX|com[0-9]|con|lpt[0-9]|nul|prn|aux)|\s|[\.]{2,})[^\\\/:*"?<>|]{1,254}(?<![\s\.])\z)RX");
  return re;
}

std::regex h5geo::fileNameNoSpaceRegExp(){
  const std::regex re(R"RX(\A(?!(?:COM[0-9]|CON|LPT[0-9]|NUL|PRN|AUX|com[0-9]|con|lpt[0-9]|nul|prn|aux)|[\.]{2,})[^\\\/:*"?<>|\s]{1,254}(?<![\.])\z)RX");
  return re;
}

std::regex h5geo::floatRegExp(){
  const std::regex re("[+-]?([0-9]*[.])?[0-9]+");
  return re;
}

std::string h5geo::fileNameToolTipText(){
  std::string str(R"(File name must not contain the following characters: \ / : * ? " < > |)");
  return str;
}

std::string h5geo::fileNameNoSpaceToolTipText(){
  std::string str(R"(File name must not contain the following characters: \ / : * ? " < > | or white-space)");
  return str;
}

void h5geo::splitHeaderNames(const std::vector<std::string> &headerNamesToSplit,
                             std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  fullHeaderNames.clear();
  shortHeaderNames.clear();
  for (size_t i = 0; i < headerNamesToSplit.size(); i++){
    std::vector<std::string> v = splitString(headerNamesToSplit[i], "\t");
    fullHeaderNames.push_back(v[0]);
    shortHeaderNames.push_back(v[1]);
  }
}

void h5geo::getTraceHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> traceHeaderNames = getRawTraceHeaderNames();
  
  splitHeaderNames(traceHeaderNames, fullHeaderNames, shortHeaderNames);
}

void h5geo::getBinHeaderNames(std::vector<std::string> &fullHeaderNames, std::vector<std::string> &shortHeaderNames){
  std::vector<std::string> binHeaderNames = getRawBinHeaderNames();

  splitHeaderNames(binHeaderNames, fullHeaderNames, shortHeaderNames);
}

size_t h5geo::getTraceHeaderCount(){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  getTraceHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames.size();
}

size_t h5geo::getBinHeaderCount(){
  std::vector<std::string> fullHeaderNames, shortHeaderNames;
  getBinHeaderNames(fullHeaderNames, shortHeaderNames);
  return shortHeaderNames.size();
}

std::string h5geo::generateName(const std::vector<std::string> &nameList, std::string baseName){
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

std::vector<std::string> h5geo::splitString(const std::string &s, const std::string delimiter) {
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

bool h5geo::isFileExist(const std::string& name){
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

ptrdiff_t h5geo::getIndexFromAttribute(
    h5gt::DataSet& dataset,
    const std::string& attributeName)
{ 
  if (!dataset.hasAttribute(attributeName))
    return -1;

  ptrdiff_t idx;
  dataset.getAttribute(attributeName).read(idx);
  return idx;
}

std::vector<std::string> h5geo::splitPath(
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

std::vector<std::string> h5geo::splitPath(
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

std::string h5geo::splitPathToParentAndObj(
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

bool h5geo::compareStrings(
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
  }
  }

}

std::string h5geo::getRelativePath(
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

void h5geo::eraseSubStr(
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

void h5geo::eraseAllSubStr(
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

float h5geo::ibm2ieee(const int& from){
  std::bitset<32> bits_from(from);
  float to;
  const int constnum1 = 2.130706432e+09; // hex2dec('7f000000') (Matlab)
  const int constnum2 = 16777215; // hex2dec('00ffffff') (Matlab)
  const float constnum3 = 16777216; // 2^24

  float sgn = (1-2*bits_from[31]);
  float expon = (from & constnum1) >> 24;
  float mantiss = (from & constnum2);

  to = sgn*float(pow(16, expon-64))*mantiss/constnum3;

  return to;
}

unsigned char h5geo::ebc_to_ascii(
    const unsigned char& ascii)
{
  unsigned char asc;

  int Tableebc2Asc[256]=
  {
    32,  32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 91, 46, 60, 40, 43, 33,
    38, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 93, 36, 42, 41, 59, 94,
    45, 47, 32, 32, 32, 32, 32, 32,
    32, 32,124, 44, 37, 95, 62, 63,
    32, 32, 32, 32, 32, 32,238,160,
    161, 96, 58, 35, 64, 39, 61, 34,
    230, 97, 98, 99,100,101,102,103,
    104,105,164,165,228,163,229,168,
    169,106,107,108,109,110,111,112,
    113,114,170,171,172,173,174,175,
    239,126,115,116,117,118,119,120,
    121,122,224,225,226,227,166,162,
    236,235,167,232,237,233,231,234,
    158,128,129,150,132,133,148,131,
    123, 65, 66, 67, 68, 69, 70, 71,
    72, 73,149,136,137,138,139,140,
    125, 74, 75, 76, 77, 78, 79, 80,
    81, 82,141,142,143,159,144,145,
    92, 32, 83, 84, 85, 86, 87, 88,
    89, 90,146,147,134,130,156,155,
    48, 49, 50, 51, 52, 53, 54, 55,
    56, 57,135,152,157,153,151, 32
  };

  asc = Tableebc2Asc[ascii];
  return (asc);
}
