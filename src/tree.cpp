#ifndef _TREE_CPP
#define _TREE_CPP

#include "tree.h"
#include "tree_gv.cpp"

// #define _FUNCTION_STATE
// #define _DEBUG

// Functions
//-------------------------------------------------------------------------------------------


#define TreeVerify(tree_pointer)   TreeVerifyDEF(tree_pointer, __FUNCTION__)
void    TreeVerifyDEF              (const Tree * const tree, const char *funk_name)
{
    PR_FST;
    NAMED_ASSERT(tree, GET_NULL_TREE_POINTER, "tree");
    HTML_HEADER(tree_html_output, 3, "Starting tree verifying by \"%s\"...", funk_name);

    #ifdef _DEBUG
        if (tree->root == NULL)
        {
            HTML_PR(tree_html_output, "* Tree root is NULL pointer. Tree is void.\n");
            return;
        }

        PrintTreeGV(tree, NULL);
    #endif

    HTML_HEADER(tree_html_output, 4, "Finish tree verifying by \"%s\".", funk_name);
    PR_FED;
    return;
}

Tree *  TreeCreator             ()
{
    PR_FST;
    Tree *tree = (Tree *) calloc(1, sizeof(Tree));

    tree->root = NULL;
    tree->element_count = 0;

    TreeVerify(tree);

    PR_FED;
    return tree;
}

void    TreeDestructor          (Tree *tree)
{
    NAMED_ASSERT(tree, GET_NULL_TREE_POINTER, "tree to delete");

    *tree = {};
    free(tree);

    return;
}

TreeN * NewNode                 (tree_element_t arg)
{
    PR_FST;
    TreeN *new_node_address = (TreeN *) calloc(1, sizeof(TreeN));
    if (new_node_address == NULL)
    {
        perror("Can't get memory to new node.");
        return new_node_address;
    }
    assert(new_node_address);

    *new_node_address = {.parent = NULL, .left = NULL, .right = NULL};
    strcpy(new_node_address->data, arg);
    // PrintNode(new_node_address);

    PR_FED;
    return new_node_address;
}

TreeN * FindTreeNodeByValueREC  (TreeN *root_node, tree_element_t arg)
{
    // NAMED_ASSERT(root_node, GET_NULL_TREE_POINTER, "tree");
    TreeN *result = NULL;

    if (root_node != NULL)
    {
        if (!strncmp(root_node->data, arg, MAX_STR_LEN))
            result = root_node;
        if (result == NULL)
            result = FindTreeNodeByValueREC(root_node->left, arg);
        if (result == NULL)
            result = FindTreeNodeByValueREC(root_node->right, arg);
    }

    return result;
}

TreeN * FinderInTree            (Tree *tree, tree_element_t find_val)
{
    PR_FST;
    TreeVerify(tree);

    TreeN *find_pointer = NULL;
    HTML_PR(tree_html_output, "Finding <" TREE_SPEC "> element...\n", find_val);

    find_pointer = FindTreeNodeByValueREC(tree->root, find_val);

    if (find_pointer == NULL)
    {
        printf("Failed to find \"" TREE_SPEC "\" in tree. Element doesn't exist.\n", find_val);
    }
    else
    {
        printf("Element \"" TREE_SPEC "\" successfully found.\n", find_pointer->data);
    }

    HTML_PR(tree_html_output, "Find \"" TREE_SPEC "\" in tree.", find_pointer->data);
    PR_FED;
    return find_pointer;
}

int     TreeAdd                 (Tree *tree, const tree_element_t arg, TreeN *parent, const TREE_SIDES side)
{
    PR_FST;
    TreeVerify(tree);

    HTML_PR(tree_html_output, "Adding <" TREE_SPEC "> to the %s side...\n", arg, ((side == LEFT) ? "left" : "right"));
    HTML_PR(tree_html_output, "Parent name = \"%p\"\n", parent);
    // tree is void
    if (tree->root == NULL)
    {
        HTML_PR(tree_html_output, "Tree haven'n root. Creating root node.\n");
        tree->root = NewNode(arg);
    }
    else if (parent != NULL)
    {
        NAMED_ASSERT(parent, GET_NULL_NODE_POINTER, "PARENT NODE");
        if (((side == LEFT) ? parent->left : parent->right) == NULL)
        //  (select side to link new node                   link new node) -> add parent link;
            (((side == LEFT) ? parent->left : parent->right) = NewNode(arg)) -> parent = parent;
        else
            printf("Node already exists. Adding new node has been broke.\n");
    }
    else
    {
        HTML_PR(tree_html_output, "Parent is null pointer. Crush addiction (" TREE_SPEC ") node to tree.\n", arg);
    }

    TreeVerify(tree);
    PR_FED;
    return global_error;
}

#define LINK_FROM_PARENT_TO_DEC ((del_node != tree->root)                                                                        ? \
                                (((del_node->parent)->left == del_node) ? (del_node->parent)->left : (del_node->parent)->right)  : \
                                (tree->root)                                                                                       )

int     TreeDelete              (Tree *tree, TreeN *del_node, const TREE_SIDES replace)
{
    PR_FST;
    TreeVerify(tree);
    if (del_node == NULL)
    {
        printf("Try to delete node by <%p> address (or node doesn't exist).\n", del_node);
        return 0;
    }
    NAMED_ASSERT(del_node, GET_NULL_NODE_POINTER, "DELETED NODE");

    HTML_PR(tree_html_output, "Deleting node %p. \nParent = %p; Left = %p; Right = %p;\n", del_node, del_node->parent, del_node->left, del_node->right);



    if ((del_node->left == NULL) && (del_node->right == NULL))
    {
        HTML_PR(tree_html_output, "Deleting node without descendants...");
        // delete parent link
        LINK_FROM_PARENT_TO_DEC = NULL;
    }
    else if (del_node->left == NULL)
    {
        HTML_PR(tree_html_output, "Deleting node with only right descendant...");
        LINK_FROM_PARENT_TO_DEC = del_node->right;
        del_node->right->parent = del_node->parent;
    }
    else if (del_node->right == NULL)
    {
        HTML_PR(tree_html_output, "Deleting node with only left descendant...");
        LINK_FROM_PARENT_TO_DEC = del_node->left;
        del_node->left ->parent = del_node->parent;
    }
    else if ((del_node->left != NULL) && (del_node->right != NULL))
    {
        HTML_PR(tree_html_output, "Deleting node with two descendants...");
        if (replace == LEFT)
        {
            LINK_FROM_PARENT_TO_DEC = del_node->left;
            del_node->left->right   = del_node->right;
            del_node->right->parent = del_node->left;
            del_node->left->parent  = del_node->parent;
        }
        else if (replace == RIGHT)
        {
            LINK_FROM_PARENT_TO_DEC  = del_node->right;
            del_node->right->left    = del_node->left;
            del_node->left->parent   = del_node->right;
            del_node->right->parent  = del_node->parent;
        }
    }
    else
    {
        HTML_PR(tree_html_output, "--> Unknown delete case! Break deleting.\n");
        return -1;
    }

    *del_node = {};
    free(del_node);

    HTML_PR(tree_html_output, "\tDeleting finished.\n");
    TreeVerify(tree);
    return global_error;
}



// data export
//--------------------------------------------------------------------------------------
int     WriteNode (FILE *file, const TreeN *node)
{
    NAMED_ASSERT(file, FILE_DOES_NOT_EXIST, "FILE NAME");

    fprintf(    file, "("); // FIXME
    if (node == NULL)
    {
        fprintf(file, "NIL");
    }
    else
    {
        NAMED_ASSERT(node, GET_NULL_NODE_POINTER, "printing node");

        fprintf(    file, TREE_SPEC, node->data);
        // fprintf(    file, "(");
        WriteNode(  file, node-> left);
        // fprintf(    file, ")(");
        WriteNode(  file, node->right);
        // fprintf(    file, ")");
    }
    fprintf(    file, ")"); // FIXME

    return 0;
}

int     WriteTree (const char *file_name, const Tree  *tree)
{
    NAMED_ASSERT(file_name, FILE_DOES_NOT_EXIST, "FILE NAME");
    TreeVerify(tree);

    FILE *writing_file = fopen(file_name, "w");

    WriteNode(writing_file, tree->root);

    fclose(writing_file);

    return 0;
}

// data import
//--------------------------------------------------------------------------------------

// FIXME sep to functions!!!

int ReadNodeData(const char *tree_data, const Tree *tree, size_t *shift, TreeN *parent, char *arg, int *temp_shift, TreeN *node)
{
    return 0;
}

TreeN * ReadNode  (const char *tree_data, const Tree *tree, size_t *shift, TreeN *parent)
{
    NAMED_ASSERT(tree_data, GET_NULL_STRING_TO_READ, "read string");
    PR("Read node (tree data pointer = %p, tree pointer = %p, shift = %zu)\n", tree_data, tree, *shift);
    char parentheses[2]   = "";
    char arg[MAX_STR_LEN] = "";
    int temp_shift;
    TreeN *new_node = NULL;

    char node_data[MAX_STR_LEN] = "";

    PR("Check opend \"(\".");
    if (1 == sscanf(tree_data + *shift, "%1[(]%n", parentheses, &temp_shift))
    {
        *shift += (size_t)temp_shift;
        PR(" Skip %d symbols.\n", temp_shift);

        // Read node data
        // ReadNodeData();
        if (1 != sscanf(tree_data + *shift, ARGUMENT_SPEC"%n", arg, &temp_shift))
        {
            PR(PRED "Error in reading node argument. Incorrect format.\n" DEF_COL);
            global_error = INCORRECT_READ_FILE_FORMAT;
        }
        *shift += (size_t)temp_shift;
        PR("Get \"%s\". Skip %d symbols.\n", arg, temp_shift);

        if (!strncmp(arg, "NIL", 4))
        {
            PR("Get NIL. Return NULL;\n");
            new_node = NULL;
        }
        else if (1 == sscanf(arg, "%[A-Za-z _-,.?!;:' 0-9 \"\t]", node_data)) // FIXME akinator spec
        {
            PR("Creation new node...");
            new_node = NewNode(node_data);
            new_node->parent = parent;
            PR(" Completed.\n");
            PR("Call left node...");
            new_node->left  = ReadNode(tree_data, tree, shift, new_node);
            PR("Completed.\n");
            PR("Call right node...");
            new_node->right = ReadNode(tree_data, tree, shift, new_node);
            PR("Completed.\n");
        }
    }

    // check close ")"
    if (1 != sscanf(tree_data + *shift, "%1[)]%n", parentheses, &temp_shift))
    {
        PR(PRED "Read file format is incorrect!\n" DEF_COL);
        global_error = INCORRECT_READ_FILE_FORMAT;
    }
    *shift += (size_t)temp_shift;

    #ifdef _DEBUG
        Tree local_tree = {};
        local_tree.root = new_node;
        if ((new_node != NULL) && (new_node->left != NULL || new_node->right != NULL))
        {
            HTML_HEADER(tree_html_output, 3, "Writing new node from file:");
            PrintTreeGV(&local_tree, NULL);
            HTML_HEADER(tree_html_output, 4, "Writing completed");
        }
    #endif

    return new_node;
}

Tree  * ReadTree  (const char *file_name)
{
    NAMED_ASSERT(file_name, FILE_DOES_NOT_EXIST, "FILE NAME");

    Tree *tree = TreeCreator();
    TreeVerify(tree);

    size_t file_size = (size_t) SizeofFile(file_name);
    PR("File size = %zu;\n", file_size);

    int tree_file    = open(file_name, O_RDONLY);
    if (tree_file == -1)
        perror("Tree file wasn't opened.");
    PR("file opend.\n");

    char *tree_data  = (char *) calloc(file_size + 2, sizeof(char));
    if (tree_data == NULL)
        perror("Can't get enough memory to read file.");
    PR("Buffer created.\n");

    if (read(tree_file, tree_data, file_size) == -1)
        perror("Unsuccessful read file.");
    PR("File read.\n");

    size_t shift = 0;
    tree->root = ReadNode(tree_data, tree, &shift, NULL);

    return tree;
}

//--------------------------------------------------------------------------------------

void    PrintNodeAsTable        (const TreeN * const node_p)
{
    NAMED_ASSERT(node_p, GET_NULL_NODE_POINTER, "node");

    PR("|-----------------------|\n");
    PR("| POINTER: "     "%-12p" " |\n",    node_p);
    PR("|-----------------------|\n");
    PR("|    DATA: " TREE_SPEC "     |\n", node_p->data);
    PR("|-----------------------|\n");
    PR("|    LEFT: "     "%-12p" " |\n",    node_p->left);
    PR("|-----------------------|\n");
    PR("|   RIGHT: "     "%-12p" " |\n",    node_p->right);
    PR("|-----------------------|\n");
    PR("\n");


    return;
}
void    PrintNode               (const TreeN * const node_p)
{
    PR_FST;
    NAMED_ASSERT(node_p, GET_NULL_NODE_POINTER, "node");

    PR("Tree node [Address = %p; Data = " TREE_SPEC "; Left = %p; Right = %p]\n", node_p, node_p->data, node_p->left, node_p->right);
    PrintNodeAsTable(node_p);

    PR_FED;
    return;
}




#endif