#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>

#define NVOTES 10

typedef enum party_code_t {
    GOOD_PARTY,
    MEDIOCRE_PARTY,
    EVIL_PARTY,
    MAX_PARTIES // 3
} party_code;

char *party_name[MAX_PARTIES] = {
    "GOOD PARTY",
    "MEDIOCRE PARTY",
    "EVIL PARTY"
};

static party_code get_vote(void)
{
    return rand()%MAX_PARTIES;   // between 0 and 2
}

typedef int hash;
typedef char *transaction;

typedef struct Block_T {
    hash previous_block_hash;
    hash block_hash; // uses all transactions in the blockchain
    transaction transactions;
} block_t;


// djb2 string hash function
int hash_string(void *string){
    int result = 5381;
    unsigned char *p;
    p = (unsigned char *)string;
    while ( *p != '\0' )
    {
        result = (result << 5) + result + *p;   // (result * 33) + *p
        ++p;
    }
    return result;
}

// linked list implementation
typedef struct {
    block_t info;
} DATA;

typedef struct node {
    DATA data;
    struct node *previous_node;
} NODE;

NODE *reverse(NODE *node);
void get_list_transactions(NODE *head, unsigned char *list_trans);

void add_node(NODE **node, DATA *data)
{
    // Placing 
    NODE *temp = malloc(sizeof(temp));
    temp->data = *data;
    temp->previous_node = *node;
    *node = temp;
    // printf("2_%p\n", (unsigned int *)*node);   
}

void free_node(NODE *node)
{
    NODE *temp = node;
    NODE *previous;
    while ( temp->previous_node != NULL )
    {
        previous = temp->previous_node;
        free(temp);
        temp = previous;
    }
}

void print_node_information(NODE *node)
{
    NODE *temp;
    puts("Printing blockchain...");
    puts("======================");
    temp = node;
    int indent = 0;
    while ( temp->previous_node != NULL )
    {
        printf("%*sPrevious hash\t%x\n", indent, "", temp->data.info.previous_block_hash);
        printf("%*sBlock hash\t%x\n", indent, "", temp->data.info.block_hash);
        printf("%*sTransaction\t%s\n", indent, "", temp->data.info.transactions);
        puts("");
        indent += 2;
        temp = temp->previous_node;
    }
}

int main(int argc, char **argv)
{
    // sets seed according to current time
    srand(time(NULL));

    int number_of_votes;
    switch ( argc )
    {
        case 1:
            number_of_votes = NVOTES;
            break;
        case 2:
            // argv[0] is the program execution (e.g. './dummyblockchain')
            number_of_votes = atoi(argv[1]); // atoi always return an integer
            if ( number_of_votes < 1 ){
                number_of_votes = NVOTES;
            }
            break;
        default:
            exit(EXIT_FAILURE);
    }
    printf("The number of votes is: %i\n", number_of_votes);

    // declaring a pointer to a struct does not mean it points to a point in
    // memory with correct byte allocation
    NODE *head = malloc(sizeof(head));
    DATA *element = malloc(sizeof(element));

    // GENESIS
    element->info.previous_block_hash = 0;
    element->info.transactions = party_name[get_vote()];
    element->info.block_hash = hash_string(element->info.transactions);
    add_node(&head, element);

    transaction trans_list = malloc(number_of_votes * sizeof(char)*24);
    for (int i = 1; i < number_of_votes; i++)
    {
        element->info.transactions = party_name[get_vote()];
        element->info.previous_block_hash = element->info.block_hash;
        strcat(trans_list, element->info.transactions);
        element->info.block_hash = hash_string(trans_list);
        add_node(&head, element);
        // printf("%i: %x\n", i, head->data.info.block_hash);

    }

    print_node_information(head);

    free_node(head);
    free(element);

    return 0;
}