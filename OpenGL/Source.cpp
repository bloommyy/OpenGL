#include <iostream>

#include "Applications/Application_GettingStarted.hpp"

int main()
{
    try
    {
        Applications::Application_GettingStarted app;

        app.Run();

        return 0;
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }
}