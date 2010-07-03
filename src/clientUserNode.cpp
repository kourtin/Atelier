
#include <clientUserNode.h>

namespace Atelier {
    ClientUserNode::ClientUserNode(const ID& in_id) : UserNode(in_id) {

    }

    ClientUserNode::~ClientUserNode() {
    }

    void ClientUserNode::render(RenderDimension, RenderPass) {
        // Nothing to render here 
    }
}