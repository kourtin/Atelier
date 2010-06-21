
#pragma once

#include "define.h"
#include "networkItemInterface.h"
#include "objectInterface.h"

namespace Atelier {
    class Tete;

    class NetworkItem : public NetworkItemInterface,
                        public virtual ObjectInterface {
    public:
        virtual ~NetworkItem();

        virtual void receive_tete_from_network(Tete&);

    };
}