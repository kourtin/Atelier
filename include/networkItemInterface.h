
#pragma once

namespace Atelier {
    class NetworkItemInterface {
    public:
        virtual void receive_tete_from_network(Tete&) = 0;
    };
}