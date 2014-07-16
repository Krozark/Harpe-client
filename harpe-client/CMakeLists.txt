cmake_minimum_required (VERSION 2.8)

# define a macro that helps defining an option
macro(set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# project name
project (harpe-client)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 4)
set(VERSION_PATCH 4)

############ QT ####################
if(WIN32)
	set(CMAKE_PREFIX_PATH "C:/Qt/Qt5.2.1/5.2.1/mingw48_32")
endif()
# Appeler automatique le moc quand nécessaire
SET(CMAKE_AUTOMOC ON)
# Les fichiers générés par le moc sont générés dans le dossier bin, dire à CMake de toujours 	
# inclure les entêtes de ce dossier
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
# Le module QtWidgets trouve ses propres dépendances
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5Network REQUIRED)
FIND_PACKAGE(Qt5Script REQUIRED)
##########################################



# add the  header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# add an option for choosing the build type (shared or static)
#set_option(BUILD_SHARED_LIBS FALSE BOOL "TRUE to build as shared libraries, FALSE to build it as static libraries")

# add an option for building the API documentation
set_option(BUILD_DOC TRUE BOOL "TRUE to generate the API documentation, FALSE to ignore it")


#set(includepath "-I${CMAKE_CURRENT_SOURCE_DIR}/include")
set(DEFINES "")

if(${WIN32})
    find_library(PDH_LIB pdh)
    find_library(PSAPI_LIB psapi)
    find_library(PTHREAD_LIBS "winpthread-1")
	find_library(SOCKET_LIBS ws2_32)
    find_library(KERNEL_LIBS kernel32)
    set(LIBS ${PDH_LIB} ${PSAPI_LIB} ${PTHREAD_LIBS} ${SOCKET_LIBS} ${KERNEL_LIBS})
else()
    find_library(PTHREAD_LIBS pthread)
    find_library(DL_LIB dl)
    set(LIBS ${PTHREAD_LIBS} ${DL_LIB})
endif()

find_library(MGF_LIB mgf)
find_library(SOCKET_LIB socket)
find_library(MONITORING_LIB Monitoring)
find_library(HARPE_ALGO_LIB harpe-algo)
find_library(UTILS_LIB utils)
find_library(UTILS_JSON_LIB utils-json)
set(LIBS ${LIBS} ${MGF_LIB} ${SOCKET_LIB} ${MONITORING_LIB} ${HARPE_ALGO_LIB} ${UTILS_LIB} ${UTILS_JSON_LIB})


set(WARNING "-Wall")
#set(FLAGS "-o3 -std=c++0x -Wl,--no-as-needed -ldl")
set(FLAGS "-o3 -std=c++0x")
set(CMAKE_CXX_FLAGS "${DEFINES} ${WARNING} ${FLAGS}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(LIB_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

# add the subdirectories
add_subdirectory(src/${CMAKE_PROJECT_NAME})

#build doc
if(BUILD_DOC)
    add_subdirectory(doc)
endif()

