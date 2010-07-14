
#pragma once

// These are usually stored in a Tete,
// in which case the base of the link (the object that
// the permissions descripbe 

#include <define.h>

namespace Atelier {
	class Identity;

	struct LinkFlags {
		bool can_read;
		bool can_modify;
		bool creator;
		bool remove; // Set this if the link should be cleaned up in the future

		LinkFlags(bool can_read = false, bool can_modify = false, 
			bool creator = false, bool remove = false) {
			this->can_read = can_read;
			this->can_modify = can_modify;
			this->creator = creator;
			this->remove = remove;
		}
	};

    class Link;

    typedef std::tr1::shared_ptr<Link> LinkPtr;
    typedef std::tr1::shared_ptr<const Link> LinkConstPtr;

	class Link {
	public:
		Link(const Identity* actor, LinkFlags);
		Link(const Identity* base, const Identity* actor, LinkFlags);

		const Identity& base();
		const Identity& base() const;

		const Identity& actor();
		const Identity& actor() const;

		const LinkFlags& flags() const;

	private:
		const Identity* base_;
		const Identity* actor_;
		LinkFlags flags_;

	////////////////////////
	// Static utility methods
	////////////////////////
	public:
		static LinkPtr get_link_from_value(const Value&);
	};


}