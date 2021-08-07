set(PROJECT_NAME OrbConnect)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "CommandParser.h"
    "Gbody.h"
    "http.h"
    "MessageStream.h"
    "mongoose.h"
    "NavigationCommandParser.h"
    "OapiCommandParser.h"
    "ObjectCommandParser.h"
    "OrbConnect.h"
    "PlanetCommandParser.h"
    "SubscriptionParser.h"
    "VesselCommandParser.h"
    "XRCtlCommandParser.h"
    "XRVesselCtrl.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Gbody.cpp"
    "http.cpp"
    "mongoose.c"
    "NavigationCommandParser.cpp"
    "OapiCommandParser.cpp"
    "ObjectCommandParser.cpp"
    "OrbConnect.cpp"
    "PlanetCommandParser.cpp"
    "SubscriptionParser.cpp"
    "VesselCommandParser.cpp"
    "XRCtlCommandParser.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE OrbConnect)

set_target_properties(${PROJECT_NAME} PROPERTIES
    COMMON_LANGUAGE_RUNTIME_RELEASE "No"
)
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    TARGET_NAME_DEBUG   "${PROJECT_NAME}"
    TARGET_NAME_RELEASE "${PROJECT_NAME}"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_DIRECTORY_DEBUG   "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Modules/Plugin"
    OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../../../../Modules/Plugin"
)
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
    MultiThreadedDLL
)
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PUBLIC
    "C:/Program Files %28x86%29/Microsoft SDKs/Windows/v7.0A/Include;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:"
        "WIN32;"
        "NDEBUG;"
        "_WINDOWS"
    ">"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Zi
        >
        $<$<CONFIG:Release>:
            /Ob1;
            /GF;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}
        >
        /W3;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
    target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /DEBUG
        >
        $<$<CONFIG:Release>:
            /DEBUG:FULL;
            /MACHINE:X86;
            /SUBSYSTEM:WINDOWS;
            /INCREMENTAL:NO
        >
        /NODEFAULTLIB:msvcirt.lib;
        /DYNAMICBASE:NO
    )
endif()

################################################################################
# Dependencies
################################################################################
set(ADDITIONAL_LIBRARY_DEPENDENCIES
    "$<$<CONFIG:Debug>:"
        "Orbitersdk"
    ">"
    "$<$<CONFIG:Release>:"
        "orbitersdk"
    ">"
    "orbiter;"
    "WS2_32;"
    "user32"
)
target_link_libraries(${PROJECT_NAME} PUBLIC "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

target_link_directories(${PROJECT_NAME} PUBLIC
    "C:/Program Files %28x86%29/Microsoft SDKs/Windows/v7.0A/Lib;"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../../lib"
)

