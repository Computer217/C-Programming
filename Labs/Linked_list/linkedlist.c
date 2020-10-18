// Linked list demo program in CSE 3100
// The APIs may not be the best 

#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <ctype.h>

/***** error message *****/
enum ErrorNumber {ERR_OK, ERR_NOMEM, 
    ERR_NODELETE, ERR_NOTFOUND, 
    ERR_NOSORT, ERR_NOREVERSE, 
    ERR_LONGTOKEN, ERR_NONUMBER, ERR_UNKNOWNTOEKN,
    ERR_END};

// print an error message by an error number, and return
// the function does not exit from the program
// the function does not return a value
void    error_message(enum ErrorNumber errno)
{
    char    *messages[] = {"OK",
        "Memory allocaton failed.",
        "Deleting a node is not supported.",
        "The number is not on the list.",
        "Sorting is not supported.",
        "Reversing is not supported.",
        "Token is too long.",
        "A number should be specified after character d, a, or p.",
        "Token is not recognized.",
        "Invalid error number."};

    if (errno < 0 || errno > ERR_END)
        errno = ERR_END;
    printf("linkedlist: %s\n", messages[errno]);
}

/***** list *****/
typedef struct node_tag {
    int    v;
    struct node_tag * next; // A pointer to this type of struct
} node; 		    // Define a type. Easier to use.

node * new_node(int v) 
{
    node *p = malloc(sizeof(node)); // Allocate memory
    if (p == NULL) {
        error_message(ERR_NOMEM);
        exit(-1);
    }

    // Set the value in the node.
    p->v = v;			// you could do (*p).v
    p->next = NULL;  
    return p;			// return
}

node * prepend(node * head, node * newnode)
{
    newnode->next = head;
    return newnode;
}

node * find_node(node * head, int v)
{
    while (head != NULL) {
        if (head->v == v)
            return head;
        head = head->next;
    }
    return head;
}

node * find_last(node * head)
{
    if (head != NULL) {
        while (head->next != NULL) 
            head = head->next;
    }
    return head;
}

node * append(node * head, node * newnode)
{
    node *p = find_last(head);

    newnode->next = NULL;
    if (p == NULL)
        return newnode;
    p->next = newnode; 
    return head;
}

node * delete_list(node * head)
{
    while (head != NULL) {
        node *p = head->next;
        free(head);
        head = p;
    }
    return head;
}

void print_list(node *head)
{
    printf("[");
    while (head) {
        printf("%d, ", head->v);
        head = head->next;
    }
    printf("]\n");
}

void print_node(node * p)
{
    printf("%p: v=%-5d next=%p\n", p, p->v, p->next);
}

void print_list_details(node *head)
{
    while (head) {
        print_node(head);
        head = head->next;
    }
}

// functions that have not been implemented

node * delete_node(node * head, int dn)
{
    // TODO
    node *temp = head;
    node *prev;
    
    if(temp != NULL && temp->v == dn)
    {
    	head = temp->next;
    	free(temp);
    	return head;
    }
    
    while(temp != NULL && temp->v != dn)
    {
    	prev = temp;
    	temp = temp->next;
    }
    
    if(temp == NULL)
    {
    	error_message(ERR_NOTFOUND);
    	return head;
    }
    
    prev->next = temp->next;
    free(temp);
    return head;
}

/*
 * Given a pointer to the head node of an acyclic list, change the
 * next links such that the nodes are linked in reverse order.
 * Allocating new nodes or copying values from one node to another
 * is not allowed, but you may use additional pointer variables.
 * Return value is a pointer to the new head node.
 */
node * reverse_list (node * head)
{
    // TODO
    node *prev = NULL;
    node *current = head;
    node *next = NULL;
    
    while(current != NULL)
    {
    	next = current->next;
    	current->next = prev;
    	prev = current;
    	current = next;
    }
    
    head = prev;
  
    return head;
}

/*
node * sort_list (node * head)
{
    // TODO
    error_message(ERR_NOSORT);
    return head;
}
*/

/***** main *****/

// Make sure the numbers match in the following two macros
#define     MAX_TOKEN_LEN   32
#define     FORMAT_STR      "%32s"

void print_help(void);

int main(int argc, char **argv)
{
    int     res;
    char    token[MAX_TOKEN_LEN + 1]; // add 1 for NUL

    node *head = NULL;

    while (1) {
        res = scanf(FORMAT_STR, token);
        if (res == EOF) 
            break;
        if (! isspace(getchar())) {
            error_message(ERR_LONGTOKEN);
            exit(-1);
        }
        // puts(token);

        if (!strcmp(token, "help")) {
            print_help();
            continue;
        } 
        else if (!strcmp(token, "reverse") || !strcmp(token, "r")) {
            head = reverse_list (head);
        } 
        else if (!strcmp(token, "info") || !strcmp(token, "i")) {
            printf("head = %p\n", head);
            print_list_details(head);
        }
        // could support more functions/commands
        else {
            // try to convert it to an integer  
            // we use atol()
            long    lv;
            char    *endptr;

            char    action = 'a';
            char    *pn = token;

            if (token[0] == 'd' || token[0] == 'a' || token[0] == 'p') {
                if (! token[1]) {
                    error_message(ERR_NONUMBER);
                    continue;
                }

                action = token[0];
                pn ++;
            }

            lv = strtol(pn, &endptr, 10); // decimal numbers
            if (*endptr) { // the entire token should be a valid number
                error_message(ERR_UNKNOWNTOEKN);
                printf("%s\n", token);
                continue;
            }

            int     i = lv;

            switch (action) {
                case 'd': 
                    head = delete_node(head, i);
                    break;
                case 'a':
                case 'p':
                    {
                        node *p; 
                        p = find_node(head, i);
                        if (p != NULL) {
                            print_node(p);
                        }
                        else {
                            p = new_node(i); 
                            head = (action == 'p') ? prepend(head, p) : append(head, p);
                        }
                        break;
                    }
                default:
                    // should not be here.
                    break;
            }
        }
        print_list(head);
    }
    head = delete_list(head);
    return 0;
}

void print_help(void)
{
    // Example of long strings
    char * helpmsg = 
        "a<n> or <n>    append the number n to the list.\n"
        "               If n is alrady on the list, print the informaton about node.\n"
        "p<n>           prepend the number n to the list.\n"
        "               If n is alrady on the list, print the informaton about node.\n"
        "d<n>           delete the number n from the list.\n"
        "               if n is not found, print \"The number is not on the list.\"\n"
        "reverse or r   reverse the list.\n"
        "info or i      print the list.\n";
    puts(helpmsg);
}
