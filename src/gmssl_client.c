//
// Created by root on 1/10/19.
//
#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/ssl.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "gmssl_debug.h"

#define SERVER_IP      "127.0.0.1"
#define SERVER_PORT    4433

#define CERTSERVER "./pem/eccsignuser.pem"
#define KEYSERVER  "./pem/eccsignuserkey.pem"
#define SM2_SERVER_ENC_CERT     "./pem/eccencuser.pem"
#define SM2_SERVER_ENC_KEY      "./pem/eccencuserkey.pem"


#define CHK_ERR(err, s) if((err) == -1) { perror(s); return -1; }else printf("%s  success!\n",s);
#define CHK_RV(rv, s) if((rv) != 1) { printf("%s error\n", s); return -1; }else printf("%s  success!\n",s);
#define CHK_NULL(x, s) if((x) == NULL) { printf("%s error\n", s); return -1; }else printf("%s  success!\n",s);
#define CHK_SSL(err, s) if((err) == -1) { ERR_print_errors_fp(stderr);  return -1;}else printf("%s  success!\n",s);

int gmssl_client_demo(unsigned short  port, char* ip ) {
    int ret;
    SSL_CTX* ctx = NULL;
    SSL_METHOD *meth;
    OpenSSL_add_ssl_algorithms();
    SSL_load_error_strings();
    //meth = (SSL_METHOD *)TLS_client_method();
    meth = (SSL_METHOD *)GMTLS_client_method();
    ctx = SSL_CTX_new (meth);
    CHK_NULL(ctx,"SSL_CTX_new");
    int rv,err;
    rv = SSL_CTX_use_certificate_file(ctx,CERTSERVER,SSL_FILETYPE_PEM);
    CHK_ERR(rv,"SSL_CTX_use_certificate_file");
    rv = SSL_CTX_use_PrivateKey_file(ctx,KEYSERVER,SSL_FILETYPE_PEM);
    CHK_RV(rv,"SSL_F_SSL_CTX_USE_PRIVATEKEY_file");
    rv = SSL_CTX_check_private_key(ctx);
    CHK_RV(rv,"ssl_ctx_check_private_key");
    rv = SSL_CTX_use_certificate_file(ctx,SM2_SERVER_ENC_CERT,SSL_FILETYPE_PEM);
    CHK_RV(rv,"SSL_CTX_use_certificate_file2");
    rv = SSL_CTX_use_PrivateKey_file(ctx,SM2_SERVER_ENC_KEY,SSL_FILETYPE_PEM);
    CHK_RV(rv,"SSL_CTX_use_private_key_file2");

    int client_socket;
    struct sockaddr_in addr_server;
    client_socket = socket (AF_INET, SOCK_STREAM, 0);
    if( client_socket == -1  ) {
        printf("socke error \n");
        return -1;
    }

    memset (&addr_server, 0, sizeof(addr_server));
    addr_server.sin_family           = AF_INET;
    addr_server.sin_addr.s_addr = inet_addr(ip);
    addr_server.sin_port             = htons (port);
    ret = connect(client_socket, (struct sockaddr*) &addr_server, sizeof(addr_server));
    if( ret == -1  ) {
       printf("connect error ret is %d .\n",ret);
        return -1;
    }
    SSL*     ssl;
    X509*    server_certificate;
    char*    str;  ssl = SSL_new (ctx);
    if( ssl == NULL ) {
        printf("ssl new_errr \n");
        return -1;
    }
    SSL_set_fd (ssl, client_socket);
    ret = SSL_connect (ssl);
    if( ret == -1 ) {
      printf("ssl_connection error \n");
        ERR_print_errors_fp(stderr);
        return -1;
    }
    ERR_print_errors_fp(stderr);
   printf("SSL connection using: %s  \n",SSL_get_cipher(ssl));
    server_certificate = SSL_get_peer_certificate (ssl);
    if( server_certificate != NULL ) {
        printf("Server certificate \n");
        str = X509_NAME_oneline (X509_get_subject_name (server_certificate),0,0);
        if( str == NULL ) {
            printf("X509_NAME_ONLINE error\n");
        }else{
            printf("subject is %s \n",str);
            OPENSSL_free(str);
        }
        str = X509_NAME_oneline (X509_get_issuer_name  (server_certificate),0,0);
        if( str == NULL ) {
            printf(" X509_NAME_ONLINE error \n");
        }else{
            printf("issuer : %s \n",str);
            OPENSSL_free(str);
        }
        X509_free(server_certificate);
    }else{
        printf("Server does not have certificate. we sould Esc! \n");
    }
    char buf[1024];
    ret = SSL_write (ssl, "Hello World!", strlen("Hello World!"));
    if( ret == -1 ) {
        printf("ssl_write error \n");
        return -1;
    }
    ret = SSL_read (ssl, buf, sizeof(buf) - 1);
    if( ret == -1 ) {
       printf("ssl_read error \n");
        return -1;
    }
    buf[ret] = '\0';
    printf("buf is %s \n",buf);
    SSL_shutdown(ssl);
    close(client_socket);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    return 0;

}