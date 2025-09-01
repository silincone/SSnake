#include "pch.h"
#include "AboutPage.xaml.h"
#if __has_include("AboutPage.g.cpp")
#include "AboutPage.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::SSnake::implementation
{
	void AboutPage::ScrollToEndButton_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
	{
		AboutPage_ScrollView().ScrollTo(0.0, AboutPage_ScrollView().ScrollableHeight());
	}

	void AboutPage::AboutPage_ScrollView_ViewChanged(winrt::Microsoft::UI::Xaml::Controls::ScrollView const&, winrt::Windows::Foundation::IInspectable const&)
	{
		if (AboutPage_ScrollView().VerticalScrollBarVisibility() == Controls::ScrollingScrollBarVisibility::Hidden)
		{
			AboutPage_ScrollView().VerticalScrollBarVisibility(Controls::ScrollingScrollBarVisibility::Visible);
		}

		double threshold = 2.0;

		if (AboutPage_ScrollView().VerticalOffset() + threshold < AboutPage_ScrollView().ScrollableHeight())
		{
			ScrollToEndButton().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Visible);
		}
		else
		{
			ScrollToEndButton().Visibility(winrt::Microsoft::UI::Xaml::Visibility::Collapsed);
		}
	}
}