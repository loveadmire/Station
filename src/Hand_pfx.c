//
// Created by root on 19-3-5.
//
#include "Hand_pfx.h"
#include <openssl/ssl.h>
#include <string.h>
#include "cJSON.h"
#include "cJSON_Utils.h"

#define false 0
#define true 1

#define OK                 0
#define OPENERR           -1
#define CONFIGNOEEXUIT    -2
#define READERR           -3
#define KEYNOTEXIT        -4

char *Test_Encode(char *source,int len){
    CURL *curl = curl_easy_init();
    char *cres = curl_easy_escape(curl,source,len);
    curl_easy_cleanup(curl);
    return cres;
}

int mite_string_2_base64_block(char *input,int input_len,char **base64)
{
    int len = 0;
    char *out = NULL;
    out = (char *)malloc((input_len*4/3+4));
    len = EVP_EncodeBlock((unsigned char *)out,(unsigned char *)input,input_len);
    *base64 = out;
    return len;
}
int get_key_string(char *file, char *key, char *key_val) {
    int file_size = 0;
    int ret = 0;
    char file_buff[1024] = { 0 };
    FILE *fp = NULL;
    if (file) {
        fp = fopen(file, "r");
        if (!fp) {
            return OPENERR;// 打开配置文件打开失败
        }
    }
    else {
        return CONFIGNOEEXUIT;// 配置文件不存在
    }
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);
    ret = fread(file_buff, 1, file_size, fp);
    if (ret <= 0) {
        fclose(fp);
        return READERR; //读取文件失败
    }
    fclose(fp);
    cJSON *json = cJSON_Parse(file_buff);
    if (cJSON_GetObjectItem(json, key)) {
        strcpy(key_val, cJSON_GetObjectItem(json, key)->valuestring);
        //key_val = key;
        //printf("key is %s key_val is %s ", key, key_val);
        cJSON_Delete(json);
        return OK;
    }
    else {
        cJSON_Delete(json);
        return KEYNOTEXIT;//获取键值失败
    }
}

//解析P12的证书 和私钥，并写入文件
bool PraseP12Cert(char *p12Cert,char *p12pass,
                  char *pubCertFile,char *priCertFile,int format_pem)
{
    if (p12Cert == NULL)
    {
        return false;
    }
    EVP_PKEY *key=NULL;
    X509 *cert=NULL;
    STACK_OF(X509) *ca = NULL;
    BIO * bio=NULL,*bioCert=NULL,*bioKey=NULL;
    PKCS12 *p12=NULL;
    int i,j,ret;

    OpenSSL_add_ssl_algorithms();
    bio=BIO_new_file(p12Cert, "r");
    p12 = d2i_PKCS12_bio(bio, NULL);
    if (!PKCS12_parse(p12,p12pass, &key, &cert, &ca))
    {
        BIO_free(bio);
        return false;
    }
    PKCS12_free(p12);
    char file_path[128] = "./";
    char client_file_name[128] = { 0 };
    char client_key_name[128] = { 0 };
//    ret = get_key_string(CONFILE, "vpnconf", file_path);
//    if (ret < 0) {
//        printf("Read config file path ERROR\n");
//        return false;
//    }
//    else {
//        sprintf(client_file_name, "%s%s", file_path, "client.crt");
//        sprintf(client_key_name, "%s%s", file_path, "client.key");
//    }
    sprintf(client_file_name, "%s%s", file_path, "client.crt");
    sprintf(client_key_name, "%s%s", file_path, "client.key");
    bioCert = BIO_new_file(client_file_name, "w");              //建立文件保存证书数据
    if (!bioCert) {
        printf(" 解析文件失败，查看用户读写权限！\n");
        return false;
    }
    bioKey = BIO_new_file(client_key_name, "w");
    if (!bioKey) {
        printf(" 解析文件失败，查看用户读写权限！\n");
        return false;
    }
    //bioKey = BIO_new_file("/usr/local/sslvpn/conf/client_p12.key", "w");           //建立文件保存本地私钥
    int  FORMAT_PEM = 1;
    int  FORMAT_DER = 0;
    char *base64_crt = NULL;
    char *buff = NULL;
    char *buff_key = NULL;
    int  base64_len = 0;
    char *base64_codec = NULL;
    int  filelen = 0;
    int  i_cert = 1;
    if(format_pem == FORMAT_PEM)
    {                                                                           //获取证书文件包含BEGIN END
        i = PEM_write_bio_X509(bioCert, cert);
        j = PEM_write_bio_PrivateKey(bioKey, key, NULL, NULL, 0, 0, NULL);
    }
    if (format_pem == FORMAT_DER)                          //获取证书文件不包BEGIN END
    {
        i = i2d_X509_bio(bioCert, cert);
        j = i2d_PrivateKey_bio(bioKey, key);
    }
    BIO_free(bio);
    BIO_free(bioCert);
    BIO_free(bioKey);
    X509_free(cert);
    EVP_PKEY_free(key);
    EVP_cleanup();
    if(!i_cert)
    {
        return false;
    }
    FILE *fp =  NULL;
    fp = fopen(client_file_name, "rb");
    if(fp == NULL )
    {
        printf("open file fail !");
        fclose(fp);
        return false;
    }
    else // 对证书文件进行base64编码
    {
        fseek(fp,0,SEEK_END);                //get file size
        filelen = ftell(fp);
        rewind(fp);
        buff = (char *)malloc(filelen + 1);  //save file data
        memset(buff,0,filelen+1);
        fread(buff,filelen,1,fp);
        base64_len = mite_string_2_base64_block(buff,filelen,&base64_crt);
        base64_codec = Test_Encode(base64_crt,base64_len);
        fclose(fp);
    }
    //将文件保存为base64编码文件用于上传验证
    fp = fopen(client_file_name,"wb");
    if(fp == NULL)
    {
        printf("Create file failure check authority!");
        fclose(fp);
        return false;
    }
    else
    {
       // fwrite(base64_codec,1,strlen(base64_codec), fp);  //寫入編碼以後的數據
        fwrite(buff,1,strlen(buff), fp);
        fclose(fp);
        free(buff);
    }
    //对密钥文件进行base64编码
    fp = fopen(client_key_name, "rb");
    if (fp == NULL) {
        printf("open file fail !");
        fclose(fp);
        return false;
    }
    else {
        fseek(fp, 0, SEEK_END);                //get file size
        filelen = ftell(fp);
        rewind(fp);
        buff_key = (char *)malloc(filelen + 1);  //save file data
        memset(buff_key, 0, filelen + 1);
        fread(buff_key, filelen, 1, fp);
        base64_len = mite_string_2_base64_block(buff_key, filelen, &base64_crt);
        base64_codec = Test_Encode(base64_crt, base64_len);
        fclose(fp);
        //保存client.key文件
        fp = fopen(client_key_name, "wb");
        if (fp == NULL) {
            printf("Create file failure check authority!");
            fclose(fp);
            return false;
        }
        else {
            //fwrite(base64_codec, 1, strlen(base64_codec), fp);
            fwrite(buff_key, 1, strlen(buff_key), fp);
            fclose(fp);
            free(buff_key);
        }
    }
    free(base64_crt);
    free(base64_codec);
    return true;
}