
#pragma once

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
    };
}
