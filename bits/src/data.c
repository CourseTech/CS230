#include <data.h>
#include <stdio.h>
#include <stdlib.h>

int convertCharToNumber(char ch) {
  if (ch >= '0' && ch <= '9') {
    return ch - '0';
  } else if (ch >= 'A' && ch <= 'F') {
    return ch - 'A' + 10;
  } else {
    return -1;
  }
}

char convertNumberToChar(int n) {
  if (n >= 0 && n <= 9) {
    return n + '0';
  } else if (n >= 10 && n <= 15) {
    return n - 10 + 'A';
  } else {
    return 0;
  }
}

/* void pop(Data src, unsigned char n) {
  int i = 1;
  DataNode* cur = src.data;
  while (i < src.len - n) {
    cur = cur->next;
    ++i;
  }
  DataNode* next = cur->next;
  cur->next = NULL;
  while (next != NULL) {
    cur = next;
    next = next->next;
    free(cur);
  }
}

void append(Data src, unsigned char n) {
  DataNode* cur = src.data;
  while (cur != NULL && cur->next != NULL) {
    cur = cur->next;
  }
  for (int i = 0; i < n; ++i) {
    cur->next = (DataNode*)malloc(sizeof(DataNode));
    cur = cur->next;
    cur->number = '0';
  }
} */

unsigned char getBit(unsigned int x, int size, int i)
{
    return (x << (size - 1 - i)) >> (size - 1);
}

unsigned int setBit(int x, unsigned char new, unsigned char i)
{
    int mask = 1 << i;
    return ((new << i) & mask) | (x & ~mask);
}

unsigned int setBits(int x, int new, unsigned char l, unsigned char r)
{
    int mask = ((1 << (l - r + 1)) - 1) << r;
    return ((new << r) & mask) | (x & ~mask);
}

unsigned int setBitsToOne(int x, unsigned char l, unsigned char r)
{
    int mask = ((1 << (l - r + 1)) - 1) << r;
    return x | mask;
}

unsigned int convert_to_unsigned_int(Data src)
{
    unsigned int power = 1;
    for (int i = 1; i < src.len; ++i)
    {
        power *= src.base;
    }
    unsigned int number = 0;
    DataNode *cur = src.data;
    while (cur != NULL)
    {
        number += power * convertCharToNumber(cur->number);
        power /= src.base;
        cur = cur->next;
    }
    return number;
}

int convert_to_int(Data src)
{
    unsigned int number = convert_to_unsigned_int(src);
    unsigned char leading_bit = getBit(number, 32, src.number_bits - 1);
    if (src.sign == 0 || leading_bit == 0)
    {
        return number;
    }
    else
    {
        int result = (-1 << (src.number_bits - 1));
        result += setBit(number, 0, src.number_bits - 1);
        return result;
    }
}

DataNode *buildDataList(unsigned int number, int power, unsigned char base)
{
    if (power <= 1)
    {
        DataNode *ptr = (DataNode *)malloc(sizeof(DataNode));
        DataNode data = {convertNumberToChar(number), NULL};
        *ptr = data;
        return ptr;
    }
    else
    {
        DataNode *cur = (DataNode *)malloc(sizeof(DataNode));
        cur->number = convertNumberToChar(number / power);
        cur->next = buildDataList(number % power, power / base, base);
        return cur;
    }
}

unsigned char getLen(DataNode *data)
{
    DataNode *cur = data;
    unsigned char len = 0;
    while (cur != NULL)
    {
        ++len;
        cur = cur->next;
    }
    return len;
}

Data convert_to_data(unsigned int number, unsigned char base, unsigned char sign, unsigned char number_bits)
{
    Data data = {base, sign, number_bits, 0, NULL};
    unsigned int power = 1;
    while (power * base <= number)
    {
        power *= base;
    }
    data.data = buildDataList(number, power, base);
    if (data.len == 0)
    {
        data.len = getLen(data.data);
    }
    return data;
}

Data convert_to_base_n(Data src, unsigned char n)
{
    return convert_to_data(convert_to_unsigned_int(src), n, src.sign, src.number_bits);
}

Data right_shift(Data src, int n)
{
    if (n <= 0)
    {
        return src;
    }
    Data b2;
    if (src.base != 2)
    {
        b2 = convert_to_base_n(src, 2);
    }
    else
    {
        b2 = src;
    }
    unsigned int number = convert_to_unsigned_int(b2);
    Data new_data;
    unsigned char leading_bit = getBit(number, 32, src.number_bits - 1);
    if (src.sign == 0 || leading_bit == 0)
    {
        new_data = convert_to_data(number >> n, b2.base, b2.sign, b2.number_bits);
        new_data.len = getLen(new_data.data);
    }
    else
    {
        number = setBitsToOne(number >> n, b2.number_bits - 1, b2.number_bits - n);
        new_data = convert_to_data(number, b2.base, b2.sign, b2.number_bits);
        new_data.len = getLen(new_data.data);
    }
    return new_data;
}

Data left_shift(Data src, int n)
{
    if (n <= 0)
    {
        return src;
    }
    Data b2;
    if (src.base != 2)
    {
        b2 = convert_to_base_n(src, 2);
    }
    else
    {
        b2 = src;
    }
    Data new_data;
    unsigned int number = setBits(convert_to_unsigned_int(b2) << n, 0, src.number_bits + n - 1, src.number_bits);
    new_data = convert_to_data(number, b2.base, b2.sign, b2.number_bits);
    return new_data;
}

void printDataList(DataNode *data, unsigned char base)
{
    printf("base %i: ", base);
    DataNode *cur = data;
    while (cur != NULL)
    {
        printf("%c", cur->number);
        cur = cur->next;
    }
    printf("\n");
}

void printData(Data *data)
{
    printf("base: %i\n", data->base);
    printf("sign: %i\n", data->sign);
    printf("bits: %i\n", data->number_bits);
    printf("len: %i\n", data->len);
    printf("number: %i\n", convert_to_int(*data));
    printDataList(data->data, data->base);
    printDataList(data->data, 10);
    printf("\n");
}
