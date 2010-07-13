
use warnings;
use strict;

use FindBin;
use lib "$FindBin::Bin/../../grids-platform/lib";

use Chatbot::Eliza;

use Grids::Address::IPv4;
use Grids::Client;
use Grids::Identity;
use Grids::Console;
use Grids::Conf;
use JSON::XS;

use Data::Dumper;

package Atelier;

my $TRUE = JSON::XS::true;
my $FALSE = JSON::XS::false;

package ElizaBot;

my $id;
my $client;
my $room;
my $name;
my $color;
my $chat_node_id;
my $last_chat_message_node_id;
my $chat_bot;
my $user_node_id;

package main;

my $network_pause = 0.1;

$Atelier::TRUE = JSON::XS::true;
$Atelier::FALSE = JSON::XS::false;

$ElizaBot::id = Grids::Identity->new();
$ElizaBot::name = "Eliza";
#$ElizaBot::color = [rand()*10000 % 255,
#                    rand()*10000 % 255,
#                    rand()*10000 % 255 ];
$ElizaBot::chat_bot = new Chatbot::Eliza $name, "script.txt";

my $con = Grids::Console->new(
    title => "$ElizaBot::name",
    prompt => "$ElizaBot::name>",
    );

run();

sub connect {
    my $address = Grids::Address::IPv4->new( address => '127.0.0.1' );
    $ElizaBot::client->connect($address);
}

sub connected_cb {
    my($c, $evt) = @_;

    # Don't crash the server
    select(undef,undef,undef, $network_pause);

    $ElizaBot::client->dispatch_event('Room.List');
}

sub list_rooms_cb {
    my ($c, $evt) = @_;

    my $rooms = $evt->args->{rooms};
    $ElizaBot::room = $rooms->[0];

    # No rooms on the server
    unless ($ElizaBot::room) { 
        create_room();
        return; 
    } 

    $con->print($ElizaBot::name . " has set room to:" . $ElizaBot::room);
}

sub create_room { $ElizaBot::client->dispatch_event('Room.Create'); }

sub create_room_cb { } # When someone else creates a room

sub broadcast_cb { } # I don't know what this method is supposed to do

sub create_object_cb { 
    my($c, $evt) = @_;

    return unless($evt);
    return if( $evt->args->{success});  # Filter out the bounceback confirmation code
    return if( $evt->args->{error}); 	
    return unless( $evt->args->{attr}->{type} );
    #return unless( $evt->args->{attr}->{type} eq "CameraNode" );

    if ($evt->args->{attr}->{type} eq "CameraNode") {
        #$con->print("Requesting UserNode");
        request_create_user_node();
    } elsif (($evt->args->{attr}->{type} eq "UserNode") && 
        (get_tete_creator_id($evt) eq $ElizaBot::id->name)) {
        $ElizaBot::user_node_id = $evt->args->{id};
    } elsif ($evt->args->{attr}->{type} eq "ChatNode") {
        #$con->print("Received ChatNode"); 
        $ElizaBot::chat_node_id = $evt->{id};
    } elsif (($evt->args->{attr}->{type} eq "ChatMessageNode") &&
        (get_tete_creator_id($evt) ne $ElizaBot::id->name)) {
        #$con->print("Received ChatMessageNode");        
    }
}

sub update_object_cb {
    my($c, $evt) = @_;

    return unless($evt);
    return if($evt->args->{success});  # Filter out the bounceback confirmation code
    return if($evt->args->{error}); 	
    return unless($evt->args->{attr}->{type});
    return unless($evt->args->{attr}->{type} eq "ChatMessageNode");
    return unless($evt->args->{attr}->{finished}); # ie enter pressed, not an intermediate keypress  

    process_chat_message_node($evt->args);
}

# Requests to the network that a UserNode be created
sub request_create_user_node {
    my $ran_x = rand(2) - 1;
    my $ran_y = rand(2) - 1;
    my $ran_z = rand(2) - 1;

    my $test_scale = 100;
    $ran_x *= $test_scale;
    $ran_y *= $test_scale;
    $ran_z *= $test_scale;

    my $node_value = { '_broadcast' => $Atelier::TRUE, 
                       pos => [$ran_x, $ran_y, $ran_z], 
                       rot => [0,0,0], 
                       scl => [1,1,1], 
                       id => $ElizaBot::id->name, 
                       room_id => $ElizaBot::room, 
                       attr => { type => 'UserNode', 
                                 links => [ { id => $ElizaBot::id->name, 
                                              name => $ElizaBot::name, 
                                              read => JSON::XS::true, 
                                              modify => JSON::XS::true, 
                                              creator => JSON::XS::true } ] } };

    $ElizaBot::client->dispatch_event('Room.CreateObject', $node_value);
}

sub process_chat_message_node {
    my ($node) = @_;

    my $message_text = $node->{attr}->{text};
    $ElizaBot::last_chat_message_node_id = $node->{id};
    my $reply = $ElizaBot::chat_bot->transform($message_text);

    create_chat_message_node($ElizaBot::chat_node_id, 
                             $ElizaBot::last_chat_message_node_id,
                             $reply);
}

# TODO: resolve position, etc
# Creates a message node, and links it to the last node it received.
sub create_chat_message_node {
    my ($chat_node_id, $chat_message_node_id, $text) = @_;
    
    my $node_value = { '_broadcast' => $Atelier::TRUE,
                       pos => [0,0,0],
                       rot => [0,0,0],
                       scl => [1,1,1],
                       id => Grids::UUID->new_id(),
                       room_id => $ElizaBot::room,
                       attr => { type => 'ChatMessageNode',
                                 text => $text,
                                 links => [ { id => $ElizaBot::id->name,
                                              name => $ElizaBot::name,
                                              read => $Atelier::TRUE,
                                              modify => $Atelier::TRUE,
                                              creator => $Atelier::TRUE },
                                            { id => $chat_node_id,
                                              read => $Atelier::TRUE,
                                              move => $Atelier::TRUE },
                                            { id => $chat_message_node_id,
                                              read => $Atelier::TRUE } ] } };

    $ElizaBot::client->dispatch_event('Room.CreateObject', $node_value);
}

sub get_tete_creator_id {
    my ($evt) = @_;
    return "" unless ($evt->args->{attr}->{links});

    my $i = 0;
    foreach ($evt->args->{attr}->{links}) {
        next unless ($evt->args->{attr}->{links}->[$i]->{creator});

        if ($evt->args->{attr}->{links}->[$i]->{creator} == $Atelier::TRUE) {
            #$con->print("Found creator: " . $evt->args->{attr}->{links}->[$i]->{id}); 
            return $evt->args->{attr}->{links}->[$i]->{id};
        }
        $i++;
    }

    return "";
}


sub run {
    # main loop condition
    my $main = AnyEvent->condvar;
    
    $ElizaBot::client = Grids::Client->new(id => $ElizaBot::id, use_encryption => 0, 
                                           debug => 0, auto_flush_queue => 1);
    
    # Refer to Grids::Node::Hooks.. for hooks
    $ElizaBot::client->register_hooks(
        'Authentication.Login' => \&connected_cb,
        'Broadcast.Event' => \&broadcast_cb,
        'Connected' => \&connected_cb,
        'Room.Create' => \&create_room_cb,
        'Room.List' => \&list_rooms_cb,
        'Room.CreateObject' => \&create_object_cb,
        'Room.UpdateObject' => \&update_object_cb,
        #'Room.ListObjects' => \&list_objects_cb,
        );

    ::connect();

    $con->listen_for_input;

    $main->recv;
}
