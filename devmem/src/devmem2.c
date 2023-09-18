/*
 * devmem2.c: Simple program to read/write from/to any location in memory.
 *
 *	Copyright (C) 2000, Jan-Derk Bakker (jdb@lartmaker.nl)
 *
 *
 * This software has been developed for the LART computing board
 * (http://www.lart.tudelft.nl/). The development has been sponsored by
 * the Mobile MultiMedia Communications (http://www.mmc.tudelft.nl/)
 * and Ubiquitous Communications (http://www.ubicom.tudelft.nl/)
 * projects.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>

#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
	__LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)

static inline void *fixup_addr(void *addr, size_t size);

// int main(int argc, char **argv) {
int devmem_init(int argc, char **argv) {
		int fd;
		void *map_base, *virt_addr;
		unsigned long read_result, write_val;
		off_t target;
		int access_type = 'w';
		char fmt_str[128];
		size_t data_size;
		unsigned int page_size, mapped_size, offset_in_page;
		unsigned int width = 1;

		if(argc < 2) {
				fprintf(stderr, "\nUsage:\t%s { address } [ type [ data ] ]\n"
						"\taddress : memory address to act upon\n"
						"\ttype		 : access operation type : [b]yte, [h]alfword, [w]ord, [l]ong\n"
						"\tdata		 : data to be written\n\n",
						argv[0]); exit(1);
		}
		target = strtoul(argv[1], 0, 0);
		if(argc > 2)
		{
			access_type = tolower(argv[2][0]);
			switch(access_type) {
				case '8': //8bit
						access_type = 'b';
				case 'b':
						width=1;
						break;
				case '1'://16bit
						access_type = 'h';
				case 'h':
						width=2;
						break;
				case '3'://32bit
						access_type = 'w';
				case 'w':
						width=4;
						break;
				case '6'://64bit
						access_type = 'l';
				case 'l':
						width=8;
						break;
				default:
						fprintf(stderr, "access_type Illegal data type '%c'.\n", access_type);
						exit(2);
			}
		}
		if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
		// printf("/dev/mem opened.\n");
		// fflush(stdout);

		mapped_size = page_size = getpagesize();
		offset_in_page = (unsigned int)target & (page_size - 1);
		if (offset_in_page + width > page_size) {
			/* This access spans pages. Must map two pages to make it possible: */
			mapped_size *= 2;
		}
		/* Map one page */
		map_base = mmap(0, mapped_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~(off_t)(page_size - 1));
		if(map_base == (void *) -1) FATAL;
		// printf("Memory mapped at address %p.\n", map_base);
		// fflush(stdout);

		virt_addr = map_base + offset_in_page;
		switch(access_type) {
				case 'b':
						data_size = sizeof(unsigned char);
						virt_addr = fixup_addr(virt_addr, data_size);
						read_result = *((unsigned char *) virt_addr);
						break;
				case 'h':
						data_size = sizeof(unsigned short);
						virt_addr = fixup_addr(virt_addr, data_size);
						read_result = *((unsigned short *) virt_addr);
						break;
				case 'w':
						data_size = sizeof(uint32_t);
						virt_addr = fixup_addr(virt_addr, data_size);
						read_result = *((uint32_t *) virt_addr);
						break;
				case 'l':
						data_size = sizeof(uint64_t);
						virt_addr = fixup_addr(virt_addr, data_size);
						read_result = *((uint64_t *) virt_addr);
						break;
				default:
						fprintf(stderr, "Illegal data type '%c'.\n", access_type);
						exit(2);
		}
		sprintf(fmt_str, "Read at address  0x%%08lX (%%p): 0x%%0%dlX\n", 2*data_size);
		printf(fmt_str, (unsigned long)target, virt_addr, read_result);
		fflush(stdout);

		if(argc > 3) {
				write_val = strtoul(argv[3], 0, 0);
				switch(access_type) {
						case 'b':
								virt_addr = fixup_addr(virt_addr, sizeof(unsigned char));
								*((unsigned char *) virt_addr) = write_val;
								read_result = *((unsigned char *) virt_addr);
								break;
						case 'h':
								virt_addr = fixup_addr(virt_addr, sizeof(unsigned short));
								*((unsigned short *) virt_addr) = write_val;
								read_result = *((unsigned short *) virt_addr);
								break;
						case 'w':
								virt_addr = fixup_addr(virt_addr, sizeof(uint32_t));
								*((uint32_t *) virt_addr) = write_val;
								read_result = *((uint32_t *) virt_addr);
								break;
						case 'l':
								virt_addr = fixup_addr(virt_addr, sizeof(uint64_t));
								*((uint64_t *) virt_addr) = write_val;
								read_result = *((uint64_t *) virt_addr);
								break;
				}
				sprintf(fmt_str, "Write at address 0x%%08lX (%%p): 0x%%0%dlX, "
								"readback 0x%%0%dlX\n", 2*data_size, 2*data_size);
				printf(fmt_str, (unsigned long)target, virt_addr,
							 write_val, read_result);
				fflush(stdout);
		}

		if(munmap(map_base, mapped_size) == -1) FATAL;
		close(fd);
		return 0;
}

static inline void *fixup_addr(void *addr, size_t size)
{
#ifdef FORCE_STRICT_ALIGNMENT
			 unsigned long aligned_addr = (unsigned long)addr;
			 aligned_addr &= ~(size - 1);
			 addr = (void *)aligned_addr;
#endif
			 return addr;
}
