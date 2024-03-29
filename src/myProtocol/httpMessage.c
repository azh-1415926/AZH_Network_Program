#include <myProtocol/httpMessage.h>

#define HTTP_VERSION "HTTP/1.1"

/* 初始化报文，根据传入的请求方式、url、http 版本、host 初始化 */
void httpMessageInitalize(struct httpMessage* pMessage,unsigned int size,const char* method,const char* url,const char* host)
{
    /* 若报文指针为空，直接返回 */
    if(pMessage==NULL)
    {
        printf("httpMessage is null,intitalize error!\n");
        return;
    }
    /* 清空报文 */
    memset(pMessage,0,size);
    /* 插入报文头 */
    pMessage->method=(char*)method;
    pMessage->url=(char*)url;
    pMessage->version=(char*)HTTP_VERSION;
    /* 插入 Host 字段 */
    pMessage->pField=(struct field_name*)malloc(sizeof(struct field_name));
    pMessage->pField->key="Host";
    pMessage->pField->value=(char*)host;
    pMessage->pField->next=NULL;
}

/* 释放报文 */
void httpMessageFree(struct httpMessage* pMessage)
{
    /* 若报文指针为空，直接返回 */
    if(pMessage==NULL)
    {
        printf("httpMessage is null,free error!\n");
        return;
    }
    /* pField 指向字段，pTemp 指向需要释放的字段 */
    struct field_name* pField=pMessage->pField;
    struct field_name* pTemp=NULL;
    while(pField->next)
    {
        pTemp=pField;
        pField=pField->next;
        free(pTemp);
    }
    free(pField);
}

/* 为报文中添加字段，根据传入的 key、value 添加 */
void httpMessageAddField(struct httpMessage* pMessage,const char*key,const char*value)
{
    /* 若报文指针为空，直接返回 */
    if(pMessage==NULL)
    {
        printf("httpMessage is null,addField error!\n");
        return;
    }
    /* pField 指向字段 */
    struct field_name* pField=pMessage->pField;
    /* 遍历到最后一个字段 */
    while(pField->next)
        pField=pField->next;
    /* 在最后一个字段插入新字段 */
    pField->next=(struct field_name*)malloc(sizeof(struct field_name));
    pMessage->pField->key=(char*)key;
    pMessage->pField->value=(char*)value;
    pMessage->pField->next=NULL;
}

/* 创建报文 */
void createHttpMessage(char *buf,struct httpMessage* pMessage)
{
    /* 若报文指针为空，直接返回 */
    if(pMessage==NULL)
    {
        printf("httpMessage is null,create error!\n");
        return;
    }
    /* 插入报文头到 buf */
    strcat(buf, pMessage->method);
    strcat(buf, " ");
    strcat(buf, pMessage->url);
    strcat(buf, " ");
    strcat(buf, pMessage->version);
    strcat(buf, "\n");
    /* 插入字段到 buf */
    struct field_name* pField=pMessage->pField;
    do
    {
        strcat(buf, pField->key);
        strcat(buf,":");
        strcat(buf, pField->value);
        strcat(buf, "\n");
        if(pField->next)
            pField=pField->next;
        else
            pField=NULL;
    }
    while(pField);
    strcat(buf, "\r\n");
}