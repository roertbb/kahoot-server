# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/roertbb/CLionProjects/kahoot-it-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/roertbb/CLionProjects/kahoot-it-server

# Include any dependencies generated for this target.
include CMakeFiles/kahoot_server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/kahoot_server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kahoot_server.dir/flags.make

CMakeFiles/kahoot_server.dir/main.cpp.o: CMakeFiles/kahoot_server.dir/flags.make
CMakeFiles/kahoot_server.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/kahoot_server.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kahoot_server.dir/main.cpp.o -c /home/roertbb/CLionProjects/kahoot-it-server/main.cpp

CMakeFiles/kahoot_server.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kahoot_server.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roertbb/CLionProjects/kahoot-it-server/main.cpp > CMakeFiles/kahoot_server.dir/main.cpp.i

CMakeFiles/kahoot_server.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kahoot_server.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roertbb/CLionProjects/kahoot-it-server/main.cpp -o CMakeFiles/kahoot_server.dir/main.cpp.s

CMakeFiles/kahoot_server.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/kahoot_server.dir/main.cpp.o.requires

CMakeFiles/kahoot_server.dir/main.cpp.o.provides: CMakeFiles/kahoot_server.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/kahoot_server.dir/build.make CMakeFiles/kahoot_server.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/kahoot_server.dir/main.cpp.o.provides

CMakeFiles/kahoot_server.dir/main.cpp.o.provides.build: CMakeFiles/kahoot_server.dir/main.cpp.o


CMakeFiles/kahoot_server.dir/src/Server.cpp.o: CMakeFiles/kahoot_server.dir/flags.make
CMakeFiles/kahoot_server.dir/src/Server.cpp.o: src/Server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/kahoot_server.dir/src/Server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kahoot_server.dir/src/Server.cpp.o -c /home/roertbb/CLionProjects/kahoot-it-server/src/Server.cpp

CMakeFiles/kahoot_server.dir/src/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kahoot_server.dir/src/Server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roertbb/CLionProjects/kahoot-it-server/src/Server.cpp > CMakeFiles/kahoot_server.dir/src/Server.cpp.i

CMakeFiles/kahoot_server.dir/src/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kahoot_server.dir/src/Server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roertbb/CLionProjects/kahoot-it-server/src/Server.cpp -o CMakeFiles/kahoot_server.dir/src/Server.cpp.s

CMakeFiles/kahoot_server.dir/src/Server.cpp.o.requires:

.PHONY : CMakeFiles/kahoot_server.dir/src/Server.cpp.o.requires

CMakeFiles/kahoot_server.dir/src/Server.cpp.o.provides: CMakeFiles/kahoot_server.dir/src/Server.cpp.o.requires
	$(MAKE) -f CMakeFiles/kahoot_server.dir/build.make CMakeFiles/kahoot_server.dir/src/Server.cpp.o.provides.build
.PHONY : CMakeFiles/kahoot_server.dir/src/Server.cpp.o.provides

CMakeFiles/kahoot_server.dir/src/Server.cpp.o.provides.build: CMakeFiles/kahoot_server.dir/src/Server.cpp.o


CMakeFiles/kahoot_server.dir/src/Client.cpp.o: CMakeFiles/kahoot_server.dir/flags.make
CMakeFiles/kahoot_server.dir/src/Client.cpp.o: src/Client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/kahoot_server.dir/src/Client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kahoot_server.dir/src/Client.cpp.o -c /home/roertbb/CLionProjects/kahoot-it-server/src/Client.cpp

CMakeFiles/kahoot_server.dir/src/Client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kahoot_server.dir/src/Client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roertbb/CLionProjects/kahoot-it-server/src/Client.cpp > CMakeFiles/kahoot_server.dir/src/Client.cpp.i

CMakeFiles/kahoot_server.dir/src/Client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kahoot_server.dir/src/Client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roertbb/CLionProjects/kahoot-it-server/src/Client.cpp -o CMakeFiles/kahoot_server.dir/src/Client.cpp.s

CMakeFiles/kahoot_server.dir/src/Client.cpp.o.requires:

.PHONY : CMakeFiles/kahoot_server.dir/src/Client.cpp.o.requires

CMakeFiles/kahoot_server.dir/src/Client.cpp.o.provides: CMakeFiles/kahoot_server.dir/src/Client.cpp.o.requires
	$(MAKE) -f CMakeFiles/kahoot_server.dir/build.make CMakeFiles/kahoot_server.dir/src/Client.cpp.o.provides.build
.PHONY : CMakeFiles/kahoot_server.dir/src/Client.cpp.o.provides

CMakeFiles/kahoot_server.dir/src/Client.cpp.o.provides.build: CMakeFiles/kahoot_server.dir/src/Client.cpp.o


CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o: CMakeFiles/kahoot_server.dir/flags.make
CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o: src/Kahoot.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o -c /home/roertbb/CLionProjects/kahoot-it-server/src/Kahoot.cpp

CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roertbb/CLionProjects/kahoot-it-server/src/Kahoot.cpp > CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.i

CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roertbb/CLionProjects/kahoot-it-server/src/Kahoot.cpp -o CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.s

CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.requires:

.PHONY : CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.requires

CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.provides: CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.requires
	$(MAKE) -f CMakeFiles/kahoot_server.dir/build.make CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.provides.build
.PHONY : CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.provides

CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.provides.build: CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o


CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o: CMakeFiles/kahoot_server.dir/flags.make
CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o: src/Buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o -c /home/roertbb/CLionProjects/kahoot-it-server/src/Buffer.cpp

CMakeFiles/kahoot_server.dir/src/Buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/kahoot_server.dir/src/Buffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/roertbb/CLionProjects/kahoot-it-server/src/Buffer.cpp > CMakeFiles/kahoot_server.dir/src/Buffer.cpp.i

CMakeFiles/kahoot_server.dir/src/Buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/kahoot_server.dir/src/Buffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/roertbb/CLionProjects/kahoot-it-server/src/Buffer.cpp -o CMakeFiles/kahoot_server.dir/src/Buffer.cpp.s

CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.requires:

.PHONY : CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.requires

CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.provides: CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.requires
	$(MAKE) -f CMakeFiles/kahoot_server.dir/build.make CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.provides.build
.PHONY : CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.provides

CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.provides.build: CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o


# Object files for target kahoot_server
kahoot_server_OBJECTS = \
"CMakeFiles/kahoot_server.dir/main.cpp.o" \
"CMakeFiles/kahoot_server.dir/src/Server.cpp.o" \
"CMakeFiles/kahoot_server.dir/src/Client.cpp.o" \
"CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o" \
"CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o"

# External object files for target kahoot_server
kahoot_server_EXTERNAL_OBJECTS =

kahoot_server: CMakeFiles/kahoot_server.dir/main.cpp.o
kahoot_server: CMakeFiles/kahoot_server.dir/src/Server.cpp.o
kahoot_server: CMakeFiles/kahoot_server.dir/src/Client.cpp.o
kahoot_server: CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o
kahoot_server: CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o
kahoot_server: CMakeFiles/kahoot_server.dir/build.make
kahoot_server: CMakeFiles/kahoot_server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable kahoot_server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kahoot_server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kahoot_server.dir/build: kahoot_server

.PHONY : CMakeFiles/kahoot_server.dir/build

CMakeFiles/kahoot_server.dir/requires: CMakeFiles/kahoot_server.dir/main.cpp.o.requires
CMakeFiles/kahoot_server.dir/requires: CMakeFiles/kahoot_server.dir/src/Server.cpp.o.requires
CMakeFiles/kahoot_server.dir/requires: CMakeFiles/kahoot_server.dir/src/Client.cpp.o.requires
CMakeFiles/kahoot_server.dir/requires: CMakeFiles/kahoot_server.dir/src/Kahoot.cpp.o.requires
CMakeFiles/kahoot_server.dir/requires: CMakeFiles/kahoot_server.dir/src/Buffer.cpp.o.requires

.PHONY : CMakeFiles/kahoot_server.dir/requires

CMakeFiles/kahoot_server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kahoot_server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kahoot_server.dir/clean

CMakeFiles/kahoot_server.dir/depend:
	cd /home/roertbb/CLionProjects/kahoot-it-server && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/roertbb/CLionProjects/kahoot-it-server /home/roertbb/CLionProjects/kahoot-it-server /home/roertbb/CLionProjects/kahoot-it-server /home/roertbb/CLionProjects/kahoot-it-server /home/roertbb/CLionProjects/kahoot-it-server/CMakeFiles/kahoot_server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kahoot_server.dir/depend

