#include "stdafx.h"

#include <cstdint>
#include <list>
#include <string>
#include <array>
#include <stdlib.h>
#include <vector>

#include "generic_typedefs.h"
#include "packet.h"
#include "create_packets.h"
#include "config.h"

using namespace std;

/****************************************************************/
/* Function:	parse_packet  					*/
/* Author:	Sam West					*/
/*								*/
/* Description:							*/
/*		Utility function for parsing a single packet    */
/*		from a byte list. Throws exception if the max.  */
/*		payload size is exceeded or an unknown packet   */
/*		type is received.				*/
/*								*/
/* Parameters:	uint8_t*& input_ptr                            	*/
/* Returns:	Packet						*/
/****************************************************************/

Packet
parse_packet( uint8_t*& input_ptr )
{

  uint8_t software_id;
  Short_Union_T sequence_id;
  uint8_t packet_type; 

  /* parse header fields */
  /* (ignoring endianness) */

  software_id = *( input_ptr++ );
  sequence_id.Byte[ 0 ] = *( input_ptr++ );
  sequence_id.Byte[ 1 ] = *( input_ptr++ );
  packet_type = *( input_ptr++ );

  /* parse specific packet types */

  switch( packet_type )
    {
    case StartDataTransfer_T:
      {
	Integer_Union_T total_payload_size;
      
	/* parse total size */
	total_payload_size.Byte[ 0 ] = *( input_ptr++ );
	total_payload_size.Byte[ 1 ] = *( input_ptr++ );
	total_payload_size.Byte[ 2 ] = *( input_ptr++ );
	total_payload_size.Byte[ 3 ] = *( input_ptr++ );
	  

	return
	  StartDataTransfer_Packet
	    (
	     software_id, sequence_id.Short,
	     total_payload_size.Integer
	     );
	break;
      }

    case StopDataTransfer_T:
      {
	uint8_t flags;
	bool reboot, verify, test;
      
	/* parse total size */
	flags = *( input_ptr++ );

	reboot = (flags & BIT_REBOOT);
	verify = (flags & BIT_VERIFY);
	test   = (flags & BIT_TEST);
	
	return
	  StopDataTransfer_Packet
	  (
	   software_id, sequence_id.Short,
	   reboot, verify, test
	   );
	break;
      }

    case Data_T:
      {
	uint8_t payload_size = *( input_ptr++ );

	/* check max payload size not exceeded */
	if( payload_size > PAYLOAD_SIZE_MAX )
	  {
	    throw Error_Max_Payload_Exceeded();
	  }
	
	vector< uint8_t > packet_data( input_ptr, input_ptr + payload_size );

	input_ptr += payload_size;
	
	return
	  Data_Packet
	  (
	   software_id, sequence_id.Short,
	   packet_data, payload_size
	   );
	break;
      }
  
    default:
      /* failed if got here */
      throw Error_Unknown_Packet_Type();
    }
}


/****************************************************************/
/* Function:	create_packets					*/
/* Author:	Sam West					*/
/*								*/
/* Description:							*/
/*		Takes a byte array of packet data and parses    */
/*		into a list of "Packet" class objects, raising  */
/*		exceptions if the first packet is not a Start   */
/*		Data Transfer packet. Nested func's also throw  */
/*		exceptions for max. payload size exceeded and	*/
/*		unknown packet type received.                	*/
/*								*/
/* Parameters:	uint8_t data[], list< Packet >& packet_list	*/
/* Returns:	void						*/
/****************************************************************/

void
create_packets
(
 uint8_t data[],
 /* uint32_t size, */
 list< Packet >& packet_list
 )
{  
  // parse consecutive packets

  uint8_t* data_ptr = data;

  /* parse first packet - expect "startDataTransfer" type */
  
  Packet packet = parse_packet( data_ptr );
  
  auto first_packet = (StartDataTransfer_Packet&) packet;
  
  if( first_packet.header.packet_type != StartDataTransfer_T )
    {
      /* Error: needed first packet to be a "StartDataTransfer" to 
	 get Total Payload Length */
      throw Error_Unexpected_First_Packet();
    }
  
  uint32_t total_payload_size =
    first_packet.get_total_payload_size();
  
  /* (assuming Total Payload Size includes size of first packet) */
  uint8_t* end = data + total_payload_size;
  
  /* parse the following bytes into packets */
  packet_list.push_back( (Packet)first_packet );
  
  while( data_ptr < end )
    {      
      packet_list.push_back
	(
	 parse_packet( data_ptr )
	 );
    }
}
