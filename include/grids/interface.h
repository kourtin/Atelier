
#pragma once

#include <string>

#include <grids/define.h>
#include <tete.h>

namespace Atelier {
	class Identity;
}

namespace Grids {
    class Protocol;

    class Interface {
    public:
        Interface();

        void init();
        void connect_to_node();

        void update();

        bool connected();
        static Interface& instance();

        static Protocol& protocol();

		// TODO: replace these with static char[]
		// see http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml?showone=Static_and_Global_Variables#Static_and_Global_Variables
        static const std::string GRIDS_CREATE_ROOM;
        static const std::string GRIDS_CREATE_OBJECT;
        static const std::string GRIDS_UPDATE_OBJECT;
        static const std::string GRIDS_LIST_ROOMS;
        static const std::string GRIDS_NULL_EVENT;

        Atelier::Tete* parse_network_event(Value&);

		void request_list_rooms();
		// TODO: You should be able to specify the ID of the room being
		// created.
		void request_create_room();

		void set_value_from_type(Atelier::Tete&, Atelier::Tete::Type);

		void send_tete(Atelier::Tete&);

    private:
		private void set_value_links(Value&, Atelier::Tete&);

        static Protocol* protocol_;
        bool connected_;
        static Interface* instance_;

        static const std::string server_address_;
    };
}