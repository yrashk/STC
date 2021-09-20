#include <stdio.h>

#define i_tag i
#define i_val int
#include <stc/clist.h>

void print_ilist(const char* s, clist_i list)
{
    printf("%s", s);
    c_foreach (i, clist_i, list) {
        printf(" %d", *i.ref);
    }
    puts("");
}

int main ()
{
    c_auto (clist_i, list1, list2)
    {
        c_emplace(clist_i, list1, {1, 2, 3, 4, 5});
        c_emplace(clist_i, list2, {10, 20, 30, 40, 50});
        print_ilist("list1:", list1);
        print_ilist("list2:", list2);

        clist_i_iter_t it = clist_i_advance(clist_i_begin(&list1), 2);
        it = clist_i_splice(&list1, it, &list2);

        puts("After splice");
        print_ilist("list1:", list1);
        print_ilist("list2:", list2);

        clist_i_splice_range(&list2, clist_i_begin(&list2), &list1, it, clist_i_end(&list1));

        puts("After splice_range");
        print_ilist("list1:", list1);
        print_ilist("list2:", list2);
    }
}