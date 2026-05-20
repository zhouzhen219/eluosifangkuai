# CMake generated Testfile for 
# Source directory: D:/a_computer/game/eluosifangkuai
# Build directory: D:/a_computer/game/eluosifangkuai/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[SmokeTest]=] "D:/a_computer/game/eluosifangkuai/build/bin/main.exe")
set_tests_properties([=[SmokeTest]=] PROPERTIES  WORKING_DIRECTORY "D:/a_computer/game/eluosifangkuai/build/bin" _BACKTRACE_TRIPLES "D:/a_computer/game/eluosifangkuai/CMakeLists.txt;42;add_test;D:/a_computer/game/eluosifangkuai/CMakeLists.txt;0;")
subdirs("_deps/sfml-build")
