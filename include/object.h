
#pragma once

#include <boost/thread/recursive_mutex.hpp>

#include <objectInterface.h>
#include <identity.h>

namespace Atelier {
    class Link;
    class Identity;

    typedef std::tr1::shared_ptr<const Link> LinkConstPtr;

    class Object : public virtual ObjectInterface {
    public:
        Object(const ID& object_id);
        virtual ~Object();

        const ID& id() const;
		virtual std::list<LinkConstPtr>& links();
		virtual const std::list<LinkConstPtr>& links() const;
        //virtual std::deque<Link> links_copy() const; // A more thread-safe method

        //Identity* identity();
        virtual const Identity* identity() const;

        virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

        virtual void create_object(const Tete&);
        virtual void update_object(const Tete&);

        // Thread utility methods
        virtual void lock_position() const;
        virtual void unlock_position() const;
        virtual void lock_rotation() const;
        virtual void unlock_rotation() const;
        virtual void lock_scale() const;
        virtual void unlock_scale() const;
        virtual void lock_links() const;
        virtual void unlock_links() const;

        virtual void update();

    protected:
        // This is how I get around const safeness in get_position, etc
        // It would be good to rethink this, maybe have some 
        mutable boost::recursive_mutex position_mutex_;
        mutable boost::recursive_mutex rotation_mutex_;
        mutable boost::recursive_mutex scale_mutex_;
        mutable boost::recursive_mutex links_mutex_;

    private:
        void set_matrix_from_tete(const Tete&);

        ID id_;
        std::list<LinkConstPtr> links_;
        Vec3D position_;
        Vec3D rotation_;
        Vec3D scale_;

        boost::recursive_mutex* foo_mutex_;

        typedef boost::recursive_mutex::scoped_lock ScopedLock;
    };

    typedef std::tr1::shared_ptr<Object> ObjectPtr;
    typedef std::tr1::shared_ptr<const Object> ObjectConstPtr;
}