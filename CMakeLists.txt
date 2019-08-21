#	\author    John Szwast
#	\year      2014-2016
#	\copyright MIT

cmake_minimum_required(VERSION 3.2)

project(PowerIterators)

# Download and unpack googletest at configure time
configure_file(CMakeLists.googletest.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)


enable_testing()


# Download and unpack memoized-member at configure time
configure_file(CMakeLists.memoized-member.in memoized-member-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX=../memoized-member-install .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/memoized-member-download)
if(result)
  message(FATAL_ERROR "CMake step for MemoizedMember failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/memoized-member-download)
if(result)
  message(FATAL_ERROR "Build step for MemoizedMember failed: ${result}")
endif()

include(${CMAKE_CURRENT_BINARY_DIR}/memoized-member-install/cmake/MM.cmake)


set(doc README.md spec/CombinationIterator.md spec/PowerSetIterator.md design/Iterators.md)
set(headers include/combination_iterator.hpp include/power_iterator.hpp)
set(sources)
set(unit_tests test/combination_iterator_test.cpp test/power_iterator_test.cpp)


add_library(PowerIterators INTERFACE)
target_include_directories(PowerIterators INTERFACE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
)
target_link_libraries(PowerIterators INTERFACE MemoizedMember)

install(TARGETS PowerIterators EXPORT PowerIterators)
install(EXPORT PowerIterators DESTINATION cmake)
install(FILES ${headers} DESTINATION include)
install(FILES ${doc} DESTINATION doc)


add_executable(pi_tests ${unit_tests})

target_link_libraries(pi_tests PowerIterators gtest_main)

add_test(NAME PowerIterators-Unit-Tests COMMAND pi_tests)
