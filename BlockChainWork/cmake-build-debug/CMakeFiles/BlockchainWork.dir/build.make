# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/zhangyuxuan/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/zhangyuxuan/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/212.5457.51/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/BlockchainWork.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/BlockchainWork.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/BlockchainWork.dir/flags.make

CMakeFiles/BlockchainWork.dir/main.cpp.o: CMakeFiles/BlockchainWork.dir/flags.make
CMakeFiles/BlockchainWork.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/BlockchainWork.dir/main.cpp.o"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/BlockchainWork.dir/main.cpp.o -c /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/main.cpp

CMakeFiles/BlockchainWork.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/BlockchainWork.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/main.cpp > CMakeFiles/BlockchainWork.dir/main.cpp.i

CMakeFiles/BlockchainWork.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/BlockchainWork.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/main.cpp -o CMakeFiles/BlockchainWork.dir/main.cpp.s

# Object files for target BlockchainWork
BlockchainWork_OBJECTS = \
"CMakeFiles/BlockchainWork.dir/main.cpp.o"

# External object files for target BlockchainWork
BlockchainWork_EXTERNAL_OBJECTS =

../build/output/bin/BlockchainWork: CMakeFiles/BlockchainWork.dir/main.cpp.o
../build/output/bin/BlockchainWork: CMakeFiles/BlockchainWork.dir/build.make
../build/output/bin/BlockchainWork: ../build/output/lib/libUTIL_LIBRARY.a
../build/output/bin/BlockchainWork: ../build/output/lib/libBLOCKCHAIN_LIBRARY.a
../build/output/bin/BlockchainWork: ../build/output/lib/libTEST_LIBRARY.a
../build/output/bin/BlockchainWork: CMakeFiles/BlockchainWork.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../build/output/bin/BlockchainWork"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/BlockchainWork.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/BlockchainWork.dir/build: ../build/output/bin/BlockchainWork
.PHONY : CMakeFiles/BlockchainWork.dir/build

CMakeFiles/BlockchainWork.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/BlockchainWork.dir/cmake_clean.cmake
.PHONY : CMakeFiles/BlockchainWork.dir/clean

CMakeFiles/BlockchainWork.dir/depend:
	cd /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug /Users/zhangyuxuan/WordAndStudy/ClionProjects/BlockChainWork/cmake-build-debug/CMakeFiles/BlockchainWork.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/BlockchainWork.dir/depend

