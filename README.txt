Pokémon Simulator

A library that allows for the recreation and full analysis of most aspects of the Pokemon franchise's various game mechanics. A full team can be constructed, complete with held items and abilities, and from there, users can perform complete strategy analysis, giving them the edge they need to create a team that excels.

This library can also interact with the games themselves, allowing for teams and trainers to be imported, tweaked, and exported back into any Generation 1-5 ROM. Aspects that can be tweaked include Pokemon moves and abilities, trainer and Pokemon ID's, and more.

Currently included applications:
 * Damage Calculator (command line) - determine the damage range, given two Pokemon and a move
 * Type Analysis (command line) - expose common type weaknesses in a given team
 * Get Type Stats (command line, graphical) - find the best and worst Pokemon of each type for all stats

---

Pokémon © 2002-2013 Pokémon. © 1995-2013 Nintendo/Creatures Inc./GAME FREAK inc. TM, ® and Pokémon character names are trademarks of Nintendo. 

Pokémon info, algorithms, etc., all taken from:
Bulbapedia (http://bulbapedia.bulbagarden.net/wiki/Main_Page)
Database used courtesy of Alex Munroe (http://www.veekun.com).

Special thanks to Michael Bond (http://github.com/codemonkey85) for consulting on game mechanics.

---

Minimum required CMake version: 2.8

Tested with:
 * GCC 4.6.3 (Ubuntu 12.04)
 * GCC 4.7.3 (Cygwin 1.7.25)
 * Clang 3.0 (Ubuntu 12.04)
 * Microsoft Visual Studio 2010 (Windows 7)

Git submodules:
 * Pokemon-sim Images (https://github.com/ncorgan/Pokemon-sim-images) - Pokemon sprites and icons
 * SQLiteCpp (https://github.com/SRombauts/SQLiteCpp) - SQLite3 C++ wrapper
 * Pokehack (https://github.com/Grazfather/pokehack) - Gen 3 save editing library
 * PokeLib-NC (https://github.com/ncorgan/PokeLib-NC) - Gen 4 save editing library
 * PKMDS-G5 (https://github.com/codemonkey85/PKMDS-G5) - Gen 5 save editing library

Dependencies:
 * Boost 1.48 or above (http://www.boost.org)
 * SQLite3 (http://www.sqlite.org)
 * Qt4 (http://www.qt-project.org)

README last updated: 2013-10-27