/* Rgb2Hires
* Copyright (C) 2016 Christophe Meneboeuf <christophe@xtof.info>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/* Standard headers */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
/* My Headers */
#include "file_io.h"


uint8_t call_to_mli( uint8_t call, void* ptr_parameter );

#define CREATE      0xC0
#define OPEN        0xC8
#define READ        0xCA
#define WRITE       0xCB
#define CLOSE       0xCC
#define FLUSH       0xCD
#define SET_MARK    0xCE

const char* VOLUME_NAME = "/PRODOS.2.4.1/";

char Filename[ 65 ];
static uint8_t Io[ 1024+256 ]; //I need an adress aligned on 256: I'll use the 1st one inside Io
static uint8_t Error = 0u;



char* get_full_path( const char* p_filename )
{
  uint8_t len = strlen(VOLUME_NAME);
  if( len > 64u ) {
    return 0;
  }
  Filename[0] = len + strlen(p_filename);
  strcpy( 1 + Filename, VOLUME_NAME );
  strcpy( 1 + Filename + len, p_filename );
  return Filename;
}

/**************************/

typedef struct {
  uint8_t param_count;
  char* pathname;
  uint8_t access;
  uint8_t file_type;
  uint16_t aux_type;
  uint8_t storage_type;
  uint16_t create_date;
  uint16_t create_time;
} create_param_t;


void file_create( char* p_filename )
{
  create_param_t param;
  char* filename = get_full_path( p_filename );
  if( filename == 0 ) {
    Error = BAD_NAME;
    return;
  }
  param.param_count = 7u;
  param.pathname = filename;
  param.access = 0xE3;
  param.file_type = 0x6;
  param.aux_type = 0;
  param.storage_type = 1;
  param.create_date = (78<<8) + (04<<4) + 10;
  param.create_time = (12<<8) + 0;
  Error = call_to_mli( CREATE, &param );
  return;
}


/*****************************/
typedef struct {
  uint8_t param_count;
  char* pathname;
  uint8_t* io_buffer;
  uint8_t ref_num;
} open_param_t;


void file_open( char* p_filename, uint8_t* ref_num )
{
  open_param_t param;
  char* filename = get_full_path( p_filename );
  if( filename == 0 ) {
    Error = OPEN_ERROR;
    return;
  }
  param.param_count = 3u;
  param.pathname = filename;
  param.io_buffer = (uint8_t*)((uint16_t)(Io)+256-(uint16_t)(Io)%256); //Getting the 256byte aligned adrress
  Error = call_to_mli( OPEN, &param );
  if( Error == 0x44 || Error == 0x46 )  {
    file_create( p_filename );
    if( Error == NO_ERROR ) {
      Error = call_to_mli( OPEN, &param );
    }
  }
  if( Error == 0 ) {
    *ref_num = param.ref_num;
  }
  return;
}

/*****************************/


typedef struct {
  uint8_t param_count;
  uint8_t ref_num;
  uint16_t position;
  uint8_t dummy; /* high byte always 0 */
} set_mark_param_t;

void file_set_offset( const uint8_t handle, const uint16_t offset )
{
  set_mark_param_t param;
  param.param_count = 2u;
  param.ref_num = handle;
  param.position = offset;
  param.dummy = 0;
  Error = call_to_mli( SET_MARK, &param );
}

/********************************/

typedef struct {
  uint8_t param_count;
  uint8_t ref_num;
  uint8_t* data_buffer;
  uint16_t request_count;
  uint16_t trans_count;
} read_param_t;

uint16_t file_read( const uint8_t handle, uint8_t* buffer, const uint16_t len )
{
  read_param_t param;
  param.param_count = 4u;
  param.ref_num = handle;
  param.data_buffer = buffer;
  param.request_count = len;
  Error = call_to_mli( READ, &param );
  return param.trans_count;
}



/********************************/

typedef struct {
  uint8_t param_count;
  uint8_t ref_num;
  uint8_t* data_buffer;
  uint16_t request_count;
  uint16_t trans_count;
} write_param_t;

uint16_t file_write( const uint8_t handle, uint8_t* buffer, const uint16_t len )
{
  write_param_t param;
  param.param_count = 4u;
  param.ref_num = handle;
  param.data_buffer = buffer;
  param.request_count = len;
  Error = call_to_mli( WRITE, &param );
  return param.trans_count;
}


/*************************/


typedef struct {
  uint8_t param_count;
  uint8_t ref_num;
} close_param_t;

void file_close( const uint8_t handle )
{
  close_param_t param;
  param.param_count = 1u;
  param.ref_num = handle;
  Error = call_to_mli( CLOSE, &param );
}



/*************************/


typedef struct {
  uint8_t param_count;
  uint8_t ref_num;
} flush_param_t;

void file_flush( const uint8_t handle )
{
  flush_param_t param;
  param.param_count = 1u;
  param.ref_num = handle;
  Error = call_to_mli( FLUSH, &param );
}


/*************************/
uint8_t file_error( void )
{
  return Error;
}
