#include "MidiEngine.h"
#include "error.h"

MidiMessage::MidiMessage(std::vector<unsigned char> vec, float stamp):_stamp(stamp)
{
  _status=_byte1=_byte2=_byte3=0;

  if(vec.size()>0)
    _status=vec[0];
  if(vec.size()>1)
    _byte1=vec[1];
  if(vec.size()>2)
    _byte2=vec[2];
  if(vec.size()>3)
    _byte3=vec[3];    
  
  // si jamais vous passez pas ce assert, donnez moi un coup de fil (Julien)
  ASSERT_ERROR(vec.size()<=4);

  std::cout<<" new midimessage stamp : "<<_stamp<<" vecsize:"<<vec.size()<<std::endl;
}

MidiEngine::MidiEngine()
{
  _midiin = new RtMidiIn();
  
  // Check available ports.
  unsigned int nPorts = _midiin->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "MidiEngine: No ports available!\n";
    delete _midiin;
    _midiin=NULL;
    return;
  }
  _midiin->openPort( 0 );

  // Don't ignore sysex, timing, or active sensing messages.
  _midiin->ignoreTypes( false, false, false );
}

void MidiEngine::clearAllMessages()
{
  for(unsigned int i=0;i<_messages.size();i++)
    delete _messages[i];
  _messages.clear();
}

// erases _messages and fill it with new pending MIDI messages
void MidiEngine::purgeAndGetNew()
{
  int nBytes;
  int count=0;
  double stamp;
  std::vector<unsigned char> msg;
  
  if(_midiin==NULL)
    return;
  
  clearAllMessages();
  
  do
  {
    stamp = _midiin->getMessage( &msg );
    nBytes = msg.size();
    for ( int i=0; i<nBytes; i++ )
      std::cout << "Byte " << i << " = " << (int)msg[i] << ", ";
    if ( nBytes > 0 )
    {
      _messages.push_back(new MidiMessage(msg,stamp));
      std::cout << "stamp = " << stamp << '\n';
      count++;
    }
  }
  while(nBytes > 0);
  //std::cout<<"count : "<<count<<std::endl;
}

MidiEngine::~MidiEngine()
{
  delete _midiin;
}

MidiEngine& MidiEngine::getInstance()
{
  static MidiEngine instance;

  return instance;
}

std::vector<MidiMessage*> MidiEngine::getMessages()
{
  return _messages;
}
