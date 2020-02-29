cmake_minimum_required(VERSION 3.1)
#add find sdl2 and sdl2 image modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wpedantic -Wsign-conversion -std=c++14")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(my-game LANGUAGES CXX)

set(SOURCE_FILES 
	#base type 
	src/NTTextureRegion.cpp

	#base tiled map
	src/NTTiledMap.cpp
	src/NTTiledMapCell.cpp
	src/NTTiledMapAnimatedTile.cpp

	#main
	src/Main.cpp
	src/Game.cpp
	src/Map.cpp
	src/TileSet.cpp
	src/Tile.cpp
        src/TileLayer.cpp
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(SDL2)
	find_package(SDL2_image)
	set(TMXLITE_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/tmxlite/linux/libtmxlite.a)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(TMXLITE_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/tmxlite/windows/)
endif()
set(TMXLITE_INCLUDE_DIR {$CMAKE_SOURCE_DIR}/libraries/tmxlite/include)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${TMXLITE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${TMXLITE_LIBRARY})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/asserts $<TARGET_FILE_DIR:${PROJECT_NAME}>/asserts
)
