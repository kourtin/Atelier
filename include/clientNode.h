
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

		virtual void receive_tete(const Tete&);

		void create_object(const Tete&);
		void update_object(const Tete&);
		void update_object_matrix(const Tete&);

	private:
		void parse_create_tete(const Tete&);
		void parse_list_rooms(const Tete&);
		void parse_create_room(const Tete&);

		bool is_our_tete(const Tete&);

		bool initializing_;
	};
}