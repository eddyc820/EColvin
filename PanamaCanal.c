#include <stdio.h>

#include <stdlib.h>

/*Struct a pointer node with the following fields*/
typedef struct pnode {

    struct pnode *next; /*next pointer*/

    struct pnode *back; /*back pointer*/

    int cost; /*cost to move*/

    char piece; /*letter that was moved*/

    int dir; /*which way the letter moved*/

    char board[12]; /*board containing the puzzle*/

} pnode;

typedef pnode *TypePosition;


/*function to create and allocate space for a new position and set the front and back pointers to null*/
TypePosition NEWPOSITION() {

    TypePosition p = (TypePosition) malloc(sizeof(pnode));

    if (p==NULL) {

        printf("Malloc for a new position failed.");

        exit(1);

        p -> next = NULL;

        p -> back = NULL;
    }

    return p;

}


/*struct a hash table that is a position array*/
typedef struct Htable {



    TypePosition HashArray[100003];
int BucketSize[100003];


}Htable;

typedef Htable *TypeHash;

/*Hash function to generate a hash key for positions*/
int Hash(char board[12]) {

    unsigned int Hvalue=0;

    int i = 0;
    while(i<12) {
        Hvalue = (((Hvalue*128) + (int)board[i++]) % 100003) %100003;
    }
    return Hvalue;
}

int isSame(pnode *po, pnode *po2) {
    int i = 0;
    while(i < 12) {
        if(po->board[i] != po2->board[i]) {
            return 0;
        } else {
            i++;
        }
    }
    return 1;
}

/*Member function to check whether or not a position is in the hash table or linked list bucket*/
int MEMBER(pnode *po, pnode *hPo) {
   
    if( hPo == NULL){
    return 0;
    }
    if(isSame(po, hPo) == 1) {
           
            return 1;
    }
        while(hPo -> next != NULL) {

            if(isSame(po, hPo) == 1) {

                return 1;

            } else {

                /*keep searching list until the position is found or is null in which case it is not a member*/
                hPo = hPo -> next;

            }

        }

        if(isSame(po, hPo) == 1) {

                return 1;
        }

        return 0;
}
/*Insert a position into a HashTable*/
void INSERT(pnode *po, Htable *H) {
TypePosition hashPosition;
    /*Obtain the hash key for the current position*/
    int key = Hash(po -> board);
    hashPosition = H->HashArray[key];

if(MEMBER(po, hashPosition) == 1) {
/* Do nothing */
    }else{
        if (hashPosition == NULL){
        H->HashArray[key] = po;
        H->BucketSize[key]++;
        }else {
            while(hashPosition -> next != NULL) {
                hashPosition = hashPosition -> next;
            }
  
            hashPosition -> next = po;
        }
    }
}


 /*struct a queue with a first and last as well as an array containing positions*/
 typedef struct circleQueue {

 int first;

 int last;

 TypePosition data[50000];
 int QueueSize;

 }*TypeQueue;
 
 /*Initialize a new queue and allocate the space for it*/
 TypeQueue NEWQUEUE() {
 TypeQueue q = malloc(sizeof(struct circleQueue));

 q -> first = 0;

 q -> last = 0;
 
 q -> QueueSize = 0;

 return q;

 }
/*Check if queue is empty by checking front and back pointers*/
int Qempty(struct circleQueue *q) {
    if(q->first == q->last) {
        return -1;
    } else {
        return 0;
    }
}

void Enqueue(struct circleQueue *q, pnode *po) {
    q->data[q->last] = po;
    /*After the position is added, it is moved to the front and the last pointer is modified to keep the queue circular*/
    q->last = (q->last+1) % 50000;
    q->QueueSize++;
 }

 /*Dequeue a position*/
 TypePosition Dequeue(struct circleQueue *q) {

    TypePosition dequeuePos = q->data[q->first];
    /*Modify the queue so it remains circular and the data is stored properly after dequeue*/
    q->first = (q->first+1) % 50000;
    q->QueueSize--;
    return dequeuePos;
 }
 /*Print the board for viewing*/
 void printBoard(pnode *po) {
    TypePosition printPos = NEWPOSITION();
    int i;
    int j;
   
    printPos = po;
    for(i = 0; i< 6; i++) {
        printf("%c ", printPos->board[i]);
    }
    printf("\n");
    for(j = 6; j< 12; j++) {
        printf("%c ", printPos->board[j]);
    }
    printf("\n");
    printf("\n");
 }

 /*Define stack struct*/
 typedef struct stack {
    int top;
    TypePosition path[50];
 }*TypeStack;

 /*Initialize here*/
 TypeStack NEWSTACK() {
    TypeStack s = malloc(sizeof(struct stack));
    s->top=0;

    return s;
 }
 /*Check if stack is empty*/
 int StackEmpty(struct stack *s) {
    if(s->top == 0) {
        return 0;
    } else {
        return 1;
    }
 }
 /*Push a position onto the stack*/
 void StackPush(struct stack *s, pnode *po) {
    s->path[++s->top] = po;
 }
 /*Pop a position off the stack*/
 TypePosition StackPop(struct stack *s) {
    return s->path[s->top--];
 }

 char *findPositions(pnode *po) {
    TypePosition downPo = NEWPOSITION();
    TypePosition upPo = NEWPOSITION();
    TypePosition rightPo = NEWPOSITION();
    TypePosition leftPo = NEWPOSITION();
    int j;

    /*Declare an array of characters*/
    static char masterArray[48];
    /*Assign new positions to the board being checked*/
    for(j = 0; j < 12; j++) {
        downPo->board[j] = po->board[j];
    }
    for(j = 0; j < 12; j++) {
        upPo->board[j] = po->board[j];
    }
    for(j = 0; j < 12; j++) {
        leftPo->board[j] = po->board[j];
    }
    for(j = 0; j < 12; j++) {
        rightPo->board[j] = po->board[j];
    }


    int i = 0;
    /*Search the board until the empty space is found*/
    while(po->board[i] != ' ') {
        i++;
    }

    /*The next few are cases for where a letter could move, up, down, left, or right or not at all. The masterArray will add these new
    positions in 4 12 index segments each indicating a certain direction*/

    /*Move down*/
    int k;
    int m;
    int n;
    int x;
    /*Move down*/
    if(i >= 0 && i <= 5) {
        char swap;
        swap = downPo->board[i];
        downPo->board[i] = downPo->board[i+6];
        downPo->board[i+6] = swap;
        for(k = 0; k < 12; k++) {
            masterArray[k] = downPo->board[k];
        }
       
    } else {
        for(k = 0; k < 12; k++) {
            masterArray[k] = downPo->board[k];
        }
    }
    /*Move up*/
    if(i >= 6 && i <= 11) {
        char swap;
        swap = upPo->board[i];
        upPo->board[i] = upPo->board[i-6];
        upPo->board[i-6] = swap;
        for(m = 12; m < 24; m++) {
            masterArray[m] = upPo->board[m-12];
        }
       
    } else {
        for(m = 12; m < 24; m++) {
            masterArray[m] = upPo->board[m-12];
        }
    }
    /*Move left*/
    if(i != 0 && i != 6) {
        char swap;
        swap = leftPo->board[i];
        leftPo->board[i] = leftPo->board[i-1];
        leftPo->board[i-1] = swap;
        for(n = 24; n < 36; n++) {
            masterArray[n] = leftPo->board[n-24];
        }
       
    } else {
        for(n = 24; n < 36; n++) {
            masterArray[n] = leftPo->board[n-24];
        }
    }
    /*Move right*/
    if(i != 5 && i != 11) {
        char swap;
        swap = rightPo->board[i];
        rightPo->board[i] = rightPo->board[i+1];
        rightPo->board[i+1] = swap;
        for(x = 36; x < 48; x++) {
            masterArray[x] = rightPo->board[x-36];
        }
       
    } else {
        for(x = 36; x < 48; x++) {
            masterArray[x] = rightPo->board[x-36];
        }
    }

    return masterArray;
 }


int main () {


    /*Create a current position and assign it the starting board CANAMA PANAL""*/
    TypePosition currentPosition = NEWPOSITION();



    currentPosition -> board[0] = 'C';

    currentPosition -> board[1] = 'A';

    currentPosition -> board[2] = 'N';

    currentPosition -> board[3] = 'A';

    currentPosition -> board[4] = 'M';

    currentPosition -> board[5] = 'A';

    currentPosition -> board[6] = 'P';

    currentPosition -> board[7] = 'A';

    currentPosition -> board[8] = 'N';

    currentPosition -> board[9] = 'A';

    currentPosition -> board[10] = 'L';

    currentPosition -> board[11] = ' ';
   


    /*Create a final position and assign it the final board PANAMA CANAL""*/
    TypePosition finalnode = NEWPOSITION();



    finalnode -> board[0] = 'P';

    finalnode -> board[1] = 'A';

    finalnode -> board[2] = 'N';

    finalnode -> board[3] = 'A';

    finalnode -> board[4] = 'M';

    finalnode -> board[5] = 'A';

    finalnode -> board[6] = 'C';

    finalnode -> board[7] = 'A';

    finalnode -> board[8] = 'N';

    finalnode -> board[9] = 'A';

    finalnode -> board[10] = 'L';

    finalnode -> board[11] = ' ';


    /*Create a hashtable and allocate the space for it as well as initalize a new queue*/
    Htable* H = (Htable*) malloc(sizeof(Htable));
    int i;
    for(i = 0; i < 100003; i++){
    H->HashArray[i] = NULL;
    H->BucketSize[i] = 0;
    }

    TypeQueue circQ = NEWQUEUE();
    TypeStack posStack = NEWSTACK();

   
    /*HERE STARTS THE MAIN PROGRAM RUN*/
   
    Enqueue(circQ, currentPosition);
    currentPosition = Dequeue(circQ);
    while(isSame(currentPosition, finalnode) == 0) {
        INSERT(currentPosition, H);
        /*ARRAY OF MY REACHABLE POSITIONS*/
        char *reachablePos;
        /*ASSIGN THE ARRAY TO THE POSITIONS FOUND FROM THE DEQUEUED POS*/
       
        reachablePos = findPositions(currentPosition);
        TypePosition downPo = NEWPOSITION();
        TypePosition upPo = NEWPOSITION();
        TypePosition rightPo = NEWPOSITION();
        TypePosition leftPo = NEWPOSITION();
       
        int i;
        int j;
        int k;
        int l;
        int count = 0;
        /*The next four loops break back down the size 48 array back into directions and they
        become the new positions*/

            for(i = 0; i < 12; i++) {
                downPo->board[i] = reachablePos[i];
            }

            for(j = 12; j < 24; j++) {
                upPo->board[count] = reachablePos[j];
                count++;
            }

            count = 0;
            for(k = 24; k < 36; k++) {
                leftPo->board[count] = reachablePos[k];
                count++;
            }

            count = 0;
            for(l = 36; l < 48; l++) {
                rightPo->board[count] = reachablePos[l];
                count++;
            }

        downPo->back = currentPosition;
        upPo->back = currentPosition;
        leftPo->back = currentPosition;
        rightPo->back = currentPosition;

        /*Check each newly found position if its a member and if not enqueue it*/
        if(MEMBER(downPo, H->HashArray[Hash(downPo->board)]) == 0) {
            Enqueue(circQ, downPo);
        }
        if(MEMBER(upPo, H->HashArray[Hash(upPo->board)]) == 0) {
            Enqueue(circQ, upPo);
        }
        if(MEMBER(leftPo, H->HashArray[Hash(leftPo->board)]) == 0) {
            Enqueue(circQ, leftPo);
        }
        if(MEMBER(rightPo, H->HashArray[Hash(rightPo->board)]) == 0) {
            Enqueue(circQ, rightPo);
        }
       
        while (MEMBER(currentPosition, H->HashArray[Hash(currentPosition->board)]) == 1) {
            currentPosition = Dequeue(circQ);

        }

    }
    int step = 0;
    /*Push positions to stack until back position reached*/
    while(currentPosition -> back != NULL) {
        StackPush(posStack, currentPosition);
        currentPosition = currentPosition -> back;
    }
    StackPush(posStack, currentPosition);
    /*Pop them off to print in order*/
    while(StackEmpty(posStack) != 0) {
        printf("\nStep: ");
        printf("%d\n", step);
        printBoard(StackPop(posStack));
        step++;
    }
   
}
