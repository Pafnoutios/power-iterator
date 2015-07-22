# Power Iterator Unit Tests
# © John Szwast

set(headers include/combination_iterator.h include/power_iterator.h)

set(sources)

source_group("Header Files" FILES ${headers})
source_group("Source Files" FILES ${sources})

add_library(power_iterators ${sources} ${headers})
set_target_properties(power_iterators PROPERTIES LINKER_LANGUAGE CXX)
