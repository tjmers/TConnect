#pragma once

/// @brief Processor for the command that starts the program.
namespace cmd_processor {

enum Mode {
    kNew,
    kOpen,
    kJoin,
    kInvalid
};

enum ErrorCode {
    kSuccess,
    kInvalidMode,
    kNoMode,
    kMultipleModes,
    kInvalidFlag,
    kInvalidCommandLine
};

struct Flags {
    Mode mode;
    bool vim;
    bool help;
};


/// @brief Parses the `line` and returns all modified flags in flags.
/// @param line Command line to parse. Should include the full command line, including the part calling the program. Ex: ./tconnect join 17.172.224.47:7000
/// @param flags The unitialized flags to set through command-line arguments.
/// @return ErrorCode reprsenting the status of this function.
ErrorCode ProcessCommandLine(const char* line, Flags& flags);

/// @brief Converts the error code to an easy-to-understand user-friendly string version.
/// @param ec Error code to convert.
/// @return The descriptive string on the error.
const char* ErrorCodeToString(ErrorCode ec);

};