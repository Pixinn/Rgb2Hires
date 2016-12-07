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

#ifndef __FILE_IO_H__
#define __FILE_IO_H__

#include <stdint.h>

void file_create( char* p_filename );
void file_open( char* p_filename, uint8_t* handle );  /* returns a handle to the file or OPEN_ERROR error code */
void file_set_offset( const uint8_t handle, const uint16_t offset );
uint16_t file_read( const uint8_t handle, uint8_t* out_buffer, const uint16_t len ); /* returns the nb bytes read */
uint16_t file_write( const uint8_t handle, uint8_t* in_buffer, const uint16_t len );  /* returns the nb bytes written */
void file_flush( const uint8_t handle );
void file_close( const uint8_t handle );
uint8_t file_error( void );  /* returns the last file error */

enum {
  NO_ERROR = 0u,
  FILE_OPENED = 2u,
  BAD_NAME,
  BAD_LENGTH = 4u,
  OPEN_ERROR = 0xFF
};


#endif
