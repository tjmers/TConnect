#include "cmd_line_args.h"

#include <cstring>

namespace cmd_processor {

/// @brief Initalizes flags to default value.
/// @param flags Flags to initalize.
void InitializeFlags(Flags& flags) {
    // Set all flags to default values here

    flags.mode = Mode::kInvalid;
    flags.vim = false;
    flags.help = false;
}

/// @brief Determines if the flags are all in valid, usable states.
/// @param flags Flags to check.
/// @return Cooresponding error codes.
ErrorCode ValidFlags(Flags& flags) {
    if (flags.mode == Mode::kInvalid) {
        return ErrorCode::kNoMode;
    }

    return ErrorCode::kSuccess;
}

/// @brief Sets the flag, or returns the corresponding error code.
/// @param flag Flag to set (not null-terminated).
/// @param flag_size Number of characters that the flag is.
/// @param flags Program flags to set.
/// @return The error code of the function.
ErrorCode SetFlag(const char* flag, std::size_t flag_size, Flags& flags) {
    if (flag_size == 3 && strncmp(flag, "vim", 3) == 0) {
        flags.vim = true;
    } else if (flag_size == 4 && strncmp(flag, "help", 4) == 0) {
        flags.help = true;
    } else {
        return ErrorCode::kInvalidFlag;
    }

    return ErrorCode::kSuccess;
}

/// @brief Sets the mode, or returns the corresponding error code.
/// @param mode The mode to set (not null-terminated).
/// @param mode_size Number of characters that the mode is.
/// @param flags Program flags to set.
/// @return The error code of the function.
ErrorCode SetMode(const char* mode, std::size_t mode_size, Flags& flags) {
    if (flags.mode != Mode::kInvalid) {
        return ErrorCode::kMultipleModes;
    }

    if (mode_size == 3 && strncmp(mode, "new", 3) == 0) {
        flags.mode = Mode::kNew;
    } else if (mode_size == 4 && strncmp(mode, "open", 4) == 0) {
        flags.mode = Mode::kOpen;
    } else if (mode_size == 4 && strncmp(mode, "join", 4) == 0) {
        flags.mode = Mode::kJoin;
    } else {
        return ErrorCode::kInvalidMode;
    }

    return ErrorCode::kSuccess;
}

/// @brief Determines if str is a flag or a mode and sets it respectively.
/// @param str Flag or mode to set, must be at least size 1 or the code will probably break (str is not null-terminated).
/// @param size Number of characters / bytes long `str` is.
/// @param flags Program flags to set.
/// @return Error code of the function.
ErrorCode SetFlagOrMode(const char* str, std::size_t size, Flags& flags) {
    if (str[0] == '-') {
        // Flag
        return SetFlag(str + 1, size - 1, flags);
    } else {
        // Mode
        return SetMode(str, size, flags);
    }
}


ErrorCode ProcessCommandLine(const char* line, Flags& flags) {

    if (!line[0]) {
        return ErrorCode::kInvalidCommandLine;
    }

    InitializeFlags(flags);

    int last_space = -1;

    // Main logic loop
    // Can start at i=1 beacuse the string is at least size 1 (function would have already returned otherwise)
    int i;
    for (i = 1; line[i]; ++i) {
        if (line[i] == ' ') {
            if (last_space == -1) {
                last_space = i;
                continue;
            }

            // Process the (english) that is from last_space + 1 to i
            ErrorCode ec = SetFlagOrMode(line + last_space + 1, i - last_space - 1, flags);

            // Return early if an error was encountered
            if (ec != ErrorCode::kSuccess) {
                return ec;
            }

            last_space = i;
        }
    }

    if (last_space == -1) return ErrorCode::kNoMode;

    // Process last one
    if (last_space != i - 1) {
        ErrorCode ec = SetFlagOrMode(line + last_space + 1, i - last_space - 1, flags);
        if (ec != ErrorCode::kSuccess) {
            return ec;
        }
    }

    

    return ValidFlags(flags);
}


const char* ErrorCodeToString(ErrorCode ec) {
    switch (ec) {
        case ErrorCode::kInvalidCommandLine:
            return "The given command line is invalid";
        case ErrorCode::kInvalidFlag:
            return "Invalid flag. Use `./tconnect -help` for more information";
        case ErrorCode::kInvalidMode:
            return "Invalid mode found. Valid modes include new, open, and join";
        case ErrorCode::kMultipleModes:
            return "Multiple modes found. Only specify one argument after ./tconnect as a mode";
        case ErrorCode::kNoMode:
            return "No mode found. Specify a mode when running this executable from the command prompt: ex. ./tconnect join";
        case ErrorCode::kSuccess:
            return "Success!";
        default:
            // This should never be reached
            return "Unknown error code.";
    }
}


};