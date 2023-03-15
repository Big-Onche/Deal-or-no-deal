#include "main.h"

using namespace std;

void conoutf(ConsoleColors background, ConsoleColors text, const char* format, ...) // print to console with ANSI colors
{
    cout << "\033[" << text + 10 << ";" << background << "m"; // Set color

    // Print formatted text
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    cout << "\033[" << C_DEF << "m"; // Reset color

}

void logoutf(const char* format, ...) // Print to log file
{
    FILE* logfile = fopen("log.txt", "w"); // Open log file in append mode

    // Print formatted text
    va_list args;
    va_start(args, format);
    vfprintf(logfile, format, args);
    vfprintf(logfile, "\n", args); //newline
    va_end(args);

    fclose(logfile); // Close log file
}

void clearConsole()
{
    #if __linux__
      system("clear");
    #elif _WIN32
      system("cls");
    #endif
}