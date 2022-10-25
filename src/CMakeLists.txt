add_library(token STATIC token.cpp)
target_include_directories(token PUBLIC ../include)
target_compile_features(token PUBLIC cxx_std_17)

add_library(ast STATIC ast.cpp)
target_include_directories(ast PUBLIC ../include)
target_link_libraries(ast PUBLIC token)
target_compile_features(ast PUBLIC cxx_std_17)

add_library(parser STATIC parser.cpp)
target_include_directories(parser PUBLIC ../include)
target_link_libraries(parser PUBLIC ast)
target_compile_features(parser PUBLIC cxx_std_17)

add_library(interpreter STATIC interpreter.cpp)
target_include_directories(interpreter PUBLIC ../include)
target_link_libraries(interpreter PUBLIC parser)
target_compile_features(interpreter PUBLIC cxx_std_17)

add_library(symboltable STATIC symboltable.cpp)
target_include_directories(symboltable PUBLIC ../include)
target_link_libraries(symboltable PUBLIC interpreter)
target_compile_features(symboltable PUBLIC cxx_std_17)

add_library(bluefin STATIC bluefin.cpp)
target_include_directories(bluefin PUBLIC ../include)
target_link_libraries(bluefin PUBLIC symboltable)
target_compile_features(bluefin PUBLIC cxx_std_17)

add_executable(blue main.cpp)
target_compile_features(blue PRIVATE cxx_std_17)
target_link_libraries(blue PUBLIC bluefin)
