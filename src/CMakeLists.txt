add_library(ast STATIC ast.cpp)
target_include_directories(ast PUBLIC ../include)
target_compile_features(ast PUBLIC cxx_std_17)

add_library(bluefin STATIC bluefin.cpp)
target_include_directories(bluefin PUBLIC ../include)
target_link_libraries(bluefin PUBLIC ast)
#target_link_libraries(bluefin PRIVATE llvm)
target_compile_features(bluefin PUBLIC cxx_std_17)

add_executable(repl main.cpp)
target_compile_features(repl PRIVATE cxx_std_17)
target_link_libraries(repl PUBLIC bluefin)
