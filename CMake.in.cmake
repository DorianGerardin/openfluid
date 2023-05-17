#
# Configuration file for CMakeLists.txt files
#
# Author : Jean-Christophe FABRE <jean-christophe.fabre@inra.fr>
#
# This file is included by the main CMakeLists.txt file, and defines variables
# to configure the build and install
#
# The variables in this file can also be overriden through the
# CMake.in.local.config file
#


################### general ###################

SET(OPENFLUID_MAIN_NAME "openfluid")


################### versions ###################

SET(OFBUILD_CUSTOM_CMAKE_VERSION "${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")

SET(OPENFLUID_VERSION_MAJOR 2)
SET(OPENFLUID_VERSION_MINOR 2)
SET(OPENFLUID_VERSION_PATCH 0)
SET(OPENFLUID_VERSION_STATUS "alpha94") # example: SET(OPENFLUID_VERSION_STATUS "rc1")

SET(OPENFLUID_VERSION_FULL "${OPENFLUID_VERSION_MAJOR}.${OPENFLUID_VERSION_MINOR}.${OPENFLUID_VERSION_PATCH}")

IF(OPENFLUID_VERSION_STATUS)
  SET(OPENFLUID_VERSION_FULL "${OPENFLUID_VERSION_FULL}~${OPENFLUID_VERSION_STATUS}")
ENDIF()

# Compute version as numeric
MATH(EXPR OPENFLUID_VERSION_NUMERIC "${OPENFLUID_VERSION_MAJOR} * 10000 + ${OPENFLUID_VERSION_MINOR} * 100 + ${OPENFLUID_VERSION_PATCH}")


################### paths and files ###################

SET(OPENFLUID_RELATIVE_PATH "${OPENFLUID_MAIN_NAME}")

SET(OPENFLUID_DEFAULT_INPUT_PATH "OPENFLUID.IN")
SET(OPENFLUID_DEFAULT_OUTPUT_PATH "OPENFLUID.OUT")
SET(OPENFLUID_WORKSPACE_PATH "workspace")
SET(OPENFLUID_WARESBIN_PATH "wares")
SET(OPENFLUID_WARESDEV_PATH "wares-dev")
SET(OPENFLUID_USERSHARED_PATH "shared")
SET(OPENFLUID_SIMULATORS_PATH "simulators")
SET(OPENFLUID_OBSERVERS_PATH "observers")
SET(OPENFLUID_FRAGMENTS_PATH "fragments")
SET(OPENFLUID_BUILDEREXTS_PATH "builderexts")
SET(OPENFLUID_EXAMPLES_PATH "examples")
SET(OPENFLUID_PROJECTS_PATH "projects")
SET(OPENFLUID_NETWORK_PATH "network")
SET(OPENFLUID_HUBS_CACHE_PATH "${OPENFLUID_NETWORK_PATH}/hubs")

SET(OPENFLUID_PROJECT_INPUT_PATH "IN")
SET(OPENFLUID_PROJECT_OUTPUT_PATHPREFIX "OUT")

SET(OFBUILD_BIN_INSTALL_PATH "bin")
SET(OFBUILD_LIB_INSTALL_PATH "lib${OFBUILD_LIBDIR_SUFFIX}")
SET(OFBUILD_INCLUDE_INSTALL_PATH "include")
SET(OFBUILD_SHARE_INSTALL_PATH "share")
SET(OPENFLUID_INCLUDE_INSTALL_PATH "${OFBUILD_INCLUDE_INSTALL_PATH}/${OPENFLUID_RELATIVE_PATH}")
SET(OPENFLUID_SHARE_INSTALL_PATH "${OFBUILD_SHARE_INSTALL_PATH}/${OPENFLUID_RELATIVE_PATH}")

SET(OPENFLUID_DESKTOPENTRY_INSTALL_PATH "${OFBUILD_SHARE_INSTALL_PATH}/applications")

SET(OPENFLUID_SHARE_COMMON_INSTALL_PATH "${OPENFLUID_SHARE_INSTALL_PATH}/common")
SET(OPENFLUID_SHARE_TRANSLATIONS_INSTALL_PATH "${OFBUILD_SHARE_INSTALL_PATH}/locale")
SET(OPENFLUID_SHARE_APPS_INSTALL_PATH "${OPENFLUID_SHARE_INSTALL_PATH}/apps")
SET(OPENFLUID_SHARE_WARESDEV_INSTALL_PATH "${OPENFLUID_SHARE_INSTALL_PATH}/waresdev")

SET(OPENFLUID_WARES_INSTALL_PATH "${OFBUILD_LIB_INSTALL_PATH}/${OPENFLUID_RELATIVE_PATH}")
SET(OPENFLUID_SIMULATORS_INSTALL_PATH "${OPENFLUID_WARES_INSTALL_PATH}/${OPENFLUID_SIMULATORS_PATH}")
SET(OPENFLUID_OBSERVERS_INSTALL_PATH "${OPENFLUID_WARES_INSTALL_PATH}/${OPENFLUID_OBSERVERS_PATH}")
SET(OPENFLUID_BUILDEREXTS_INSTALL_PATH "${OPENFLUID_WARES_INSTALL_PATH}/${OPENFLUID_BUILDEREXTS_PATH}")

IF (WIN32 OR APPLE)
  SET(OPENFLUID_CMAKE_MODULES_INSTALL_PATH "${OFBUILD_LIB_INSTALL_PATH}/cmake")
  SET(OPENFLUID_CMAKE_HELPERSSCRIPTS_INSTALL_PATH "${OFBUILD_LIB_INSTALL_PATH}/cmake")
ELSE()
  SET(OPENFLUID_CMAKE_MODULES_INSTALL_PATH "${OFBUILD_LIB_INSTALL_PATH}/${OPENFLUID_RELATIVE_PATH}/cmake")
  SET(OPENFLUID_CMAKE_HELPERSSCRIPTS_INSTALL_PATH "${OFBUILD_LIB_INSTALL_PATH}/${OPENFLUID_RELATIVE_PATH}/cmake")
ENDIF()

SET(OPENFLUID_DOC_INSTALL_PATH "${OFBUILD_SHARE_INSTALL_PATH}/doc/${OPENFLUID_RELATIVE_PATH}")
SET(OPENFLUID_MAIN_DOC_INSTALL_PATH "${OPENFLUID_DOC_INSTALL_PATH}/main")

SET(OPENFLUID_EXAMPLES_INSTALL_PATH "${OPENFLUID_DOC_INSTALL_PATH}/${OPENFLUID_EXAMPLES_PATH}")
SET(OPENFLUID_EXAMPLES_PROJECTS_INSTALL_PATH "${OPENFLUID_EXAMPLES_INSTALL_PATH}/${OPENFLUID_PROJECTS_PATH}")
SET(OPENFLUID_EXAMPLES_WARESDEV_INSTALL_PATH "${OPENFLUID_EXAMPLES_INSTALL_PATH}/${OPENFLUID_WARESDEV_PATH}")
SET(OPENFLUID_EXAMPLES_SRC_INSTALL_PATH "${OPENFLUID_EXAMPLES_INSTALL_PATH}/src")
SET(OPENFLUID_EXAMPLES_DATA_INSTALL_PATH "${OPENFLUID_EXAMPLES_INSTALL_PATH}/datasets")
SET(OPENFLUID_EXAMPLES_DOCPDF_INSTALL_PATH "${OPENFLUID_EXAMPLES_INSTALL_PATH}")


SET(OPENFLUID_DEFAULT_CONFIGFILE "${OPENFLUID_MAIN_NAME}-settings.json")
SET(OPENFLUID_PROJECT_FILE "${OPENFLUID_MAIN_NAME}-project.json")
SET(OPENFLUID_WORKSPACE_CONFIGFILE "${OPENFLUID_MAIN_NAME}-workspace.json")


################### source and output directories ###################

SET(OFBUILD_SOURCE_EXAMPLES_DIR "${CMAKE_SOURCE_DIR}/${OPENFLUID_EXAMPLES_PATH}")
SET(OFBUILD_SOURCE_EXAMPLES_PROJECTS_DIR "${OFBUILD_SOURCE_EXAMPLES_DIR}/projects")
SET(OFBUILD_SOURCE_EXAMPLES_WARESDEV_DIR "${OFBUILD_SOURCE_EXAMPLES_DIR}/wares-dev")

SET(OFBUILD_3RDPARTY_nlohmannjson_VERSION "3.10.2")
SET(OFBUILD_3RDPARTY_tinyxml2_VERSION "9.0.0")

SET(OFBUILD_DOC_LAYOUT_DIR "${CMAKE_SOURCE_DIR}/doc/layout")
SET(OFBUILD_DOC_CONTENTS_DIR "${CMAKE_SOURCE_DIR}/doc/contents")
SET(OFBUILD_DOC_SNIPPETS_DIR "${CMAKE_SOURCE_DIR}/doc/snippets")
SET(OFBUILD_DOC_SRCTOPARSE_DIR "${CMAKE_SOURCE_DIR}/src/openfluid")
SET(OFBUILD_DOC_DIRSTOEXCLUDE "tinyxml2* nlohmann* .git .svn")

SET(OFBUILD_EXAMPLES_SIMULATORS_DOC_DIR "${CMAKE_SOURCE_DIR}/resources/docs/${OPENFLUID_EXAMPLES_PATH}/${OPENFLUID_SIMULATORS_PATH}")


SET(OFBUILD_DIST_DIR "${CMAKE_BINARY_DIR}/dist")
SET(OFBUILD_DIST_LIB_DIR "${OFBUILD_DIST_DIR}/${OFBUILD_LIB_INSTALL_PATH}")
SET(OFBUILD_DIST_BIN_DIR "${OFBUILD_DIST_DIR}/${OFBUILD_BIN_INSTALL_PATH}")

SET(OFBUILD_DIST_WARES_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_WARES_INSTALL_PATH}")
SET(OFBUILD_DIST_SIMULATORS_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_SIMULATORS_INSTALL_PATH}")
SET(OFBUILD_DIST_OBSERVERS_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_OBSERVERS_INSTALL_PATH}")
SET(OFBUILD_DIST_BUILDEREXTS_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_BUILDEREXTS_INSTALL_PATH}")

SET(OFBUILD_DIST_CMAKE_MODULES_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_CMAKE_MODULES_INSTALL_PATH}")

SET(OFBUILD_DIST_MAIN_DOC_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_MAIN_DOC_INSTALL_PATH}")

SET(OFBUILD_DIST_TRANSLATIONS_DIR "${OFBUILD_DIST_DIR}/${OPENFLUID_SHARE_TRANSLATIONS_INSTALL_PATH}")

SET(OFBUILD_DOCBUILD_DIR "${CMAKE_BINARY_DIR}/docbuild")

SET(OFBUILD_TESTS_BINARY_DIR "${CMAKE_BINARY_DIR}/tests-bin")
SET(OFBUILD_TESTS_USERDATA_DIR "${CMAKE_BINARY_DIR}/tests-userdata")
SET(OFBUILD_TESTS_TEMP_DIR "${CMAKE_BINARY_DIR}/tests-temp")
SET(OFBUILD_TESTS_INPUT_DATASETS_DIR "${CMAKE_SOURCE_DIR}/resources/tests/datasets")
SET(OFBUILD_TESTS_INPUT_MISCDATA_DIR "${CMAKE_SOURCE_DIR}/resources/tests/miscdata")
SET(OFBUILD_TESTS_INPUT_MISCSRC_DIR "${CMAKE_SOURCE_DIR}/resources/tests/miscsrc")
SET(OFBUILD_TESTS_OUTPUT_DATA_DIR "${CMAKE_BINARY_DIR}/tests-output")
SET(OPENFLUID_TESTS_TEMP_PATH ${OFBUILD_TESTS_TEMP_DIR}) # for usage in OPENFLUID_ADD_TEST macro
SET(OPENFLUID_TESTS_USERDATA_PATH ${OFBUILD_TESTS_USERDATA_DIR}) # for usage in OPENFLUID_ADD_TEST macro


################### URLs and mail ###################

SET(OPENFLUID_NETWORK_DOMAIN "openfluid-project.org")
SET(OPENFLUID_NETWORK_DOMAIN_WWW "www.${OPENFLUID_NETWORK_DOMAIN}")
SET(OPENFLUID_NETWORK_DOMAIN_COMMUNITY "community.${OPENFLUID_NETWORK_DOMAIN}")
SET(OPENFLUID_NETWORK_DOMAIN_DEV "dev.${OPENFLUID_NETWORK_DOMAIN}")

SET(OPENFLUID_EMAIL_CONTACT "contact@${OPENFLUID_NETWORK_DOMAIN}")

SET(OPENFLUID_URL_OFFICIAL "https://${OPENFLUID_NETWORK_DOMAIN_WWW}")
SET(OPENFLUID_URL_COMMUNITY "https://${OPENFLUID_NETWORK_DOMAIN_COMMUNITY}")
SET(OPENFLUID_URL_NEWS_API "${OPENFLUID_URL_OFFICIAL}/api")
SET(OPENFLUID_URL_GITHUB "https://github.com/OpenFLUID")
SET(OPENFLUID_URL_ISSUES "${OPENFLUID_URL_GITHUB}/openfluid/issues")
SET(OPENFLUID_URL_EMAIL_CONTACT "mailto:${OPENFLUID_EMAIL_CONTACT}")
SET(OPENFLUID_URL_DOC "${OPENFLUID_URL_OFFICIAL}/resources/docs/manuals/en/openfluid/${OPENFLUID_VERSION_FULL}/main/html/")
SET(OPENFLUID_URL_OFFICIAL_API "${OPENFLUID_URL_OFFICIAL}/api")


################### applications ###################

SET(OPENFLUID_CMD_APP "openfluid")
SET(OPENFLUID_BUILDER_APP "openfluid-builder")
SET(OPENFLUID_DEVSTUDIO_APP "openfluid-devstudio")
SET(OPENFLUID_LOGEXPLORER_APP "openfluid-logexplorer")
SET(OPENFLUID_GITASKPASS_APP "openfluid-gitaskpass")

IF(APPLE)
  SET(OPENFLUID_BUILDER_APP "OpenFLUID-Builder")
  SET(OPENFLUID_DEVSTUDIO_APP "OpenFLUID-Devstudio")
  SET(OPENFLUID_LOGEXPLORER_APP "OpenFLUID-Logexplorer")
ENDIF()


################### compilation and build ###################

SET(OFBUILD_DEBUG_COMPILE_OPTIONS "")
SET(OFBUILD_RELEASE_COMPILE_OPTIONS "")
IF((CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  SET(OFBUILD_${OFBUILD_BUILD_TYPE}_COMPILE_OPTIONS -Wall -Wextra)
ENDIF()

IF(APPLE)
  SET(CMAKE_FIND_FRAMEWORK LAST)
ENDIF()

SET(OPENFLUID_PLUGINS_BINARY_EXTENSION "${CMAKE_SHARED_LIBRARY_SUFFIX}")
SET(OPENFLUID_PLUGINS_GHOST_EXTENSION ".json")

SET(OPENFLUID_SIMULATORS_SUFFIX "_ofware${OPENFLUID_VERSION_NUMERIC}-sim")
SET(OPENFLUID_OBSERVERS_SUFFIX "_ofware${OPENFLUID_VERSION_NUMERIC}-obs")
SET(OPENFLUID_BUILDEREXTS_SUFFIX "_ofware${OPENFLUID_VERSION_NUMERIC}-bext")

SET(OPENFLUID_GHOSTSIMULATORS_SUFFIX "_ofghost-sim")

SET(OPENFLUID_DEBUG_PREFIX "[OpenFLUID debug]")

SET(OFBUILD_GITASKPASS_ENVVAR_PREFIX "OFLD_GITASKPASS_")
SET(OFBUILD_GITASKUSER_ENVVAR_PREFIX "OFLD_GITASKUSER_")


################### logfiles ###################

SET(OPENFLUID_MESSAGES_LOG_FILE "openfluid-messages.log")

SET(OPENFLUID_CUMULATIVE_PROFILE_FILE "openfluid-profile-cumulative.log")
SET(OPENFLUID_SCHEDULE_PROFILE_FILE "openfluid-profile-schedule.log")
SET(OPENFLUID_TIMEINDEX_PROFILE_FILE "openfluid-profile-timeindex.log")


################### waresdev ###################

SET(OPENFLUID_WARESDEV_SRC_CMAKESTDFILE "CMakeLists.txt")
SET(OPENFLUID_WARESDEV_SRC_MAINFILE "WareMain.cpp")
SET(OPENFLUID_WARESDEV_SRC_PARAMSUIFILE "WareUI.cpp")
SET(OPENFLUID_WARESDEV_SRC_DIR "src")  # TOIMPL use this config variable wherever possible
SET(OPENFLUID_WARESDEV_TESTS_DIR "tests") # TOIMPL use this config variable wherever possible
SET(OPENFLUID_WARESDEV_DOC_DIR "doc") # TOIMPL use this config variable wherever possible
SET(OPENFLUID_WARESDEV_DOC_MAIN_FILEROOT "main") # TOIMPL use this config variable wherever possible
SET(OPENFLUID_WARESDEV_README_FILEROOT "README")
SET(OPENFLUID_WARESDEV_WAREMETA_FILE "${OPENFLUID_MAIN_NAME}-ware.json")
SET(OPENFLUID_WARESDEV_BUILD_MAINSIGN "main_signature.cpp")
SET(OPENFLUID_WARESDEV_BUILD_MAININFO "main_info.cmake")
SET(OPENFLUID_WARESDEV_BUILD_PARAMSUISIGN "paramsui_signature.cpp")
SET(OPENFLUID_WARESDEV_BUILD_PARAMSUIINFO "paramsui_info.cmake")
SET(OPENFLUID_WARESDEV_PARAMSUI_IDSUFFIX "-paramsUI")


################### i18n ###################

SET(OFBUILD_TRANSLATIONS_TSDIR "${CMAKE_SOURCE_DIR}/resources/translations")
SET(OFBUILD_TRANSLATIONS_DIRSTOSCAN "${CMAKE_SOURCE_DIR}/src/openfluid/ui"
                                    "${CMAKE_SOURCE_DIR}/src/openfluid/ui/config.hpp.in"
                                    "${CMAKE_SOURCE_DIR}/src/openfluid/waresdev"
                                    "${CMAKE_SOURCE_DIR}/src/apps/openfluid-builder"
                                    "${CMAKE_SOURCE_DIR}/src/apps/openfluid-devstudio")
SET(OPENFLUID_TRANSLATIONS_FILEROOT openfluid)
SET(OPENFLUID_TRANSLATIONS_LANGS fr_FR)


################### tests configuration ###################

# return code for skipped tests
SET(OFBUILD_UNITTESTS_SKIP_CODE 42)

# set options passed to unit tests programs. ex: "--log_level=all"
SET(OFBUILD_UNITTESTS_RUN_OPTIONS "")

SET(OFBUILD_TESTS_DEV_HOST ${OPENFLUID_NETWORK_DOMAIN_DEV})
SET(OFBUILD_TESTS_RESTSERVICE_URL_HTTP "http://${OFBUILD_TESTS_DEV_HOST}/testing/rest")
SET(OFBUILD_TESTS_RESTSERVICE_URL_HTTPS "https://${OFBUILD_TESTS_DEV_HOST}/testing/rest")
SET(OFBUILD_TESTS_FLUIDHUB_V0_URL_HTTP "http://${OFBUILD_TESTS_DEV_HOST}/testing/fluidhub-api")
SET(OFBUILD_TESTS_FLUIDHUB_V0_URL_HTTPS "https://${OFBUILD_TESTS_DEV_HOST}/testing/fluidhub-api")
SET(OFBUILD_TESTS_FLUIDHUB_V0_URL_REDIRECT_HTTPS "https://${OFBUILD_TESTS_DEV_HOST}/redirect/testing/fluidhub-api")
SET(OFBUILD_TESTS_FLUIDHUB_V1_URL_HTTP "${OFBUILD_TESTS_FLUIDHUB_V0_URL_HTTP}/v1")
SET(OFBUILD_TESTS_FLUIDHUB_V1_URL_HTTPS "${OFBUILD_TESTS_FLUIDHUB_V0_URL_HTTPS}/v1")
SET(OFBUILD_TESTS_WARESHUB_URL_HTTP ${OFBUILD_TESTS_FLUIDHUB_V0_URL_HTTP})
SET(OFBUILD_TESTS_WARESHUB_URL_HTTPS ${OFBUILD_TESTS_FLUIDHUB_V0_URL_HTTPS})
SET(OFBUILD_TESTS_WARESHUB_URL_REDIRECT_HTTPS ${OFBUILD_TESTS_FLUIDHUB_V0_URL_REDIRECT_HTTPS})
SET(OFBUILD_TESTS_WARESHUB_USERNAME "")
SET(OFBUILD_TESTS_WARESHUB_PASSWORD "")
SET(OFBUILD_TESTS_GITHELPER_URL "https://${OFBUILD_TESTS_DEV_HOST}/testing/git/standard")
SET(OFBUILD_TESTS_GITHELPER_EMPTYREPOS_URL "https://${OFBUILD_TESTS_DEV_HOST}/testing/git/empty")
SET(OFBUILD_TESTS_GITHELPER_AUTH_URL "https://${OFBUILD_TESTS_DEV_HOST}/testing/git/auth")
SET(OFBUILD_TESTS_GITHELPER_AUTH_USERNAME "gitof")
SET(OFBUILD_TESTS_GITHELPER_AUTH_PASSWORD "gitof")
SET(OFBUILD_TESTS_GITHELPER_CURRENT_BRANCH "master")
SET(OFBUILD_TESTS_GITHELPER_OTHER_BRANCH "first-merge")
SET(OFBUILD_TESTS_GITHELPER_UNEXISTING_BRANCH "bad-name")
SET(OFBUILD_TESTS_FILEDOWNLOAD_URL "http://${OFBUILD_TESTS_DEV_HOST}/testing/download/lorem_ipsum.txt")


################### cppcheck ###################

SET(OFBUILD_CPPCHECK_EXTRA_OPTIONS "-q"
                           "--enable=style,information,performance,portability,missingInclude"
                           "--suppress=variableScope"
                           "--force")
