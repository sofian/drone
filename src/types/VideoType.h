#ifndef VIDEOTYPE_INCLUDED
#define VIDEOTYPE_INCLUDED

class VideoType : public AbstractType
{
public:
  VideoType() : _canvas(0) 
  {
    _canvas = new Canvas();
  }

  virtual ~VideoType() { delete _canvas;}

  std::string name() const { return "VideoType";}
  QColor color() const { return QColor(31,47,124);}

  Canvas* canvas() const { return _canvas;}

private:
  Canvas *_canvas;
};

#endif //VIDEOTYPE_INCLUDED
