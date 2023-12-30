#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct nodedef {
    int number;
    struct node *previousNode;
    struct node *nextNode;
} node;

node *newNode(int value) {
    node *node1 = malloc(sizeof(node));
    node1->number = value;
    node1->previousNode = NULL;
    node1->nextNode = NULL;
    return node1;
}

typedef struct stackdef {
    node *headnode;
    node *lastnode;
    int size;
} argstack;

argstack *newStack() {
    argstack *stack1 = malloc(sizeof(argstack));
    stack1->headnode = NULL;
    stack1->lastnode = NULL;
    stack1->size = 0;
    return stack1;
}

node *push(argstack *stack, node *node) {
    if (stack->headnode == NULL)
        stack->headnode = node;
    else {
        node->previousNode = (struct node *) stack->lastnode;
        stack->lastnode->nextNode = (struct node *) node;
    }
    stack->lastnode = node;
    stack->size++;
    return node;
}

int pop(argstack *stack) {
    node *tail = stack->lastnode;
    if (tail->previousNode != NULL) {
        stack->lastnode = (node *) tail->previousNode;
        stack->lastnode->nextNode = NULL;
    } else {
        stack->headnode = NULL;
    }
    int result = tail->number;
    free(tail);
    stack->size--;
    return result;
}

void freeNodes(argstack *stack) {
    node *currentNode = stack->headnode;
    while (currentNode != NULL) {
        node *next = (node *) currentNode->nextNode;
        free(currentNode);
        currentNode = next;
    }
}

int isnumberarg(char *str) {
    if ((*str == '-' || *str == '+') && *(str + 1) != '\0') {
        str++;
    }
    while (*str != '\0') {
        if (!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}

void printstack(argstack *stack) {
    node *currentnode = stack->headnode;
    printf("Stack has nodes with value: ");
    while (currentnode != NULL) {
        node *nextNode = (node *) currentnode->nextNode;
        printf("%d ", currentnode->number);
        currentnode = nextNode;
    }
    printf("\n");
}

int executeoperation(argstack *stack, char *arg) {
    if (stack->size < 2) {
        fprintf(stderr, "Not enough numbers");
        return 0;
    }
    if (*(arg + 1) != '\0') {
        fprintf(stderr, "Invalid argument: %c\n", *arg);
        return 0;
    }
    switch (*arg) {
        case '+': {
            push(stack, newNode(pop(stack) + pop(stack)));
            break;
        }
        case '-': {
            push(stack, newNode(pop(stack) - pop(stack)));
            break;
        }
        case '*': {
            push(stack, newNode(pop(stack) * pop(stack)));
            break;
        }
        case '/': {
            push(stack, newNode(pop(stack) / pop(stack)));
            break;
        }
        default: {
            fprintf(stderr, "Invalid argument: %c\n", *arg);
            return 0;
        }
    }
    return 1;
}

void compute(int cnt, char **vals) {
    argstack *stack = newStack();
    for (int i = 0; i < cnt; i++) {
        char *arg = vals[i];
        if (isnumberarg(arg)) {
            push(stack, newNode(atoi(arg)));
        } else {
            executeoperation(stack, arg);
        }
    }
    printf("Final value is: %d\n", pop(stack));
    freeNodes(stack);
    free(stack);
}

int processargs(char **argv2, char *ptr) {
    int argc2 = 0;
    while (*ptr != '\0') {
        if (!isspace(*ptr)) {
            argv2[argc2++] = ptr;
            while (*ptr != '\0' && !isspace(*ptr))
                ptr++;
        } else {
            *ptr++ = '\0';
        }
    }
    return argc2;
}

int argcount(char *buf) {
    int cnt = 0;
    while (*buf != '\0') {
        if (!isspace(*buf)) {
            cnt++;
            while (*buf != '\0' && !isspace(*buf)) buf++;
        } else {
            buf++;
        }
    }
    return cnt;
}

int main(int argc, char *argv[]) {
    printf("%d\n", argc);
    if (argc == 1) {
        char buf[100];
        fgets(buf, 100, stdin);
        printf("Buffer %s\n", buf);

        char *argv2[argcount(buf)];
        int argc2 = processargs(argv2, buf);
        printf("argc2: %d\n", argc2);
        compute(argc2, argv2);
    } else {
        compute(argc - 1, argv + 1);
    }
    return 0;
}
