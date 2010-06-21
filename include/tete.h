
#pragma once

#include "define.h"
#include "identity.h"

namespace Atelier {
    class Tete {
    public:
        Tete();
        Tete(const Identity&, const Value&);

        enum Type {
            INVALID,
            CREATE,
            UPDATE_VALUE,
            UPDATE_MATRIX
        };

        const Identity& owner();
        const Value& value();
        Type type();

    private:
        Value value_;
        const Identity* owner_;
        Type type_;
    };
}