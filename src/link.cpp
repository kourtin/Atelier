
#include <iostream> // for NULL

#include <cinder/app/App.h>

#include <link.h>
#include <identity.h>
#include <define.h>

namespace Atelier {
	Link::Link(const Identity* actor_, LinkFlags flags_) {
		this->actor_ = actor_;
		this->base_ = NULL;
		this->flags_ = flags_;
	}

	Link::Link(const Identity* base_, const Identity* actor_, LinkFlags flags_) {
		this->actor_ = actor_;
		this->base_ = base_;
		this->flags_ = flags_;
	}

	const Identity& Link::base() {
		return *base_;
	}

	const Identity& Link::base() const {
		return *base_;
	}

	const Identity& Link::actor() {
		return *actor_;
	}

	const Identity& Link::actor() const {
		return *actor_;
	}

	const LinkFlags& Link::flags() const {
		return flags_;
	}

	LinkPtr Link::get_link_from_value(const Value& val) {
		const Identity* ident = Identity::get_identity_from_value(val);

		if (ident == NULL)
			ident = Identity::create_identity(val);

        if (ident == NULL) {
            ci::app::console() << "CRITICAL: unable to create identity for: " << std::endl 
                << val.toStyledString() << std::endl;
            return LinkPtr();
        }

		bool read_flag = val["read"].asBool();
		bool modify_flag = val["modify"].asBool();
		bool creator_flag = val["creator"].asBool();
		bool remove_flag = val["remove"].asBool();

        LinkPtr new_link(new Link(ident, LinkFlags(read_flag, modify_flag, 
			creator_flag, remove_flag)));

		return new_link;
	}
}
