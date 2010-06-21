
#include "interactItem.h"

namespace Atelier {
    InteractItem::~InteractItem() {
    }

    void InteractItem::set_detect_selection(bool sel) {
        detect_selection_ = sel;
    }
}