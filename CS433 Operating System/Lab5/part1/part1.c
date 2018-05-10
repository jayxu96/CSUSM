//============================================================
// CS 433 Operating System
// Assignment 5: Memory 
// Part 1: Find the page # and offset of a virtual address
// Name: Runyu Xu
// Date: 05/03/2018
// File type: driver file for part 1
// Description: This is a program to find the page numeber and 
// offset of a virtual address in a system that has a 32-bit 
// virtual address with 4-KB page size.
//============================================================
#include <stdio.h>
#include <cstdlib>
int main(int argc, char *argv[])
{   //data field
	unsigned long long int address;
	unsigned long long int page;
	unsigned long long int offset;

	if (argc < 2 || argc > 2) {
		printf("You should enter: ./part1 <virtual address>");
		return 0;
	}
	address = atoll(argv[1]); //read address from user's input

	printf("---------------------------------------------------------\n");
	printf("// Name: Runyu Xu\n");
	printf("// Date : May 3rd, 2018\n");
	printf("// Course : CS433 - Operating Systems\n");
	printf("// Assignment 5: Memory \n" );
	printf("// Part1: find the page # and offset \n");
	printf("---------------------------------------------------------\n\n");

	// print the information of a virtual address
	// Since the system uses 4-KB page size, we need 12 bits for offset and the 
	// remaining 20 bits are for page number
	printf("The Adress %llu contains: \n", address); // print the address
	page = address / 4096; // calculate the page # 
	printf("Page number: %llu\n", page);
	offset = address & 0x00000FFF; // calculate the offset using & method to keep the last 12 bits 
	printf("Offset = %llu\n\n", offset);
	return 0;
}