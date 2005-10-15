#include "OSCUtils.h"

OSCData string2data(const std::string& line)
{
  std::vector<std::string> splitted = split(line);
  return OSCData(todouble(splitted[OSC_INDEX_TIMESTAMP]),
                 splitted[OSC_INDEX_PATH],
                 splitted[OSC_INDEX_TYPES],
                 split(splitted[OSC_INDEX_VALUES]));
}

std::vector<OSCData> get_data_from_path(const std::string& path, std::istream& in)
{
  std::vector<OSCData> vec;
  char buffer[1024];
  while (!in.eof())
  {
    std::string word;
    in >> word;
    //std::string line = pgetline(in);
    in.getline(buffer, 1024);
    std::string line (buffer);
    std::cout << "line : [" << line << "]" << std::endl;
    if (line == "")
      continue;
    OSCData data = string2data(line);
    if (data.path == path)
      vec.push_back(data);
  }
  return vec;
}
