BUILD_PATH=./lib/
NAME_TRACKER=drone_artkp_kernel_Tracker
NAME_TRACKER_MULTI_MARKER=drone_artkp_kernel_TrackerMultiMarker
NAME_TRACKER_MULTI_MARKER_IMPL=drone_artkp_kernel_TrackerMultiMarkerImpl

all: 
	${MAKE} $(BUILD_PATH)lib$(NAME_TRACKER).so
	${MAKE} $(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER).so
	${MAKE} $(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER_IMPL).so

$(NAME_TRACKER).o: $(NAME_TRACKER).cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o $(NAME_TRACKER).o $(NAME_TRACKER).cpp

$(BUILD_PATH)lib$(NAME_TRACKER).so: $(NAME_TRACKER).o
	g++ -shared -Wl,-soname,lib$(NAME_TRACKER).so -o $(BUILD_PATH)lib$(NAME_TRACKER).so $(NAME_TRACKER).o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

$(NAME_TRACKER_MULTI_MARKER).o: $(NAME_TRACKER_MULTI_MARKER).cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o $(NAME_TRACKER_MULTI_MARKER).o $(NAME_TRACKER_MULTI_MARKER).cpp

$(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER).so: $(NAME_TRACKER_MULTI_MARKER).o
	g++ -shared -Wl,-soname,lib$(NAME_TRACKER_MULTI_MARKER).so -o $(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER).so $(NAME_TRACKER_MULTI_MARKER).o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

$(NAME_TRACKER_MULTI_MARKER_IMPL).o: $(NAME_TRACKER_MULTI_MARKER_IMPL).cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o $(NAME_TRACKER_MULTI_MARKER_IMPL).o $(NAME_TRACKER_MULTI_MARKER_IMPL).cpp

$(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER_IMPL).so: $(NAME_TRACKER_MULTI_MARKER_IMPL).o
	g++ -shared -Wl,-soname,lib$(NAME_TRACKER_MULTI_MARKER_IMPL).so -o $(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER_IMPL).so $(NAME_TRACKER_MULTI_MARKER_IMPL).o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

clean:
	rm $(NAME_TRACKER).o $(BUILD_PATH)lib$(NAME_TRACKER).so
	rm $(NAME_TRACKER_MULTI_MARKER).o $(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER).so
	rm $(NAME_TRACKER_MULTI_MARKER_IMPL).o $(BUILD_PATH)lib$(NAME_TRACKER_MULTI_MARKER_IMPL).so
