build_path=./

TARGETS = ptolemy_actor_lib_artkp_kernel_Tracker 


all: 
	${MAKE} $(build_path)libTracker.so
	${MAKE} $(build_path)libTrackerMultiMarker.so
	${MAKE} $(build_path)libTrackerMultiMarkerImpl.so

ptolemy_actor_lib_artkp_kernel_Tracker.o: ptolemy_actor_lib_artkp_kernel_Tracker.cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o ptolemy_actor_lib_artkp_kernel_Tracker.o ptolemy_actor_lib_artkp_kernel_Tracker.cpp

$(build_path)libTracker.so: ptolemy_actor_lib_artkp_kernel_Tracker.o
	g++ -shared -Wl,-soname,libTracker.so -o $(build_path)libTracker.so ptolemy_actor_lib_artkp_kernel_Tracker.o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.o: ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.o ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.cpp

$(build_path)libTrackerMultiMarker.so: ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.o
	g++ -shared -Wl,-soname,libTrackerMultiMarker.so -o $(build_path)libTrackerMultiMarker.so ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.o: ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.cpp
	g++ -mtune=pentium4 -march=pentium4 -msse2 -msse -O2 -fPIC -I$(ARTKP)/include/ -c -o ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.o ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.cpp

$(build_path)libTrackerMultiMarkerImpl.so: ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.o
	g++ -shared -Wl,-soname,libTrackerMultiMarkerImpl.so -o $(build_path)libTrackerMultiMarkerImpl.so ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.o -static -L$(ARTKP)/lib/ -lc -lARToolKitPlus

clean: 
	rm ptolemy_actor_lib_artkp_kernel_Tracker.o $(build_path)libTracker.so
	rm ptolemy_actor_lib_artkp_kernel_TrackerMultiMarker.o $(build_path)libTrackerMultiMarker.so
	rm ptolemy_actor_lib_artkp_kernel_TrackerMultiMarkerImpl.o $(build_path)libTrackerMultiMarkerImpl.so
