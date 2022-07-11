#include <iostream>
#include <cassert>
#include "headers/chat.h"

int main() {
    auto ch = new Chat;
    ch->reg("name", "test", 4);
    bool right = ch->login("name", "test", 4);
    bool wrong = ch->login("name", "tost", 4);
    assert(right == true);
    assert(wrong == false);
    return 0;
}
