
#pragma once

#include <object.h>
#include <cinder2DInteractItem.h>
#include <cinderGraphicItem.h>
#include <gridsNetworkItem.h>

namespace Atelier {
	class Node2D : public Object, public Cinder2DInteractItem,
		public CinderGraphicItem, public GridsNetworkItem {
	public:
		Node2D(const ID&);
		virtual ~Node2D();

		const ID& id() const;
		virtual std::list<LinkConstPtr>& links();
		virtual const std::list<LinkConstPtr>& links() const;

		virtual const InteractItem& container();
		void set_container(InteractItem&);

		virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

        virtual void create_object(const Tete&);
        virtual void update_object(const Tete&);

        void activate(const Identity&);

	private:
		InteractItem* container_;
	};
}