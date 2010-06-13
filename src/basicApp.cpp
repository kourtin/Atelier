
#include <vector>

#include "cinder/app/AppBasic.h"
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"
#include "Resources.h"
#include "cinder/ImageIo.h"

#include "define.h"
#include "basicApp.h"

CinderApp::CinderApp() : client_(this) {
}

void CinderApp::prepareSettings( Settings *settings ) {
	settings->setWindowSize( 1024, 768 );
	settings->setFrameRate( 60.0f );
	settings->setResizable( true );
	settings->setFullScreen( false );
}

void CinderApp::setup() {
    client_.setup();
}

void CinderApp::update() {
    client_.update();   
}

void CinderApp::draw() {
    client_.draw();
}


// This line tells Cinder to actually create the application
CINDER_APP_BASIC( CinderApp, ci::app::RendererGl )