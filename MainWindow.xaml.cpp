#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SSnake::implementation
{
    SSnake::GAME_MODE MainWindow::m_gameMode = GAME_MODE::EASY;
    SSnake::GAME_STATE MainWindow::m_gameState = GAME_STATE::NOT_STARTED;
    SSnake::MainWindow MainWindow::m_currentWindow = nullptr;
    
    MainWindow::MainWindow()
    {
        m_currentWindow = *this;
    }

    winrt::SSnake::MainWindow MainWindow::CurrentWindow()
    {
        return m_currentWindow;
    }

	void MainWindow::AppTitleBar_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
        // Set the custom title bar Height
        AppTitleBar().Height(AppWindow().TitleBar().Height());

        // Set the custom title bar
        // AppTitleBar() is the x:Name of the Grid in XAML
        SetTitleBar(AppTitleBar());

        // Define draggable regions
        // These are the areas where the user can click and drag to move the window.
        // The system automatically handles the caption buttons (min/max/close) on the right.
        // So, we typically make the area *excluding* the caption buttons draggable.

        if (ExtendsContentIntoTitleBar())
        {
            // Get the width of the system caption buttons area
            int32_t captionButtonWidth = AppWindow().TitleBar().RightInset(); // RightInset gives the width of system buttons

            double titleBarWidth = AppTitleBar().ActualWidth();

            double draggableWidth = titleBarWidth - captionButtonWidth;
            // Create a Rect for the draggable region
            Windows::Graphics::RectInt32 draggableRect;
            draggableRect.X = 0;
            draggableRect.Y = 0;
            draggableRect.Width = static_cast<int32_t>(draggableWidth);
            draggableRect.Height = static_cast<int32_t>(AppTitleBar().Height());

            // Set the draggable regions for the AppWindow's title bar
            AppWindow().TitleBar().SetDragRectangles({ draggableRect });

            // Set the app title in the system title bar
            AppWindow().Title(L"SSnake");
        }
	}

    winrt::Windows::Foundation::IAsyncAction winrt::SSnake::implementation::MainWindow::SettingsButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        winrt::apartment_context ui_thread;
        co_await ui_thread;

        if (m_gameState == GAME_STATE::PLAYING)
        {
            m_gameState = GAME_STATE::PAUSED;
        }

        Controls::ContentDialog settingsDialog;
        settingsDialog.Title(box_value(L"Settings"));
        
        winrt::Windows::UI::Color color = winrt::Windows::UI::ColorHelper::FromArgb(0xFF, 0x00, 0x00, 0x00);
        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush brush(color);
        settingsDialog.Background(brush);

        auto settings = winrt::make<winrt::SSnake::implementation::Settings>();
        settings.select_game_mode(m_gameMode);
        settingsDialog.Content(settings);

        settingsDialog.PrimaryButtonText(L"Ok");
        settingsDialog.CloseButtonText(L"Cancel");

        settingsDialog.DefaultButton(Controls::ContentDialogButton::Primary);

        settingsDialog.XamlRoot(MainWindow_Grid().XamlRoot());

        try
        {
            Controls::ContentDialogResult result = co_await settingsDialog.ShowAsync();

            if (result == Controls::ContentDialogResult::Primary)
            {
                if (!(m_gameState == GAME_STATE::PAUSED))
                {
                    MainWindow::m_gameMode = settings.get_game_mode();
                }
                else
                {
                    m_gameState = GAME_STATE::PLAYING;
                }
            }
            
            PlayGround_Page().Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
        }
        catch (winrt::hresult_error const& ex)
        {
            OutputDebugStringW((L"Error showing GameModes: " + ex.message() + L"\n").c_str());
        }
    }

    winrt::Windows::Foundation::IAsyncAction MainWindow::HeartButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        HeartButton().Content().as<winrt::Microsoft::UI::Xaml::Controls::FontIcon>().Glyph(L"\uEB52");

        winrt::apartment_context ui_thread;
        co_await ui_thread;

        if (m_gameState == GAME_STATE::PLAYING)
        {
            m_gameState = GAME_STATE::PAUSED;
        }
        
        Controls::ContentDialog aboutDialog;
        aboutDialog.Title(box_value(L"About SSnake"));

        winrt::Windows::UI::Color color = winrt::Windows::UI::ColorHelper::FromArgb(0xFF, 0x00, 0x00, 0x00);
        winrt::Microsoft::UI::Xaml::Media::SolidColorBrush brush(color);
        aboutDialog.Background(brush);

        aboutDialog.Content(winrt::make<winrt::SSnake::implementation::AboutPage>());

        aboutDialog.PrimaryButtonText(L"OK");
        aboutDialog.DefaultButton(Controls::ContentDialogButton::Primary);
        aboutDialog.XamlRoot(MainWindow_Grid().XamlRoot());

        try
        {
            co_await aboutDialog.ShowAsync();

            HeartButton().Content().as<winrt::Microsoft::UI::Xaml::Controls::FontIcon>().Glyph(L"\uEB51");

            if (m_gameState == GAME_STATE::PAUSED) { m_gameState = GAME_STATE::PLAYING; }

            PlayGround_Page().Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
        }
        catch (winrt::hresult_error const& ex)
        {
            OutputDebugStringW((L"Error showing GameModes: " + ex.message() + L"\n").c_str());
        }
    }
}

