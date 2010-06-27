
#include <json/json.h>

#include <gridsNetworkItem.h>
#include <identity.h>
#include <grids/interface.h>
#include <grids/protocol.h>
#include <client.h>
#include <basicApp.h>
#include <utility.h>

namespace Atelier {
    GridsNetworkItem::~GridsNetworkItem() {
    }
    
    void GridsNetworkItem::activate(const Identity& ident) {
        Value val;

        Client::grids_interface().protocol().send_request(
            Grids::Interface::GRIDS_LIST_ROOMS, val, false);
    }

    void GridsNetworkItem::request_create_room(const Value&) {
        Grids::Interface::instance().protocol().send_request(
            Grids::Interface::GRIDS_CREATE_ROOM);
    }

    void GridsNetworkItem::request_create_object(Value& val) {
        Value new_val;
        new_val["attr"] = val;
    }

    void GridsNetworkItem::request_create_object(const Vec3D& pos,
        const Vec3D& rot, const Vec3D& scl, Value& val) {
        Value new_val;
        new_val["attr"] = val;

        new_val["pos"][0u] = pos.x;
        new_val["pos"][1u] = pos.x;
        new_val["pos"][2u] = pos.x;

        new_val["rot"][0u] = rot.x;
        new_val["rot"][1u] = rot.x;
        new_val["rot"][2u] = rot.x;

        new_val["scl"][0u] = scl.x;
        new_val["scl"][1u] = scl.x;
        new_val["scl"][2u] = scl.x;

        request_create_object_internal(new_val);
    }

    void GridsNetworkItem::request_create_object_internal(Value& val) {
        val["_method"] = Grids::Interface::GRIDS_CREATE_OBJECT;
        val["room_id"] = Client::user_identity().current_room();
        val["id"] = Utility::create_uuid();

        Grids::Interface::instance().protocol().send_request(
            Grids::Interface::GRIDS_CREATE_OBJECT,
            val);
    }

    void GridsNetworkItem::request_update_object(Value& val) {
        Value new_val;
        new_val["attr"] = val;

        request_update_object_internal(new_val);
    }

    void GridsNetworkItem::request_update_object_internal(Value& val) {
        val["_method"] = Grids::Interface::GRIDS_UPDATE_OBJECT;
        val["room_id"] = Client::user_identity().current_room();
        val["id"] = id(); // TODO: generate id

        Grids::Interface::instance().protocol().send_request(
            Grids::Interface::GRIDS_UPDATE_OBJECT, val);
    }

    void GridsNetworkItem::request_update_position(const Vec3D& pos) {
        Value val;

        val["pos"][0u] = pos.x;
        val["pos"][1u] = pos.y;
        val["pos"][2u] = pos.z;

        request_update_object_internal(val);
    }
    
    void GridsNetworkItem::request_update_rotation(const Vec3D& rot) {
        Value val;

        val["rot"][0u] = rot.x;
        val["rot"][1u] = rot.y;
        val["rot"][2u] = rot.z;

        request_update_object_internal(val);
    }

    void GridsNetworkItem::request_update_scale(const Vec3D& scl) {
        Value val;

        val["scl"][0u] = scl.x;
        val["scl"][1u] = scl.y;
        val["scl"][2u] = scl.z;

        request_update_object_internal(val);
    }

    void GridsNetworkItem::request_update_object_matrix(const Vec3D& pos,
        const Vec3D& rot, const Vec3D& scl) {
        Value val;

        val["pos"][0u] = pos.x;
        val["pos"][1u] = pos.y;
        val["pos"][2u] = pos.z;

        val["rot"][0u] = rot.x;
        val["rot"][1u] = rot.y;
        val["rot"][2u] = rot.z;

        val["scl"][0u] = scl.x;
        val["scl"][1u] = scl.y;
        val["scl"][2u] = scl.z;

        request_update_object_internal(val);
    }

	//////////////////////////////////
	// Static
	/////////////////////////////////

	void GridsNetworkItem::request_create_object(Tete& tete) {
		Grids::Interface::instance().set_value_from_type(tete, Tete::CREATE);
		Grids::Interface::instance().set_value_from_broadcast(tete, true);

		// Send stuff to the network
		Grids::Interface::instance().send_tete(tete);
	}
}