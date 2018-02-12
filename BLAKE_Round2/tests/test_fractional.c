/** 
 * Test program to check how well the SAH-3 algorithms submitted to NIST
 * competition at end of 2008 handle changes in the bits beyond the 
 * MsgLength of a given Msg. Does changing a bit in the sequence beyond the
 * length of concern (msgbitlen) change the computed MD or not. 
 * author: Nicky Mouha, M Raunak
 * date: May 03, 2017
 */
 
 
#include <stdio.h>
#include <string.h>
#include "test_header.h"

#define MD_LENGTHS 4

const int MAX_MSGBITLEN = 2048;

void fprintBstr(FILE *fp, char *S, BitSequence *A, int L);

int main() {
  int i;
  int bitlens[MD_LENGTHS] = { 224, 256, 384, 512 };
  BitSequence Msg[MAX_MSGBITLEN/8];

  FILE* fp_out;
  fp_out = stdout;
  int counter=0;
  
  for (i=0; i<MD_LENGTHS; i++) {
    int msgbitlen;
    int hashbitlen = bitlens[i];

    for (msgbitlen=0; msgbitlen<=MAX_MSGBITLEN; msgbitlen++) {
      int j;
	  // checking 32 bits outside of the Msg length
      for (j=msgbitlen; j<msgbitlen+32 && j<MAX_MSGBITLEN; j++) {

        BitSequence digest1[64]; // 512 bits
        BitSequence digest2[64]; // 512 bits

        memset(Msg,0,MAX_MSGBITLEN/8); // zeroing out the whole message
		
        Hash(hashbitlen, Msg, msgbitlen, digest1); // compute the hash for Msg using a call to Hash()
		
        Msg[j/8] |= 1 << (7-(j%8)); // set one bit of the message (setting all the bits one after another in sequence)
		
        Hash(hashbitlen, Msg, msgbitlen, digest2);  // compute the hash with the new Msg
	
		// if the two digests are not the same, we have problem!
        if (memcmp(digest1,digest2,hashbitlen/8) != 0) {
			counter++;
			fprintf(fp_out, "FRACTIONAL ERROR");
			fprintf(fp_out, "\nhashbitlen = %d\n", hashbitlen);
			fprintBstr(fp_out, "Msg = ", Msg, MAX_MSGBITLEN/8);
			fprintf(fp_out, "Len = %d\n", msgbitlen);
			fprintBstr(fp_out, "MD1 = ", digest1, hashbitlen/8);
			fprintBstr(fp_out, "MD2 = ", digest2, hashbitlen/8);
        }
      }
    }
  }   
  if (counter == 0) {
	fprintf(fp_out, "NO FRACTIONAL ERROR FOUND\n");
  } else {
	fprintf(fp_out, "Num of Fractional Errors Found: %d\n", counter);
  }
  
  return 0; 
}

void  fprintBstr(FILE *fp, char *S, BitSequence *A, int L)
{
	int		i;
	fprintf(fp, "%s", S);
	for ( i=0; i<L; i++ )
		fprintf(fp, "%02X", A[i]);

	if ( L == 0 )
		fprintf(fp, "00");

	fprintf(fp, "\n");
}
