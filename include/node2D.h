
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

		const ID& id();
		std::list<const Link*>& links();
		const std::list<const Link*>& links() const;

		virtual const InteractItem& container();
		void set_container(InteractItem&);

		virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

	private:
		InteractItem* container_;
	};
}