#include "tree.h"

size_t MAX_AKINATOR_ACTIONS = 1000;

enum GUESS_STATE
{
    START = 23454,
    CONTINUE,
    BREAK,
    CORRECT_PERSON,
    INCORRECT_PERSON,
    CHECK_GUESS,
};

enum USER_ANSWERS
{
    ANS_UNKNOWN = -1,
    ANS_YES = RIGHT,
    ANS_NO  = LEFT,
};

struct MENU_OPTIONS
{
    int number;
    const char *name;
    GUESS_STATE game_state;
};

MENU_OPTIONS menu_options[] = {
    {.name = "Start game", .game_state = CONTINUE},
    {.name = "Exit",       .game_state = BREAK},
};

// Init functions
//-------------------------------------------------------------------------------------------

uint HashString(const char *string);
USER_ANSWERS GetUserAnswer();
GUESS_STATE CreateNewParameter(TreeN *node);
GUESS_STATE ShowMenu();
GUESS_STATE AskQuestion(Tree *atree, TreeN **current_node_address_pointer);
GUESS_STATE IsItCorrectAnswer(TreeN *ans_node);
GUESS_STATE IncorrectAnswer(Tree *atree, TreeN *node);
GUESS_STATE CreateRootQuestion(Tree *atree);


//-------------------------------------------------------------------------------------------



void AkinatorRun(Tree *atree)
{
    GUESS_STATE current_state = START;
    size_t iterations_counter = 0;
    TreeN *current_node = atree->root;

    while (current_state != BREAK)
    {
        if (iterations_counter++ > MAX_AKINATOR_ACTIONS)
        {
            HTML_HEADER(tree_html_output, 2, "The Akinator has reached the maximum allowed number of actions.");
            break;
        }

        switch (current_state)
        {
            case START:
            {
                PR(PYELLOW "Run start option.\n" DEF_COL);
                current_node = atree->root;
                current_state = BREAK;
                current_state = ShowMenu();
                break;
            }
            case CONTINUE:
            {
                current_state = BREAK;
                PR(PYELLOW "Run continue option.\n" DEF_COL);
                current_state = AskQuestion(atree, &current_node);
                break;
            }
            case CHECK_GUESS:
            {
                current_state = IsItCorrectAnswer(current_node);
                break;
            }
            case CORRECT_PERSON:
            {
                PR(PYELLOW "Run correct person option.\n" DEF_COL);
                printf("Congratulations! Going to the start menu.\n");
                current_state = START;
                break;
            }
            case INCORRECT_PERSON:
            {
                current_state = BREAK;
                PR(PYELLOW "Run incorrect person option.\n" DEF_COL);
                current_state = IncorrectAnswer(atree, current_node);
                break;
            }
            default:
            {
                current_state = BREAK;
                PR(PYELLOW "Run unknown option.\n" DEF_COL);
                break;
            }
        }
    }
}

// TODO hash, binsearch
#define CHECK_ANS(template, return_value) if (!strcmp(user_ans, template)) {PR("Get \"%s\" as answer.\n", user_ans); return return_value;}
USER_ANSWERS GetUserAnswer()
{
    char user_ans[MAX_STR_LEN] = "";
    while (!scanf("%s", user_ans))
        ClearTerm();
    ClearTerm();

    CHECK_ANS("yes",    ANS_YES);
    CHECK_ANS("y",      ANS_YES);

    CHECK_ANS("no",     ANS_NO);
    CHECK_ANS("n",      ANS_NO);

    PR("Unknown answer: \"%s\".\n", user_ans);
    return ANS_UNKNOWN;
}

// показывает меню, считывает действия в нем
GUESS_STATE ShowMenu()
{
    printf(PGREEN "----------------- Welcome to AKINATOR! -----------------\n" DEF_COL);
    printf(         "--> Select option:\n");
    for (int i = 0; i < sizeof(menu_options)/sizeof(menu_options[0]); i++)
    {
        menu_options[i].number = i + 1;
        printf(     "%2d) %s\n", menu_options[i].number, menu_options[i].name);
    }
    printf(         "Your choice (input option number): ");

    int user_choice = 0;
    while (!scanf("%d", &user_choice))
        ClearTerm();
    ClearTerm();

    PR(PCYAN "* User select %d option.\n", user_choice);
    user_choice--;

    PR("* Return %d state.\n" DEF_COL, menu_options[user_choice].game_state);
    return menu_options[user_choice].game_state;
}


// задает вопрос, принимает указатель на текущий узел
GUESS_STATE AskQuestion(Tree *atree, TreeN **current_node_address_pointer)
{
    assert(current_node_address_pointer);
    TreeVerify(atree);

    // if tree have no data
    if ((*current_node_address_pointer == atree->root) && (atree->root == NULL))
    {
        printf("Database is empty. You should enter the first parameter.\n");
        CreateRootQuestion(atree);
        *current_node_address_pointer = atree->root;
        return START;
    }

    assert(*current_node_address_pointer);

    // is it correct person?
    if (((*current_node_address_pointer)->left == NULL) && ((*current_node_address_pointer)->right == NULL))
        return CHECK_GUESS;

    printf("It is " TREE_SPEC "?\n", (*current_node_address_pointer)->data);
    // get user answer
    USER_ANSWERS ans = GetUserAnswer();

    switch (ans)
    {
        case ANS_NO:
        {
            PR(PCYAN "Get NO, select left side.\n" DEF_COL);
            *current_node_address_pointer = (*current_node_address_pointer)->left;
            return CONTINUE;
        }
        case ANS_YES:
        {
            PR(PCYAN "Get YES, select right side.\n" DEF_COL);
            *current_node_address_pointer = (*current_node_address_pointer)->right;
            return CONTINUE;
        }
        case ANS_UNKNOWN:
        {
            PR(PCYAN "Get UNKNOWN answer.\n" DEF_COL);
            return CONTINUE;
        }
        default: break;
    }

    return BREAK;
}


GUESS_STATE IsItCorrectAnswer(TreeN *ans_node)
{
    assert(ans_node);

    printf("Is it \"" TREE_SPEC "\"?\n", ans_node->data);
    USER_ANSWERS ans = GetUserAnswer();

    switch (ans)
    {
        case ANS_NO:        return INCORRECT_PERSON;
        case ANS_YES:       return CORRECT_PERSON;
        case ANS_UNKNOWN:   PR(PCYAN "Get UNKNOWN answer.\n" DEF_COL);
        default:            break;
    }
    return BREAK;
}



GUESS_STATE CreateNewParameter(TreeN *node)
{
    return BREAK;
}


#define GET_ANS_TO_BUF(buffer) while (!scanf(ARGUMENT_SPEC, buffer)) ClearTerm(); ClearTerm();

GUESS_STATE IncorrectAnswer(Tree *atree, TreeN *node)
{
    assert(node);
    TreeVerify(atree);

    char buffer_new_name[MAX_STR_LEN]     = "";
    char buffer_new_question[MAX_STR_LEN] = "";

    printf("What did you think of?\nIt is ");
    GET_ANS_TO_BUF(buffer_new_name);

    printf(TREE_SPEC ", unlike " TREE_SPEC" ", buffer_new_name, node->data);
    GET_ANS_TO_BUF(buffer_new_question);

    // no case
    TreeAdd(atree, buffer_new_name, node, RIGHT);

    // yes case
    TreeAdd(atree, node->data, node, LEFT);

    // change node
    strncpy(node->data, buffer_new_question, MAX_STR_LEN);

    TreeVerify(atree);
    return START;
}

GUESS_STATE CreateRootQuestion(Tree *atree)
{
    char buffer[MAX_STR_LEN] = "";

    // first question 
        printf("Input first question: Is it ");
        GET_ANS_TO_BUF(buffer);
        TreeAdd(atree, buffer, NULL, LEFT);

    // right node
        printf("Input YES character: ");
        GET_ANS_TO_BUF(buffer);
        TreeAdd(atree, buffer, atree->root, RIGHT);

    // lift node
        printf("Input NO character: ");
        GET_ANS_TO_BUF(buffer);
        TreeAdd(atree, buffer, atree->root, LEFT);

    HTML_HEADER(tree_html_output, 2, "Create first question.");
    PrintTreeGV(atree, atree->root);

    return START;
}

























































// Akinator
//-------------------------------------------------------------------------------------------

// USER_REPLY Ask(const char *question)
// {
//     printf("--> %s\n" PGREEN "Yes or no? (y/n): " DEF_COL);
//     char user_reply[MAX_STR_LEN] = "";

//     scanf("%s\n", user_reply);
//     if (!(strncmp(user_reply, "yes", 1)))
//     {
//         return YES;
//     }
//     else
//     {
//         return NO;
//     }
// }

// TreeN *WhoRecursion(TreeN *node)
// {
//     if ((node->left == NULL) || (node->right == NULL))
//     {
//         return ((node->left == NULL) ? node->left : node->right);
//     }
//     else
//     {
//         USER_REPLY side = Ask(node->data);
//         return WhoRecursion((side == NO) ? node->left : node->right);
//     }
//     return NULL;
// }


// int GuessWho(Tree *tree, const char *name)
// {
//     NAMED_ASSERT(name, INCORRECT_NAME, "NAME TO GUESS");
//     TreeVerify(tree);

//     GUESS_STATE current_state = START;

//     while (current_state != BREAK)
//     {
//         switch (current_state)
//         {
//             case START:
//             {
//                 // ShowMenu(&current_state);
//                 break;
//             }
//             case CONTINUE:
//             {
//                 // Ask();
//                 break;
//             }
//             case CORRECT_PERSON:
//             {
//                 break;
//             }
//             case INCORRECT_PERSON:
//             {
//                 break;
//             }
//             default:
//             {
//                 break;
//             }
//         }
//     }

//     return 0;
// }



uint HashString(const char *string)
{
    PR("* Hash (%8s) = ", string);
    uint len = strnlen(string, MAX_STR_LEN);

    unsigned long long hash = 0;
    unsigned long long p = 53;
    unsigned long long m = 2147483647;

    for (size_t i = 0; i < len; i++)
    {
        hash = (hash * p + (unsigned char)string[i]) % m;
    }

    PR("%lld;\n", hash);
    return (uint)hash;

}