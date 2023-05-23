#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct Player
{
    char* firstName;
    char* secondName;
    int points;
};
typedef struct Player JUCATOR;

struct lista
{
    int nr_jucatori;
    JUCATOR* x;
    char* numeEchipa;
    float teampoints;   //camp pentru stocarea punctelor fiecarei echipe
    struct lista* next;
};
typedef struct lista ECHIPA;

struct coad
{
    ECHIPA* team_node;
    struct coad* next;
};
typedef struct coad coada;

struct Q
{
    ECHIPA *front, *rear;
};
typedef struct Q Queue;

Queue* createQueue()
{
    Queue*q;
    q=(Queue*)malloc(sizeof(Queue));
    if(q==NULL)
    {
        return NULL;
        printf("ALLOCATION ERROR \n");
    }
    q->front=q->rear=NULL;
    return q;
}
int isEmptyQueue(Queue* queue)
{
    return(queue->front==NULL);
}

ECHIPA* deQueue(Queue* q)
{
    if(isEmptyQueue(q)==NULL){
     printf("queue empty\n");
     exit(1);
    }
    ECHIPA *aux, *address;
    address=(ECHIPA*)malloc(sizeof(ECHIPA));
    aux=q->front;
    address=q->front;
    q->front=(q->front)->next;
    free(aux);
    return address;
}

void enQueue(Queue *q, ECHIPA* team)
{    // incerc sa memorez intr-un nod din coada cate o adresa 
     // a unei echipe din lista principala
    if(q==NULL || team==NULL)
    {
        printf("null \n");
        exit(1);
    }
    ECHIPA* newNode=(ECHIPA*)malloc(sizeof(ECHIPA));
    newNode=team;    //nodul nou initiat primeste ca valoare o adresa a unei echipe
    newNode->next=NULL;  // comentarea acestui rand face ca for-ul din main sa se parcurga corect, daca este lasat necomentat
    if(q->rear==NULL) q->rear=newNode; // for-ul se parcurge doar o data
    else
    {
        (q->rear)->next=newNode;
        q->rear=newNode;
    }
    if(q->front==NULL) q->front=q->rear;
}


struct stiv
{
    ECHIPA* valoare;
    struct stiv* next;
};
typedef struct stiv stiva;

void push(stiva** top, ECHIPA* team)
{   // valoarea unui element al stivei este un nod apartinand unei echipe
    stiva* newNode=(stiva*)malloc(sizeof(stiva));
    newNode->valoare=team;
    newNode->next=*top;
    *top=newNode;
}
ECHIPA* pop(stiva** top)
{
    //verificare stiva goala
    stiva* temp=(*top);
    ECHIPA* aux=temp->valoare;
    *top=(*top)->next;
    free(temp);
    return aux;
}

int calculPutere (int n_echipe)
{               // functie pentru calculul celui mai mare numar n, putere a lui 2 mai mic
                // decat nr de echipe
    int a=2;    // pornesc cu nr 2
    while(a<n_echipe)
    {
        a=a*2;            // cat timp este mai mic decat nr de echipe, inmultesc cu 2
    }
    a=a/2;          // dupa ce iese din while, inseamna ca este primul numar mai mare
                    // decat nr de echipe putere a lui 2, deci impart la 2
    return a;
}

int calculPunctaj(ECHIPA* team)
{
    int i;
    int punctaj_echipa;
    for(i=0; i<team->nr_jucatori; i++)
    {
        punctaj_echipa=punctaj_echipa+team->x[i].points;
    }
    return punctaj_echipa;
}

void delete_team (ECHIPA** Node)
{
    int j;      // fct pentru stergerea unei echipe
    float punctaj_echipa;
    ECHIPA *aux, *p, *aux2;
    aux2=NULL;
    float min=9999;
    for(p=*Node; p!=NULL; p=p->next)      // parcurg toate echipele, calculand numarul de puncte al fiecarei echipe
    {
        punctaj_echipa=0;
        for(j=0; j<p->nr_jucatori; j++)
        {
            punctaj_echipa=punctaj_echipa+p->x[j].points;
        }
        p->teampoints=punctaj_echipa;
        if(p->teampoints<min)
            min=p->teampoints;          // calculez totodata, la fiecare pas al forului si minimul, fiind mereu actualizat.                                  
    }                                   // cand iese din for, am si minimul actualizat
    for(aux=*Node; aux!=NULL; aux=aux->next)
    {
        if(aux->teampoints==min)                // reparcurg toata lista, daca campul echipei actuale coincide cu minimul,                                       
        {                                       // echipa va fi stearsa din lista
            if(aux==*Node)
            {
                aux2=*Node;                     // daca nodul care are punctajul = minimul este chiar primul nod din lista
                *Node=(*Node)->next;
                free(aux2);
                return;                         // return pentru a evita segmentation fault la atribuirea aux2=aux;
            }
            else
            {
                aux2->next=aux->next;           // daca nodul care are punctajul este pe oricare alta pozitie decat prima
                free(aux);
                return;
            }
        }
        aux2=aux;
    }

}

void createList(ECHIPA** head, FILE*f)
{
    int nr_echipe;
    fscanf(f,"%d", &nr_echipe);       // citesc nr de echipe ca fiind primul intreg din fisier
    int i,j,k=0;
    char c;
    for(i=0; i<nr_echipe; i++)        // populez lista, citind pe rand pentru fiecare echipa in parte
    {
        ECHIPA* newNode;
        newNode=(ECHIPA*)malloc(sizeof(ECHIPA));
        fscanf(f,"%d",&newNode->nr_jucatori); // citesc nr de jucatori al fiecarei echipe
        fgetc(f);
        char buffer[40];
        fgets(buffer,40,f);     // citesc numele echipei intr-un buffer si aloc dinamic apoi dimensiunea exacta
        newNode->numeEchipa=(char*)malloc((strlen(buffer)+1)*sizeof(char));
        strcpy(newNode->numeEchipa,buffer);
        newNode->x=(JUCATOR*)malloc((newNode->nr_jucatori)*sizeof(JUCATOR)); // aloc dinamic numarul pentru vectorul de jucatori
        for(j=0; j<newNode->nr_jucatori; j++)
        {
            fscanf(f,"%s",buffer); //citesc si atribui pentru fiecare jucator numele si prenumele
            newNode->x[j].firstName=(char*)malloc((strlen(buffer)+1)*sizeof(char));
            strcpy(newNode->x[j].firstName,buffer);
            fscanf(f,"%s",buffer);
            newNode->x[j].secondName=(char*)malloc((strlen(buffer)+1)*sizeof(char));
            strcpy(newNode->x[j].secondName, buffer);
            fscanf(f,"%d", &newNode->x[j].points); //citesc si numarul de puncte al fiecarui jucator in variabila corespunzatoare

        }
        newNode->next=*head;
        *head=newNode;
    }
}

int main(int argc, char* argv[])
{
    ECHIPA *head;
    head=NULL;
    int teams,i;
    int nr_teams=0, no_teams;
    int cerinte[5];
    FILE* f;
    FILE* r;
    f=fopen(argv[1], "rt"); //deschid fisierul pentru cerinte
    if(f==NULL)
    {
        printf("ERROR f \n");
        exit(1);
    }
    for(i=0; i<5; i++)
    {
        fscanf(f,"%d",&cerinte[i]);  // populez vectorul de cerinta in functie de fisierul c.in
    }
    fclose(f);
    if(cerinte[0]==1)           // pentru task-ul 1
    {
        f=fopen(argv[2], "rt");
        if(f==NULL)
        {
            printf("ERROR \n");  // deschid fisierul d.in in citire
            exit(1);
        }
        r=fopen(argv[3], "w");  //deschid fisierul r.out in scriere
        if(r==NULL)
        {
            printf("ERROR \n");
            exit(1);
        }
        createList(&head,f);   // apelez functia pentru crearea listei
        ECHIPA* headcpy;
        for(headcpy=head; headcpy!=NULL; headcpy=headcpy->next) //parcurg lista si printez in fisier fiecare nume de echipa
        {
            fprintf(r,"%s", headcpy->numeEchipa);
        }
        fclose(f);
        fclose(r);
    }
    if(cerinte[1]==1)
    {
        ECHIPA* aux;
        for(aux=head; aux!=NULL; aux=aux->next) 
        {
            nr_teams++;     // parcurg lista de la cap la coada si incrementez de fiecare data pentru a stoca numarul echipelor
        }                   // si in afara functiei createList
        no_teams=nr_teams;  // folosit pentru task-ul 3
        r=fopen(argv[3], "wt");
        if(r==NULL)
        {
            printf("ERROR \n");
            exit(1);
        }
        int b,k;
        b=calculPutere(nr_teams); // in variabila b stochez cel mai mare numar n mai mic decat nr echipe, putere a lui 2
        for(k=0; k<(nr_teams-b); k++)
        {
            delete_team(&head);   // apoi, numarul de echipe de sters este diferenta de la nr de echipe actuale, pana la numarul
        }                         // dorit de echipe, adica b
        for(aux=head; aux!=NULL; aux=aux->next)
        {
            fprintf(r,"%s", aux->numeEchipa); // printez fiecare nume de echipa din lista modificata mai sus in fisierul r.out
        }
        fclose(r);
    }



    if(cerinte[2]==1)
    {
        r=fopen(argv[3], "at"); // deschid fisierul r.out in completare, pentru a nu scrie peste
        if(r==NULL)
        {
            printf("ERROR \n");
            exit(1);
        }
        ECHIPA *headcopy;  // iau o variabila headcopy pentru a parcurge lista si a adauga intr-o coada toate echipele avute
        Queue *q;
        q=createQueue(); // creez coada, goala initial
        for(headcopy=head; headcopy!=NULL; headcopy=headcopy->next)
        {
            enQueue(q,headcopy); // populez coada, functia este insa cu probleme, facand doar o singura adaugare
        }                        
        int contor=0;
        fprintf(r,"--- ROUND NO:%d",contor+1);  
        for(i=0; i<no_teams/2; i++) // incerc sa scot din coada cate 2 echipe o data, pentru a crea cate un meci la fiecare
        {                           // iteratie a forului
            ECHIPA *team1,*team2;
            team1=deQueue(q);
            team2=deQueue(q);
            fprintf(r,"%-33s%s%-33s",team1->numeEchipa,"-",team2->numeEchipa); //incerc sa printez echipele in asa fel incat
        }                                                                      // caracterul '-' sa fie pe pozitia 34 de fiecare data

        stiva* stackTop=NULL;       // incepusem crearea stivei, dar fiind blocat la coada nu puteam continua
        fclose(r);
    }
    return 0;
}

