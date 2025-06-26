cmake_minimum_required(VERSION 3.10)


# Set C standsards
# Step 1: COmpile the C part of the magemin as a shared library
project(MAGEMin C CXX)

set(VERSION 1.7.9)
set(CMAKE_C_STANDARD 17)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O0 -Wno-unused-variable -Wno-unused-but-set-variable -march=native -funroll-loops -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O0 -fPIC -std=c++17 -flto")
# g flag is removed

# Platform specific settings
# if
if(APPLE)
  include_directories(/opt/homebrew/include)
endif()

if(UNIX AND NOT APPLE)
  set(LIBS m)

  # lapacke block
  find_package(LAPACK REQUIRED)
  find_library(LAPACKE_LIB lapacke HINTS ${LAPACKE_LIB} REQUIRED)
  find_library(BLAS_LIB blas REQUIRED)

  find_library(nlopt_lib nlopt PATHS "/nobackup/scxn98/installations/nloptInstall/lib64" REQUIRED)
  list(APPEND LIBS LAPACK::LAPACK ${LAPACKE_LIB} ${BLAS_LIB} ${nlopt_lib})


  # include_directories(/usr/lib/x86_64-linux-gnu/openmpi/include/ /usr/include/)
endif()


# Add MPI packages
find_package(MPI REQUIRED)
message(STATUS "Run: ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} ${MPIEXEC_MAX_NUMPROCS} ${MPIEXEC_PREFLAGS} EXECUTABLE ${MPIEXEC_POSTFLAGS} ARGS")
list(APPEND LIBS MPI::MPI_CXX)
# target_link_libraries(MyTarget PUBLIC MPI::MPI_CXX)


# OPENMP block
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
  list(APPEND LIBS OpenMP::OpenMP_CXX)
  # target_link_libraries(MyTarget PUBLIC OpenMP::OpenMP_CXX)
endif()

# lapacke block
# find_package(LAPACK REQUIRED)
# list(APPEND LIBS LAPACK::LAPACK)



# Find all c file sources
file(GLOB_RECURSE C_SRC_FILES src/*.c)
set(CPP_wrapper src/MAGEMin_cpp.cc)

add_library(MAGEMin_cpp SHARED ${CPP_wrapper} ${C_SRC_FILES})


target_include_directories(MAGEMin_cpp PUBLIC /nobackup/scxn98/installations/nloptInstall/include/)
target_include_directories(MAGEMin_cpp PUBLIC src/ src/SB_database/ src/TC_database/)
target_link_libraries(MAGEMin_cpp PUBLIC ${LIBS})

set_target_properties(MAGEMin_cpp PROPERTIES
  OUTPUT_NAME "MAGEMin_cpp"
  BUILD_WITH_INSTALL_RPATH TRUE
  INSTALL_RPATH "$ORIGIN"
  INTERPROCEDURAL_OPTIMIZATION TRUE
)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include)
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/)

message(STATUS "MAGEMin_cpp library version ${VERSION} configured from ${CMAKE_CURRENT_SOURCE_DIR}")

# INSTALL(TARGETS MAGEMin_cpp
#   EXPORT MAGEMinTARGETS
#   LIBRARY DESTINATION lib
# )

# include(CMakePackageConfigHelpers)
# configure_package_config_file(MAGEMinConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/PerpleXConfig.cmake
#   INSTALL_DESTINATION ${LIB_INSTALL_DIR}/MAGEMin/cmake
#   PATH_VARS INCLUDE_INSTALL_DIR )
#
# write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/MAGEMinConfigVersion.cmake
#   VERSION ${VERSION}
#   COMPATIBILITY ExactVersion )
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MAGEMinConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/MAGEMinConfigVersion.cmake
#   DESTINATION ${LIB_INSTALL_DIR}/MAGEMin/cmake )
#
#
# # INSTALL(TARGETS ${TARGET_SRC} RUNTIME DESTINATION bin COMPONENT RUNTIME)
# INSTALL(TARGETS MAGEMin_cpp LIBRARY DESTINATION lib COMPONENT RUNTIME)
#
# # headers:
# INSTALL(DIRECTORY include/
#   DESTINATION include
#   COMPONENT includes
# )
