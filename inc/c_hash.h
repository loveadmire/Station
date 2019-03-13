#ifndef C_HASH_H
#define C_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_NUM 1280

struct person
{
    char *name;
    int age;
};

struct store_person
{
    struct person *per;
    struct store_person *next;
    char *key;
    int nHashA;
    int nHashB;
};

struct store_person *stuff;
unsigned long cryptTable[0x500]; //1280

void prepareCryptTable();
unsigned long HashString( char *keystr, unsigned long dwHashType );
int GetHashTablePos( char *key, struct store_person *per);
void hash_set(char *keystr, struct person *per);
int hash_get(char *key, struct store_person *per);
int test_hash();

#endif // C_HASH_H

