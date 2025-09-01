#include "pch.h"
#include "Settings.xaml.h"
#if __has_include("Settings.g.cpp")
#include "Settings.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SSnake::implementation
{
	winrt::SSnake::GAME_MODE Settings::get_game_mode()
	{
		return GameModes_Settings().get_game_mode();
	}

	void Settings::select_game_mode(winrt::SSnake::GAME_MODE mode)
	{
		return GameModes_Settings().select_game_mode(mode);
	}
}
