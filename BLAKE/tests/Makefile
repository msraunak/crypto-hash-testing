#Makefile for BLAKE 
CC=gcc
CFLAGS=-Wall -O3
IDIR=../ref_impl

all: collision compare compare_csv fractional

collision:
	$(CC) $(CFLAGS) test_collision.c $(IDIR)/*.c -o test_collision
	
compare:
	$(CC) $(CFLAGS) test_compare.c $(IDIR)/*.c -o test_compare
	
compare_csv:
	$(CC) $(CFLAGS) test_compare_csv.c $(IDIR)/*.c -o test_compare_csv

fractional:
	$(CC) $(CFLAGS) test_fractional.c $(IDIR)/*.c -o test_fractional
