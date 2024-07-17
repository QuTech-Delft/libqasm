message(STATUS "Building docs")

find_program(MKDOCS mkdocs REQUIRED)

set(sources
    src/about/contributing.md
    src/about/license.md
    src/about/release-notes.md
    src/api/cpp.md
    src/api/emscripten.md
    src/api/python.md
    src/dev-guide/dev-guide.md
    src/dev-guide/cpp.md
    src/dev-guide/emscripten.md
    src/dev-guide/python.md
    src/user-guide/user-guide.md
    src/user-guide/cpp.md
    src/user-guide/emscripten.md
    src/user-guide/python.md
    src/index.md
)

add_custom_target(doc ALL
    COMMAND ${CMAKE_COMMAND}
    -E env PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../scripts/python
    ${MKDOCS} build -f ${CMAKE_CURRENT_SOURCE_DIR}/mkdocs.yml
    # MkDocs requires the site dir to be outside of the doc dir.
    --site-dir ${CMAKE_CURRENT_BINARY_DIR}/html
    --no-directory-urls
    SOURCES ${sources}
)

include(GNUInstallDirs)
install(
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html/
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/doc/libqasm OPTIONAL
)
