add_library(synth STATIC)
target_sources(synth PRIVATE env.c lfo.c osc.c)
target_include_directories(synth PUBLIC . ${DEVICE_HEADERS})
target_link_libraries(synth PRIVATE log)