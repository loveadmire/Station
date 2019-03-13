//
// Created by root on 19-3-5.
//

#ifndef STATION_HAND_PFX_H
#define STATION_HAND_PFX_H

#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/pkcs12.h>
#include <curl/curl.h>

typedef int bool;

#define CONFILE "./config"

bool PraseP12Cert(char *p12Cert,char *p12pass,
                  char *pubCertFile,char *priCertFile,int format_pem);
int mite_string_2_base64_block(char *input,int input_len,char **base64);
char *Test_Encode(char *source,int len);

int get_key_string(char *file, char *key, char *key_val);

#endif //STATION_HAND_PFX_H
