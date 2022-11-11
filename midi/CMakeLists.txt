add_library(midi STATIC)
target_sources(midi PRIVATE midi.c)
target_include_directories(midi PUBLIC . ${DEVICE_HEADERS})

target_link_libraries(midi PRIVATE log)