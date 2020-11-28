#include "stdafx.h"

#include <stdlib.h>
#include <list>
#include <iostream>
#include "packet.h"
#include "decode_packets.h"
#include <iomanip>

using namespace std;

/****************************************************************/
/* Function:	decode_packets					*/
/* Author:	Sam West					*/
/*								*/
/* Description:							*/
/*		Takes a list of Packets and decodes the header  */
/*		information and fields specific to each packet  */
/*		type then prints the information to console.    */
/*								*/
/* Parameters:	list< Packet >                                 	*/
/* Returns:	voic  						*/
/****************************************************************/

void
decode_packets
(
 list< Packet > packet_list
 )
{  
  auto iter = packet_list.begin();
  auto end = packet_list.end();

  #define WIDTH 16
  #define TABLE_WIDTH ((WIDTH * 5) + 40)

  cout << string( TABLE_WIDTH, '_' ) << endl;

  cout << setw( WIDTH ) << left << "Count:";
  cout << setw( WIDTH ) << left << "Software ID:";
  cout << setw( WIDTH ) << left << "Sequence ID:";
  cout << setw( WIDTH ) << left << "Packet Type:";
  cout << setw( WIDTH ) << left << "Fields:";
  cout << endl;
  
  int count = -1;
  
  while( iter != end )
    {
      Packet& packet = *(iter++);
      count++;

      cout << setw( WIDTH ) << left << count;
      cout << setw( WIDTH ) << left << (int)packet.header.software_id;
      cout << setw( WIDTH ) << left << (int)packet.header.sequence_id;

      switch( packet.header.packet_type )
	{
	case StartDataTransfer_T:
	  {
	    StartDataTransfer_Packet& start_packet =
	      static_cast< StartDataTransfer_Packet& >( packet );
	    cout << setw( WIDTH ) << left << "\"Start\"";
	    cout << left
		 << "total_payload_size => "
		 << (int)start_packet.get_total_payload_size();
	    break;
	  }
	case Data_T:
	  {
	    Data_Packet& data_packet =
	      static_cast< Data_Packet& >( packet );

	    cout << setw( WIDTH ) << left << "\"data\"";

	    cout << left
		 << "payload_size => "
		 << (int)data_packet.get_payload_size()
		 << ";\t";
		      
	    cout << left << "bytes => {";

	    if( data_packet.get_payload_size() > 0 )
	      {
		vector< uint8_t > data = data_packet.get_data_ref();
 		uint8_t size = data_packet.get_payload_size();
		uint8_t max = ( size < 3 ) ? size : 3;

		auto iter = data.begin();
		for( int i = 0; i < max; i++ )
		  {
		    cout << (int)*( iter++ ) << ", ";
		  }

		cout << "...";
	      }
	    cout << "}";
	    break;
	  }
	case StopDataTransfer_T:

	  {
	    StopDataTransfer_Packet& stop_packet =
	      static_cast< StopDataTransfer_Packet& >( packet );
	    cout << setw( WIDTH ) << left << "\"stop\"";
	    cout << left
		 << "Reboot => "
		 << (bool)stop_packet.get_reboot()
		 << "; "
		 << "Verify => "
		 << (bool)stop_packet.get_verify()
		 << "; "
		 << "Test => "
		 << (bool)stop_packet.get_test();
	    break;
	  }
	default:
	  {
	    cout << "Error: Unknown packet type" << endl;
	    break;
	  }
	}

      cout << endl;
    }
}
