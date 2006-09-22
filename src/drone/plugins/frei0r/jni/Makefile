build_path=../../../../../build/drone/plugins/frei0r/
drone_plugins_frei0r_actors_Frei0r.o : drone_plugins_frei0r_actors_Frei0r.cpp
	g++ -g -I/System/Library/Frameworks/JavaVM.framework/Headers -Icontrib/Frei0r/spec -c -o drone_plugins_frei0r_actors_Frei0r.o drone_plugins_frei0r_actors_Frei0r.cpp
$(build_path)libFrei0rJNI.jnilib : drone_plugins_frei0r_actors_Frei0r.o 
	g++ -bundle -framework JavaVM -o $(build_path)libFrei0rJNI.jnilib drone_plugins_frei0r_actors_Frei0r.o
all : 
	${MAKE} $(build_path)libFrei0rJNI.jnilib
clean : 
	rm $(build_path)libFrei0rJNI.jnilib

 
 