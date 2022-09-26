#ifndef __DATA
#define __DATA

typedef struct DataNode DataNode;
typedef struct Data Data;

// Represents a node in the linked list.
// '0' ~ '9' or 'A' ~ 'F'

struct DataNode {
  unsigned char number;
  DataNode *next;
};

// Represents data in base 2 - 16.
// indicate what base this data is
// indicate whether this data is signed or unsigned
// number. 0 means unsigned and 1 means signed.
// the number of bits can be used to represent
// this number in binary.
// The maximal number of bits is 32.
// length of linkedList
// represent data in LinkedList

struct Data {
  unsigned char base;
  unsigned char sign;
  unsigned char number_bits;
  unsigned char len;
  DataNode *data;
};

// return decimal number of ch in base 2 ~ 16
// return -1 if ch is not a valid number
int convertCharToNumber(char ch);

// return char representation of n in base 2 ~ 16
// return '\0' if n is not a number of 0 ~ 15
char convertNumberToChar(int n);

// Return a new Data that represents src in base n
Data convert_to_base_n(Data src, unsigned char n);

// convert data to an int
int convert_to_int(Data src);

// Return a new Data in base 2 that represents an
// application of binary operator left shift on src
// where shift is less than src.number_bits
Data left_shift(Data src, int shift);

// Return a new Data in base 2 that represents an
// application of binary operator right shift on
// src where shift is less than src.number_bits
Data right_shift(Data src, int shift);  

#endif
