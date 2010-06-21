
#include "tete.h"
#include "identity.h"

namespace Atelier {
    Tete::Tete() {
        type_ = INVALID;
        owner_ = NULL;
    }

    Tete::Tete(const Identity& ident, const Value& val) {
        owner_ = &ident;
        value_ = val;
        // Parse value_ to figure out Type
    }

    Tete::Type Tete::type() {
        return type_;
    }

    const Identity& Tete::owner() {
        return *owner_;
    }

    const Value& Tete::value() {
        return value_;
    }

}