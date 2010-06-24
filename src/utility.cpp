
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
}

//std::string s((char*)(uuid_.data), uuid_.size());
//std::string s(uuid_.size(), '\0');
//std::copy(uuid_.begin(), uuid_.end(), s.begin());
//std::string s(uuid_.begin(), uuid_.end());