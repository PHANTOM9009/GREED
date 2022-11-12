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


#ifndef TGUI_BACKEND_GUI_SFML_HPP
#define TGUI_BACKEND_GUI_SFML_HPP

#include <TGUI/Backend/Window/BackendGui.hpp>

#include <SFML/Window.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API BackendGuiSFML : public BackendGui
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor
        ///
        /// @warning setGuiWindow has to be called by the subclass that inherits from this base class before the gui is used
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        BackendGuiSFML() = default;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Passes the event to the widgets
        ///
        /// @param event  The event that was polled from the window
        ///
        /// @return Has the event been consumed?
        ///         When this function returns false, then the event was ignored by all widgets.
        ///
        /// You should call this function in your event loop.
        /// @code
        /// sf::Event event;
        /// while (window.pollEvent(event) {
        ///     gui.handleEvent(event);
        /// }
        /// @endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool handleEvent(sf::Event event);
        using BackendGui::handleEvent;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Give the gui control over the main loop
        ///
        /// @param clearColor  background color of the window
        ///
        /// This function is only intended in cases where your program only needs to respond to gui events.
        /// For multimedia applications, games, or other programs where you want a high framerate or do a lot of processing
        /// in the main loop, you should use your own main loop.
        ///
        /// You can consider this function to execute something similar (but not identical) to the following code:
        /// @code
        /// while (window.isOpen())
        /// {
        ///     while (window.waitEvent(event))
        ///     {
        ///         gui.handleEvent(event);
        ///         if (event.type == sf::Event::Closed)
        ///             window.close();
        ///     }
        ///
        ///     window.clear({240, 240, 240});
        ///     gui.draw();
        ///     window.display();
        /// }
        /// @endcode
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void mainLoop(Color clearColor = {240, 240, 240}) override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Helper function that converts an SFML event to a TGUI event
        /// @param eventSFML  The input SFML event
        /// @param eventTGUI  The output TGUI event
        /// @return Did the SFML event match on a TGUI event and has the output event been written to?
        ///
        /// You typically don't need this function as you can call handleEvent directly with an sf::Event.
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool convertEvent(const sf::Event& eventSFML, Event& eventTGUI);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Sets the window which the gui should use
        ///
        /// @param window  The SFML window that will be used by the gui
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        virtual void setGuiWindow(sf::Window& window);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Updates the view and changes the size of the root container when needed
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void updateContainerSize() override;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    protected:

        sf::Window* m_window = nullptr;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    };
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_BACKEND_GUI_SFML_HPP
