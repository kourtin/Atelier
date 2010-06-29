
#pragma once

#include <node2D.h>

namespace Atelier {
	class GenericNode2D : public Node2D {
	public:
		GenericNode2D(const ID&);
		virtual ~GenericNode2D();

		virtual Rect bounding_rect() const;


	};
}