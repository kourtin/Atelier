
#pragma once

#include <object.h>
#include <cinderGraphicItem.h>
#include <gridsNetworkItem.h>
#include <cinder3DInteractItem.h>

//class CinderApp;

namespace Atelier {
    class Node : public Object, public Cinder3DInteractItem, 
        public CinderGraphicItem, public GridsNetworkItem {
    public:
        Node(const ID&);
        virtual ~Node();

        const ID& id();

        virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

        virtual void activate(const Identity&);
        virtual std::list<const Link*>& links();
		virtual const std::list<const Link*>& links() const;

        virtual void create_object(const Tete&);

        virtual void update_object(const Tete&);
    };
}