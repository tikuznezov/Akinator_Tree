#define _DEBUG

#include "tree.cpp"
#include "akinator.cpp"



// Main
//-------------------------------------------------------------------------------------------

int main(void)
{
    printf(PGREEN "\n\n------------------ RUN MAIN ------------------\n\n" DEF_COL);

    Tree *tree1 = ReadTree("strTree1.txt");
    // Tree *tree1 = TreeCreator();
    AkinatorRun(tree1);
    WriteTree("strTree1.txt", tree1);

    // tree_html_output = InitHTML("tree_debug.html");

    // TreeAdd(tree1, "porno", NULL, RIGHT);

    // TreeAdd(tree1, "prn1 gay porn", FinderInTree(tree1, "porno"), RIGHT);

    // WriteTree("strTree1.txt", tree1);

    // Tree *tree1CPY = ReadTree("strTree1.txt");
    // TreeVerify(tree1);


    // CloseHTML(tree_html_output);

    printf(PYELLOW "\n\n------------------ END MAIN ------------------\n" DEF_COL);
    return 0;
}