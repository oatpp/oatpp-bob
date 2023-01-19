add_executable(module-tests
        oatpp-bob/IntegerTest.cpp
        oatpp-bob/IntegerTest.hpp
        oatpp-bob/ObjectMapperTest.cpp
        oatpp-bob/ObjectMapperTest.hpp
        oatpp-bob/SkipTest.cpp
        oatpp-bob/SkipTest.hpp
        oatpp-bob/tests.cpp
        oatpp-bob/UtilsTest.cpp
        oatpp-bob/UtilsTest.hpp
)

set_target_properties(module-tests PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

target_include_directories(module-tests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(module-tests ${LIB_OATPP_EXTERNAL})
endif()

add_dependencies(module-tests ${OATPP_THIS_MODULE_NAME})

target_link_oatpp(module-tests)

target_link_libraries(module-tests
        PRIVATE ${OATPP_THIS_MODULE_NAME}
)

## TODO link dependencies here (if some)

add_test(module-tests module-tests)