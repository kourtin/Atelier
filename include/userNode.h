
// This is sort of like your buddy icon, or something

#pragma once

#include <node.h>

namespace Atelier {
	class UserNode : Node {
		UserNode(const ID&);
		virtual ~UserNode();

        virtual Prism bounding_prism() const;
        virtual AABox bounding_aabox() const;
		virtual Rect bounding_rect() const;

		virtual void render(RenderDimension, RenderPass);
	};
}