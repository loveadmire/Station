//
// Created by root on 1/29/19.
//

#include <string.h>
#include <openssl/evp.h>

void show_hex(unsigned char *s,int len){
    int i = 0;
    printf("\n");
    for(i = 0;i<len;i++){
        if(i%16 == 0) printf("\n");
        printf("%02x ",s[i]);
    }
    printf("\n");

}

int ssl_evp_demo(){
    int ret,which=1;
    EVP_CIPHER_CTX *ctx;
    const EVP_CIPHER *cipher;
    unsigned char key[24],iv[8],in[100],out[108],de[100];
    int i,len,inl,outl,total=0;
    for(i=0;i<24;i++)
    {
        memset(&key[i],i,1);
    }
    for(i=0;i<8;i++)
    {
        memset(&iv[i],i,1);
    }
    for(i=0;i<100;i++)
    {
        memset(&in[i],i+1,1);
    }
    EVP_CIPHER_CTX_init(ctx);
    printf("please select :\n");
    printf("1: EVP_des_ede3_ofb\n");
    printf("2: EVP_des_ede3_cbc\n");
    scanf("%d",&which);
    if(which==1)
        cipher=EVP_des_ede3_ofb();
    else
        cipher=EVP_des_ede3_cbc();
    printf("原文：\n");
    show_hex(in, 100);
    ret=EVP_EncryptInit_ex(ctx,cipher,NULL,key,iv);
    if(ret!=1)
    {
        printf("EVP_EncryptInit_ex err1!\n");
        return -1;
    }
    inl=50;
    len=0;
    EVP_EncryptUpdate(ctx,out+len,&outl,in,inl);
    len+=outl;
    EVP_EncryptUpdate(ctx,out+len,&outl,in+50,inl);
    len+=outl;
    EVP_EncryptFinal_ex(ctx,out+len,&outl);
    len+=outl;
    printf("加密结果长度： %d\n",len);
    printf("加密结果：\n");
    show_hex(out, len);
    EVP_CIPHER_CTX_cleanup(ctx);

    /* 解密 */
    EVP_CIPHER_CTX_init(ctx);
    ret=EVP_DecryptInit_ex(ctx,cipher,NULL,key,iv);
    if(ret!=1)
    {
        printf("EVP_DecryptInit_ex err1!\n");
        return -1;
    }
    total=0;
    EVP_DecryptUpdate(ctx,de+total,&outl,out,44);
    total+=outl;
    EVP_DecryptUpdate(ctx,de+total,&outl,out+44,len-44);
    total+=outl;
    ret=EVP_DecryptFinal_ex(ctx,de+total,&outl);
    total+=outl;
    printf("解密结果长度： %d\n",total);
    printf("解密结果：\n");
    show_hex(de, total);
    if(ret!=1)
    {
        EVP_CIPHER_CTX_cleanup(ctx);
        printf("EVP_DecryptFinal_ex err\n");
        return -1;
    }
    if((total!=100) || (memcmp(de,in,100)))
    {
        printf("err!\n");
        return -1;
    }
    EVP_CIPHER_CTX_cleanup(ctx);
    printf("test ok!\n");
    return 0;

}