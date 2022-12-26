cmake_minimum_required( VERSION 3.16 )

project( VisiX )

set(CMAKE_CXX_STANDARD 17)

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( glfw )

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

##include
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/rendering)
include_directories(${CMAKE_SOURCE_DIR}/include/maths)
include_directories(${CMAKE_SOURCE_DIR}/include/maths/geometry)
include_directories(${CMAKE_SOURCE_DIR}/include/utils)

include_directories(${CMAKE_SOURCE_DIR}/glfw/include/GLFW)
include_directories(${CMAKE_SOURCE_DIR}/include/lib)

## src
set(RENDERING_SRCS
    ${CMAKE_SOURCE_DIR}/src/rendering/Camera.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Uniform.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Buffer.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/EBO.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Particles.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Node.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Drawer.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Renderable.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Object.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/ShaderClass.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Texture.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/VAO.cpp
    ${CMAKE_SOURCE_DIR}/src/rendering/Viewer.cpp)

set(MATHS_SRCS
    )

## set(UTILS_SRCS )

set(LIB_SRCS
    ${CMAKE_SOURCE_DIR}/src/lib/stb.cpp)

## header
set(RENDERING_HDRS
    ${CMAKE_SOURCE_DIR}/include/rendering/Vertex.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Movable.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Uniform.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Buffer.h
    ${CMAKE_SOURCE_DIR}/include/rendering/GLObject.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Camera.h
    ${CMAKE_SOURCE_DIR}/include/rendering/EBO.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Mesh.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Particles.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Node.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Drawer.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Renderable.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Object.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Scene.h
    ${CMAKE_SOURCE_DIR}/include/rendering/ShaderClass.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Texture.h
    ${CMAKE_SOURCE_DIR}/include/rendering/VAO.h
    ${CMAKE_SOURCE_DIR}/include/rendering/Viewer.h)

set(MATHS_HDRS
    )


## glad
add_library(glad ${CMAKE_SOURCE_DIR}/src/lib/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/src)


## executable
add_executable( VisiX ${CMAKE_SOURCE_DIR}/src/main.cpp ${RENDERING_SRCS} ${MATHS_SRCS} ${LIB_SRCS} ${RENDERING_HDRS} ${MATHS_HDRS})
target_link_libraries( VisiX ${OPENGL_LIBRARIES} glfw glad)
if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'VisiX' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT VisiX )
    endif()
endif()
