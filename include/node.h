
#pragma once

#include "object.h"
#include "cinderGraphicItem.h"
#include "networkItem.h"
#include "cinder3DInteractItem.h"

class CinderApp;

namespace Atelier {
    class Node : public Object, public Cinder3DInteractItem, 
        public CinderGraphicItem, public NetworkItem {
    public:
        Node(ID);
        virtual ~Node();

        const ID& id();
        const ID& owner();

        virtual Vec3D position();
        virtual Vec3D rotation();
        virtual Vec3D scale();
    };
}