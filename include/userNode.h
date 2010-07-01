
// This is sort of like your buddy icon, or something

#pragma once

#include <node.h>

namespace Atelier {
	class UserNode : public Node {
	public:
		UserNode(const ID&);
		virtual ~UserNode();

		std::string type() { return "UserNode"; }

		static void request_create(const ID&);

		virtual void create_object(const Tete&);
		virtual void update_object(const Tete&);
		virtual void update_object_matrix(const Tete&);

		virtual void receive_tete(const Tete&);

        virtual Prism bounding_prism() const;
        virtual AABox bounding_aabox() const;
		virtual Rect bounding_rect() const;

		virtual void render(RenderDimension, RenderPass);
	};
}