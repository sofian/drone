include (../../../../../config.pro)

TEMPLATE=lib

SOURCES+=Gear_TA_MapInfoLayer.cpp \
  ../../../../../lib/agg23/svg/agg_svg_parser.cpp \
  ../../../../../lib/agg23/svg/agg_svg_path_renderer.cpp \
  ../../../../../lib/agg23/svg/agg_svg_path_tokenizer.cpp

HEADERS+=Gear_TA_MapInfoLayer.h


INCLUDEPATH+=../../../../core/ ../../core/ ../../../../core/types ../../../../../lib/agg23/include ../../../../../lib/agg23/svg
unix:!macx:LIBS+=-L../../../../../lib/ -ldroneCore -ldroneTACore -L../../../../../lib/agg23/src/ -lagg
unix:!macx:TARGET=../../../../../gears/Gear_TA_MapInfoLayer
unix:!macx:QMAKE_RPATH = -Wl,-rpath,lib/,-rpath,        

#osx
macx:LIBS+=-L../../../../../drone.app/Contents/Frameworks -ldroneCore -ldroneTACore -L../../../../../lib/
macx:TARGET=../../../../../drone.app/Contents/PlugIns/Gear_TA_MapInfoLayer
macx:QMAKE_LFLAGS+=-read_only_relocs suppress

