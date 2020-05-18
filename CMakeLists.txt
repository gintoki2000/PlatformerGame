cmake_minimum_required(VERSION 3.1)
#add find sdl2 and sdl2 image modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wpedantic -Wsign-conversion -std=c++14")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(my-game LANGUAGES CXX)
file(GLOB SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(SDL2)
	find_package(SDL2_image)
	find_package(SDL2_mixer)
	set(TMXLITE_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/tmxlite/linux/libtmxlite.a)
	set(BOX2D_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/box2d/linux/libbox2d.a)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	#----------------------------------------------------------------
	#set(TMXLITE_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/tmxlite/windows)
    set(TMXLITE_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/libraries/tmxlite/windows/libtmxlite-s.lib)
    set(TMXLITE_LIBRARY ${CMAKE_CURRENT_LIST_DIR}/libraries/box2d/windows/box2d.lib)
	#-------------------------------
	#set(SDL2_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libraries/SDL2/include)
	#set(SDL2_LIBRARIES ${CMAKE_SOURCE_DIR}/libraries/SDL2/windows)
	#----
	# Support both 32 and 64 bit builds
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  		set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x64/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/libraries/lib/x64/SDL2main.lib")
	else ()
		set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib;${CMAKE_CURRENT_LIST_DIR}/libraries/lib/x86/SDL2main.lib")
		
	#----------------------------------------------------------------
	#set(SDL2_IMAGE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libraries/SDL2_image/include)
	#set(SDL2_IMAGE_LIBRARIES ${CMAKE_SOURCE_DIR}/libraries/SDL2_image/windows)
	#----
	if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  		set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/libraries/lib/x64/SDL2_image.lib")
	else ()
  		set(SDL2_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/libraries/lib/x86/SDL2_image.lib")
	endif ()
	#----------------------------------------------------------------

endif()
set(TMXLITE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/tmxlite/include)
set(BOX2D_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/box2d/include)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
