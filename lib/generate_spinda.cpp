/*
 * Copyright (c) 2014 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

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
    std::string generate_spinda_image(uint32_t personality, bool gen3, bool shiny)
    {
        /*
         * All Generation III games share a Spinda sprites, as do all Generation IV-V
         * games. Therefore, these two options are sufficient.
         */
        std::string generation_str = (gen3) ? "generation-3/ruby"
                                            : "generation-4/diamond";

        //Construct path of original Spinda sprite
        fs::path original_path = fs::path(get_images_dir()) / generation_str;
        if(shiny) original_path /= "shiny";
        original_path /= "327.png";
        std::string original = original_path.string();

        //Create path of temporary image location, return if already exists
        std::string image_filename = (shiny) ? str(boost::format("spinda_%ds.png") % personality)
                                             : str(boost::format("spinda_%d.png") % personality);
        fs::path tmp_image_path = fs::path(get_tmp_dir()) / image_filename;
        if(fs::exists(tmp_image_path)) return tmp_image_path.string();

        //Since it doesn't exist, create it, then return it
        QImage spinda_input, spinda_output;
        QString input_path = QString::fromStdString(original);
        QString output_path = QString::fromStdString(tmp_image_path.string());

        //If for some reason we can't create something in the tmp directory, return the original
        if(not spinda_input.load(input_path)) return original;

        return tmp_image_path.string();
    }
}
