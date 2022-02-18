add_library(ast STATIC ast.cpp)
target_include_directories(ast PUBLIC ../include)
target_compile_features(ast PUBLIC cxx_std_17)

add_library(parser STATIC parser.cpp)
target_include_directories(parser PUBLIC ../include)
target_link_libraries(parser PUBLIC ast)
target_compile_features(parser PUBLIC cxx_std_17)

add_library(interpreter STATIC interpreter.cpp)
target_include_directories(interpreter PUBLIC ../include)
target_link_libraries(interpreter PUBLIC parser)
target_compile_features(interpreter PUBLIC cxx_std_17)

add_library(bluefin STATIC bluefin.cpp)
target_include_directories(bluefin PUBLIC ../include)
target_link_libraries(bluefin PUBLIC interpreter)
target_compile_features(bluefin PUBLIC cxx_std_17)

add_executable(repl main.cpp)
target_compile_features(repl PRIVATE cxx_std_17)
target_link_libraries(repl PUBLIC bluefin)
