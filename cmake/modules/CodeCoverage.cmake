#
# 2012-01-31, Lars Bilke
# - Enable Code Coverage
#
# 2013-09-17, Joakim Söderberg
# - Added support for Clang.
# - Some additional usage instructions.
#
# USAGE:

# 0. (Mac only) If you use Xcode 5.1 make sure to patch geninfo as described here:
#      http://stackoverflow.com/a/22404544/80480
#
# 1. Copy this file into your cmake modules path.
#
# 2. Add the following line to your CMakeLists.txt:
#      INCLUDE(CodeCoverage)
#
# 3. Set compiler flags to turn off optimization and enable coverage:
#    SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
#	 SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
#
# 3. Use the function SETUP_TARGET_FOR_COVERAGE to create a custom make target
#    which runs your test executable and produces a lcov code coverage report:
#    Example:
#	 SETUP_TARGET_FOR_COVERAGE(
#				my_coverage_target  # Name for custom target.
#				test_driver         # Name of the test driver executable that runs the tests.
#									# NOTE! This should always have a ZERO as exit code
#									# otherwise the coverage generation will not complete.
#				coverage            # Name of output directory.
#				)
#
# 4. Build a Debug build:
#	 cmake -DCMAKE_BUILD_TYPE=Debug ..
#	 make
#	 make my_coverage_target
#
#

# Check prereqs
FIND_PROGRAM( GCOV_PATH gcov )
FIND_PROGRAM( LCOV_PATH lcov )
FIND_PROGRAM( GENHTML_PATH genhtml )
FIND_PROGRAM( GCOVR_PATH gcovr PATHS ${CMAKE_SOURCE_DIR}/tests)

IF(NOT GCOV_PATH)
	MESSAGE(FATAL_ERROR "gcov not found! Aborting...")
ENDIF() # NOT GCOV_PATH

IF(NOT CMAKE_COMPILER_IS_GNUCXX)
	# Clang version 3.0.0 and greater now supports gcov as well.
	MESSAGE(WARNING "Compiler is not GNU gcc! Clang Version 3.0.0 and greater supports gcov as well, but older versions don't.")

	IF(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		MESSAGE(FATAL_ERROR "Compiler is not GNU gcc! Aborting...")
	ENDIF()
ENDIF() # NOT CMAKE_COMPILER_IS_GNUCXX

SET(CMAKE_CXX_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fprofile-update=atomic"
    CACHE STRING "Flags used by the C++ compiler during coverage builds."
    FORCE )
SET(CMAKE_C_FLAGS_COVERAGE
    "-g -O0 --coverage -fprofile-arcs -ftest-coverage -fprofile-update=atomic"
    CACHE STRING "Flags used by the C compiler during coverage builds."
    FORCE )
SET(CMAKE_EXE_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used for linking binaries during coverage builds."
    FORCE )
SET(CMAKE_SHARED_LINKER_FLAGS_COVERAGE
    ""
    CACHE STRING "Flags used by the shared libraries linker during coverage builds."
    FORCE )
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_COVERAGE
    CMAKE_C_FLAGS_COVERAGE
    CMAKE_EXE_LINKER_FLAGS_COVERAGE
    CMAKE_SHARED_LINKER_FLAGS_COVERAGE )

IF ( NOT (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "Coverage"))
  MESSAGE( WARNING "Code coverage results with an optimized (non-Debug) build may be misleading" )
ENDIF() # NOT CMAKE_BUILD_TYPE STREQUAL "Debug"


# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests.
#						MUST return ZERO always, even on errors.
#						If not, no coverage report will be created!
# Param _outputname     lcov output is generated as _outputname.info
#                       HTML report is generated in _outputname/index.html
# Param _srctokeep      Source files directory to keep in the report.
# Optional fourth parameter is passed as arguments to _testrunner
#   Pass them in list form, e.g.: "-j;2" for -j 2
FUNCTION(SETUP_TARGET_FOR_COVERAGE _targetname _testrunner _outputname _srctokeep)

	IF(NOT LCOV_PATH)
		MESSAGE(FATAL_ERROR "lcov not found! Aborting...")
	ENDIF() # NOT LCOV_PATH

	IF(NOT GENHTML_PATH)
		MESSAGE(FATAL_ERROR "genhtml not found! Aborting...")
	ENDIF() # NOT GENHTML_PATH

	SET(IGNORE_LCOV_LIST --ignore-errors inconsistent,gcov,unused,mismatch )

	# Setup target
	ADD_CUSTOM_TARGET(${_targetname}

		# Cleanup lcov
		${LCOV_PATH} --directory . --zerocounters

		# Run tests
		COMMAND ${_testrunner} ${ARGV3}

		# Capturing lcov counters and generating report
		# Capture the baseline and test coverage
		COMMAND ${LCOV_PATH} --directory . --capture --initial --output-file ${_outputname}_baseline.info ${IGNORE_LCOV_LIST}
		# Capture the test coverage
		COMMAND ${LCOV_PATH} --directory . --capture --output-file ${_outputname}_tests.info ${IGNORE_LCOV_LIST}
		# Merge the baseline and test coverage
		COMMAND ${LCOV_PATH} --directory . --add-tracefile ${_outputname}_baseline.info --add-tracefile ${_outputname}_tests.info --output-file ${_outputname}.info
		# Extract the source files we want to keep
		COMMAND ${LCOV_PATH} --extract ${_outputname}.info ${_srctokeep} --output-file ${_outputname}.info.cleaned ${IGNORE_LCOV_LIST}
		# Generate HTML report
		COMMAND ${GENHTML_PATH} -o ${_outputname} ${_outputname}.info.cleaned
		# Remove the intermediate files
		COMMAND ${CMAKE_COMMAND} -E remove ${_outputname}.info ${_outputname}.info.cleaned
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters and generating report."
	)

	# Show info where to find the report
	ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
		COMMAND ;
		COMMENT "Open ./${_outputname}/index.html in your browser to view the coverage report."
	)

ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE

# Param _targetname     The name of new the custom make target
# Param _testrunner     The name of the target which runs the tests
# Param _outputname     cobertura output is generated as _outputname.xml
# Optional fourth parameter is passed as arguments to _testrunner
#   Pass them in list form, e.g.: "-j;2" for -j 2
FUNCTION(SETUP_TARGET_FOR_COVERAGE_COBERTURA _targetname _testrunner _outputname)

	IF(NOT PYTHON_EXECUTABLE)
		MESSAGE(FATAL_ERROR "Python not found! Aborting...")
	ENDIF() # NOT PYTHON_EXECUTABLE

	IF(NOT GCOVR_PATH)
		MESSAGE(FATAL_ERROR "gcovr not found! Aborting...")
	ENDIF() # NOT GCOVR_PATH

	ADD_CUSTOM_TARGET(${_targetname}

		# Run tests
		${_testrunner} ${ARGV3}

		# Running gcovr
		COMMAND ${GCOVR_PATH} -x -r ${CMAKE_SOURCE_DIR} -e '${CMAKE_SOURCE_DIR}/tests/' -e '${CMAKE_SOURCE_DIR}/build/'  -o ${_outputname}.xml
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		COMMENT "Running gcovr to produce Cobertura code coverage report."
	)

	# Show info where to find the report
	ADD_CUSTOM_COMMAND(TARGET ${_targetname} POST_BUILD
		COMMAND ;
		COMMENT "Cobertura code coverage report saved in ${_outputname}.xml."
	)

ENDFUNCTION() # SETUP_TARGET_FOR_COVERAGE_COBERTURA
