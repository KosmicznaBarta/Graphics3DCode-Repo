# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-src")
  file(MAKE_DIRECTORY "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-src")
endif()
file(MAKE_DIRECTORY
  "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-build"
  "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix"
  "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix/tmp"
  "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix/src/cxxopt-populate-stamp"
  "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix/src"
  "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix/src/cxxopt-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix/src/cxxopt-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/user/Documents/Graphics3DCode-Repo/build/_deps/cxxopt-subbuild/cxxopt-populate-prefix/src/cxxopt-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
