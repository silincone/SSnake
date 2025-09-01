#pragma once

#include "MainWindow.g.h"

#include "Settings.xaml.h"
#include "AboutPage.xaml.h"

namespace winrt::SSnake::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        void AppTitleBar_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction SettingsButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction HeartButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

    public:
        static winrt::SSnake::GAME_MODE m_gameMode;
        static winrt::SSnake::GAME_STATE m_gameState;
        static winrt::SSnake::MainWindow m_currentWindow;

    public:
        static winrt::SSnake::MainWindow CurrentWindow();
    };
}

namespace winrt::SSnake::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
