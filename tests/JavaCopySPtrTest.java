/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

import org.nc.*;

/*
 * This test's LibPKMN's internal functionality for copying shared pointers,
 * which comes into place in custom copy constructors and assignment operators.
 */

public class JavaCopySPtrTest
{
    public static void main(String[] args)
    {
        TeamPokemonSPtr TeamPKMN = TeamPokemon.make("Darmanitan", "X", 70, "None", "None", "None", "None");

        BasePokemonSPtr BasePKMN1 = TeamPKMN.getBasePokemon(true);
        BasePokemonSPtr BasePKMN2 = TeamPKMN.getBasePokemon(false);

        BasePKMN1.setForm("Standard");
        BasePKMN2.setForm("Zen");

        if(TeamPKMN.getPokemonID() == BasePKMN1.getPokemonID())
        {
            throw new RuntimeException("Failed to copy BasePokemonSPtr from TeamPokemonSPtr!");
        }
        if(TeamPKMN.getPokemonID() != BasePKMN2.getPokemonID())
        {
            throw new RuntimeException("Improper BasePokemonSPtr returned from TeamPokemonSPtr!");
        }
    }
}
