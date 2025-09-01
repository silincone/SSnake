#pragma once

#include "AboutPage.g.h"

namespace winrt::SSnake::implementation
{
    struct AboutPage : AboutPageT<AboutPage>
    {
        AboutPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
        void ScrollToEndButton_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void AboutPage_ScrollView_ViewChanged(winrt::Microsoft::UI::Xaml::Controls::ScrollView const& sender, winrt::Windows::Foundation::IInspectable const& args);
    };
}

namespace winrt::SSnake::factory_implementation
{
    struct AboutPage : AboutPageT<AboutPage, implementation::AboutPage>
    {
    };
}
