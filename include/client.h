
#pragma once

#include "renderer.h"

class CinderApp;

namespace Atelier {
    class Client {
    public:
        Client(CinderApp*);

        void setup();
        void update();
        void draw();

        static CinderApp& app();
        static Renderer& renderer();

    private:
        static CinderApp* app_;
        static Renderer renderer_;

    };
}