#include <cstdio>
#include <utility>
#include <vector>

struct Test {
    const char* label;
    Test(const char* label): label(label) { }
    Test(): label("Default constructed") { }
    Test(const Test&): label("Copy constructed") { }
    Test& operator=(const Test&) { label = "Copy assigned"; return *this; }
    Test(Test&&) noexcept : label("Move constructed") { }
    Test& operator=(Test&&) noexcept { label = "Move assigned"; return *this; }
};

// Test MakeTest() { return { }; }
// void UseTest(const Test& test) { puts(test.label); }

int main() {
    // Test a;
    std::vector<Test> vec;
    vec.push_back({});
    puts(vec[0].label);
    vec.resize(3);
    puts(vec[0].label);
}
