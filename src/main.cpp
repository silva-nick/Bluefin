#include "bluefin.hpp"

void runFile(char *file) {
    FILE *f = fopen(file, "r");

    // Find end of file and allocate string
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    char *code = new char[size];

    // Reset pointer to start of file and copy value to string
    rewind(f);
    fread(code, sizeof(char), size, f);

    // Run Bluefin on the text
    std::cout << bluefin::run(std::string(code)) << std::endl;

    delete[] code;
}

void runREPL() {
    while (true) {
        // Read
        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);

        // Check for escape characters
        if (input == "q" || input == "quit") {
            break;
        }

        // Eval and print
        std::cout << bluefin::run(input) << std::endl;
        bluefin::ERROR_STATUS = 0;
    }

    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc > 2) {
        std::cout << "Usage: blue [file]";
        return 1;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runREPL();
    }

    return bluefin::ERROR_STATUS;
}
