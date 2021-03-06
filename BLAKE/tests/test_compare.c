/** Test program to check the MDs produced by a single call to Hash() 
 * vs. calls to init() -> update()+ -> final() 
 * two calls to updates are done with different fragments of the Msg.
 * If the two MDs are not the same, we have a problem! 
 * author: Nicky Mouha
 * date: May 03, 2017
 */

 #include <stdio.h>
 #include <string.h>
 #include "test_header.h"
	
 #define MD_LENGTHS 4

 const int FRAG1_MAX = 2048;
 const int FRAG2_MAX = 2048;
 const int MAX_MSGBITLEN = 8096;

 void fprintBstr(FILE *fp, char *S, BitSequence *A, int L);

 BitSequence M[2448] = {
	0x34, 0x5b, 0x1c, 0x61, 0xcd, 0x3d, 0x08, 0xc1, 0xee, 0xff, 0x7c, 0x70, 0x47, 0x9f, 0x56, 0x64,
	0x5a, 0xd7, 0xc2, 0x36, 0xeb, 0xa0, 0x63, 0xdd, 0xa5, 0xe4, 0x9f, 0x60, 0xc6, 0x2e, 0x53, 0x78,
	0x4b, 0xaf, 0x02, 0x55, 0xf3, 0xd9, 0x7b, 0x82, 0xbe, 0x1b, 0xe8, 0x75, 0xdf, 0xf3, 0xcb, 0xcf,
	0xe3, 0x35, 0x11, 0xd6, 0xab, 0x1c, 0x50, 0xd6, 0x2e, 0x83, 0x5d, 0x4b, 0x72, 0x63, 0x90, 0x68,
	0x5f, 0x49, 0xaa, 0x0e, 0xe1, 0x5e, 0x61, 0xc0, 0x9b, 0x7f, 0x43, 0xc5, 0xc1, 0x00, 0xb1, 0xea,
	0x3a, 0x80, 0x2f, 0x6f, 0xd6, 0xfc, 0xcb, 0x1d, 0x73, 0xee, 0x01, 0x0f, 0xd5, 0x48, 0xf5, 0xea,
	0xb3, 0xac, 0x7d, 0xb6, 0x68, 0xc2, 0xbf, 0x20, 0x06, 0xa4, 0xae, 0xcc, 0xfd, 0x53, 0x37, 0xfa,
	0x50, 0x0d, 0x19, 0x7f, 0x2f, 0x5e, 0x0e, 0x40, 0x49, 0xa5, 0xdb, 0x06, 0x14, 0x54, 0x6f, 0xb9,
	0x24, 0x11, 0x0b, 0x4e, 0x63, 0xbb, 0x5e, 0x62, 0x5d, 0xf9, 0xb3, 0x83, 0x30, 0x68, 0x12, 0x14,
	0xdf, 0xe7, 0x6c, 0x70, 0x5d, 0x24, 0x55, 0x52, 0x90, 0xa4, 0x1f, 0x3c, 0x2c, 0xfe, 0x8b, 0x6a,
	0xb9, 0x95, 0xae, 0x51, 0x59, 0x02, 0xe3, 0x8a, 0x48, 0xdc, 0xfe, 0x84, 0xbb, 0x69, 0x41, 0x8a,
	0x0c, 0x72, 0xb1, 0x5a, 0x28, 0xea, 0x90, 0xc9, 0xab, 0x00, 0xd2, 0xcf, 0x44, 0x0e, 0x86, 0x71,
	0x50, 0x06, 0xf1, 0x0b, 0x74, 0xa6, 0xb2, 0xdc, 0x8a, 0xe6, 0x2f, 0xed, 0x45, 0x79, 0xbf, 0x5b,
	0x7f, 0x6f, 0x86, 0xa4, 0x50, 0x33, 0xc7, 0x0a, 0xc3, 0xe5, 0xb6, 0x49, 0x7b, 0xfb, 0x23, 0x64,
	0xcd, 0xc0, 0x40, 0xb5, 0x99, 0x11, 0x23, 0x3b, 0x92, 0x66, 0xff, 0x2a, 0x1f, 0xfb, 0x75, 0xdd,
	0x15, 0x91, 0xf8, 0xa4, 0x62, 0x72, 0x83, 0x45, 0xca, 0x1d, 0x92, 0x9c, 0xa0, 0x4f, 0x8a, 0xdd,
	0xaa, 0x74, 0x8a, 0xe6, 0x72, 0x0b, 0xc9, 0x63, 0x40, 0x5b, 0xbc, 0xd0, 0x11, 0x09, 0x7b, 0xa9,
	0xb1, 0xcf, 0x0e, 0x86, 0x8f, 0x92, 0x30, 0x8e, 0xd9, 0x89, 0x36, 0x34, 0xdd, 0x87, 0x97, 0x2b,
	0xdd, 0x38, 0xe1, 0x5e, 0xb2, 0xe3, 0x86, 0x39, 0x72, 0x58, 0xde, 0x13, 0xaf, 0xf5, 0x78, 0xb9,
	0xfd, 0x51, 0x80, 0x0b, 0x8e, 0x56, 0x0f, 0xcc, 0xbe, 0xbb, 0x19, 0x26, 0xc9, 0x25, 0x6d, 0x13,
	0xb9, 0xfd, 0x73, 0xec, 0x05, 0x24, 0x66, 0x1c, 0x87, 0x5f, 0x14, 0x9d, 0x39, 0x32, 0xf0, 0xa6,
	0x1d, 0xf0, 0xb4, 0x98, 0x4f, 0xa9, 0xcb, 0x40, 0xf0, 0xd9, 0x6d, 0xd2, 0x91, 0x67, 0x0f, 0xb9,
	0xea, 0x7f, 0xf3, 0x58, 0xdf, 0x8b, 0x58, 0x0e, 0xa8, 0x20, 0x55, 0xb3, 0xac, 0x62, 0xd5, 0xb5,
	0xf9, 0xb2, 0x70, 0x22, 0x61, 0xae, 0x19, 0x64, 0x71, 0x0b, 0x33, 0x83, 0xae, 0x4f, 0x01, 0x16,
	0x56, 0x48, 0x18, 0x48, 0x36, 0x0f, 0x95, 0xef, 0x09, 0x73, 0x40, 0x7a, 0x32, 0x48, 0x77, 0xe9,
	0x2c, 0x54, 0x45, 0x9a, 0xe4, 0xc9, 0x89, 0xee, 0xda, 0x1f, 0x3a, 0xc5, 0xdc, 0xc8, 0xcf, 0x39,
	0x04, 0x5c, 0xc9, 0x33, 0x98, 0xe8, 0x95, 0xf5, 0x1b, 0x83, 0xbc, 0x36, 0xcc, 0xda, 0x3f, 0x96,
	0xac, 0x91, 0x2e, 0xd5, 0x1b, 0x97, 0xe0, 0x2f, 0x19, 0x5d, 0x1a, 0x9f, 0x0d, 0xf5, 0x1a, 0xfa,
	0x2e, 0x17, 0x1d, 0x30, 0x91, 0xc2, 0xf8, 0x21, 0x42, 0xf6, 0x0e, 0xc2, 0x85, 0x3e, 0x67, 0x35,
	0xb4, 0xfb, 0xdd, 0x69, 0x4b, 0x3e, 0x77, 0x01, 0x82, 0xf4, 0xf8, 0xd4, 0x5f, 0xd7, 0xa3, 0x9b,
	0xf3, 0xcf, 0xa1, 0x3d, 0x74, 0xbb, 0x0d, 0x57, 0x86, 0x20, 0x72, 0x3b, 0x86, 0x4e, 0xd9, 0x37,
	0x61, 0xaf, 0x8d, 0x71, 0x5d, 0x1c, 0xbd, 0xc8, 0x0e, 0x73, 0x84, 0xe4, 0x2e, 0xab, 0x86, 0xf3,
	0x23, 0xcb, 0x4a, 0xb5, 0xfa, 0x71, 0x96, 0xf4, 0x2c, 0xcb, 0xf9, 0x04, 0x74, 0xf8, 0x8a, 0xce,
	0x09, 0xdf, 0x4b, 0x09, 0x08, 0xed, 0x4d, 0xc5, 0x18, 0x9b, 0x94, 0x0d, 0x05, 0x71, 0x30, 0x24,
	0xea, 0xd4, 0xac, 0x32, 0x1e, 0xa1, 0x0c, 0xf6, 0x92, 0x58, 0xd6, 0xd7, 0x8e, 0x43, 0x34, 0xc0,
	0xf9, 0xbe, 0x6f, 0x68, 0xad, 0x61, 0x32, 0x5a, 0x60, 0x0f, 0x08, 0x87, 0x9c, 0xb3, 0x4d, 0x45,
	0x35, 0x27, 0xfd, 0xfc, 0x1a, 0x49, 0x4e, 0x36, 0x90, 0x19, 0x86, 0x8b, 0xfe, 0xa6, 0x54, 0x60,
	0x89, 0xd0, 0xa2, 0x60, 0xd6, 0x04, 0x0f, 0x9c, 0x6a, 0xac, 0x04, 0xaf, 0xa2, 0xf5, 0xed, 0x54,
	0x15, 0x07, 0x11, 0xbd, 0xcd, 0x27, 0x38, 0xc9, 0xda, 0x21, 0x6e, 0x62, 0x01, 0xd1, 0x5e, 0x57,
	0xb6, 0xa7, 0x77, 0xe8, 0xc7, 0xe2, 0x65, 0x8d, 0x00, 0x0c, 0x63, 0xca, 0x12, 0xef, 0x78, 0x79,
	0x5a, 0x1d, 0x33, 0xff, 0xbd, 0xae, 0x40, 0xba, 0x9f, 0x5c, 0xb8, 0x5f, 0xd0, 0x1b, 0x3a, 0x89,
	0x58, 0x82, 0xca, 0x5e, 0x4c, 0x0a, 0x83, 0x4f, 0x35, 0x99, 0x43, 0xc4, 0xf3, 0x13, 0xdf, 0x4f,
	0x4f, 0x07, 0xd4, 0xee, 0xd2, 0xb0, 0x00, 0x01, 0x6c, 0x9b, 0xec, 0xda, 0x98, 0x64, 0xbd, 0x6a,
	0xe8, 0xc1, 0xef, 0x0d, 0x81, 0x2a, 0xb2, 0xf5, 0x46, 0x78, 0x89, 0x2b, 0xc5, 0x27, 0x70, 0xf1,
	0x33, 0x01, 0x07, 0x79, 0x8f, 0x98, 0xf5, 0x6b, 0x83, 0x6b, 0x1c, 0xba, 0xdb, 0x06, 0x4b, 0x53,
	0xa8, 0x64, 0xdd, 0xf8, 0x20, 0xaf, 0x7e, 0xb1, 0xc8, 0xbf, 0x27, 0xf1, 0xe5, 0xe9, 0x59, 0xf2,
	0xec, 0x14, 0xb7, 0x95, 0x7a, 0x1e, 0xa3, 0x9f, 0x4b, 0x4b, 0x3c, 0x0f, 0x32, 0xb9, 0x29, 0x0a,
	0x59, 0x3f, 0xfe, 0x0a, 0x6b, 0xb9, 0x09, 0x79, 0x9c, 0xa0, 0xb5, 0xb2, 0x5f, 0xa1, 0x40, 0x25,
	0x2d, 0x1e, 0xf6, 0x34, 0xa5, 0x20, 0xed, 0xee, 0x89, 0x10, 0xbe, 0xe7, 0x7f, 0xa9, 0xb1, 0x82,
	0xe0, 0x01, 0x9c, 0xe0, 0x96, 0x41, 0xfb, 0xbd, 0xb0, 0x14, 0xec, 0x38, 0x71, 0xbb, 0x9b, 0x0a,
	0x15, 0x67, 0x47, 0x70, 0x64, 0xe2, 0x3c, 0x5d, 0x97, 0x68, 0x13, 0xbc, 0x0d, 0x3e, 0xde, 0x7f,
	0xbd, 0x7e, 0x2e, 0x15, 0x0c, 0x6f, 0x3f, 0xca, 0x3b, 0x66, 0x23, 0xa3, 0xbb, 0xe9, 0x94, 0xc5,
	0x6e, 0x7b, 0x43, 0x13, 0xc5, 0x44, 0x21, 0xc7, 0x5a, 0xb9, 0xc8, 0xdf, 0x7d, 0xb7, 0x7e, 0x0d,
	0x68, 0x40, 0x58, 0xdf, 0x90, 0x14, 0x6a, 0x00, 0x1a, 0xfe, 0xae, 0x24, 0x44, 0x9f, 0xe2, 0x55,
	0x42, 0xad, 0x8f, 0xf5, 0xbb, 0x32, 0xec, 0x30, 0x1a, 0x53, 0xc2, 0x85, 0x92, 0xef, 0x77, 0x79,
	0x91, 0x85, 0x20, 0xeb, 0xcb, 0xf2, 0xc5, 0x05, 0xc7, 0x0a, 0xec, 0xd0, 0x00, 0xa3, 0x16, 0xf9,
	0x95, 0xfb, 0x0f, 0x3d, 0xb4, 0xb4, 0xd6, 0xdb, 0x14, 0x90, 0x6f, 0x5c, 0xd6, 0xd3, 0x4d, 0x10,
	0x7f, 0x24, 0x9b, 0x77, 0x3e, 0x4d, 0x17, 0x60, 0x34, 0x41, 0x51, 0x61, 0x53, 0xa9, 0x49, 0x13,
	0x67, 0x01, 0x87, 0xf8, 0xc8, 0x27, 0x11, 0x22, 0x67, 0x41, 0xa5, 0x66, 0x04, 0x1d, 0x48, 0x1f,
	0x25, 0xe0, 0xb7, 0x22, 0x7c, 0xb6, 0xfb, 0x52, 0xb8, 0xe2, 0xa1, 0x8d, 0x50, 0xda, 0x61, 0x0b,
	0xf3, 0x27, 0x94, 0x27, 0xa8, 0xc9, 0x64, 0x55, 0x0e, 0xa5, 0xa1, 0xe5, 0xaa, 0x90, 0x92, 0x6e,
	0x56, 0xf6, 0xad, 0xe0, 0xd5, 0x9d, 0xb4, 0x76, 0x11, 0x8c, 0x79, 0xf1, 0x44, 0x4b, 0x36, 0xac,
	0x1d, 0x4c, 0xd3, 0xae, 0xe5, 0xa1, 0x72, 0x64, 0x44, 0xef, 0xf1, 0xb6, 0xda, 0x01, 0x19, 0xe6,
	0x3c, 0x79, 0x24, 0x9b, 0x6a, 0x4f, 0x37, 0xc7, 0xed, 0x93, 0x33, 0x0a, 0x83, 0x6f, 0x6f, 0x47,
	0x8f, 0x1e, 0xff, 0xcf, 0xa0, 0x74, 0x9a, 0xe6, 0x9b, 0x0f, 0xd3, 0xec, 0x11, 0xc3, 0x7b, 0x3a,
	0xfd, 0x72, 0xf7, 0x26, 0x7a, 0x81, 0xe6, 0xe9, 0x4f, 0x45, 0xf7, 0x53, 0x97, 0xfb, 0xdc, 0x84,
	0x53, 0x91, 0xf2, 0xb6, 0xc9, 0x3b, 0xfd, 0x61, 0xe4, 0x17, 0x14, 0xcb, 0x7d, 0x28, 0xb9, 0xde,
	0xba, 0x45, 0x70, 0x47, 0xb5, 0xff, 0x35, 0x85, 0x5e, 0x35, 0x51, 0x60, 0x56, 0xcc, 0x42, 0x81,
	0xd5, 0x07, 0xa8, 0xa6, 0x16, 0x15, 0x6e, 0x63, 0x01, 0x13, 0x13, 0x70, 0xed, 0x00, 0x37, 0x8e,
	0xf8, 0x7b, 0x18, 0x1c, 0xbe, 0x5b, 0x9e, 0x4c, 0x95, 0x6f, 0x69, 0x3f, 0x3c, 0xcd, 0xc0, 0xd5,
	0xb9, 0x86, 0x28, 0x30, 0x04, 0x1a, 0x74, 0x0a, 0x24, 0x72, 0xf1, 0xfd, 0x2a, 0xfd, 0xd4, 0x23,
	0x74, 0xc4, 0xfb, 0xc3, 0x38, 0x74, 0xfd, 0xa3, 0x0d, 0x4d, 0xc5, 0xed, 0xb1, 0x1b, 0x4a, 0xa0,
	0x66, 0xb2, 0xa6, 0x8b, 0x86, 0x18, 0x57, 0x9e, 0xea, 0x06, 0xf9, 0x84, 0x5e, 0x17, 0x2f, 0xb2,
	0xd7, 0xb1, 0x6a, 0x90, 0x68, 0x0f, 0x38, 0xe1, 0x98, 0x05, 0x80, 0x5f, 0x39, 0x73, 0x4f, 0xe3,
	0xb1, 0xdf, 0x30, 0xcf, 0x1d, 0x40, 0xee, 0xc6, 0xcc, 0x8a, 0x24, 0x5b, 0x0d, 0x05, 0x56, 0x86,
	0x91, 0xc6, 0x38, 0x17, 0x02, 0x66, 0x55, 0x1f, 0xa2, 0x50, 0x1f, 0xf5, 0x9e, 0x5c, 0xea, 0xe3,
	0x1f, 0x5b, 0x0d, 0xb5, 0x2f, 0xc9, 0x7d, 0xf3, 0x19, 0xdc, 0xf7, 0x9a, 0xd6, 0xa2, 0x72, 0x9d,
	0xd7, 0x10, 0xb9, 0xde, 0xcf, 0xd9, 0xa8, 0xed, 0x25, 0xc4, 0x19, 0x58, 0xc3, 0xf6, 0x57, 0x22,
	0xba, 0x8b, 0x30, 0xac, 0x90, 0x01, 0x59, 0xba, 0x8e, 0xd8, 0x80, 0xca, 0x2e, 0x50, 0xdf, 0xaf,
	0x80, 0x7f, 0xcf, 0x20, 0x9a, 0x3b, 0x00, 0xfa, 0xd5, 0x59, 0x86, 0x9a, 0xca, 0xa2, 0xad, 0x8d,
	0xaa, 0x5b, 0x79, 0x95, 0x7e, 0x24, 0xb0, 0x8e, 0xf5, 0x1e, 0x50, 0x87, 0x87, 0xb8, 0xd3, 0x47,
	0x65, 0x07, 0xda, 0x49, 0x98, 0x4b, 0xde, 0xd8, 0x49, 0xb1, 0x83, 0x0f, 0x82, 0x28, 0x9a, 0x6e,
	0xd4, 0x7d, 0x72, 0x23, 0xf5, 0xb6, 0x05, 0x19, 0xc7, 0xb0, 0x85, 0xa5, 0x3c, 0x39, 0x12, 0x25,
	0xd5, 0xc5, 0x93, 0x72, 0x5d, 0x2e, 0xc7, 0x9a, 0x30, 0xdd, 0x9d, 0x1b, 0x48, 0xd8, 0x1f, 0x1d,
	0x61, 0xdb, 0xbd, 0xb9, 0xe5, 0xd9, 0x86, 0x78, 0x43, 0x4f, 0x0b, 0x0c, 0xb2, 0x16, 0x95, 0x10,
	0xdc, 0x9c, 0x13, 0x05, 0xd0, 0xe4, 0xaf, 0x7d, 0xff, 0xb6, 0x0e, 0x25, 0x4e, 0xa7, 0x34, 0x83,
	0xce, 0x88, 0x0a, 0x1f, 0x32, 0xc6, 0x41, 0xb7, 0xaf, 0x2c, 0xb9, 0xdb, 0xde, 0x4d, 0x19, 0x99,
	0x46, 0xd5, 0x5e, 0x5f, 0xe8, 0x82, 0x48, 0x78, 0x4c, 0x50, 0xc6, 0x0f, 0x62, 0x41, 0xc4, 0x37,
	0xc0, 0xec, 0x37, 0x8d, 0x14, 0x8c, 0x5f, 0x21, 0x6d, 0x28, 0xd9, 0x14, 0x12, 0xc8, 0x94, 0xb0,
	0xea, 0xa7, 0x2e, 0x64, 0x5f, 0xe7, 0xb2, 0xd2, 0x36, 0x8b, 0x63, 0x0d, 0xb8, 0x1b, 0x1f, 0xc8,
	0xe1, 0x2f, 0x9e, 0x0f, 0xcd, 0x19, 0x5c, 0xa0, 0x2d, 0xd4, 0x8c, 0xaf, 0x3b, 0x1e, 0xda, 0x85,
	0x4f, 0x60, 0x47, 0x24, 0x4b, 0xa1, 0x54, 0x11, 0x4d, 0x61, 0x25, 0x2a, 0xb0, 0xd5, 0xa1, 0xab,
	0xf8, 0xa9, 0x32, 0x6d, 0xb0, 0x58, 0x79, 0x45, 0x36, 0x43, 0x63, 0x62, 0xa8, 0xb2, 0xa9, 0xd5,
	0x45, 0xca, 0x78, 0x1f, 0x7c, 0x29, 0x09, 0x7a, 0x70, 0x2c, 0x85, 0x4a, 0x6d, 0xa8, 0xf7, 0x4a,
	0xfd, 0x8f, 0x3f, 0x89, 0xf4, 0x0f, 0xd9, 0x0f, 0xc5, 0xec, 0x28, 0x5e, 0xe8, 0xfc, 0x5b, 0x08,
	0x72, 0xfd, 0xef, 0x0a, 0x02, 0xed, 0x74, 0xd7, 0x2c, 0x0b, 0x27, 0x8a, 0x5a, 0xf6, 0xe9, 0x00,
	0xbd, 0x65, 0x42, 0x42, 0x40, 0x49, 0x4f, 0x88, 0x49, 0x6f, 0x3c, 0x8c, 0xe0, 0x6b, 0x2f, 0xbe,
	0x5c, 0x2c, 0x3f, 0x84, 0x65, 0xd9, 0x49, 0x12, 0xfa, 0xa8, 0x31, 0xae, 0x58, 0x84, 0x74, 0xff,
	0x5f, 0x5d, 0x83, 0x32, 0x13, 0x9a, 0x18, 0x73, 0x39, 0x61, 0xf8, 0xe1, 0xd6, 0x6a, 0x37, 0x03,
	0xaf, 0x4d, 0x3c, 0x60, 0x02, 0xe0, 0xfb, 0x9c, 0x0b, 0x39, 0xed, 0x5d, 0xbf, 0xd6, 0xe6, 0x47,
	0x2c, 0x23, 0x9d, 0x97, 0x2a, 0x5c, 0x2b, 0x39, 0x05, 0x6f, 0x7f, 0x71, 0xf3, 0x11, 0x96, 0xa4,
	0x78, 0x86, 0x69, 0x70, 0xe6, 0x91, 0xa9, 0xe4, 0xd5, 0xd0, 0x48, 0xf9, 0x98, 0x10, 0x70, 0x47,
	0x30, 0xfc, 0x28, 0x7b, 0x4e, 0x71, 0x6f, 0xed, 0xc8, 0x53, 0x39, 0x8c, 0xb0, 0x85, 0xf3, 0xdc,
	0xb9, 0x32, 0x79, 0xb2, 0xb3, 0xd8, 0xc1, 0x72, 0xcf, 0x11, 0x27, 0xaa, 0xd6, 0x74, 0x8c, 0x1c,
	0x33, 0xd7, 0xe5, 0x0f, 0x4f, 0x5e, 0x1f, 0x84, 0xd4, 0xa7, 0x1a, 0x8f, 0x33, 0x2e, 0x52, 0xc8,
	0x51, 0x23, 0xcc, 0x62, 0xa3, 0x3c, 0x91, 0x41, 0x82, 0x01, 0x94, 0x1a, 0x66, 0xf6, 0xf3, 0xda,
	0xe2, 0x8e, 0xfa, 0x16, 0x49, 0xbf, 0xff, 0x3e, 0x82, 0xbf, 0x47, 0xe1, 0x83, 0xc6, 0xff, 0x17,
	0xe0, 0x33, 0xe4, 0xe8, 0xbe, 0x0e, 0x93, 0x26, 0x41, 0x85, 0x2a, 0x02, 0x1a, 0xeb, 0x27, 0xca,
	0xcd, 0x0c, 0xdc, 0xad, 0x1f, 0xd3, 0x42, 0xf7, 0xf0, 0xce, 0x49, 0xce, 0xde, 0xd9, 0x90, 0x37,
	0x41, 0x17, 0xbf, 0xaa, 0x42, 0x6c, 0x7c, 0x87, 0x3c, 0x09, 0xe7, 0x02, 0x79, 0xf5, 0x57, 0x86,
	0x6c, 0x81, 0xe0, 0x2c, 0xfc, 0xf4, 0xa1, 0xb4, 0x0d, 0x16, 0x85, 0xa8, 0xf1, 0x6f, 0x4f, 0xfa,
	0x91, 0x1d, 0xf0, 0x14, 0x10, 0xdd, 0x2e, 0xaa, 0xfd, 0xe8, 0xd0, 0x69, 0xd9, 0x1e, 0x3e, 0x3f,
	0xfd, 0xe8, 0x40, 0xaa, 0x02, 0xa6, 0xf6, 0x0a, 0x33, 0xf9, 0xa5, 0x97, 0x50, 0xd6, 0xef, 0x35,
	0x1b, 0x61, 0xfe, 0x7e, 0x54, 0x9d, 0x33, 0x02, 0xf1, 0x02, 0xc3, 0x25, 0x87, 0x76, 0x2e, 0xfe,
	0x22, 0x34, 0x09, 0xdb, 0xf2, 0x5a, 0xb7, 0xf9, 0xc6, 0xb5, 0x02, 0x2c, 0x50, 0xaa, 0xbc, 0xfe,
	0xef, 0x69, 0xa9, 0xce, 0x52, 0xac, 0xd3, 0x3f, 0x87, 0xac, 0x53, 0x4a, 0x4e, 0x29, 0xf4, 0x78,
	0x58, 0x01, 0xd9, 0x96, 0x60, 0x5a, 0x01, 0xd8, 0x55, 0x0b, 0x7e, 0x6e, 0xd6, 0x5e, 0xb2, 0xeb,
	0x10, 0x0c, 0x3f, 0x1b, 0xf7, 0xe4, 0x19, 0xce, 0x62, 0x15, 0x15, 0xf2, 0xbc, 0x0b, 0x2f, 0xac,
	0xaf, 0xf2, 0xf7, 0x46, 0x35, 0xa6, 0x56, 0x6e, 0x1d, 0x3a, 0xaf, 0x63, 0xe8, 0x4f, 0xb0, 0x11,
	0x68, 0x76, 0xfc, 0xb9, 0x91, 0x0c, 0xbd, 0x69, 0x7b, 0x9e, 0x04, 0x97, 0x0f, 0xfd, 0xd1, 0x18,
	0x74, 0x52, 0x90, 0x5c, 0xf0, 0x27, 0x05, 0x4e, 0x80, 0x50, 0xc0, 0x84, 0x93, 0x67, 0xb2, 0xab,
	0xce, 0xc0, 0x42, 0x68, 0xbd, 0x2b, 0x7a, 0xec, 0xc4, 0x83, 0xc3, 0x16, 0x64, 0xd3, 0x0f, 0x3d,
	0x5e, 0x25, 0x7b, 0x2e, 0x1f, 0x7a, 0xa0, 0x41, 0xaa, 0xf5, 0x0c, 0x8b, 0x62, 0xd1, 0xee, 0x5c,
	0xc9, 0xf7, 0xc2, 0x41, 0xfa, 0xea, 0x44, 0x68, 0x12, 0x27, 0xee, 0xcf, 0x6f, 0x5b, 0x9a, 0x38,
	0x29, 0xaf, 0xad, 0x6f, 0xf3, 0x2c, 0x66, 0x8f, 0xe8, 0xb2, 0x74, 0x22, 0x27, 0x51, 0xa5, 0x83,
	0x94, 0xfa, 0xf6, 0xe7, 0x33, 0xb7, 0xf3, 0x9d, 0x53, 0xb4, 0x8b, 0x13, 0x40, 0x39, 0x80, 0xa1,
	0xeb, 0x83, 0xf2, 0xa7, 0x7d, 0x6d, 0x80, 0xf6, 0xf9, 0x52, 0x80, 0x1f, 0x03, 0x18, 0x61, 0x3c,
	0x41, 0x98, 0x71, 0xbd, 0xdb, 0x73, 0x49, 0x8c, 0x18, 0x1c, 0x33, 0x80, 0x7f, 0x9e, 0x71, 0xb2,
	0xff, 0x6f, 0xec, 0x43, 0x2e, 0x76, 0x51, 0xc6, 0x22, 0x56, 0xad, 0x23, 0xb5, 0xc5, 0xed, 0x44,
	0x20, 0x04, 0xf3, 0x3d, 0x93, 0x2e, 0xfa, 0x9b, 0xd8, 0xee, 0xd1, 0x28, 0xed, 0x5a, 0x6a, 0x97,
	0x63, 0x19, 0x5f, 0xe7, 0x7c, 0x47, 0x2a, 0xc8, 0x3a, 0x34, 0x1d, 0x4d, 0x8d, 0xb4, 0xe7, 0x88,
	0x57, 0xa1, 0xb4, 0xc7, 0x4d, 0x15, 0x81, 0x5d, 0x40, 0x75, 0x6b, 0x03, 0xb4, 0xd2, 0x6c, 0x1a,
	0xed, 0x97, 0xdf, 0x62, 0x51, 0xe0, 0x08, 0x39, 0xd3, 0x55, 0x3f, 0x6b, 0x39, 0x68, 0x5f, 0x9d,
	0x46, 0x10, 0x60, 0xcf, 0x54, 0x6b, 0x42, 0x0a, 0x1e, 0x10, 0xc5, 0xa5, 0x53, 0x83, 0xb5, 0xf7,
	0x71, 0xea, 0x62, 0xc1, 0xc1, 0x3b, 0x9c, 0xde, 0xb0, 0xd1, 0xf2, 0x94, 0x98, 0x4e, 0xcb, 0x83,
	0x21, 0x45, 0xf8, 0xb7, 0xf7, 0x3c, 0xb4, 0x53, 0xac, 0x6f, 0x28, 0x78, 0x52, 0x84, 0xe8, 0x32,
	0x6d, 0x59, 0x10, 0x99, 0xfa, 0x89, 0x21, 0x44, 0x0a, 0x8f, 0x13, 0x61, 0x05, 0xe1, 0x38, 0xcf,
	0x5a, 0x6e, 0x26, 0x7f, 0x8f, 0x5c, 0x20, 0xf8, 0xf6, 0x30, 0xcf, 0xdc, 0xa6, 0xdf, 0x01, 0x0d,
	0x94, 0x2e, 0x14, 0x77, 0x2e, 0xd2, 0xb6, 0x7b, 0xbd, 0x1c, 0xf5, 0x64, 0xfb, 0x95, 0x89, 0x3d,
	0x5e, 0x9a, 0x7c, 0xd0, 0xe6, 0xbb, 0x3d, 0xfe, 0x06, 0xe8, 0x8b, 0x6f, 0x7d, 0x58, 0xeb, 0x55,
	0x1e, 0x80, 0x1a, 0x5b, 0x0c, 0xab, 0x88, 0xe4, 0x1a, 0x25, 0xd9, 0xe4, 0xd0, 0xd3, 0xbd, 0x84,
	0xcf, 0xf1, 0xa5, 0xc4, 0xac, 0x6c, 0xa0, 0xd1, 0x60, 0xcf, 0x2b, 0xc0, 0x51, 0xb0, 0x86, 0xb2,
	0x7a, 0x54, 0xcc, 0x6b, 0x0a, 0x86, 0x60, 0x96, 0x70, 0xbf, 0x12, 0x64, 0xd8, 0x63, 0x8f, 0x71,
	0x85, 0x2b, 0x29, 0x7b, 0xa5, 0x9f, 0x98, 0xc6, 0xa8, 0x14, 0xcd, 0xfa, 0xa2, 0x46, 0xef, 0x57,
	0xec, 0xa2, 0xab, 0x5d, 0x48, 0x59, 0x15, 0x9f, 0xbf, 0x12, 0x42, 0x98, 0xf9, 0x75, 0xc5, 0x66,
	0x15, 0x83, 0x1a, 0xc1, 0x20, 0x64, 0x3a, 0x23, 0x2f, 0x88, 0xdc, 0x7f, 0x49, 0x42, 0x26, 0xd1,
	0xc6, 0x04, 0xf0, 0xb2, 0xf5, 0xb0, 0x60, 0x02, 0x90, 0x52, 0x8a, 0xdd, 0xc6, 0xb6, 0xb6, 0xf9,
	0x63, 0x8b, 0x4e, 0xaf, 0x2c, 0x15, 0x5b, 0x46, 0x83, 0x7e, 0xc9, 0xe1, 0x90, 0x95, 0xc2, 0x5e,
	0x09, 0xdd, 0x84, 0xc4, 0x51, 0x52, 0x63, 0xfc, 0xf5, 0xaa, 0x6d, 0xec, 0xfd, 0xe2, 0x50, 0x0d,
	0xea, 0x97, 0x21, 0x97, 0xfc, 0x0a, 0xcb, 0x11, 0x7c, 0x8c, 0x33, 0x8b, 0x3f, 0xa0, 0x7b, 0x46,
	0xd8, 0x1a, 0x5f, 0x85, 0x92, 0x10, 0xa2, 0x15, 0x5f, 0x00, 0xab, 0xd3, 0xcf, 0x59, 0xc2, 0x05,
	0x9c, 0x8d, 0xd4, 0xf6, 0x79, 0xbb, 0xe7, 0x15, 0x30, 0x73, 0xb2, 0x90, 0x95, 0x62, 0xd6, 0xec,
	0xed, 0xf5, 0x79, 0xe4, 0x98, 0x1d, 0xa2, 0xc7, 0x99, 0x6c, 0xb1, 0x17, 0xb9, 0x8b, 0x42, 0x4a
	};

	int main() {
	  int i;
	  int f1, f2;
	  int bitlens[4] = { 224, 256, 384, 512 };
	  BitSequence Msg[MAX_MSGBITLEN/8];
	  hashState state;

	  FILE* fp_out;
	  fp_out = stdout;
	
	int cmpError = 0;
	  
	  for (i=0; i<MD_LENGTHS; i++) {
		int msgbitlen;
		int hashbitlen = bitlens[i];
		for (f1=0; f1<FRAG1_MAX; f1+=8) {
		   for (f2=0; f2<FRAG2_MAX; f2++) {
			msgbitlen = f1+f2;
			memset(Msg,0,MAX_MSGBITLEN/8);
			memcpy(Msg,M,(msgbitlen+7)/8);
			Msg[msgbitlen/8] &= ~(0xff >> (msgbitlen & 0x7));
		   
			BitSequence digest1[64];
			BitSequence digest2[64];
			Hash(hashbitlen, Msg, msgbitlen, digest1);
		 
			Init(&state, hashbitlen);
			Update(&state, Msg, f1);
			Update(&state, Msg+f1/8, f2);
			Final(&state, digest2);
		
			if (memcmp(digest1,digest2,hashbitlen/8) != 0) {
				cmpError++;
				fprintf(fp_out, "\nhashbitlen = %d\n", hashbitlen);
				fprintBstr(fp_out, "Msg = ", Msg, (msgbitlen+7)/8);
				fprintf(fp_out, "Len = %d\n", msgbitlen);
				fprintBstr(fp_out, "MD1 = ", digest1, hashbitlen/8);
				fprintf(fp_out, "Len1 = %d, Len2 = %d\n", f1, f2);
				fprintBstr(fp_out, "MD2 = ", digest2, hashbitlen/8);
			}
			
			if (cmpError==1024)
				goto final; // jump out of all the for loops and go to final 
		  }
		}
	  }
	final:
	  if (cmpError == 0)
		  fprintf(fp_out, "No Comparison Errors Found!\n");
	  else if (cmpError == 1024)
		  fprintf(fp_out, "Many Comparison Errors Found! Stopped after %d", cmpError);
	  else
		  fprintf(fp_out, "Total Comparison Errors Found: %d\n", cmpError);
	  
	  return 0; 
	}

	void
	fprintBstr(FILE *fp, char *S, BitSequence *A, int L)
	{
		int		i;

		fprintf(fp, "%s", S);

		for ( i=0; i<L; i++ )
			fprintf(fp, "%02X", A[i]);

		if ( L == 0 )
			fprintf(fp, "00");

		fprintf(fp, "\n");
	}
