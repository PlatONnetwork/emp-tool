cmake_minimum_required (VERSION 2.8.11)
if(POLICY CMP0054)
	 cmake_policy(SET CMP0054 NEW)
endif()

project (emp-tool)

SET(NAME "emp-tool")
SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# will set on the upper cmakelist
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

include(cmake/common.cmake)
include(cmake/source_of_randomness.cmake)
include(cmake/threading.cmake)

find_package(OpenSSL)

if (NOT WIN32)
	find_package(GMP)
	include_directories(${GMP_INCLUDE_DIR})
	link_libraries(${GMP_LIBRARIES})
endif()

if(NOT OT_NP_USE_MIRACL)
	message(STATUS "Use RELIC")
    if(WIN32)
        message(FATAL "Not support relic on Windows !")
    endif()

	# setup directory where we should look for cmake files
	list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
	find_package(relic) # compile relic should refer to emp-readme github repository (https://github.com/emp-toolkit/emp-readme)
	#set(RELIC_LIBRARIES relic_s)
	include_directories(${RELIC_INCLUDE_DIR})
	message(STATUS "RELIC_INCLUDE_DIR: ${RELIC_INCLUDE_DIR}")
	
	link_libraries(${RELIC_LIBRARIES})
else()
	message(STATUS "Use MIRACL")
	ADD_DEFINITIONS(-DOT_NP_USE_MIRACL=1)
	include_directories(../miracl/include)
	link_libraries(miracl)
endif()

# openssl dependencies
include_directories(${OPENSSL_INCLUDE_DIR})
link_libraries(${OPENSSL_LIBRARIES})

if(WIN32)
	link_directories(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
	# for windows socket
	link_libraries(Ws2_32)
else()
	link_directories(${CMAKE_BINARY_DIR}/lib)
endif()

# protobuf dependencies
include_directories(../protobuf/src)
if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
	if (WIN32)
		SET(LIB_PROTOBUF libprotobufd)
	else()
		SET(LIB_PROTOBUF protobufd)
	endif()
else()
	if (WIN32)
		SET(LIB_PROTOBUF libprotobuf)
	else()
		SET(LIB_PROTOBUF libprotobuf)
	endif()
endif()
link_libraries(${LIB_PROTOBUF})


# includes
include_directories(./)
include_directories(./include)

# source list
aux_source_directory(./emp-tool          SRC_LIST)
aux_source_directory(./emp-tool/utils    SRC_LIST)
aux_source_directory(./emp-tool/circuits       SRC_LIST)
aux_source_directory(./emp-tool/io       SRC_LIST)
aux_source_directory(./emp-tool/gc       SRC_LIST)
aux_source_directory(./emp-tool/garble  SRC_LIST)
aux_source_directory(./emp-tool/execution       SRC_LIST)
aux_source_directory(./inlcude  SRC_LIST)


## dynamic
#if(NOT WIN32)
	add_library(${NAME} SHARED ${SRC_LIST})
#endif()

if(WIN32)
    target_compile_options(${NAME} PRIVATE /D "EMP_MPC_EXPORTS")
	target_link_libraries(${NAME} ${LIB_PROTOBUF} miracl)
endif()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "emp-tool"
    )

# static
add_library(${NAME}_static STATIC ${SRC_LIST})
if(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "EMP_MPC_STATIC")
	SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES FOLDER ${NAME})
	#target_link_libraries(${NAME}_static PUBLIC miracl)
endif()

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    OUTPUT_NAME ${NAME}_static
    FOLDER ${NAME}
)

# installation
install(DIRECTORY emp-tool DESTINATION include/)
install(DIRECTORY cmake/ DESTINATION cmake/)
#install(TARGETS ${NAME} DESTINATION lib)

# emp test macro
macro (emp_test _name)
	message(STATUS "emp tool test: ${_name}")
	if (WIN32)
		ADD_DEFINITIONS(-DEMP_MPC_STATIC=1)
	endif()

    #aux_source_directory(./testutils EXE_SRC_LIST)
    add_executable(emp_tool_${_name} ./test/${_name}.cpp)
    
    if(OT_NP_USE_MIRACL)
    	message(STATUS "the link libarary: ${NAME} , the target exe name: ${_name}")
    	if (WIN32)
        	target_link_libraries(emp_tool_${_name} ${NAME}_static ${OPENSSL_LIBRARIES} ${LIB_PROTOBUF})
        else()
        	target_link_libraries(emp_tool_${_name} ${NAME} ${OPENSSL_LIBRARIES} ${LIB_PROTOBUF})
        endif()
   	else()
   		message(STATUS "the link libarary: ${NAME} , the target exe name: ${_name}")
		if (NOT WIN32)
   			target_link_libraries(emp_tool_${_name} ${NAME} ${OPENSSL_LIBRARIES} ${RELIC_LIBRARIES} ${GMP_LIBRARIES} ${LIB_PROTOBUF})
		else()	
			target_link_libraries(emp_tool_${_name} ${OPENSSL_LIBRARIES} ${LIB_PROTOBUF} miracl emp-tool_static)
		endif()
   	endif()

    #link_libraries(${GMP_LIBRARIES} ${LIB_PROTOBUF})
    SET_TARGET_PROPERTIES(emp_tool_${_name} PROPERTIES FOLDER "emp-toolkit-test")
    
    #INSTALL(TARGETS ${_name} RUNTIME DESTINATION ${CMAKE_PREFIX_PATH}/bin)
endmacro()

# tests
emp_test(prg)
emp_test(hash)
emp_test(prp)
emp_test(com)
emp_test(netio)
emp_test(bit)
emp_test(int)
emp_test(float)
emp_test(garble)
emp_test(gen_circuit)

