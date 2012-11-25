#include <pkmnsim/base_pkmn.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    base_pkmn foo = get_pokemon(argv[1]);
    foo.print_verbose();

    return 0;
}
