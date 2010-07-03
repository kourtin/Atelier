
/* 
    This is like a UserNode, but turns off the rendering and 
    interaction aspects of the object.
*/

#pragma once

#include <userNode.h>

namespace Atelier {
    class ClientUserNode : public UserNode {
    public:
        ClientUserNode(const ID&);
        virtual ~ClientUserNode();

        virtual void render(RenderDimension, RenderPass);
    };
}