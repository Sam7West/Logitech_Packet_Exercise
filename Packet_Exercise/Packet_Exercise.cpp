#include "stdafx.h"

#include <io.h>
#include <string>
#include "create_packets.h"
#include <list>
#include <cstdint>
#include <stdlib.h>

#include "packet.h"
#include "create_packets.h"
#include "config.h"
#include "decode_packets.h"
#include "test.h"

using namespace std;

int
main( void )
{
  /* generate random test packet bytes */ 

  uint8_t* data;
  test( data );

  /* parse into list of Packet objects */

  list< Packet > packet_list;

  try
    {
      create_packets( data, packet_list );
    }
  catch( Error_Unknown_Packet_Type& e)
    {
      /* Optional: add extra code to handle this exception */
      cerr << e.what() << endl;
    }
  catch( Error_Unexpected_First_Packet& e)
    {
      /* Optional: add extra code to handle this exception */
      cerr << e.what() << endl;
    }  
  catch( Error_Max_Payload_Exceeded& e)
    {
      /* Optional: add extra code to handle this exception */
      cerr << e.what() << endl;
    }  
  catch( exception& e)
    {
      cerr << e.what() << endl;
    }

  /* print to console */

  decode_packets( packet_list );
  
  return EXIT_SUCCESS;
}
