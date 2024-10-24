
STRING(RANDOM LENGTH 6 ALPHABET "0123456789" OFBUILD_GITASKPASS_ENVVAR_SUFFIX)
SET(OFBUILD_GITASKPASS_ENVVAR "${OFBUILD_GITASKPASS_ENVVAR_PREFIX}${OFBUILD_GITASKPASS_ENVVAR_SUFFIX}")
SET(OFBUILD_GITASKUSER_ENVVAR "${OFBUILD_GITASKUSER_ENVVAR_PREFIX}${OFBUILD_GITASKPASS_ENVVAR_SUFFIX}")


IF(OFBUILD_ENABLE_TESTING)
  FIND_PACKAGE(Boost REQUIRED COMPONENTS unit_test_framework)
ENDIF()

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/src" "${PROJECT_BINARY_DIR}/src" ${GDAL_INCLUDE_DIRS})

LINK_DIRECTORIES(${GDAL_LIBRARY_DIRS})


# Set tmp directory
# Precedence order:
#  1) check OPENFLUID_TEMP_PATH env var
#  2) check TMP env var
#  3) set as '/tmp'
SET(OFBUILD_TMP_DIR "$ENV{OPENFLUID_TEMP_PATH}")

IF(OFBUILD_TMP_DIR STREQUAL "")
  SET(OFBUILD_TMP_DIR_RAW "$ENV{TMP}")  # Env var known on Windows OS
  STRING(REPLACE "\\" "/" OFBUILD_TMP_DIR "${OFBUILD_TMP_DIR_RAW}")
ENDIF()

IF(OFBUILD_TMP_DIR STREQUAL "")
  SET(OFBUILD_TMP_DIR "/tmp")  # Generic tmp location on Unix/Mac OS
ENDIF()

MESSAGE(STATUS "Tmp path used: ${OFBUILD_TMP_DIR}")



# Configuration of RPATH for Linux and MacOS
SET(CMAKE_SKIP_BUILD_RPATH FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${OFBUILD_LIB_INSTALL_PATH}")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_MACOSX_RPATH TRUE)


ADD_SUBDIRECTORY(openfluid)

ADD_SUBDIRECTORY(apps)

ADD_SUBDIRECTORY(observers)

IF(OFBUILD_ENABLE_APP_BUILDER)
  ADD_SUBDIRECTORY(builderexts)
ENDIF()


IF(OFBUILD_ENABLE_TESTING)
  ADD_SUBDIRECTORY(tests)
ENDIF() 
