/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <iostream>
#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <QImage>
#include <QString>

#include <pkmn/paths.hpp>

namespace fs = boost::filesystem;

/*
 * This code is heavily based on Michael Bond's Spinda generation
 * code from the repository listed below:
 *
 * https://github.com/codemonkey85/PKMDS-Win32-DLL
 */

namespace pkmn
{
    namespace Spot_Locations
    {
        enum locations
        {
            TOP_LEFT,
            TOP_RIGHT,
            BOTTOM_LEFT,
            BOTTOM_RIGHT
        };
    }

    namespace Spinda_Gen3
    {
        enum colors
        {
            SPOT_DARK = 0xB05828,
            SPOT_NORMAL = 0xD86838,
            SPOT_LIGHT = 0xD88848
        };
    }

    namespace Spinda_Gen3Shiny
    {
        enum colors
        {
            SPOT_DARK = 0xD86838,
            SPOT_NORMAL = 0x90A038,
            SPOT_LIGHT = 0xB0C058
        };
    }

    namespace Spinda_Gen4
    {
        enum colors
        {
            SPOT_DARK = 0xBD4A31,
            SPOT_NORMAL = 0xEF524A,
            SPOT_LIGHT = 0xEF947B
        };
    }

    namespace Spinda_Gen4Shiny
    {
        enum colors
        {
            SPOT_DARK = 0x7B9C00,
            SPOT_NORMAL = 0xA5CE10,
            SPOT_LIGHT = 0xCED65A
        };
    }

    std::string generate_spinda_image(uint32_t personality, bool gen3, bool shiny)
    {
        /*
         * All Generation III games share a Spinda sprites, as do all Generation IV-V
         * games. Therefore, these two options are sufficient.
         */
        std::string generation_str = (gen3) ? "generation-3/ruby-sapphire"
                                            : "generation-4/diamond-pearl";

        //Construct path of original Spinda sprite
        fs::path original_path = fs::path(get_images_dir()) / generation_str;
        if(shiny) original_path /= "shiny";
        original_path /= "327.png";
        std::string original = original_path.string();

        //Create path of temporary image location, return if already exists
        std::string image_filename = (shiny) ? str(boost::format("spinda%d_%ds.png")
                                                   % (gen3 ? 3 : 4)
                                                   % personality)
                                             : str(boost::format("spinda%d_%d.png")
                                                   % (gen3 ? 3 : 4)
                                                   % personality);
        fs::path tmp_image_path = fs::path(get_tmp_dir()) / image_filename;
        if(fs::exists(tmp_image_path)) return tmp_image_path.string();

        //Since it doesn't exist, create it, then return it
        QImage spinda_image;
        QString input_path = QString::fromStdString(original);
        QString output_path = QString::fromStdString(tmp_image_path.string());

        //If for some reason we can't create something in the tmp directory, return the original
        if(not spinda_image.load(input_path, "PNG")) return original;

        /*
         * After image manipulation, save to temporary location and return path.
         * If saving fails, return original.
         */
        if(not spinda_image.save(output_path, "PNG")) return original;

        return tmp_image_path.string();
    }
}
