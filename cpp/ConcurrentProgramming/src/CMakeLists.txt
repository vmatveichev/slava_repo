set ("${PROJECT}_VERSION_MAJ" 0)
set ("${PROJECT}_VERSION_MIN" 1)
set ("${PROJECT}_VERSION_A" 1)
set ("${PROJECT}_VERSION_B" 1)
set ("${PROJECT}_VERSION" ${${PROJECT}_VERSION_MAJ}0${${PROJECT}_VERSION_MIN}0${${PROJECT}_VERSION_A}0${${PROJECT}_VERSION_B})

message(STATUS ${${PROJECT}_VERSION})

# main program file
set (MAIN_SOURCES
    prog.cpp
    )

# unpublic pairs of sources (like if it's core.cpp-core.h files). I don't specify prog here
# because it's main program file
#set (PRIVATE_CLASSES
#        core
#    )
set (PRIVATE_CLASSES)

# only header files which doesn't have pairs of sources
SET (HEADERS_ONLY
        prog_thread_cvar
        prog_thread_dlock
        prog_thread_fprom
        prog_thread
        prog_thread_mu
        prog_thread_ulock
        prog_thread_calobj
        prog_thread_packtask
    )

# public sources
set (PUBLIC_CLASSES)

# Used libraries
set (ADDITIONAL_LIBRARIES
    pthread
    )

set (PUBLIC_HEADERS)
set (SOURCES)

foreach (class ${PRIVATE_CLASSES})
    LIST (APPEND SOURCES ${class}.cpp)
    LIST (APPEND HEADERS ${class}.h)
endforeach ()

foreach (class ${HEADERS_ONLY})
    LIST (APPEND HEADERS ${class}.h)
endforeach ()
    
foreach (class ${PUBLIC_CLASSES})
    LIST (APPEND SOURCES ${class}.cpp)
    LIST (APPEND HEADERS ../include/${PROJECT}/${class}.h)
    LIST (APPEND PUBLIC_HEADERS ../include/${PROJECT}/${class}.h)
endforeach ()

add_executable (${PROJECT} ${MAIN_SOURCES} ${SOURCES})

target_link_libraries (${PROJECT} ${ADDITIONAL_LIBRARIES})

set_target_properties(${PROJECT} PROPERTIES VERSION "${${PROJECT}_VERSION}" SOVERSION "0")

INSTALL (
    TARGETS
    ${PROJECT}
    DESTINATION
    lib${LIB_SUFFIX}
)

INSTALL (
    FILES
    ${PUBLIC_HEADERS}
    DESTINATION
    include/${PROJECT}
)
