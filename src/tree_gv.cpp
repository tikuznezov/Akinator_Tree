#include "tree.h"


// GraphViz
//-------------------------------------------------------------------------------------------

// void CompPng(const char *name)
// {
//     const size_t SYS_COMMAND_LENGTH = 100;
//     static size_t comp_pic_number = 1;
//     extern FILE *gv;
//     fclose(gv);
//         gv = NULL;
//         char sys_com[SYS_COMMAND_LENGTH];
//         snprintf(sys_com, SYS_COMMAND_LENGTH, "dot -Tsvg Tree.gv -o pictures/%s_%zu.svg", name, comp_pic_number++);
//         system(sys_com);
// }

const size_t SYS_COMMAND_LENGTH = 100;
void CompPng(const char *name)
{
    static size_t comp_pic_number = 1;
    if (gv)
    {
        fclose(gv);
        gv = NULL;
        char sys_com[SYS_COMMAND_LENGTH];
        snprintf(sys_com, SYS_COMMAND_LENGTH, "dot -Tsvg Tree.gv -o pictures/%s_%zu.svg", name, comp_pic_number);
        system(sys_com);
        snprintf(sys_com, SYS_COMMAND_LENGTH, "pictures/%s_%zu.svg", name, comp_pic_number);
        comp_pic_number++;
        if (tree_html_output != NULL)
        {
            // #define HTML_HEADER(html_output, lvl, text, ...) fprintf(html_output, "<h%d> " text "</h%d>", lvl, ## __VA_ARGS__, lvl);
            HTML_HEADER(tree_html_output, 6, "%s", sys_com);
            PicToHTML(tree_html_output, sys_com);
            // PR(PRED "Writing <%s> to HTML...\n" DEF_COL, sys_com);
        }
    }
}


// recursion
int PrintTreeNode(TreeN *node)
{
    if (node == NULL) return 0;
    static size_t recursion_iter = 0;

    // Create node
    fp("\n\tn%p [label=\"{Tree element %p.| " TREE_SPEC " | parent = %p |{ left = %p | right = %p }}\"];\n", \
            node,                       node, node->data,   node->parent,  node->left, node->right);

    if ((node->parent != NULL) && (((node->parent)->left != node) && ((node->parent)->right != node)))
        fp(  "\tn%p:n -> n%p:s [color = green];\n", node, node->parent);

    // Left descendant
    if (node->left == NULL)
    {
        // Create NULL descendant
        // fp("\tNULL_L_%p [label=\"NULL\", fillcolor = \"#000000\"];\n", node);
        // fp("\tn%p -> NULL_L_%p;\n", node, node);
    }
    else
    {
        fp("\tn%p -> n%p [taillabel = \"no\"];\n", node, node->left);
    }

    // Right descendant
    if (node->right == NULL)
    {
        // create NULL descendant
        // fp("\tNULL_R_%p [label=\"NULL\", fillcolor = \"#000000\"];\n", node);
        // fp("\tn%p -> NULL_R_%p;\n", node, node);
    }
    else
    {
        fp("\tn%p -> n%p [taillabel = \"yes\"];\n", node, node->right);
    }

    // break if infinite recursion
    if (++recursion_iter > NODE_RECURSION_LIMIT)
    {
        printf(PRED "--> Recursion call limit has been reached in %s after %zu iterations!\n" "Limit = %zu.\n" DEF_COL, __FUNCTION__, recursion_iter, NODE_RECURSION_LIMIT);
        global_error = PRINT_NODE_RECURSION_LIMIT;
        return PRINT_NODE_RECURSION_LIMIT;
    }
    // continue recursion
    else
    {
        PrintTreeNode(node->left );
        PrintTreeNode(node->right);
    }
    return 0;
}


// run recursion
int PrintTreeGV_DEF(const Tree * const tree, const TreeN *marked_node, const char *name)
{
    PR_FST;
    extern FILE *gv;
    gv = fopen("Tree.gv", "w");

    PR(PCYAN "Start printing to GV...");

    // graph info
    fp("\n\ndigraph %s {\n", name);
    fp("rankdir=TB; splines=ortho; nodesep = 1; bgcolor=\"#FFFFFF\";\n");
    fp("node [shape = Mrecord, style = filled, fillcolor = \"#626ae0\", fontcolor = \"#ffffff\", fontname=\"Helvetica-Bold\"];\n");

    // tree nodes
    PrintTreeNode(tree->root);

    // mark node
    if (marked_node != NULL) fp("\tn%p [fillcolor = green];\n", marked_node);

    fp("}");

    CompPng("tree_pic");

    PR(" Completed.\n\n" DEF_COL);

    PR_FED;
    return 0;
}