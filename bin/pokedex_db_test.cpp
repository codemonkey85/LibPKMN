#include <sqlite3.h>
#include <pkmnsim/base_pkmn.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    std::string foo = argv[1];

    base_pkmn bar = get_pokemon(foo);

    return 0;
}
