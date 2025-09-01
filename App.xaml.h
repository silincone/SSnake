#pragma once

#include "App.xaml.g.h"

namespace winrt::SSnake::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
    
        static winrt::Microsoft::UI::Xaml::Window window;
    };
}
