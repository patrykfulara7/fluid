target_sources(${PROJECT_NAME}
    PRIVATE
    main.cxx
)

target_precompile_headers(${PROJECT_NAME}
    PRIVATE
    pch.hxx
)

configure_file(CMakePlaceholders.hxx.in ${CMAKE_CURRENT_LIST_DIR}/CMakePlaceholders.hxx)