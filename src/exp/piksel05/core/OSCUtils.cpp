#include "OSCUtils.h"

OSCData string2data(const std::string& line)
{
  std::vector<std::string> splitted = split(line);
  return OSCData(todouble(splitted[OSC_INDEX_TIMESTAMP]),
                 splitted[OSC_INDEX_PATH],
                 splitted[OSC_INDEX_TYPES],
                 split(splitted[OSC_INDEX_VALUES]));
}

std::vector<OSCData> get_data_from_path(const std::string& path, const std::string& in)
{
  std::vector<OSCData> vec;
  std::vector<std::string> lines = getNonBlankLines(in);
  for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); ++it)
  {
    OSCData data = string2data(*it);
    if (data.path == path)
      vec.push_back(data);
  }
  return vec;
}
