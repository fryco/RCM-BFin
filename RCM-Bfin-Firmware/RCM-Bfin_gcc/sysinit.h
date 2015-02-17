/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  sysinit.h - header for system initialization routines
 *
 *  Part of the RCM-BFin GCC v2.0 firmware for the RCM-BFin robot
 *    
 *  Copyright (C) 2015  Engineering^3
 *
 *  Based on the original Surveyor firmware, written and copyrighted
 *  by Surveyor Corp. 
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details (www.gnu.org/licenses)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
/*
 *
 *
 */
 
#ifndef __SYSINIT_H__
#define __SYSINIT_H__

//--------------------------------------------------------------------------//
// Prototypes
//--------------------------------------------------------------------------//
void InitTMR0(void);
void Init_Interrupts(void);
void init_program_clocks(void);

#endif