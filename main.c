#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct{
    double *elem;
    int ptr;
    int space;
}Stack;

bool is_empty(Stack *st){
    return st->ptr == 0;
}

void resize(Stack *st, int n){
    st->space *= n;
    double *newelem = malloc(st->space * sizeof(double));
    for(int i = 0; i < st->ptr; ++i)
        newelem[i] = st->elem[i];
    free(st->elem);
    st->elem = newelem;
}

void push(Stack *st, double x){
    if(st->ptr >= st->space)
        resize(st, 2);
    st->elem[st->ptr++] = x;
}

double pop(Stack *st){
    if(!is_empty(st))
        return st->elem[--st->ptr];
    else
        return 0;
}

Stack *new_Stack(void){
    Stack *st = malloc(sizeof(*st));
    st->ptr = 0;
    st->space = 8;
    st->elem = malloc(st->space * sizeof(double));
    return st;
}

void free_Stack(Stack *st){
    free(st->elem);
    free(st);
}

void add(Stack *st){push(st, pop(st) + pop(st));}
void sub(Stack *st){double a = pop(st); push(st, pop(st) - a);}
void mult(Stack *st){push(st, pop(st) * pop(st));}
void division(Stack *st){double a = pop(st); push(st, pop(st) / a);}
void power(Stack *st){double a = pop(st); push(st, pow(pop(st), a));}
void sqroot(Stack *st){push(st, sqrt(pop(st)));}
void sinus(Stack *st){push(st, sin(pop(st)));}

double opn(char *expr) {
    static const char *op[] = {"+", "-", "*", "/", "pow", "sin", "sqrt"};
    static void (*fun[])(Stack *) = {add, sub, mult, division, power, sinus, sqroot};
    int size = sizeof(op) / sizeof(*op);
    Stack *st = new_Stack();

    while (*expr != '\0'){
        if(*expr >= '0' && *expr <= '9')
            push(st, strtod(expr, &expr));
        else{
            for(int i = 0; i < size; ++i){
                if(*expr == op[i][0]){
                    bool matching = true;
                    int j = 1;
                    while(op[i][j] != '\0' && matching){
                        matching = *(expr + j) == op[i][j];
                        j++;
                    }
                    if(matching){
                        expr += j;
                        (fun[i])(st);
                        break;
                    }
                }
            }
        }
        while(*expr == ' ')
            expr++;
    }

    double tmp = pop(st);
    free_Stack(st);
    return tmp;
}

int main(void) {
    double a = opn("4 7 + 3 / 2 19 - * sin");
    printf("\n%lf\n", a);
    return 0;
}
