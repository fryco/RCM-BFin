/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  edit.c - line editor for the RCM-Bfin Blackfin robot.
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
 
#include "system.h"
#include "rcm-bfin.h"
#include "intuart.h"
#include <stdbool.h>

#define upper(c)  (((unsigned char)c > 0x60) ? (c & 0x5F) : c)

bool EditorRunning;

void insert_char(unsigned char), delete_char(), print_lines(int), print_help();
unsigned char *find_next_line(), *find_previous_line(), *find_buffer_end();
static unsigned char *pcur, *pend, *pstart, *pflashbufend;
static void EditPrint(char *);

// Since printf() (in rcm_bfin_gcc v2.0 and above) mucks with
// 'realtime' serial back and forth with the PC, we now use our own
// printing function so we bypass the streamed nature of printf()
void EditPrint(char * Str)
{
	while (*Str)
	{
		uart0SendChar(*Str);
		Str++;
	}
	PacketEnd(true);
	PacketBegin();
}

void edit (unsigned char *flashbuf)
{
    unsigned char ch;

    pstart = flashbuf;
    pcur = flashbuf;    
    pflashbufend = flashbuf + 0x00010000;
    pend = find_buffer_end();
    print_help();
        
    while (1) {
        EditPrint("\r\n* ");  // generate prompt
        ch = getch();
        switch (upper(ch)) { 
            case 'T':   // move to top of buffer
                EditPrint("TOP\r\n");
                pcur = flashbuf;
                break;
            case 'B':   // move to bottom of buffer
                pend = find_buffer_end();
                pcur = pend;
                EditPrint("BOTTOM\r\n");
                break;
            case 'P':   // move to previous line and display
                pcur = find_previous_line();
                print_lines(1);
                break;
            case 'N':   // move to next line and display
                pcur = find_next_line();
                print_lines(1);
                break;
            case 'L':   // list 20 lines
                print_lines(20);
                break;
            case 'I':   // insert text, end with ESC
                pend = find_buffer_end();
                EditPrint("INSERT\r\n");
                // loop until ESC char or ctrl-C
				while ((ch = getch()) != 0x1B && ch != 0x03) 
				{  
					if (ch == 0)   // filter out any nulls from the console
                       continue; 
					if (ch == 0x0d)
					{
						uart0SendChar(0x0d);
						ch = 0x0a;
					}
                    insert_char(ch);
                    uart0SendChar(ch);
					PacketEnd(true);
					PacketBegin();
                }
                break;
            case 'D':   // delete char
                delete_char();
                /*while (*pcur != 0) {
                    if (*pcur == 0x0A)
                        delete_char();
                        break;
                    delete_char();
                }*/
                print_lines(1);
                break;
            case 'H':   // print help screen
                print_help();
                break;
            case 'X':   // exit
                EditPrint("leaving editor \r\n");
                return;
        }
    }
}

void print_help() {
    EditPrint("(T)op (B)ottom (P)revious line (N)ext line (L)ist \r\n");
    EditPrint("(I)nsert until ESC (D)elete line (H)elp (X)exit \r\n");
}

void insert_char(unsigned char c) {
    unsigned char *cp;

    if (pcur > pflashbufend) {
        EditPrint("BUFFER FULL\r\n");
        return;
    }
    for(cp=pend; cp>=pcur; cp--)
        *(cp+1) = *cp;
    *pcur++ = c;
    pend++;
}

void delete_char() {
    unsigned char *cp;
    
    for(cp=pcur; cp<=pend; cp++)
        *cp = *(cp+1);
    if (pcur == pstart)
        return;
    pcur--;
    pend--;
}

unsigned char *find_next_line() {
    unsigned char *cp;
    for(cp=pcur; cp<pend; cp++) 
        if (*cp == 0x0A) 
            return (cp+1);
    return pcur;
}

unsigned char *find_previous_line() {
    unsigned char *cp;
    for(cp=pcur-2; cp>pstart; cp--) 
        if (*cp == 0x0A) 
            return (cp+1);
    return pstart;
}

unsigned char *find_buffer_end() {
    unsigned char *cp;
    for(cp=pstart; cp<pflashbufend; cp++) 
        if (*cp == 0) 
            return cp;
    return pstart;
}

void print_lines(int num) {
    unsigned char *cp;
    int ix;

    uart0SendChar(0x0A);
    uart0SendChar(0x0D);
    ix = num;
    for (cp=pcur; cp<pend && ix>0; cp++) {  
        if (*cp == 0)  // end of data
            return;
        if (*cp == 0x0A) {
            uart0SendChar(0x0D);
            ix--;
        }
        uart0SendChar(*cp);
    }
	PacketEnd(true);
	PacketBegin();
}


