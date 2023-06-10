#include "myhttp.h"
#define HTTP_VERSION "HTTP/1.1"
void httpMessageInitalize(struct httpMessage* pMessage,int size,const char* method,const char* url,const char* host){
    if(pMessage==NULL){
        printf("httpMessage is null,intitalize error!\n");
        return;
    }
    memset(pMessage,0,size);
    pMessage->method=(char*)method;
    pMessage->url=(char*)url;
    pMessage->version=(char*)HTTP_VERSION;
    pMessage->pField=(struct field_name*)malloc(sizeof(struct field_name));
    pMessage->pField->key="Host";
    pMessage->pField->value=(char*)host;
    pMessage->pField->next=NULL;
}
void httpMessageFree(struct httpMessage* pMessage){
    if(pMessage==NULL){
        printf("httpMessage is null,free error!\n");
        return;
    }
    struct field_name* pField=pMessage->pField;
    struct field_name* pTemp=NULL;
    while(pField->next){
        pTemp=pField;
        pField=pField->next;
        free(pTemp);
    }
    free(pField);
}
void httpMessageAddField(struct httpMessage* pMessage,const char*key,const char*value){
    if(pMessage==NULL){
        printf("httpMessage is null,addField error!\n");
        return;
    }
    struct field_name* pField=pMessage->pField;
    while(pField->next){
        pField=pField->next;
    }
    pField->next=(struct field_name*)malloc(sizeof(struct field_name));
    pMessage->pField->key=(char*)key;
    pMessage->pField->value=(char*)value;
    pMessage->pField->next=NULL;
}
void createHttpMessage(char *buf,struct httpMessage* pMessage){
    if(pMessage==NULL){
        printf("httpMessage is null,create error!\n");
        return;
    }
    strcat(buf, pMessage->method);
    strcat(buf, " ");
    strcat(buf, pMessage->url);
    //
    strcat(buf, " ");
    strcat(buf, pMessage->version);
    strcat(buf, "\n");
    struct field_name* pField=pMessage->pField;
    do{
        strcat(buf, pField->key);
        strcat(buf,":");
        strcat(buf, pField->value);
        strcat(buf, "\n");
        if(pField->next){
            pField=pField->next;
        }else{
            pField=NULL;
        }
    }while(pField);
    strcat(buf, "\r\n");
}