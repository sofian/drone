build_path=./

TARGETS = drone_artkp_kernel_Tracker 


all: 
	${MAKE} $(build_path)libTracker.so
	${MAKE} $(build_path)libTrackerMultiMarker.so
	${MAKE} $(build_path)libTrackerMultiMarkerImpl.so

drone_artkp_kernel_Tracker.o: drone_artkp_kernel_Tracker.cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o drone_artkp_kernel_Tracker.o drone_artkp_kernel_Tracker.cpp

$(build_path)libTracker.so: drone_artkp_kernel_Tracker.o
	g++ -shared -Wl,-soname,libTracker.so -o $(build_path)libTracker.so drone_artkp_kernel_Tracker.o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

drone_artkp_kernel_TrackerMultiMarker.o: drone_artkp_kernel_TrackerMultiMarker.cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o drone_artkp_kernel_TrackerMultiMarker.o drone_artkp_kernel_TrackerMultiMarker.cpp

$(build_path)libTrackerMultiMarker.so: drone_artkp_kernel_TrackerMultiMarker.o
	g++ -shared -Wl,-soname,libTrackerMultiMarker.so -o $(build_path)libTrackerMultiMarker.so drone_artkp_kernel_TrackerMultiMarker.o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

drone_artkp_kernel_TrackerMultiMarkerImpl.o: drone_artkp_kernel_TrackerMultiMarkerImpl.cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o drone_artkp_kernel_TrackerMultiMarkerImpl.o drone_artkp_kernel_TrackerMultiMarkerImpl.cpp

$(build_path)libTrackerMultiMarkerImpl.so: drone_artkp_kernel_TrackerMultiMarkerImpl.o
	g++ -shared -Wl,-soname,libTrackerMultiMarkerImpl.so -o $(build_path)libTrackerMultiMarkerImpl.so drone_artkp_kernel_TrackerMultiMarkerImpl.o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

clean: 
	rm drone_artkp_kernel_Tracker.o $(build_path)libTracker.so
	rm drone_artkp_kernel_TrackerMultiMarker.o $(build_path)libTrackerMultiMarker.so
	rm drone_artkp_kernel_TrackerMultiMarkerImpl.o $(build_path)libTrackerMultiMarkerImpl.so
