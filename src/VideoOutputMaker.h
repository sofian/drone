#ifndef VIDEOOUTPUT_MAKER_INCLUDED
#define VIDEOOUTPUT_MAKER_INCLUDED

#include <map>
#include <string>
#include <vector>

class VideoOutput;

class VideoOutputMaker  
{
public:
  VideoOutputMaker(std::string type);

  static VideoOutput* makeVideoOutput(std::string type);
  static void getAllVideoOutputsType(std::vector<std::string> &videoOutputTypes);

private:
  static std::map<std::string, VideoOutputMaker*> *_Registry;
  virtual VideoOutput* internalMakeVideoOutput()=0;
};

#define Register_VideoOutput(MMAKERNAME, MCLASSNAME, MTYPE) \
class MMAKERNAME : public VideoOutputMaker \
{ \
public: \
    MMAKERNAME() : VideoOutputMaker(MTYPE){}; \
private: \
    VideoOutput* internalMakeVideoOutput() \
    { \
        return new MCLASSNAME();\
    } \
    static MMAKERNAME _RegisterThis;\
};\
MMAKERNAME MMAKERNAME::_RegisterThis;



#endif 
