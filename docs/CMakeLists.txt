message(STATUS "Building docs")

find_program(MKDOCS mkdocs REQUIRED)

set(sources
    about/authors.md
    about/contributing.md
    about/license.md
    about/release-notes.md
    api/cpp.md
    api/emscripten.md
    api/python.md
    dev-guide/dev-guide.md
    dev-guide/cpp.md
    dev-guide/emscripten.md
    dev-guide/python.md
    user-guide/user-guide.md
    user-guide/cpp.md
    user-guide/emscripten.md
    user-guide/python.md
    index.md
)

add_custom_target(doc ALL
    COMMAND ${CMAKE_COMMAND}
    -E env PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../scripts/python
    ${MKDOCS} build
    -f ${CMAKE_CURRENT_SOURCE_DIR}/mkdocs.yml
    # MkDocs requires the site dir to be outside of the doc dir.
    --site-dir ${CMAKE_CURRENT_BINARY_DIR}/site
    SOURCES ${sources}
)

include(GNUInstallDirs)
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/site/
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/libqasm OPTIONAL
)
