#include <iostream>

#include "Applications/Application_Lighting.hpp"

int main()
{
    try
    {
        Applications::Application_Lighting app;

        app.Run();

        return 0;
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return -1;
    }
}