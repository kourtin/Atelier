
#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

namespace Atelier {
    class Utility {
    public:
        Utility();
        ~Utility();

        static std::string create_uuid();

    private:
        static boost::uuids::uuid uuid_;
    };
}