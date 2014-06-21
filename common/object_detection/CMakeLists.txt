cmake_minimum_required(VERSION 2.8)
project( OBJECT_DETECTION )

ADD_DEFINITIONS(
	-std=c++11 
	-std=c++0x
	# Other flags
)

find_package( OpenCV REQUIRED )

include_directories( 
	$(OBJECT_DETECTION_SOURCE_DIR)/include 
	${OpenCV_INCLUDE_DIRS} 
)

add_executable( 
object_training
	src/main_training.cpp
)

add_executable( 
object_detection
	src/main_detection.cpp
)

target_link_libraries( object_training ${OpenCV_LIBS} )
target_link_libraries( object_detection ${OpenCV_LIBS} )


