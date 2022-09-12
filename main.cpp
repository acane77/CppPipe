#include <iostream>
#include <functional>

#include "Pipe.h"

using namespace Acane::Pipe;

int main() {
    auto add_two =
            [] (int x, int y, int z) { return x+y+z; };

    std::string sum = 1 | Proxy(add_two, 2, 3)
                        | Add(2) | Sub(5) | Mul(7) | Neg()
                        | ToString() | Append(" Hello") | Append(" World");

    int ret = printf | Acane::Pipe::Call("%d %d %d\n", 1, 2, 3);
    printf("%d\n", ret);

    printf("%s\n", sum.c_str());

    std::vector<int> vec {1,2,3};
    size_t s = vec | Acane::Pipe::GetSize();
    printf | Acane::Pipe::Call("sizeof vec is %zu\n", s);

    return 0;
}
