add_subdirectory(support/gtest)

set(TEST_MAIN_SRC support/test-main.cc support/gtest-extra.cc
                  support/gtest-extra.h)
add_library(test-main STATIC ${TEST_MAIN_SRC})
target_include_directories(
  test-main PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                   $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/support>)
target_link_libraries(test-main gtest fmt)
if (NOT FMT_OS)
  target_compile_definitions(test-main PUBLIC FMT_USE_FCNTL=0)
endif ()

function (add_fmt_test name)
  cmake_parse_arguments(ADD_FMT_TEST "HEADER_ONLY" "" "" ${ARGN})

  set(source_dirs core embedded support)
  foreach (source_dir ${source_dirs})
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/${source_dir}/${name}.cc")
      set(main_source "${CMAKE_CURRENT_LIST_DIR}/${source_dir}/${name}.cc")
      break()
    endif ()
  endforeach ()
  set(sources ${main_source} ${ADD_FMT_TEST_UNPARSED_ARGUMENTS})
  if (ADD_FMT_TEST_HEADER_ONLY)
    set(sources ${sources} ${TEST_MAIN_SRC})
    if (FMT_OS)
      set(sources ${sources} ../src/os.cc)
    endif ()
    set(libs gtest fmt-header-only)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set(PEDANTIC_COMPILE_FLAGS ${PEDANTIC_COMPILE_FLAGS} -Wno-weak-vtables)
    endif ()
  else ()
    set(libs test-main fmt)
  endif ()
  add_executable(${name} ${sources})
  if (NOT FMT_OS)
    target_compile_definitions(${name} PRIVATE FMT_USE_FCNTL=0)
  endif ()
  target_include_directories(
    ${name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/support
                    ${CMAKE_CURRENT_LIST_DIR}/embedded
                    ${CMAKE_CURRENT_LIST_DIR})
  target_link_libraries(${name} ${libs})

  if (FMT_PEDANTIC)
    target_compile_options(${name} PRIVATE ${PEDANTIC_COMPILE_FLAGS})
  endif ()
  if (FMT_WERROR)
    target_compile_options(${name} PRIVATE ${WERROR_FLAG})
  endif ()
  add_test(NAME ${name} COMMAND ${name})
endfunction ()

add_fmt_test(args-test)
add_fmt_test(base-test)
add_fmt_test(assert-test)
add_fmt_test(enum-test)
add_fmt_test(format-test ${CMAKE_CURRENT_LIST_DIR}/embedded/mock-allocator.h)
add_fmt_test(format-impl-test HEADER_ONLY
             ${CMAKE_CURRENT_LIST_DIR}/embedded/header-only-test.cc)
add_fmt_test(compile-test)
add_fmt_test(printf-test)
add_fmt_test(no-builtin-types-test HEADER_ONLY)
add_fmt_test(enforce-checks-test)
target_compile_definitions(enforce-checks-test
                           PRIVATE -DFMT_ENFORCE_COMPILE_STRING)

add_executable(perf-sanity embedded/perf-sanity.cc)
target_link_libraries(perf-sanity fmt::fmt)

if (FMT_PEDANTIC)
  if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    check_cxx_compiler_flag(-fno-exceptions HAVE_FNO_EXCEPTIONS_FLAG)
  endif ()
  if (HAVE_FNO_EXCEPTIONS_FLAG)
    add_library(noexception-test ../src/format.cc embedded/noexception-test.cc)
    target_include_directories(noexception-test
                               PRIVATE ${PROJECT_SOURCE_DIR}/include)
    target_compile_options(noexception-test PRIVATE -fno-exceptions)
    target_compile_options(noexception-test PRIVATE ${PEDANTIC_COMPILE_FLAGS})
  endif ()

  add_library(nolocale-test ../src/format.cc)
  target_include_directories(nolocale-test
                             PRIVATE ${PROJECT_SOURCE_DIR}/include)
  target_compile_definitions(nolocale-test
                             PRIVATE FMT_STATIC_THOUSANDS_SEPARATOR=1)
endif ()
