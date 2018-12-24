find_package(OpenSSL REQUIRED)

if (NOT OT_NP_USE_MIRACL)
find_package(relic REQUIRED)
find_package(gmp REQUIRED)
endif(OT_NP_USE_MIRACL)

find_package(Boost REQUIRED COMPONENTS system)
find_path(EMP-TOOL_INCLUDE_DIR NAMES emp-tool/emp-tool.h)
find_library(EMP-TOOL_LIBRARY NAMES emp-tool)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(EMP-TOOL DEFAULT_MSG EMP-TOOL_INCLUDE_DIR EMP-TOOL_LIBRARY)

add_definitions(-DEMP_CIRCUIT_PATH=${EMP-TOOL_INCLUDE_DIR}/emp-tool/circuits/files/)
if(EMP-TOOL_FOUND)
	set(EMP-TOOL_LIBRARIES ${EMP-TOOL_LIBRARY} ${RELIC_LIBRARIES} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${GMP_LIBRARIES})
	set(EMP-TOOL_INCLUDE_DIRS ${EMP-TOOL_INCLUDE_DIR} ${RELIC_INCLUDE_DIR} ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIRS} ${GMP_INCLUDE_DIR})
endif()
