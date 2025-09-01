#include "pch.h"
#include "GameMode.h"
#if __has_include("GameMode.g.cpp")
#include "GameMode.g.cpp"
#endif

namespace winrt::SSnake::implementation
{
	GameMode::GameMode(winrt::hstring modeName, winrt::hstring imagePath) : m_modeName{ modeName }, m_imagePath{ imagePath } {}

    winrt::hstring GameMode::ModeName()
    {
        return m_modeName;
    }

    winrt::hstring GameMode::ImagePath()
    {
        return m_imagePath;
    }

    void GameMode::ModeName(winrt::hstring modeName)
    {
        m_modeName = modeName;
    }

    void GameMode::ImagePath(winrt::hstring imagePath)
    {
        m_imagePath = imagePath;
    }

    void GameMode::SetGameModeData(winrt::hstring modeName, winrt::hstring imagePath)
    {
        m_modeName = modeName;
        m_imagePath = imagePath;
    }
}
