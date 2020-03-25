cmake_minimum_required(VERSION 3.1)
#add find sdl2 and sdl2 image modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wnon-virtual-dtor -Wpedantic -Wsign-conversion -std=c++14")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
project(my-game LANGUAGES CXX)

set(SOURCE_FILES 
	#base type 
	src/NTTextureRegion.cpp
    src/NTVec.cpp
    src/NTLayer.cpp

	#base tiled map
	src/NTTileLayer.cpp
	src/NTTileLayerCell.cpp
	src/NTTileLayerAnimatedTile.cpp
	src/NTTileLayerStaticTile.cpp

	#main
	src/Constances.cpp
	src/Main.cpp
	src/Game.cpp
	src/MainState.cpp
	src/GameState.cpp
	src/Locator.cpp
	src/StateManager.cpp
	src/TileSet.cpp
	src/Tile.cpp
	src/TiledMap.cpp
	src/WorldRenderer.cpp
	src/Animator.cpp
	src/Animation.cpp
	src/ObjectList.cpp
	src/Level.cpp
	src/SpriteSheet.cpp
	src/Input.cpp
	src/Builder.cpp
	src/GameObject.cpp
	src/Player.cpp

	#monster
	src/Monster.cpp
	src/Slime.cpp
)
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	find_package(SDL2)
	find_package(SDL2_image)
	set(TMXLITE_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/tmxlite/linux/libtmxlite.a)
	set(BOX2D_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/box2d/linux/libbox2d.a)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(TMXLITE_LIBRARY ${CMAKE_SOURCE_DIR}/libraries/tmxlite/windows)
	set(SDL2_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libraries/SDL2/include)
	set(SDL2_LIBRARIES ${CMAKE_SOURCE_DIR}/libraries/SDL2/windows)
	set(SDL2_IMAGE_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/libraries/SDL2_image/include)
	set(SDL2_IMAGE_LIBRARIES ${CMAKE_SOURCE_DIR}/libraries/SDL2_image/windows)
endif()
set(TMXLITE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/tmxlite/include)
set(BOX2D_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/libraries/box2d/include)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${TMXLITE_INCLUDE_DIR} ${BOX2D_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${TMXLITE_LIBRARY} ${BOX2D_LIBRARY})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/asserts $<TARGET_FILE_DIR:${PROJECT_NAME}>/asserts
)
