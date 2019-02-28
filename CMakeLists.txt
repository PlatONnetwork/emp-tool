cmake_minimum_required (VERSION 2.8.11)
if(POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

project (emp-tool)

SET(NAME "emp-tool")
SET(CURRENT_DIR ${CMAKE_CURRENT_SOURCE_DIR})


list(APPEND CMAKE_MODULE_PATH ${CURRENT_DIR}/cmake)
include(cmake/common.cmake)
include(cmake/source_of_randomness.cmake)
include(cmake/threading.cmake)


###### includes here
include_directories(./)
include_directories(./include)

# source list
aux_source_directory(./emp-tool             SRC_LIST)
aux_source_directory(./emp-tool/utils       SRC_LIST)
aux_source_directory(./emp-tool/circuits    SRC_LIST)
aux_source_directory(./emp-tool/io          SRC_LIST)
aux_source_directory(./emp-tool/gc          SRC_LIST)
aux_source_directory(./emp-tool/garble      SRC_LIST)
aux_source_directory(./emp-tool/execution   SRC_LIST)
aux_source_directory(./inlcude              SRC_LIST)


###### libraries here
if(WIN32)
	link_libraries(Ws2_32)
endif()

link_relic_or_miracl() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake
link_protobuf() # ref ${CMAKE_SOURCE_DIR}/cmake/func.cmake


###### build targets here
# dynamic
if(WIN32 AND OT_NP_USE_MIRACL)
	add_library(${NAME} SHARED ${SRC_LIST} ${CMAKE_CURRENT_SOURCE_DIR}/include/emp-tool-miracl.def)
elseif(WIN32 AND OT_NP_USE_RELIC_WIN)
	add_library(${NAME} SHARED ${SRC_LIST} ${CMAKE_CURRENT_SOURCE_DIR}/include/emp-tool-relic.def)
else()
	add_library(${NAME} SHARED ${SRC_LIST})
endif()

if(WIN32)
    target_compile_options(${NAME} PRIVATE /D "EMP_MPC_EXPORTS")
endif()

SET_TARGET_PROPERTIES(${NAME} PROPERTIES
    OUTPUT_NAME ${NAME}
    FOLDER "emp-tool"
	VERSION ${SO_VERSION}
)


# static
add_library(${NAME}_static STATIC ${SRC_LIST})
if(WIN32)
	target_compile_options(${NAME}_static PRIVATE /D "EMP_MPC_STATIC")
else()
    SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES OUTPUT_NAME ${NAME})
endif()

SET_TARGET_PROPERTIES(${NAME}_static PROPERTIES
    FOLDER "emp-tool"
	VERSION ${SO_VERSION}
)

# installation
install(DIRECTORY emp-tool DESTINATION include/)
install(DIRECTORY cmake/ DESTINATION cmake/)
install_libraries(${NAME} ${NAME}_static)


# emp test macro
macro (emp_test _name)
	message(STATUS "emp tool test project name: ${_name}")

    add_executable(emp_tool_${_name} ./test/${_name}.cpp)
    target_link_libraries(emp_tool_${_name} ${NAME})

    #IF(NOT WIN32 OR OT_NP_USE_RELIC_WIN)
    #    target_link_libraries(emp_tool_${_name} ${GMP_LIBRARIES})
    #ENDIF()

    SET_TARGET_PROPERTIES(emp_tool_${_name} PROPERTIES FOLDER "emp-toolkit-test")
endmacro()


# tests
if(NOT OT_NP_USE_RELIC_WIN)
emp_test(prg)
endif()
emp_test(hash)
emp_test(prp)
emp_test(com)
emp_test(netio)
emp_test(bit)
emp_test(int)
emp_test(float)
emp_test(garble)
emp_test(gen_circuit)
