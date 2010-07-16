
// Sort of a dummy object 

#pragma once

#include <object.h>
#include <gridsNetworkItem.h>

namespace Atelier {
	class ClientNode : public Object, public GridsNetworkItem {
	public:
		ClientNode(const ID&);
		virtual ~ClientNode();

        virtual std::string type() { return "ClientNode"; }

        virtual const ID& id();

        virtual std::list<const Link*>& links();
		virtual const std::list<const Link*>& links() const;

        virtual Vec3D position() const;
        virtual Vec3D rotation() const;
        virtual Vec3D scale() const;

        virtual void set_position(Vec3D);
        virtual void set_rotation(Vec3D);
        virtual void set_scale(Vec3D);

		virtual void receive_tete(const Tete&);

		void create_object(const Tete&);
		void update_object(const Tete&);
		void update_object_matrix(const Tete&);

        virtual void activate(const Identity&);

	private:
		void parse_create_tete(const Tete&);
		void parse_list_rooms(const Tete&);
		void parse_create_room(const Tete&);

		bool is_our_tete(const Tete&);

		bool initializing_;
	};

    typedef std::tr1::shared_ptr<ClientNode> ClientNodePtr;
}