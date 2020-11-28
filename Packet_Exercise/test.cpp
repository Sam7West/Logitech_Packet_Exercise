#include "stdafx.h"

#include <io.h>
#include <iostream>
#include <string>
#include <list>
#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <random>

#include "packet.h"
#include "create_packets.h"
#include "config.h"
#include "decode_packets.h"

using namespace std;

/* 
/****************************************************************/
/* Function:	gen_random_payload    				*/
/* Author:	Sam West					*/
/*								*/
/* Description:							*/
/*		Generates random data of random length up to    */
/*		the maximum size.                               */
/*								*/
/* Parameters:	                                               	*/
/* Returns:	      						*/
/****************************************************************/

void
gen_random_payload
(
 default_random_engine random_engine,
 uint8_t* data,
 uint8_t size 
 )
{  
  uniform_int_distribution<> distr( 0, 0xff );
  
  for( int i = 0; i < size; i++ )
    {            
      data[ i ] = (uint8_t) distr( random_engine );
    }
}

/****************************************************************/
/* Function:	gen_random_stop_flags				*/
/* Author:	Sam West					*/
/*								*/
/* Description:							*/
/*		Utility func. which randomly sets the bit flags */
/*		in a the "Stop Data Transfer" packet.		*/
/*								*/
/* Parameters:	default_random_engine&                         	*/
/* Returns:	uint8_t						*/
/****************************************************************/

uint8_t
gen_random_stop_flags
(
 default_random_engine& random_engine
 )
{
  uniform_int_distribution<> distr( 0, 0xff );

  return
    ( BIT_REBOOT | BIT_VERIFY | BIT_TEST )
    & ( (uint8_t) distr( random_engine ) );
}


/****************************************************************/
/* Function:	test          					*/
/* Author:	Sam West					*/
/*								*/
/* Description:							*/
/*		Generates byte array of "Start Data Transfer",  */
/*		"Data" and "Stop Data Transfer" packets with a  */
/*		payload of random data and random length.       */
/*								*/
/* Parameters:	uint8_t*& data                                 	*/
/* Returns:	void  						*/
/****************************************************************/

void
test
(
 uint8_t*& data
 )
{
  static uint8_t
    static_data[ TEST_TOTAL_PAYLOAD_SIZE_MAX ] = { 0 };
  
  data = static_data;

  /* reset array mem. */
  memset( static_data, 0, TEST_TOTAL_PAYLOAD_SIZE_MAX );
  
  random_device device;
  default_random_engine random_engine( device() );
  uniform_int_distribution<>
    payload_size_distr( 0, PAYLOAD_SIZE_MAX );

  Short_Union_T sequence_id = { 0 };
  uint8_t payload_size = 0;

  uint32_t i = 0;
  
  /* Start Data Transfer Packet */

  data[ i++ ] = SOFTWARE_ID;
  data[ i++ ] = sequence_id.Byte[ 0 ];
  data[ i++ ] = sequence_id.Byte[ 1 ];
  data[ i++ ] = StartDataTransfer_T;

  /* skip "Total Payload Size" - gets set below */
  i += 4;

  sequence_id.Short++;

  /* Data Packet */

  data[ i++ ] = SOFTWARE_ID;
  data[ i++ ] = sequence_id.Byte[ 0 ];
  data[ i++ ] = sequence_id.Byte[ 1 ];
  data[ i++ ] = Data_T;

  payload_size = (uint8_t)payload_size_distr( random_engine );
  data[ i++ ] = payload_size;
  gen_random_payload( random_engine, &data[ i ],  payload_size );
  
  i += payload_size;

  sequence_id.Short++;
  
  /* Stop Data Transfer Packet */
  
  data[ i++ ] = SOFTWARE_ID;
  data[ i++ ] = sequence_id.Byte[ 0 ];
  data[ i++ ] = sequence_id.Byte[ 1 ];
  data[ i++ ] = StopDataTransfer_T;

  data[ i++ ] = gen_random_stop_flags( random_engine );


  /* set Total Payload Size field in first packet */ 

  Integer_Union_T total_payload_size;
  total_payload_size.Integer = i;

  data[ 4 ] = total_payload_size.Byte[0];
  data[ 5 ] = total_payload_size.Byte[1];
  data[ 6 ] = total_payload_size.Byte[2];
  data[ 7 ] = total_payload_size.Byte[3];
}
