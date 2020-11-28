#ifndef PACKET_HH
#define PACKET_HH

#include <string>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include <cstddef>

using namespace std;

#define BIT_REBOOT 0b00100000
#define BIT_VERIFY 0b01000000
#define BIT_TEST   0b10000000

enum E_Packet {
  StartDataTransfer_T = 1,
  Data_T = 2,
  StopDataTransfer_T = 3
};
 
typedef struct {
  uint8_t software_id;
  uint32_t sequence_id;
  uint8_t packet_type;
} Header;

class Packet {

private:
  bool freed = 0;
  
public:

  Header header;
  string name;

  Packet
  (
   uint8_t software_id,
   uint32_t sequence_id,
   uint8_t packet_type
   )
  {
    header.software_id = software_id;
    header.sequence_id = sequence_id;
    header.packet_type = packet_type;
  }
  
protected:

  uint32_t total_payload_size;
  bool reboot, verify, test;
  //uint8_t* data = NULL;
  vector< uint8_t > data;
  uint8_t payload_size;
  
};


class StartDataTransfer_Packet
  : public Packet
{
public:

  StartDataTransfer_Packet
  ( 
   uint8_t software_id,
   uint32_t sequence_id,
   uint32_t total_payload_size
    ):
    Packet( software_id, sequence_id, StartDataTransfer_T )
  {
    this -> name = "StartDataTransfer Packet";  
    this -> total_payload_size = total_payload_size;
  }

  void
  set_total_payload_size( uint32_t total_payload_size )
  {
    this -> total_payload_size = total_payload_size;
  }

  uint32_t
  get_total_payload_size( void )
  {
    return total_payload_size;
  }
};

class StopDataTransfer_Packet
  : public Packet
{
public:

  StopDataTransfer_Packet
  ( 
   uint8_t software_id,
   uint32_t sequence_id,
   bool reboot,
   bool verify,
   bool test
    ):
    Packet( software_id, sequence_id, StopDataTransfer_T )
  {
    this -> name = "StopDataTransfer Packet";  
    this -> reboot = reboot;
    this -> verify = verify;
    this -> test = test;
  }

  bool
  get_reboot( void )
  {
    return reboot;
  }   

  void
  set_reboot( bool reboot )
  {
    this -> reboot = reboot;
  }   

  bool
  get_verify( void )
  {
    return verify;
  }

  void
  set_verify( bool verify )
  {
    this -> verify = verify;
  }   

  bool
  get_test( void )
  {
    return test;
  }
  
  void
  set_test( bool test )
  {
    this -> test = test;
  }
};

class Data_Packet
  : public Packet
{
public:

  Data_Packet
  (
   uint8_t software_id,
   uint32_t sequence_id,
   vector< uint8_t > data,
   uint8_t payload_size
   ) :
    Packet( software_id, sequence_id, Data_T )
  {
    this -> name = "Data Packet";
    this -> data = data;
    this -> payload_size = payload_size;
  }

  uint8_t
  get_payload_size( void )
  {
    return payload_size;
  }

  void
  set_payload_size( uint8_t payload_size )
  {
    this -> payload_size = payload_size;
  }

  vector< uint8_t >&
  get_data_ref( void )
  {
    return data;
  }
};

#endif
