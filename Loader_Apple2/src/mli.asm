; Rgb2Hires
; Copyright (C) 2016 Christophe Meneboeuf <christophe@xtof.info>
;
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
;
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
; GNU General Public License for more details.
;
; You should have received a copy of the GNU General Public License
; along with this program. If not, see <http://www.gnu.org/licenses/>.



;
; Handling calls to Apple ProDOS 8 MLI
;

        .import         __dos_type
        .import         popa

        .export _call_to_mli

.code

_call_to_mli:
        ; Store parameters
        STA     Mli_Param
        STX     Mli_Param+1
        ; Store call number
        JSR     popa
        STA     Mli_Call

        ; Call MLI and return
        JSR     $BF00           ; MLI call entry point
Mli_Call:
        .byte   $00
Mli_Param:
        .addr   $00
        RTS

        .code
