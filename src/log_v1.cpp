#ifndef _LOG
#define _LOG

#include <stdio.h>
#include "term_colors.h"

const size_t MAX_STR_LEN = 200;

void ClearTerm(void);

// WRITE LOG
//---------------------------------------------------------------------------------------------------------------------

#ifdef _DEBUG
// const char *LOG_FILE_NAME = "LOG.log";
// FILE *const log_file = fopen(LOG_FILE_NAME, "w");

    #define LOG(name, ...)  fprintf(log_file, name, ## __VA_ARGS__);
    #define LOG_FUNC_INFO   LOG("\n[INFO] [%s:%d] (%s) --> started...\n", file_name, line_num, __FUNCTION__)
    #define END_FUNC_LOG    LOG("[INFO] (%s) --> completed.\n\n", __FUNCTION__)
    #define CRUSH_FUNC_LOG  LOG("[WARN] (%s) --> CRUSHED!!!\n\n", __FUNCTION__)
    #define PAUSE ClearTerm();
    #define PR(name, ...)                                                                               \
        printf(name, ## __VA_ARGS__)
#else
    #define LOG(name, ...)
    #define LOG_FUNC_INFO
    #define END_FUNC_LOG
    #define CRUSH_FUNC_LOG
    #define PAUSE
    #define PR(name, ...)
#endif

// PRINTING
//---------------------------------------------------------------------------------------------------------------------

#ifdef _FUNCTION_STATE

#define PR_FST                                                                                      \
    printf("\\/----------------------------------------------------------------------------------\\/\n"); \
    printf(PYELLOW "- RUN %s:\n\n" DEF_COL, __FUNCTION__)
#define PR_FED                                                                                      \
    printf(PCYAN "\n- END %s;\n" DEF_COL, __FUNCTION__); ClearTerm();                               \
    printf("/\\----------------------------------------------------------------------------------/\\\n")
#else
    #define PR_FST
    #define PR_FED
#endif

//---------------------------------------------------------------------------------------------------------------------

int global_error = 0;

#ifdef _DEBUG
#define NAMED_ASSERT(pointer, err_code, name_of_element)                                                \
    char user_reply[MAX_STR_LEN] = "";                                                                  \
    if (global_error != 0)                                                                              \
        {                                                                                               \
            PR("\n\n--> Assert global error not equal 0.\n");                                           \
            PR("Called function \"%s\".\n", __PRETTY_FUNCTION__);                                       \
            PR(PRED "--> Global error = %d. Do you want to continue? (y/n)\n" DEF_COL, global_error);   \
            scanf("%s", user_reply);                                                                    \
            if (strncmp(user_reply, "yes", 1) == 0)                                                     \
                exit(global_error);                                                                     \
        }                                                                                               \
        if (pointer == NULL)                                                                            \
        {                                                                                               \
            global_error = err_code;                                                                    \
                                                                                                        \
            PR(PRED "\n\n--> \"%s\" get %s as null pointer!\n" DEF_COL, __FUNCTION__, name_of_element); \
            PR("Do you want crush program? (y/n)\n");                                                   \
            scanf("%s", user_reply);                                                                    \
            if (strncmp(user_reply, "yes", 1) == 0)                                                     \
                exit(err_code);                                                                         \
        }
#else
    #define NAMED_ASSERT(pointer, err_code, name_of_element) assert(pointer);
#endif

// clear stdin
void ClearTerm(void)
{
    int symbol = 0;
    while ((symbol != EOF) && (symbol != '\n'))
        {symbol = getchar();}
}

/// @brief calculate file size by stat structure
/// @param file_name file name
/// @return file size in bites
ssize_t SizeofFile(const char *file_name)
{
    PR_FST;
    assert(file_name);

    struct stat statistics = {};
    ssize_t fsize = -1;

    if (stat(file_name, &statistics) == 0)
    {
        fsize = (ssize_t)statistics.st_size;
        PR("Размер файла: %zd байт\n", fsize);
    }
    else 
    {
        perror("Ошибка при вызове stat");
    }

    PR_FED;
    return fsize;
}

// HTML
//---------------------------------------------------------------------------------------------------------------------

#define HTML_HEADER(html_output, lvl, text, ...) fprintf(html_output, "<h%d> " text, lvl, ## __VA_ARGS__); \
                                                 fprintf(html_output, " <br></h%d>\n", lvl)

#define HTML_PR(html_output, text, ...)          fprintf(html_output, "<p> " text " <p>", ## __VA_ARGS__)

FILE *InitHTML(const char *file_name)
{
    assert(file_name);

    // open file
    FILE *html_output = fopen(file_name, "w");
    if (html_output == NULL)
    {
        perror("HTML file wasn't opened.");
        return NULL;
    }

    // time to mark created file
    time_t current_time = {};
    assert(time(&current_time));

    // file parameters
    fprintf(html_output, "<!DOCTYPE html>\n <html lang=\"ru\">\n <head>\n  \
                          <meta charset=\"UTF-8\">\n <title> %s (%s) </title>\n </head>\n <body>\n", file_name, ctime(&current_time));

    // first header with file name and time
    HTML_HEADER(html_output, 1, "%s (%s)\n", file_name, ctime(&current_time));

    return html_output;
}

void CloseHTML(FILE *html_output)
{
    assert(html_output);

    fprintf(html_output, "\n</body>\n</html>");
    fclose(html_output);
}

void PicToHTML(FILE *html_output, const char *picture_name)
{
    assert(html_output);
    assert(picture_name);

    if (html_output)
    {
        fprintf(html_output, "<img src=\"%s\" alt=\"%s\" width=\"900\">\n", picture_name, picture_name);
    }

}

//---------------------------------------------------------------------------------------------------------------------

#endif