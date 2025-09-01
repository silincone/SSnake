#include "pch.h"
#include "PlayGround.xaml.h"
#if __has_include("PlayGround.g.cpp")
#include "PlayGround.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

using namespace Microsoft::UI::Xaml::Controls;
using namespace std::chrono_literals;
using namespace Windows::System;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SSnake::implementation
{
	PlayGround::PlayGround()
	{
		m_interval = 80ms;

		m_gameTimer = winrt::Microsoft::UI::Xaml::DispatcherTimer();
		m_gameTimer.Interval(m_interval);
		m_gameTimer.Tick({ this, &PlayGround::UpdateGame });
	}

	winrt::Windows::Foundation::IAsyncAction PlayGround::PlayGround_MainGrid_Loaded(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		winrt::apartment_context ui_thread;
		co_await ui_thread;

		winrt::Microsoft::UI::Xaml::Controls::ContentDialog startGameDialog;
		startGameDialog.Title(box_value(L"Start Game?"));

		winrt::Windows::UI::Color color = winrt::Windows::UI::ColorHelper::FromArgb(0xFF, 0x00, 0x00, 0x00);
		winrt::Microsoft::UI::Xaml::Media::SolidColorBrush brush(color);
		startGameDialog.Background(brush);

		auto gameModes = winrt::make<winrt::SSnake::implementation::GameModes>();
		startGameDialog.Content(gameModes);

		startGameDialog.PrimaryButtonText(L"Yes!");
		startGameDialog.CloseButtonText(L"No");

		startGameDialog.DefaultButton(Controls::ContentDialogButton::Primary);

		startGameDialog.XamlRoot(PlayGround_MainGrid().XamlRoot());

		try
		{
			Controls::ContentDialogResult result = co_await startGameDialog.ShowAsync();

			if (result == Controls::ContentDialogResult::Primary)
			{
				MainWindow::m_gameMode = gameModes.get_game_mode();

				ResetGame();
				StartGame();

			}
			
			// Giving focus to the PlayGround_MainGrid (for keyboard input)
			Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
		}
		catch (winrt::hresult_error const& ex)
		{
			OutputDebugStringW((L"Error showing GameModes: " + ex.message() + L"\n").c_str());
		}
	}

	void PlayGround::PlayGround_MainGrid_SizeChanged(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::SizeChangedEventArgs const&)
	{
		double h = PlayGround_MainGrid().ActualHeight();
		double w = PlayGround_MainGrid().ActualWidth();

		auto margin = PlayGround_Grid().Margin();
		auto borderThickness = PlayGround_Grid().BorderThickness();
		h -= margin.Top + margin.Bottom + borderThickness.Top + borderThickness.Bottom + Score_Grid().ActualHeight();;
		w -= margin.Left + margin.Right + borderThickness.Left + borderThickness.Right;

		int no_of_rows = static_cast<int>(std::floor(h / 20));
		int no_of_columns = static_cast<int>(std::floor(w / 20));

		no_of_rows = (0 > no_of_rows) ? (0) : (no_of_rows);
		no_of_columns = (0 > no_of_columns) ? (0) : (no_of_columns);

		PlayGround_Grid().RowDefinitions().Clear();
		PlayGround_Grid().ColumnDefinitions().Clear();

		for (int i = 0; i < no_of_rows; ++i)
		{
			RowDefinition rowDef;
			rowDef.Height(GridLengthHelper::FromPixels(static_cast<double>(20)));
			PlayGround_Grid().RowDefinitions().Append(rowDef);
		}

		for (int i = 0; i < no_of_columns; ++i)
		{
			ColumnDefinition colDef;
			colDef.Width(GridLengthHelper::FromPixels(static_cast<double>(20)));
			PlayGround_Grid().ColumnDefinitions().Append(colDef);
		}

		m_PlayGround_X = no_of_columns;
		m_PlayGround_Y = no_of_rows;

		// if the focus is lost during resizing, get back the focus.
		bool hasMainWindowPopup = CheckForPopups(MainWindow::CurrentWindow().Content().XamlRoot());
		bool hasPlayGroundPopup = CheckForPopups(this->XamlRoot());
		
		if (!hasMainWindowPopup && !hasPlayGroundPopup)
		{
			Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
		}

		DrawGame();
	}

	void PlayGround::OnKeyDown(winrt::Microsoft::UI::Xaml::Input::KeyRoutedEventArgs const& e)
	{
		// Key input processing
		if (MainWindow::m_gameState != GAME_STATE::PLAYING)
		{
			if (e.Key() == VirtualKey::Space)
			{
				ResetGame();
				StartGame();
			}
		}

		winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard storyboardToPlay{ nullptr };
		winrt::Microsoft::UI::Xaml::Controls::FontIcon currentIcon{ nullptr };
	
		if (e.Key() == VirtualKey::Left || e.Key() == VirtualKey::A)
		{
			storyboardToPlay = LeftIcon().Resources().Lookup(box_value(L"LeftIconFadeOut")).try_as<winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard>();
			currentIcon = LeftIcon();
			m_moveBuffer.push(S_SNAKE_DIRECTION::LEFT);
		}
		else if (e.Key() == VirtualKey::Up || e.Key() == VirtualKey::W)
		{
			storyboardToPlay = UpIcon().Resources().Lookup(box_value(L"UpIconFadeOut")).try_as<winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard>();
			currentIcon = UpIcon();
			m_moveBuffer.push(S_SNAKE_DIRECTION::UP);
		}
		else if (e.Key() == VirtualKey::Right || e.Key() == VirtualKey::D)
		{
			storyboardToPlay = RightIcon().Resources().Lookup(box_value(L"RightIconFadeOut")).try_as<winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard>();
			currentIcon = RightIcon();
			m_moveBuffer.push(S_SNAKE_DIRECTION::RIGHT);
		}
		else if (e.Key() == VirtualKey::Down || e.Key() == VirtualKey::S)
		{
			storyboardToPlay = DownIcon().Resources().Lookup(box_value(L"DownIconFadeOut")).try_as<winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard>();
			currentIcon = DownIcon();
			m_moveBuffer.push(S_SNAKE_DIRECTION::DOWN);
		}
		else if (e.Key() == VirtualKey::P)
		{
			if (MainWindow::m_gameState == GAME_STATE::PLAYING)
			{
				storyboardToPlay = PauseIcon().Resources().Lookup(box_value(L"PauseIconFadeOut")).try_as<winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard>();
				currentIcon = PauseIcon();
				m_gameTimer.Stop();
				MainWindow::m_gameState = GAME_STATE::PAUSED;
				ShowGamePaused();
			}
			else if (MainWindow::m_gameState == GAME_STATE::PAUSED)
			{
				storyboardToPlay = PlayIcon().Resources().Lookup(box_value(L"PlayIconFadeOut")).try_as<winrt::Microsoft::UI::Xaml::Media::Animation::Storyboard>();
				currentIcon = PlayIcon();
				MainWindow::m_gameState = GAME_STATE::PLAYING;
				m_gameTimer.Start();
			}
		}
	
		if (storyboardToPlay != nullptr)
		{
			storyboardToPlay.Stop();
	
			// Explicitly set the icon's opacity to 1.0 and visibility to Visible BEFORE starting the animation
			if (currentIcon != nullptr) // Ensure we have a valid icon reference
			{
				currentIcon.Opacity(1.0);
				currentIcon.Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
			}
	
			storyboardToPlay.Begin();
		}
	}

	bool PlayGround::CheckForPopups(winrt::Microsoft::UI::Xaml::XamlRoot xamlRoot)
	{
		if (!xamlRoot)
		{
			return false;
		}

		auto popups = winrt::Microsoft::UI::Xaml::Media::VisualTreeHelper::GetOpenPopupsForXamlRoot(xamlRoot);

		return popups.Size() > 0;
	}

	void PlayGround::StartGame()
	{
		// Updating game state and starting the game timer
		MainWindow::m_gameState = GAME_STATE::PLAYING;
		m_gameTimer.Start();
	}

	void PlayGround::ResetGame()
	{
		// Reset stuff
		MainWindow::m_gameState = GAME_STATE::NOT_STARTED;
		m_gameTimer.Stop();
		m_interval = 80ms;
		m_gameTimer.Interval(m_interval);
		m_score = 0;

		if (!m_snake)
		{
			m_snake = std::make_unique<S_SNAKE>();
			m_snake->reset();
		}
		else
		{
			m_snake->reset();
		}

		std::queue<S_SNAKE_DIRECTION> empty;
		m_moveBuffer.swap(empty);
		m_moveBuffer.push(m_snake->get_direction());

		if (MainWindow::m_gameMode == GAME_MODE::DIFFICULT)
		{
			m_prevScoreForIntervalChange = -1;
			m_prevScoreForPlayGroundColorChange = -1;
			m_playGroundWhite = false;
		}

		// Update GUI
		PlayGround_Grid().Background(Media::SolidColorBrush{ winrt::Windows::UI::ColorHelper::FromArgb(0x00, 0x00, 0x00, 0x00) });
		UpdateScoreText();
		GenerateFood();
		DrawGame();
	}

	void PlayGround::GenerateFood()
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distribX(0, m_PlayGround_X - 1);
		std::uniform_int_distribution<> distribY(0, m_PlayGround_Y - 1);

		S_COORD newFoodPos;
		bool foundValidPosition = false;

		while (!foundValidPosition)
		{
			newFoodPos.X(distribX(gen));
			newFoodPos.Y(distribY(gen));

			foundValidPosition = true;
			for (const auto& segment : m_snake->get_snake())
			{
				if (segment.X() == newFoodPos.X() && segment.Y() == newFoodPos.Y())
				{
					foundValidPosition = false;
					break;
				}
			}
			// New food position shouldn't be same as before, although, it isn't strictly necessary
			if (newFoodPos.X() == m_foodPosition.X() && newFoodPos.Y() == m_foodPosition.Y()) { foundValidPosition = false; }
		}
		m_foodPosition = newFoodPos;
	}

	void PlayGround::DrawGame()
	{
		if (m_snake == nullptr || m_snake->get_snake().empty())
		{
			return;
		}

		// Check Game Mode.
		if (MainWindow::m_gameMode == GAME_MODE::DIFFICULT &&
			m_score == 0 &&
			m_prevScoreForPlayGroundColorChange < m_score)
		{
			PlayGround_Grid().Background(Media::SolidColorBrush{ winrt::Windows::UI::ColorHelper::FromArgb(0x22, 0x00, 0x00, 0x00) });
			m_prevScoreForPlayGroundColorChange = m_score;
		}
		else if (MainWindow::m_gameMode == GAME_MODE::DIFFICULT &&
			m_score > 0 &&
			m_prevScoreForPlayGroundColorChange < m_score &&
			!(m_score % 5))
		{
			winrt::Windows::UI::Color playGroundColor;

			m_playGroundWhite = !m_playGroundWhite;
			if (m_playGroundWhite) { playGroundColor = winrt::Windows::UI::ColorHelper::FromArgb(0xFF, 0xFF, 0xFF, 0xFF); }
			else				   { playGroundColor = winrt::Windows::UI::ColorHelper::FromArgb(0x22, 0x00, 0x00, 0x00); }
			winrt::Microsoft::UI::Xaml::Media::SolidColorBrush playGroundColorBrush(playGroundColor);
			PlayGround_Grid().Background(playGroundColorBrush);

			m_prevScoreForPlayGroundColorChange = m_score;
		}

		PlayGround_Grid().Children().Clear();

		winrt::Microsoft::UI::Xaml::Shapes::Rectangle foodRect;
		foodRect.Fill(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ winrt::Windows::UI::Colors::Red() });

		Grid::SetRow(foodRect, m_foodPosition.Y());
		Grid::SetColumn(foodRect, m_foodPosition.X());

		PlayGround_Grid().Children().Append(foodRect);

		bool isHead = true;
		for (const auto& segment : m_snake->get_snake()) // Iterate through S_SNAKE's body
		{
			winrt::Microsoft::UI::Xaml::Shapes::Rectangle segmentRect;
			if (isHead)
			{
				segmentRect.Fill(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ Windows::UI::Colors::DarkGreen() }); // Head color
				isHead = false;
			}
			else
			{
				segmentRect.Fill(winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{ Windows::UI::Colors::LimeGreen() }); // Body color
			}

			Grid::SetRow(segmentRect, segment.Y());
			Grid::SetColumn(segmentRect, segment.X());
			PlayGround_Grid().Children().Append(segmentRect);
		}
	}

	void PlayGround::CheckCollision()
	{
		S_COORD head = m_snake->get_head();
		if (head.X() < 0 || head.X() >= m_PlayGround_X ||
			head.Y() < 0 || head.Y() >= m_PlayGround_Y)
		{
			m_gameTimer.Stop();
			ateYourself = false;
			MainWindow::m_gameState = GAME_STATE::GAME_OVER;
			ShowGameOver();
		}

		if (m_snake->check_self_collision())
		{
			m_gameTimer.Stop();
			ateYourself = true;
			MainWindow::m_gameState = GAME_STATE::GAME_OVER;
			ShowGameOver();
		}
	}

	void PlayGround::UpdateGame(winrt::Windows::Foundation::IInspectable const&, winrt::Windows::Foundation::IInspectable const&)
	{
		// Core UI logic 
		if (MainWindow::m_gameState != GAME_STATE::PLAYING) { return; }

		if (!m_moveBuffer.empty())
		{
			m_snake->set_direction(m_moveBuffer.front()), m_currentDirection = m_moveBuffer.front();
			m_moveBuffer.pop();
		}

		m_snake->move();

		CheckCollision();
		if (MainWindow::m_gameState == GAME_STATE::GAME_OVER) { return; }

		bool ateFood{ false };
		if (m_snake->get_head() == m_foodPosition)
		{
			m_score += 1;
			UpdateScoreText();
			GenerateFood();
			ateFood = true;
		}
		m_snake->remove_tail(ateFood);

		if (MainWindow::m_gameMode == GAME_MODE::DIFFICULT && m_score > 0 && m_prevScoreForIntervalChange < m_score && !(m_score % 5) && m_interval > 15ms)
		{
			m_interval -= 10ms;
			m_gameTimer.Interval(m_interval);

			m_prevScoreForIntervalChange = m_score;
		}

		DrawGame();
	}

	void PlayGround::UpdateScoreText()
	{
		ScoreTextBlock().Text(to_hstring(m_score));
	}

	winrt::Windows::Foundation::IAsyncAction PlayGround::ShowGameOver()
	{
		Controls::ContentDialog gameOverDialog;
		gameOverDialog.Title(box_value(L"Game Over!"));

		winrt::Windows::UI::Color color = winrt::Windows::UI::ColorHelper::FromArgb(0xFF, 0x00, 0x00, 0x00);
		winrt::Microsoft::UI::Xaml::Media::SolidColorBrush brush(color);
		gameOverDialog.Background(brush);

		if (ateYourself) { gameOverDialog.Content(box_value(L"The snake ate itself!")); }
		else { gameOverDialog.Content(box_value(L"The snake tried crossing the bounds!")); }
		gameOverDialog.PrimaryButtonText(L"Play Again!");
		gameOverDialog.CloseButtonText(L"Close");
		gameOverDialog.DefaultButton(Controls::ContentDialogButton::Primary);
		gameOverDialog.XamlRoot(PlayGround_MainGrid().XamlRoot());

		Controls::ContentDialogResult result = co_await gameOverDialog.ShowAsync();

		try
		{
			if (result == Controls::ContentDialogResult::Primary)
			{
				ResetGame();
				StartGame();
			}
			
			// Giving focus to the PlayGround_MainGrid (for keyboard input)
			PlayGround_Grid().Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
		}
		catch (winrt::hresult_error const& ex)
		{
			OutputDebugStringW((L"Showind error in ShowGameOver: " + ex.message() + L"\n").c_str());
		}
	}

	winrt::Windows::Foundation::IAsyncAction PlayGround::ShowGamePaused()
	{
		Controls::ContentDialog gamePausedDialog;
		gamePausedDialog.Title(box_value(L"Game Paused!"));

		winrt::Windows::UI::Color color = winrt::Windows::UI::ColorHelper::FromArgb(0xFF, 0x00, 0x00, 0x00);
		winrt::Microsoft::UI::Xaml::Media::SolidColorBrush brush(color);
		gamePausedDialog.Background(brush);

		gamePausedDialog.Content(box_value(L"The game has been paused, want to continue?"));
		gamePausedDialog.PrimaryButtonText(L"Yes!");
		gamePausedDialog.CloseButtonText(L"No");
		gamePausedDialog.DefaultButton(Controls::ContentDialogButton::Primary);
		gamePausedDialog.XamlRoot(PlayGround_Grid().XamlRoot());

		Controls::ContentDialogResult result = co_await gamePausedDialog.ShowAsync();

		if (result == Controls::ContentDialogResult::Primary)
		{
			StartGame();
		}
		
		Focus(winrt::Microsoft::UI::Xaml::FocusState::Programmatic);
	}
}
