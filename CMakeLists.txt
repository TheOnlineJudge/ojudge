CMAKE_MINIMUM_REQUIRED (VERSION 3.1)
PROJECT (ojudge)
set(Boost_NO_BOOST_CMAKE ON)
FIND_PACKAGE (Wt REQUIRED Wt HTTP )
FIND_PACKAGE (Boost COMPONENTS filesystem program_options REQUIRED)

SET (CMAKE_CXX_FLAGS "-O2 -std=c++17 -g")

SET(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/build)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

SET (OJUDGE_VERSION_MAJOR 0)
SET (OJUDGE_VERSION_MINOR 1)
SET (OJUDGE_VERSION_RELEASE 1)

add_custom_command (
	OUTPUT version.h
	DEPENDS genversion.sh
	COMMAND ./genversion.sh ${OJUDGE_VERSION_MAJOR} ${OJUDGE_VERSION_MINOR} ${OJUDGE_VERSION_RELEASE}
)

add_custom_target (
	do_generateversion ALL
	DEPENDS version.h
)

add_subdirectory (lib)

add_executable (ojudgeapp ojudge.cpp)

add_dependencies (ojudgeapp do_generateversion)

TARGET_LINK_LIBRARIES (ojudgeapp
	${Boost_LIBRARIES}
	Wt::Wt
	Wt::HTTP
        Wt::Dbo
        Wt::DboPostgres
	ojudge
)
