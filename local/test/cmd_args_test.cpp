#include "../src/cmd_line_args.h"

#include <cassert>
#include <iostream>

void test_basic_join();
void test_basic_open();
void test_basic_new();
void test_no_mode();
void test_flags();
void test_flags_reversed();
void test_flags_no_mode();
void test_multiple_modes();
void test_invalid_mode();
void test_invalid_flag();

int main() {
    test_basic_join();
    test_basic_open();
    test_basic_new();
    test_no_mode();
    test_flags();
    test_flags_reversed();
    test_flags_no_mode();
    test_multiple_modes();
    test_invalid_mode();
    test_invalid_flag();

    std::cout << "All test cases passed\n";

    return 0;
}


void test_basic_join() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect join", f);
    assert(!f.help);
    assert(!f.vim);
    assert(f.mode == cmd_processor::Mode::kJoin);
    assert(ec == cmd_processor::ErrorCode::kSuccess);
}


void test_basic_open() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect open", f);
    assert(!f.help);
    assert(!f.vim);
    assert(f.mode == cmd_processor::Mode::kOpen);
    assert(ec == cmd_processor::ErrorCode::kSuccess);
}

void test_basic_new() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect new", f);
    assert(!f.help);
    assert(!f.vim);
    assert(f.mode == cmd_processor::Mode::kNew);
    assert(ec == cmd_processor::ErrorCode::kSuccess);
}

void test_no_mode() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect", f);
    assert(!f.help);
    assert(!f.vim);
    assert(ec == cmd_processor::ErrorCode::kNoMode);
}

void test_flags() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect new -vim", f);
    assert(!f.help);
    assert(f.vim);
    assert(f.mode == cmd_processor::Mode::kNew);
    assert(ec == cmd_processor::ErrorCode::kSuccess);
}

void test_flags_reversed() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect -vim new", f);
    assert(!f.help);
    assert(f.vim);
    assert(f.mode == cmd_processor::Mode::kNew);
    assert(ec == cmd_processor::ErrorCode::kSuccess);
}

void test_flags_no_mode() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect -help", f);
    assert(f.help);
    assert(!f.vim);
    assert(ec == cmd_processor::ErrorCode::kNoMode);
}

void test_multiple_modes() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect new join", f);
    assert(ec == cmd_processor::ErrorCode::kMultipleModes);
}

void test_invalid_mode() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect create", f);
    assert(ec == cmd_processor::ErrorCode::kInvalidMode);
}

void test_invalid_flag() {
    cmd_processor::Flags f;
    cmd_processor::ErrorCode ec = cmd_processor::ProcessCommandLine("./tconnect new -vim -invalid -help", f);
    assert(ec == cmd_processor::ErrorCode::kInvalidFlag);
}