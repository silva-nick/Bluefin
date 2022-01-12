add_library(ast STATIC ast.cpp)
target_include_directories(ast PUBLIC ../include)
target_compile_features(ast PUBLIC cxx_std_17)

add_library(calc STATIC calc.cpp)
target_include_directories(calc PUBLIC ../include)
target_link_libraries(calc PUBLIC ast)
#target_link_libraries(calc PRIVATE llvm)
target_compile_features(calc PUBLIC cxx_std_17)

add_executable(bluefin main.cpp)
target_compile_features(bluefin PRIVATE cxx_std_17)
target_link_libraries(bluefin PUBLIC calc)
