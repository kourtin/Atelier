
#pragma once

#include "networkItem.h"

namespace Atelier {
    class GridsNetworkItem : public NetworkItem {
    public:
        virtual ~GridsNetworkItem();

        virtual void activate(const Identity&);
    };
}