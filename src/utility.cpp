
#include <string>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <cinder/app/App.h>

#include <utility.h>

namespace Atelier {
    Utility::Utility() {
    }

    boost::uuids::uuid Utility::uuid_;

    std::string Utility::create_uuid() {
        uuid_ = boost::uuids::random_generator()();
        std::stringstream convert;
        convert << uuid_;
        return convert.str();
    }

    void Utility::host_to_network_uint32(boost::uint32_t& x) {
        // TODO: replace this with real code
        endian_swap(x);
    }
    void Utility::network_to_host_uint32(boost::uint32_t& x) {
        // TODO: replace this with real code
        endian_swap(x);
    }

    void Utility::endian_swap(boost::uint32_t& x) {
        x = (x>>24) |
            ((x<<8) & 0x00FF0000) |
            ((x>>8) & 0x0000FF00) |
            (x<<24);
    }
}
