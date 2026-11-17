#ifndef TREE_H
#define TREE_H

typedef char * tree_element_t;
#define TREE_SPEC "%s"
#define ARGUMENT_SPEC "%[A-Za-z _-,.?!;:' 0-9 \"\t]"

#include     <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include      <time.h>
#include      <math.h>
#include   <stdbool.h>
#include    <assert.h>
#include  <sys/stat.h>
#include    <unistd.h>
#include     <fcntl.h>
#include <sys/types.h>
#include     <ctype.h>
#include    <stdarg.h>

#include "log.h"
#include "term_colors.h"

FILE *tree_html_output = NULL;

// Defines and structures
//-------------------------------------------------------------------------------------------

enum ERR_CODE
{
    UNKNOWN_ERROR         = -1,
    NO_ERROR              = 0,
    GET_NULL_TREE_POINTER = 't'+'r'+'e'+'e',
    GET_NULL_NODE_POINTER,
    PRINT_NODE_RECURSION_LIMIT,
    FILE_DOES_NOT_EXIST,
    GET_NULL_STRING_TO_READ,
    INCORRECT_READ_FILE_FORMAT,
    INCORRECT_NAME
};

struct TreeN
{
    char data[MAX_STR_LEN];
    TreeN * parent;
    TreeN * left;
    TreeN * right;
};

enum TREE_SIDES
{
    RIGHT = 0,
    LEFT = 1
};

struct Tree
{
    TreeN * root;
    size_t element_count;
};


// Functions
//-------------------------------------------------------------------------------------------


Tree  * ReadTree                (const char  * file_name);
void    TreeVerify              (const Tree  * const tree);
void    PrintNodeAsTable        (const TreeN * const node_p);
void    PrintNode               (const TreeN * const node_p);
Tree  * TreeCreator             ();
void    TreeDestructor          (Tree *tree);
TreeN * NewNode                 (tree_element_t arg);
TreeN * FindTreeNodeByValueREC  (TreeN *root_node, tree_element_t arg);
TreeN * FinderInTree            (Tree *tree, tree_element_t find_val);
int     TreeAdd                 (Tree *tree, const tree_element_t arg, TreeN *parent, const TREE_SIDES side);
int     TreeDelete              (Tree *tree, TreeN *del_node, const TREE_SIDES replace);
int     WriteNode               (FILE *file, const TreeN *node);
int     WriteTree               (const char *file_name, const Tree *tree);
TreeN * ReadNode                (const char *tree_data, const Tree *tree, size_t *shift, TreeN *parent);
Tree  * ReadTree                (const char *file_name);


// ------------------- GraphViz -------------------
void CompPng(const char *name);
int PrintTreeNode(TreeN *node);
int PrintTreeGV_DEF(const Tree * const tree, const TreeN *marked_node, const char *name);

#define PrintTreeGV(tree_p, marked_node) PrintTreeGV_DEF(tree_p, marked_node, __FUNCTION__)
#define fp(string, ...)  fprintf(gv, string, ## __VA_ARGS__);
FILE *gv = NULL;
const size_t NODE_RECURSION_LIMIT = 10000;
// ------------------------------------------------




#endif