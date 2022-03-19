list(APPEND MINIZ_SRC "miniz/miniz.c")
list(APPEND MINIZ_HPP "miniz/miniz.h")

source_group("" FILES ${MINIZ_SRC} ${MINIZ_HPP})

add_library("miniz" ${MINIZ_SRC} ${MINIZ_HPP})

set_property(TARGET "miniz" PROPERTY FOLDER "External Libraries")

disable_project_warnings("miniz")

if(MSVC)
  target_compile_definitions("miniz" PRIVATE _MBCS)
endif(MSVC)
