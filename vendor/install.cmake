if((NOT RXCPP_INCLUDE_DIR) OR (NOT EXISTS ${RXCPP_INCLUDE_DIR}))
    message("Unable to find rxcpp, cloning...")

    execute_process(COMMAND git submodule update --init -- vendor/rxcpp
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    set(RXCPP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/rxcpp/include/
            CACHE PATH "rxcpp include directory")

    install(DIRECTORY ${RXCPP_INCLUDE_DIR}/rxcpp DESTINATION include)

    # Setup a target

    add_library(rxcpp INTERFACE)
    target_include_directories(rxcpp INTERFACE
            $<BUILD_INTERFACE:${RXCPP_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>)

    install(TARGETS rxcpp EXPORT rxcpp DESTINATION include)
endif()
