/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2022 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_THEME_LOADER_HPP
#define TGUI_THEME_LOADER_HPP


#include <TGUI/String.hpp>
#include <TGUI/Loading/DataIO.hpp>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <set>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Base class for theme loader implementations
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API BaseThemeLoader
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Virtual destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual ~BaseThemeLoader() = default;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Optionally already do some work when only the primary parameter is known yet
        ///
        /// @param primary    Primary parameter of the loader (filename of the theme file in DefaultThemeLoader)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void preload(const String& primary);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the global property-value pairs from the theme
        ///
        /// @param primary  Primary parameter of the loader
        ///
        /// For the default loader, the primary parameter is the filename while the secondary parameter is the section name.
        ///
        /// @return Map of property-value pairs
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual std::map<String, String> getGlobalProperties(const String& primary);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the property-value pairs from the theme
        ///
        /// @param primary    Primary parameter of the loader
        /// @param secondary  Secondary parameter of the loader
        ///
        /// For the default loader, the primary parameter is the filename while the secondary parameter is the section name.
        ///
        /// @return Map of property-value pairs
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual const std::map<String, String>& load(const String& primary, const String& secondary) = 0;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Check if the requested property-value pairs are available
        ///
        /// @param primary    Primary parameter of the loader
        /// @param secondary  Secondary parameter of the loader
        ///
        /// For the default loader, the primary parameter is the filename while the secondary parameter is the section name.
        ///
        /// @return Whether a map op property-value pairs is available
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual bool canLoad(const String& primary, const String& secondary) = 0;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Turns texture and font filenames into paths relative to the theme file
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void injectThemePath(const std::unique_ptr<DataIO::Node>& node, const String& path) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Resolves references to sections
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void resolveReferences(std::map<String, std::reference_wrapper<const std::unique_ptr<DataIO::Node>>>& sections,
                               const std::map<String, String>& globalProperties,
                               const std::unique_ptr<DataIO::Node>& node) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Default implementation for theme loading
    ///
    /// Themes are stored on disk in files which contain sections with a syntax similar to CSS.
    /// This loader will be able to extract the data from these files.
    ///
    /// On first access, the entire file will be cached, the next times the cached map is simply returned.
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class TGUI_API DefaultThemeLoader : public BaseThemeLoader
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the theme file in cache
        ///
        /// @param filename  Filename of the theme file to load
        ///
        /// @exception Exception when finding syntax errors in the file
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void preload(const String& filename) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the global property-value pairs from the theme
        ///
        /// @param filename  Filename of the theme file to load
        ///
        /// @return Map of property-value pairs
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::map<String, String> getGlobalProperties(const String& filename) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Loads the property-value pairs from the theme file
        ///
        /// @param filename   Filename of the theme file
        /// @param section    Name of the section inside the theme file
        ///
        /// @return Map of property-value pairs
        ///
        /// @exception Exception when finding syntax errors in the file
        /// @exception Exception when file did not contain requested class name
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::map<String, String>& load(const String& filename, const String& section) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Check if the requested property-value pairs are available
        ///
        /// @param filename   Filename of the theme file
        /// @param section    Name of the section inside the theme file
        ///
        /// @return Whether a map op property-value pairs is available
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool canLoad(const String& filename, const String& section) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Empties the caches and force files to be reloaded.
        ///
        /// @param filename  File to remove from cache.
        ///                  If no filename is given, the entire cache is cleared.
        ///
        /// The first time a filename is loaded, its contents is fully cached. If this function isn't called then loading the
        /// same file will only read the cache instead of reading the file from disk again.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        static void flushCache(const String& filename = "");


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Reads and return the contents of the entire file
        ///
        /// @param filename  Filename of the file to read
        ///
        /// @return Root node of the parsed file, obtained by calling DataIO::parse(fileContents)
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual std::unique_ptr<DataIO::Node> readFile(const String& filename) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:
        static std::map<String, std::map<String, std::map<String, String>>> m_propertiesCache;
        static std::map<String, std::map<String, String>> m_globalPropertiesCache;
    };


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_THEME_LOADER_HPP
