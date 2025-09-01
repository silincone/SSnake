#pragma once

#include "Settings.g.h"

namespace winrt::SSnake::implementation
{
    struct Settings : SettingsT<Settings>
    {
        Settings()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        winrt::SSnake::GAME_MODE get_game_mode();
        void select_game_mode(winrt::SSnake::GAME_MODE mode);
    };
}

namespace winrt::SSnake::factory_implementation
{
    struct Settings : SettingsT<Settings, implementation::Settings>
    {
    };
}
