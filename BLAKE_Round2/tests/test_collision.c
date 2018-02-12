/**
 * Test for Collisions in the SHA-3 algorithm implementations 
 * submitted in SHA-3 competitions in 2008 and later
 * author: Nicky Mouha, M Raunak
 * date: May 03, 2017
 */
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_header.h"

#define MD_LENGTHS 4
#define MAX_MSGBITLEN 2048

// BitSequence is defined as unsinged char
void fprintBstr(FILE *fp, char *S, BitSequence *A, int L);
int mycompare(const void* a, const void* b);

typedef struct vec {
	int mlen, dlen; // message length and digest lenth
	BitSequence message[MAX_MSGBITLEN/8]; // the message
	BitSequence digest[64];	// the computed digest
} vec;
	
 int main() {

	int i;
	int idx;

	int bitlens[MD_LENGTHS] = { 224, 256, 384, 512 }; // message digest (output bits) lengths 
  
	// BitSequence is unsigned char; Msg has 256 bytes of space
	// The highest MD output is of 512 bits or 64 bytes
	BitSequence Msg[MAX_MSGBITLEN/8], MD[64]; 
	int colFound = 0; // counter for number of collisions found

	FILE* fp_out;
	fp_out = stdout; // Setting the file pointer to print everything to standard output.
	
	// allocate memory for the table where hashes will be stored for a large number of messages
	vec* hashes = (vec*) malloc(MD_LENGTHS*(MAX_MSGBITLEN*(1+MAX_MSGBITLEN)/2+MAX_MSGBITLEN+1)*sizeof(vec));
	idx = 0;
	
	for (i=0; i<MD_LENGTHS; i++) { // for each MD length
		int msgbitlen;
		int hashbitlen = bitlens[i]; // desired output of the hash bit length
		// for every msg of length 0 to MAX_MSGBITLEN
		for (msgbitlen=0; msgbitlen<=MAX_MSGBITLEN; msgbitlen++) {
			int j;
			for (j=0; j<msgbitlen+1; j++) {
				memset(Msg,0,MAX_MSGBITLEN/8); // First zero out the whole Msg
				if (j!=msgbitlen) {
					Msg[j/8] |= 1 << (7-(j%8)); // set one bit of the message
				}
				Hash(hashbitlen, Msg, msgbitlen, MD);
				hashes[idx].mlen = msgbitlen;
				memcpy(hashes[idx].message,Msg,MAX_MSGBITLEN/8);
				hashes[idx].dlen = hashbitlen;
				memcpy(hashes[idx].digest,MD,64);
				idx++;
			}
		}
	}
  

  // apply quicksort on hashes
  qsort(hashes, idx, sizeof(vec), mycompare);

  for(i = 0; i < idx-1; i++) {
    if (mycompare(&hashes[i],&hashes[i+1]) == 0) { // if the two hashes in the sorted table are identical
		colFound++;
		fprintf(fp_out, "\nLen = %d\n", hashes[i].mlen);
		fprintBstr(fp_out, "Msg = ", hashes[i].message,(hashes[i].mlen+7)/8);
		fprintf(fp_out, "Len = %d\n", hashes[i+1].mlen);
		fprintBstr(fp_out, "Msg = ", hashes[i+1].message,(hashes[i+1].mlen+7)/8);
		fprintf(fp_out, "hashbitlen = %d\n", hashes[i].dlen);
		fprintBstr(fp_out, "MD = ", hashes[i].digest, hashes[i].dlen/8);
		fprintBstr(fp_out, "MD = ", hashes[i+1].digest, hashes[i].dlen/8);
    }
	if (colFound == 1024)
		break; // break out of the loop 
  }

  if (colFound == 0)
	  fprintf(fp_out, "No Collision Found!");
  else if (colFound == 1024)
	  fprintf(fp_out, "Many Collisions Found! %d reported here", colFound);
  else
	  fprintf(fp_out, "Collisions Found in this test run: %d", colFound);
  
  return 0;
}

 int mycompare(const void* a, const void* b) {
	vec* first  = (vec*) a;
    vec* second = (vec*) b;
    return memcmp(first->digest,second->digest,64);
  }

/* This function is part of the code originally provided by NIST for generating 
  Known Answer Test outputs. Copied and used here for convenience */
void fprintBstr(FILE *fp, char *S, BitSequence *A, int L)
{
	int		i;
	fprintf(fp, "%s", S);

	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}
