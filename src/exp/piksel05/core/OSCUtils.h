#ifndef OSCUTILS_INCLUDED
#define OSCUTILS_INCLUDED

#include "StringUtils.h"
#include <iostream>
#include <vector>

#define OSC_INDEX_TIMESTAMP 0
#define OSC_INDEX_PATH      1
#define OSC_INDEX_TYPES     2
#define OSC_INDEX_VALUES    3

struct OSCData
{
  double timestamp;
  std::string path;
  std::string types;
  std::vector<std::string> values;
  OSCData() {}
  OSCData(double timestamp_,
          const std::string path_,
          const std::string types_,
          const std::vector<std::string> values_)
    : timestamp(timestamp_), path(path_), types(types_), values(values_)
  {}
};

OSCData string2data(const std::string& line);
std::vector<OSCData> get_data_from_path(const std::string& path, std::istream& in);

#endif
