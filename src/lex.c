#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"lex.h"

static void free_token(void *t)
{
    free(t) ;
}

static void set_num_token(token *t,int t1,int t2)
{
    t->tag = t1 ;
    t->lecture.value = t2 ;
}
static void set_identity_token(token *t,char *t2)
{
    t->tag = ID ;
    strcpy(t->lecture.str,t2) ;
}

static void print_token(token *t)
{
    if(t->tag == ID)
    {
        printf("%d  %s\n",t->tag,t->lecture.str) ;
    }
    else if(t->tag == NUM)
    {
        printf("%d  %d\n",t->tag,t->lecture.value) ;
    }
    else printf("%d  %c\n",t->tag,t->lecture.value) ;
}

void init_readbuf()
{
    Node *p_fir = FIR ;
    Node *p_sec = SEC ;
    Node *p_thi = THI ;
    for(int i=1;i<READSIZE;++i)
    {
        Node *temp = (Node *)malloc(sizeof(Node)) ;
        temp->next = SEC ;
        p_fir->next = temp ;
        p_fir = temp ;
    }
    for(int i=1;i<READSIZE;++i)
    {
        Node *temp = (Node *)malloc(sizeof(Node)) ;
        temp->next = THI;
        p_sec->next = temp ;
        p_sec = temp ;
    }
    for(int i=1;i<READSIZE;++i)
    {
        Node *temp = (Node *)malloc(sizeof(Node)) ;
        temp->next = FIR ;
        p_thi->next = temp ;
        p_thi= temp ;
    }
}

void init_num_set(char *set)
{
    for(int i=0;i<NUMSIZE;++i) set[i] = i+48;
}

void init_case_set(char *set)
{
    for(int i=0;i<CASESIZE;++i) set[i] = i+65 ;
    for(int i=0;i<CASESIZE;++i) set[i+CASESIZE] = i+97 ;
}

int is_num(Node *ch,char *set)
{
    int i ;
    for(i=0;i<NUMSIZE;++i)
    {
        if(ch->ch == set[i]) break ;
    }

    if(i == NUMSIZE) return -1;
    return i;

}

int is_case(Node *ch,char *set)
{
    int i;
    for(i=0;i<CASESIZE*2;++i)
    {
        if(ch->ch == set[i]) break ;
    }
    if(i == CASESIZE*2) return -1 ;
    return i ;
}

int pret(FILE *fp,Node *chToken)
{
    /*此处应该有一个头文件处理*/
    //处理头文件()；
    /*处理注释*/
    char chTmp ;
    int i ;
    for(i=0;i < READSIZE;++i)
    {
        chTmp = getc(fp) ;
        if(chTmp == '\n' || chTmp == '\r') ;
        else if(chTmp == EOF)
        {
            chToken = NULL ;
            break ;
        }
        else
        {
            chToken->ch = chTmp ;
            chToken = chToken->next ;
        }
    }



    return i ;
}

Node* analysis_num(Node *p,char *set,token *tmp)
{
    int v = 0;
    int c = is_num(p,set);
    while(c != -1)
    {
        v = v*10 + c ;
        p = p->next ;
        if(p == NULL)
        {
            tmp->tag = NUM ;
            tmp->lecture.value = v ;
            return NULL ;
        }
        if(p == FIR || p==SEC || p==THI) pret(fp,p) ;
        c = is_num(p,set) ;
    }

    tmp->tag = NUM ;
    tmp->lecture.value = v ;

    return p ;

}

Node* analysis_case(Node *p,char *setCase,char *setNum,token *tmp)
{
    char buf[IDENTISIZE] ;
    int i =0 ;

    do
    {
        buf[i] = p->ch ;
        p = p->next ;
        if(p==NULL)
        {
            tmp->tag = ID ;
            strcpy(tmp->lecture.str,buf) ;
            return NULL ;
        }
        if(p == FIR || p==SEC || p==THI) pret(fp,p) ;
        i++ ;
    }while(is_case(p,setCase) != -1 || is_num(p,setNum) != -1) ;

    buf[i] = '\0' ;
    tmp->tag = ID ;
    strcpy(tmp->lecture.str,buf) ;

    return p;
}

void scan(FILE *fp,Node *start,char *setNum,char *setCase)
{
    Node *p = start ;
    //利用pnext来回退，比如**,++,--,<=,<=,<<,>>等
    Node *pnext = p->next ;
    token *tmp = malloc(sizeof(Node)) ;

    while(1)
    {
        pnext = p->next ;
        if(p == FIR || p == SEC || p== THI) pret(fp,p) ;
        switch(p->ch)
        {
        case ' ':case '/t':case '\r' case '\n':
        {
            p=pnext ;
            goto BREAK ;
        }
//利用peer指针可扩展字符集，+和++,--和-
        case '+' :
        {
            set_num_token(tmp,OP,'+') ;
            p=pnext ;
            break ;
        }
        case '-' :
        {
            set_num_token(tmp,OP,'-') ;
            p = pnext ;
            break ;
        }
        case '*' :
        {
            set_num_token(tmp,OP,'*') ;
            p=pnext ;
            break ;
        }
        case '/' :
        {
            set_num_token(tmp,OP,'/') ;
            p=pnext ;
            break ;
        }
        case '>' :
        {
            set_num_token(tmp,OP,'>') ;
            p=pnext ;
            break ;
        }
        case '<' :
        {
            set_num_token(tmp,OP,'<') ;
            p=pnext ;
            break ;
        }
        case '=' :
        {
            set_num_token(tmp,OP,'=') ;
            p=pnext ;
            break ;
        }
        case ';' :
        {
            set_num_token(tmp,OP,';') ;
            p=pnext ;
            break ;
        }
        case ',' :
        {
            set_num_token(tmp,OP,',') ;
            p=pnext ;
            break ;
        }
        case '{' :
        {
            set_num_token(tmp,OP,'{') ;
            p=pnext ;
            break ;
        }
        case '}' :
        {
            set_num_token(tmp,OP,'}') ;
            p=pnext ;
            break ;
        }
        case '[' :
        {
            set_num_token(tmp,OP,'[') ;
            p=pnext ;
            break ;
        }
        case ']' :
        {
            set_num_token(tmp,OP,']') ;
            p=pnext ;
            break ;
        }
        case '(' :
        {
            set_num_token(tmp,OP,'(') ;
            p=pnext ;
            break ;
        }
        case ')' :
        {
            set_num_token(tmp,OP,')') ;
            p=pnext ;
            break ;
        }
        default:
        {
            if(is_num(p,setNum) != -1)
            {
                p = analysis_num(p,setNum,tmp) ;
                if(p == NULL)
                {
                    print_token(tmp) ;
                    return ;
                }
                break ;
            }
            else if(is_case(p,setCase) != -1)
            {
                p = analysis_case(p,setCase,setNum,tmp) ;
                if(p==NULL)
                {
                    print_token(tmp) ;
                    return ;
                }
                break ;
            }
            else return ;
        }
        }
        print_token(tmp) ;
        BREAK:
            ;
        if(p==NULL) return ;
    }

}

int main(int argc,char *argv[])
{
   char setNum[10] ;
   char setCase[52] ;
   FIR = malloc(sizeof(Node)) ;
   SEC = malloc(sizeof(Node)) ;
   THI = malloc(sizeof(Node)) ;

   init_num_set(setNum) ;
   init_case_set(setCase) ;
   fp = fopen("test.c","r") ;
   init_readbuf() ;
   pret(fp,FIR) ;
   scan(fp,FIR,setNum,setCase) ;

   return 0 ;
}
