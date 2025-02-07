#include <stc/cstr.h>
#include <stc/algo/coroutine.h>
#include <errno.h>

// Read file line by line using coroutines:

struct file_nextline {
    const char* filename;
    int cco_state;
    FILE* fp;
    cstr line;
};

bool file_nextline(struct file_nextline* U)
{
    cco_begin(U)
        U->fp = fopen(U->filename, "r");
        U->line = cstr_init();

        while (cstr_getline(&U->line, U->fp))
            cco_yield(true);

        cco_final: // required label.
            printf("finish\n");
            cstr_drop(&U->line);
            fclose(U->fp);
    cco_end(false);
}

int main(void) {
    struct file_nextline z = {__FILE__};
    int n = 0;
    while (file_nextline(&z)) {
        printf("%3d %s\n", ++n, cstr_str(&z.line));
    }
}
