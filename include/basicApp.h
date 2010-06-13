
#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

#include "define.h"
#include "client.h"

// We'll create a new Cinder Application by deriving from the CinderApp class
class CinderApp : public ci::app::AppBasic {
    public:
    CinderApp();

    void prepareSettings(Settings*);
    void setup();
    void update();
    void draw();

private:
    Atelier::Client client_;
};