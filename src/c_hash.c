#include "c_hash.h"




void prepareCryptTable()
{
    unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;
    for( index1 = 0; index1 < 0x100; index1++ )
    {
        for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 )
        {
            unsigned long temp1, temp2;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB;
            temp2 = (seed & 0xFFFF);
            cryptTable[index2] = ( temp1 | temp2 );
        }
    }
}

unsigned long HashString( char *keystr, unsigned long dwHashType )
{
    unsigned char *key  = (unsigned char *)keystr;
    unsigned long seed1 = 0x7FED7FED;
    unsigned long seed2 = 0xEEEEEEEE;
    int ch;
    while(*key != 0)
    {
        ch = toupper(*key++);
        seed1 = cryptTable[(dwHashType << 8) + ch] ^ (seed1 + seed2);
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3;
    }
    return seed1;
}

// int GetHashTablePos(char *key, struct store_person *per )
// //lpszString要在Hash表中查找的字符串，per为存储字符串Hash值的Hash表。
// {
//  const int HASH_OFFSET = 0;
//     int nHash = HashString(key,HASH_OFFSET);  //调用上述函数二，返回要查找字符串lpszString的Hash值。
//     int nHashPos = nHash % MAX_NUM;
//     if ( !strcmp( per[nHashPos].key, key ) )
//      //如果找到的Hash值在表中存在，且要查找的字符串与表中对应位置的字符串相同
//     {
//         return nHashPos;    //则返回上述调用函数二后，找到的Hash值
//     }
//     else
//     {
//         return -1;
//     }
// }

int GetHashTablePos( char *key, struct store_person *per)
{
    const int  HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    int  nHash = HashString(key, HASH_OFFSET) % MAX_NUM;
    int  nHashA = HashString(key, HASH_A) % MAX_NUM;
    int  nHashB = HashString(key, HASH_B) % MAX_NUM;

    printf("nHash ==%d\n", nHash);
    while ( per[nHash].key )
    {
        /* 如果仅仅是判断在该表中时候存在这个字符串，就比较这两个hash值就可以了，不用对结构体中的字符串进行比较。这样会加快运行的速度？减少hash表占用的空间？这种
           方法一般应用在什么场合？*/
        if (per[nHash].nHashA == nHashA
            &&  per[nHash].nHashB == nHashB )
        {
            return nHash;
        }
        else
        {
            nHash = (nHash + 1) % MAX_NUM;
        }

    }
    return -1;
}

void hash_set(char *keystr, struct person *per)
{
    const int  HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
    int  nHash = HashString(keystr, HASH_OFFSET) % MAX_NUM;
    int  nHashA = HashString(keystr, HASH_A) % MAX_NUM;
    int  nHashB = HashString(keystr, HASH_B) % MAX_NUM;
    struct store_person *store = &stuff[nHash];
    if(store->key == NULL)
    {
        store->key = keystr;
        store->per = per;
        store->nHashA = nHashA;
        store->nHashB = nHashB;
       // printf("store ok ,position is %d\n", nHash);
        return;
    }
}
int hash_get(char *key, struct store_person *per)
{
    return GetHashTablePos( key, per);
}

//#include <openssl/ossl_typ.h>
//#include <../inc/crypto/evp/digest.c>
int main_hash()
{
//    EVP_MD_CTX *md_ctx = NULL;
//    md_ctx = EVP_MD_CTX_new();

    printf("test hash %s %s \n",__FUNCTION__,__FILE__);
    prepareCryptTable();
    stuff = (struct store_person*)malloc(sizeof(struct store_person) * MAX_NUM);
    if(stuff == NULL)
        return -1;
    struct person pvalue1;
    pvalue1.name = "haha";
    pvalue1.age = 18;
    hash_set("key1", &pvalue1);

    struct person pvalue2;
    pvalue2.name = "haha";
    pvalue2.age = 18;
    hash_set("key2",&pvalue2);

    struct person pvalue3;
    pvalue3.name = "lala";
    pvalue3.age = 12;
    hash_set("key3", &pvalue3);

    int pos = hash_get("key1", stuff);
    printf("key1 pos is %d\n", pos);
    pos = hash_get("key1", stuff);
    printf("pos is %d\n", pos);

    pos = hash_get("key2", stuff);
    printf("key2 pos is %d\n", pos);
    printf("person name ->%s, person age ->%d\n", stuff[pos].per->name, stuff[pos].per->age);
    return 0;
}

