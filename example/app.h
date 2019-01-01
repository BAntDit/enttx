//
// Created by bantdit on 1/1/19.
//

#ifndef ENTTX_APP_H
#define ENTTX_APP_H

#include <exception>
#include <iostream>
#include "options.h"

template<class Application>
class BaseApp {
public:
    auto init(Options const& options) -> Application&;

    auto run() -> Application&;

    void done();
};

template<class Application>
auto BaseApp<Application>::init(Options const& options) -> Application&
{
    return static_cast<Application*>(this)->init(options);
}

template<class Application>
auto BaseApp<Application>::run() -> Application&
{
    return static_cast<Application*>(this)->run();
}

template<class Application>
void BaseApp<Application>::done()
{
    static_cast<Application*>(this)->done();
}

#define APP(cls)                                                                                               \
    int main(int argc, const char* argv[]) { return letsGo<cls>(argc, argv); }

template<class Application>
int letsGo(int argc, const char* argv[])
{
    try {
        Options options(argc, argv);

        Application app;

        app.init(options).run().done();
    } catch (std::exception const& e) {
        std::cout << "an exception has occurred: " << e.what() << std::endl;

        throw;
    }

    return 0;
}

#endif //ENTTX_APP_H
