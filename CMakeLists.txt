cmake_minimum_required(VERSION 3.10)

project(
  coax
  VERSION 0.1
  DESCRIPTION "C utility library"
  HOMEPAGE_URL "https://github.com/codebrainz/coax"
  LANGUAGES C
)

set(coax-lib-headers
  include/coax/alloc.h
  include/coax/array.h
  include/coax/buffer.h
  include/coax/coax.h
  include/coax/crc32.h
  include/coax/dict.h
  include/coax/filefuncs.h
  include/coax/funcs.h
  include/coax/list.h
  include/coax/log.h
  include/coax/macros.h
  include/coax/md5.h
  include/coax/pair.h
  include/coax/queue.h
  include/coax/ringbuf.h
  include/coax/stack.h
  include/coax/str.h
  include/coax/strfuncs.h
)

add_library(coax-lib
  lib/alloc.c
  lib/array.c
  lib/buffer.c
  lib/crc32.c
  lib/dict.c
  lib/filefuncs.c
  lib/list.c
  lib/log.c
  lib/md5.c
  lib/pair.c
  lib/queue.c
  lib/ringbuf.c
  lib/stack.c
  lib/str.c
  lib/strfuncs.c
  "${coax-lib-headers}"
)

target_compile_definitions(coax-lib PUBLIC CX_INSIDE_COMPILATION=1)
target_include_directories(coax-lib PUBLIC include)

set_target_properties(coax-lib
  PROPERTIES
    OUTPUT_NAME "coax"
    PUBLIC_HEADER "${coax-lib-headers}"
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS ON
)

install(TARGETS coax-lib
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/coax
)

install(FILES LICENSE README.md DESTINATION share/coax)

add_executable(coax-bin src/main.c)
target_include_directories(coax-bin PUBLIC include)
target_link_libraries(coax-bin coax-lib)
set_target_properties(coax-bin PROPERTIES OUTPUT_NAME "coax")
