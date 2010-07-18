
#pragma once

#include <cinder/app/MouseEvent.h>

#include "interactItemInterface.h"
#include "objectInterface.h"

namespace Atelier {
    class InteractItem : public InteractItemInterface,
                         public virtual ObjectInterface {
    public:
        virtual ~InteractItem();

        void set_detect_selection(bool);
       
    protected:
        bool detect_selection_;
        bool draggable_;
        bool drag_in_progress_;
    };
}
