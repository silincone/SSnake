#pragma once

#include "PlayGround.g.h"

#include "MainWindow.xaml.h"
#include "GameModes.xaml.h"
#include "S_Snake.h"

using namespace S_Snake;

namespace winrt::SSnake::implementation
{
    struct PlayGround : PlayGroundT<PlayGround>
    {
        PlayGround();
        winrt::Windows::Foundation::IAsyncAction PlayGround_MainGrid_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void PlayGround_MainGrid_SizeChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const& e);
        void OnKeyDown(winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& e);
        bool CheckForPopups(winrt::Microsoft::UI::Xaml::XamlRoot xamlRoot);

    public:
        void StartGame();
        void ResetGame();
        void GenerateFood();
        void DrawGame();
        void CheckCollision();

    public:
        void UpdateGame(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::Foundation::IInspectable const& e);
        void UpdateScoreText();

        winrt::Windows::Foundation::IAsyncAction ShowGameOver();
        winrt::Windows::Foundation::IAsyncAction ShowGamePaused();

    private:
        // snakey boi
        std::unique_ptr<S_SNAKE> m_snake{ nullptr };

        // directions boi
        std::queue<S_SNAKE_DIRECTION> m_moveBuffer;
        S_SNAKE_DIRECTION m_currentDirection{ S_SNAKE_DIRECTION::INVALID };

        int m_score{ 0 };

        S_COORD m_foodPosition;

        // PlayGround Boundary
        std::uint32_t m_PlayGround_X{ 0 };
        std::uint32_t m_PlayGround_Y{ 0 };

        bool m_playGroundWhite{ false };

        // Main Stuff that makes the game run (game loop kinda)
        winrt::Microsoft::UI::Xaml::DispatcherTimer m_gameTimer{ nullptr };
        std::chrono::milliseconds m_interval;

        // YaY
        bool ateYourself{ false };

        int m_prevScoreForIntervalChange{ -1 };
        int m_prevScoreForPlayGroundColorChange{ -1 };
    };
}

namespace winrt::SSnake::factory_implementation
{
    struct PlayGround : PlayGroundT<PlayGround, implementation::PlayGround>
    {
    };
}
