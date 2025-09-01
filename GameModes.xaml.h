#pragma once

#include "GameModes.g.h"

#include "GameMode.h"

namespace winrt::SSnake::implementation
{
    struct GameModes : GameModesT<GameModes>
    {
        GameModes();

    public:
        void GameModes_ItemsView_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        winrt::SSnake::GAME_MODE get_game_mode();
        void select_game_mode(winrt::SSnake::GAME_MODE mode);

        winrt::hstring to_modeName(winrt::SSnake::GAME_MODE mode);
    private:
        winrt::Windows::Foundation::Collections::IObservableVector<SSnake::GameMode> m_gameModeArray;
        winrt::SSnake::GAME_MODE m_selectMode{ GAME_MODE::EASY };
    };
}

namespace winrt::SSnake::factory_implementation
{
    struct GameModes : GameModesT<GameModes, implementation::GameModes>
    {
    };
}
