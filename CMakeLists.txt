cmake_minimum_required(VERSION 2.8)

project("MemRex")

#-------------------------------------------------------------------------------
#add local find scripts to CMAKE path
LIST(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Utils
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake/Packages
)

#-------------------------------------------------------------------------------
#set up OpenGL system variable
set(GLSystem "Desktop OpenGL" CACHE STRING "The OpenGL system to be used")
set_property(CACHE GLSystem PROPERTY STRINGS "Desktop OpenGL" "OpenGL ES")

#-------------------------------------------------------------------------------
#check if we're running on Raspberry Pi
MESSAGE("Looking for bcm_host.h")
if(EXISTS "/opt/vc/include/bcm_host.h")
    MESSAGE("bcm_host.h found")
    set(BCMHOST found)
    set(GLSystem "OpenGL ES")
else()
    MESSAGE("bcm_host.h not found")
endif()

#-------------------------------------------------------------------------------
#check if we're running on olinuxino
MESSAGE("Looking for libMali.so")
if(EXISTS "/usr/lib/libMali.so")
    MESSAGE("libMali.so found")
    set(GLSystem "OpenGL ES")
else()
    MESSAGE("libMali.so not found")
endif()

#finding necessary packages
#-------------------------------------------------------------------------------
if(${GLSystem} MATCHES "Desktop OpenGL")
    find_package(OpenGL REQUIRED)
else()
    find_package(OpenGLES REQUIRED)
endif()
find_package(Freetype REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

set(MAIN_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(${GLSystem} MATCHES "Desktop OpenGL")
    add_definitions(-DUSE_OPENGL_DESKTOP)
else()
    add_definitions(-DUSE_OPENGL_ES)
endif()


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3")

if(${GLSystem} MATCHES "Desktop OpenGL")
    add_definitions(-DUSE_OPENGL_DESKTOP)
else()
    add_definitions(-DUSE_OPENGL_ES)
endif()



set(MAIN_SOURCES
	${MAIN_SOURCE_DIR}/main.cpp
	${MAIN_SOURCE_DIR}/font.cpp
	${MAIN_SOURCE_DIR}/renderer.cpp
	${MAIN_SOURCE_DIR}/memcardview.cpp
	${MAIN_SOURCE_DIR}/memcard.cpp
)

set(COMMON_INCLUDE_DIRS
    ${FREETYPE_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
)

if(DEFINED BCMHOST)
    LIST(APPEND COMMON_INCLUDE_DIRS
        "/opt/vc/include"
        "/opt/vc/include/interface/vcos"
        "/opt/vc/include/interface/vmcs_host/linux"
        "/opt/vc/include/interface/vcos/pthreads"
    )
else()
    if(${GLSystem} MATCHES "Desktop OpenGL")
        LIST(APPEND COMMON_INCLUDE_DIRS
            ${OPENGL_INCLUDE_DIR}
        )
    else()
        LIST(APPEND COMMON_INCLUDE_DIRS
            ${OPENGLES_INCLUDE_DIR}
        )
    endif()
endif()

if(DEFINED BCMHOST)
    link_directories(
        "/opt/vc/lib"
    )
endif()

set(COMMON_LIBRARIES
    ${FREETYPE_LIBRARIES}
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
)



if(DEFINED BCMHOST)
    LIST(APPEND COMMON_LIBRARIES
	bcm_host
	EGL
        ${OPENGLES_LIBRARIES}
    )
else()
    if(MSVC)
        LIST(APPEND COMMON_LIBRARIES
            winmm
        )
    endif()
    if(${GLSystem} MATCHES "Desktop OpenGL")
        LIST(APPEND COMMON_LIBRARIES
            ${OPENGL_LIBRARIES}
        )
    else()
        LIST(APPEND COMMON_LIBRARIES
            EGL
            ${OPENGLES_LIBRARIES}
        )
    endif()
endif()

#MESSAGE(${OPENGLES_LIBRARIES})

include_directories(${COMMON_INCLUDE_DIRS})

add_executable(memrex ${MAIN_SOURCES})
target_link_libraries(memrex ${COMMON_LIBRARIES})

if(MSVC)
	add_custom_command(TARGET memrex POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy "$ENV{SDL_WIN32_PATH}\\bin\\SDL2.dll" $(OutDir)
		COMMAND ${CMAKE_COMMAND} -E copy "$ENV{SDL_WIN32_PATH}\\bin\\SDL2_image.dll" $(OutDir)
	)
endif()
