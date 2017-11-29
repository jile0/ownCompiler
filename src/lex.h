/********************
*���ܴ������ı���
*ֻ֧��c���Ե��Ӽ�
*δ��������
*�������������ɵ����黺������ʹ�ñ�־λ�����ǲ��Ǹ��ã������������ݣ�
*���tokenֻ��Ϊ����ҵҪ����������Ӧ�÷��ظ��﷨����������
*��һ��hash�������洢��ʶ��ı�ʶ�������ڹ���
*/
#ifndef LEX_H_INCLUDED
#define LEX_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#define READSIZE 1024//���뻺������󳤶�
#define IDENTISIZE 32//��ʶ����󳤶�
#define NUMSIZE 10
#define CASESIZE 26

#define NUM 260
#define ID 261
#define OP 262

#define FIR fir
#define SEC sec
#define THI thi

typedef struct
{
    int tag ;
    union _value
    {
        int value ;
        char str[READSIZE] ;
    } lecture ;
} token ;

typedef struct _Node
{
    char ch ;
    struct _Node *next ;
} Node ;

static Node *fir ;
static Node *sec ;
static Node *thi ;
static FILE *fp ;

static void free_token(void *t) ;
static void set_num_token(token *t,int t1,int t2) ;
static void set_str_token(token *t,int t1,char *t2) ;
static void print_token(token *t) ;
void init_readbuf() ;
void init_num_set(char *set) ;
void init_case_set(char *set) ;
int is_num(Node *ch,char *set) ;
int is_case(Node *ch,char *set) ;
Node* analysis_case(Node *p,char *setCase,char *setNum,token *tmp) ;
Node* analysis_num(Node *peer,char *set,token *tmp) ;
int pret(FILE *fp,Node *chToken) ;
void scan(FILE *fp,Node *start,char *setNum,char *setCase) ;


#endif // LEX_H_INCLUDED
