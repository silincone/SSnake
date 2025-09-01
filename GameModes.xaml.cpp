#include "pch.h"
#include "GameModes.xaml.h"
#if __has_include("GameModes.g.cpp")
#include "GameModes.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SSnake::implementation
{
    GameModes::GameModes()
    {
        m_gameModeArray = winrt::single_threaded_observable_vector<SSnake::GameMode>();

        m_gameModeArray.Append(winrt::make<winrt::SSnake::implementation::GameMode>(L"Easy", L"Assets/e_SSnake.png"));
        m_gameModeArray.Append(winrt::make<winrt::SSnake::implementation::GameMode>(L"Difficult", L"Assets/e_SSnake.png"));
    }

    winrt::SSnake::GAME_MODE GameModes::get_game_mode()
    {
        auto selectedItem = GameModes_ItemsView().SelectedItem();

        if (selectedItem)
        {
            auto gameMode = selectedItem.as<winrt::SSnake::GameMode>();
            auto modeName = gameMode.ModeName();

            if (modeName == L"Easy") { return winrt::SSnake::GAME_MODE::EASY; }
            else { return winrt::SSnake::GAME_MODE::DIFFICULT; }

        }
        
        return winrt::SSnake::GAME_MODE::EASY;
    }

    void GameModes::select_game_mode(winrt::SSnake::GAME_MODE mode)
    {
        m_selectMode = mode;
    }

    winrt::hstring GameModes::to_modeName(winrt::SSnake::GAME_MODE mode)
    {
        if (mode == GAME_MODE::EASY)
        {
            return L"Easy";
        }
        else if (mode == GAME_MODE::DIFFICULT)
        {
            return L"Difficult";
        }

        return L"Easy";
    }
    
    void GameModes::GameModes_ItemsView_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        GameModes_ItemsView().ItemsSource(m_gameModeArray);

        auto itemsView = GameModes_ItemsView();
        auto items = itemsView.ItemsSource().try_as<winrt::Windows::Foundation::Collections::IVector<winrt::Windows::Foundation::IInspectable>>();

        if (items)
        {
            winrt::hstring targetModeName = to_modeName(m_selectMode);

            // Loop through the items to find the one that matches the game mode
            for (uint32_t i = 0; i < items.Size(); ++i)
            {
                auto item = items.GetAt(i).as<winrt::SSnake::GameMode>();
                if (item.ModeName() == targetModeName)
                {
                    // Select the item at the found index
                    itemsView.Select(i);
                    return;
                }
            }
        }
    }
}

