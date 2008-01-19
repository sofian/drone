SUBDIRS = drone_artkp_kernel_Tracker drone_artkp_kernel_TrackerMultiMarker drone_artkp_kernel_TrackerMultiMarkerImpl

all:
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} all; cp lib/* ../lib ) ; done
	
clean:
	for dir in ${SUBDIRS} ; do ( cd $$dir ; ${MAKE} clean ) ; done
