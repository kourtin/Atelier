
#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace Atelier {
    class Utility {
    public:
        Utility();
        ~Utility();

        static std::string create_uuid();

        static void host_to_network_uint32(boost::uint32_t&);
        static void network_to_host_uint32(boost::uint32_t&);

    private:
        static void endian_swap(boost::uint32_t&);

        static boost::uuids::uuid uuid_;
    };
}