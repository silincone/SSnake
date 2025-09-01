#pragma once

#include "GameMode.g.h"

namespace winrt::SSnake::implementation
{
    struct GameMode : GameModeT<GameMode>
    {
        GameMode() = default;
        GameMode(winrt::hstring modeName, winrt::hstring imagePath);

        winrt::hstring ModeName();
        winrt::hstring ImagePath();

        void ModeName(winrt::hstring modeName);
        void ImagePath(winrt::hstring imagePath);

        void SetGameModeData(winrt::hstring modeName, winrt::hstring imagePath);
    private:
        winrt::hstring m_modeName, m_imagePath;
    };
}

namespace winrt::SSnake::factory_implementation
{
    struct GameMode : GameModeT<GameMode, implementation::GameMode>
    {
    };
}
