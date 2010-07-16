
#pragma once

#include <boost/thread/recursive_mutex.hpp>

#include <objectInterface.h>
#include <identity.h>

namespace Atelier {
    class Link;
    class Identity;

    class Object : public virtual ObjectInterface {
    public:
        Object(const ID& object_id);
        virtual ~Object();

        const ID& id();
		std::list<const Link*>& links();
		const std::list<const Link*>& links() const;
        // A more thread-safe method
        std::vector<Link> links_copy() const;
        //Identity* identity();
        const Identity* identity() const;

        virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

        virtual void create_object(const Tete&);
        virtual void update_object(const Tete&);

        // Thread utility methods
        void lock_position() const;
        void unlock_position() const;
        void lock_rotation() const;
        void unlock_rotation() const;
        void lock_scale() const;
        void unlock_scale() const;
        void lock_links() const;
        void unlock_links() const;

    private:
        void set_matrix_from_tete(const Tete&);

        ID id_;
        std::list<const Link*> links_;
        Vec3D position_;
        Vec3D rotation_;
        Vec3D scale_;

        // This is how I get around const safeness in get_position, etc
        // It would be good to rethink this, maybe have some 
        mutable boost::recursive_mutex position_mutex_;
        mutable boost::recursive_mutex rotation_mutex_;
        mutable boost::recursive_mutex scale_mutex_;
        mutable boost::recursive_mutex links_mutex_;

        boost::recursive_mutex* foo_mutex_;

        typedef boost::recursive_mutex::scoped_lock ScopedLock;
    };

    typedef std::tr1::shared_ptr<Object> ObjectPtr;
}