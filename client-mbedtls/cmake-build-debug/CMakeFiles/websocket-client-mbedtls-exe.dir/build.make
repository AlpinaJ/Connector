# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/73/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/73/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/julia/Desktop/intership/example-websocket/client-mbedtls

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/websocket-client-mbedtls-exe.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/websocket-client-mbedtls-exe.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/websocket-client-mbedtls-exe.dir/flags.make

CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.o: CMakeFiles/websocket-client-mbedtls-exe.dir/flags.make
CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.o: ../src/App.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.o -c /home/julia/Desktop/intership/example-websocket/client-mbedtls/src/App.cpp

CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julia/Desktop/intership/example-websocket/client-mbedtls/src/App.cpp > CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.i

CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julia/Desktop/intership/example-websocket/client-mbedtls/src/App.cpp -o CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.s

# Object files for target websocket-client-mbedtls-exe
websocket__client__mbedtls__exe_OBJECTS = \
"CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.o"

# External object files for target websocket-client-mbedtls-exe
websocket__client__mbedtls__exe_EXTERNAL_OBJECTS =

websocket-client-mbedtls-exe: CMakeFiles/websocket-client-mbedtls-exe.dir/src/App.cpp.o
websocket-client-mbedtls-exe: CMakeFiles/websocket-client-mbedtls-exe.dir/build.make
websocket-client-mbedtls-exe: libwebsocket-client-mbedtls-lib.a
websocket-client-mbedtls-exe: /usr/local/lib/oatpp-0.19.4/liboatpp-websocket.a
websocket-client-mbedtls-exe: /usr/local/lib/oatpp-0.19.4/liboatpp-mbedtls.a
websocket-client-mbedtls-exe: /usr/local/lib/oatpp-0.19.4/liboatpp-test.a
websocket-client-mbedtls-exe: /usr/local/lib/oatpp-0.19.4/liboatpp.a
websocket-client-mbedtls-exe: /usr/local/lib/libmbedtls.a
websocket-client-mbedtls-exe: /usr/local/lib/libmbedx509.a
websocket-client-mbedtls-exe: /usr/local/lib/libmbedcrypto.a
websocket-client-mbedtls-exe: CMakeFiles/websocket-client-mbedtls-exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable websocket-client-mbedtls-exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/websocket-client-mbedtls-exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/websocket-client-mbedtls-exe.dir/build: websocket-client-mbedtls-exe

.PHONY : CMakeFiles/websocket-client-mbedtls-exe.dir/build

CMakeFiles/websocket-client-mbedtls-exe.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/websocket-client-mbedtls-exe.dir/cmake_clean.cmake
.PHONY : CMakeFiles/websocket-client-mbedtls-exe.dir/clean

CMakeFiles/websocket-client-mbedtls-exe.dir/depend:
	cd /home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/julia/Desktop/intership/example-websocket/client-mbedtls /home/julia/Desktop/intership/example-websocket/client-mbedtls /home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug /home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug /home/julia/Desktop/intership/example-websocket/client-mbedtls/cmake-build-debug/CMakeFiles/websocket-client-mbedtls-exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/websocket-client-mbedtls-exe.dir/depend

