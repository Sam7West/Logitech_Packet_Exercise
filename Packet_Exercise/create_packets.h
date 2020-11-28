#ifndef CREATE_PACKETS_HH
#define CREATE_PACKETS_HH

#include <string>
#include <array>
#include <list>
#include <stdlib.h>
#include "generic_typedefs.h"
#include "packet.h"

using namespace std;

void
create_packets
(
 uint8_t data[],
 /* uint32_t size, */
 list< Packet >& packet_list
 );

struct Error_Unknown_Packet_Type : public exception {
  const char* what () const throw () {
    return "Error: received unknown packet type";
  }
};

struct Error_Max_Payload_Exceeded : public exception {
  string message () const throw () {
    return "Error: maximum packet payload exceeded";
  }
};

struct Error_Unexpected_First_Packet : public exception {
  string message () const throw () {
    return "Error: expected \"StartDataTransfer\" type for first packet";
  }
};

#endif


