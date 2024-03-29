#ifndef _MY_HTTP_H_
#define _MY_HTTP_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 通用头文件 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* field_name 为字段 */
struct field_name
{
    char* key;
    char* value;
    struct field_name* next;
};
/* httpMessage 为 http 报文 */
struct httpMessage
{
    char* method;
    char* url;
    char* version;
    struct field_name* pField;
};

void httpMessageInitalize(struct httpMessage* pMessage,unsigned int size,const char* method,const char* url,const char* host);
void httpMessageFree(struct httpMessage* pMessage);
void httpMessageAddField(struct httpMessage* pMessage,const char*key,const char*value);
void createHttpMessage(char *buf,struct httpMessage* pMessage);

#ifdef __cplusplus
}
#endif

#endif