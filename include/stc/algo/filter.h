/* MIT License
 *
 * Copyright (c) 2023 Tyge Løvset
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
/* 
#include <stdio.h>
#define i_val int
#include <stc/cstack.h>
#include <stc/algo/filter.h>

int main()
{
    c_with (cstack_int stk = c_make(cstack_int, {1, 2, 3, 4, 5, 6, 7, 8, 9}),
            cstack_int_drop(&stk))
    {
        c_foreach (i, cstack_int, stk)
            printf(" %d", *i.ref);
        puts("");
        
        c_forfilter (i, cstack_int, stk,
                        c_flt_skipwhile(i, *i.ref < 3) &&
                        (*i.ref & 1) == 0              && // even only
                        c_flt_take(i, 2))                 // break after 2
            printf(" %d", *i.ref);
        puts("");
    }
}
*/
#ifndef STC_FILTER_H_INCLUDED
#define STC_FILTER_H_INCLUDED

#include <stc/ccommon.h>

#define c_flt_skip(i, n) (c_flt_count(i) > (n))
#define c_flt_skipwhile(i, pred) ((i).b.s2[(i).b.s2top++] |= !(pred))
#define c_flt_take(i, n) _flt_take(&(i).b, n)
#define c_flt_takewhile(i, pred) _flt_takewhile(&(i).b, pred)
#define c_flt_count(i) ++(i).b.s1[(i).b.s1top++]
#define c_flt_last(i) (i).b.s1[(i).b.s1top - 1]

#define c_forfilter(i, C, cnt, filter) \
    c_forfilter_it(i, C, C##_begin(&cnt), filter)

#define c_forfilter_it(i, C, start, filter) \
    for (struct {struct _flt_base b; C##_iter it; C##_value *ref;} \
         i = {.it=start, .ref=i.it.ref} ; !i.b.done & (i.it.ref != NULL) ; \
         C##_next(&i.it), i.ref = i.it.ref, i.b.s1top=0, i.b.s2top=0) \
      if (!(filter)) ; else

// ------------------------ private -------------------------
#ifndef c_NFILTERS
#define c_NFILTERS 32
#endif

struct _flt_base {
    uint32_t s1[c_NFILTERS];
    bool s2[c_NFILTERS], done;
    uint8_t s1top, s2top;
};

static inline bool _flt_take(struct _flt_base* b, uint32_t n) {
    uint32_t k = ++b->s1[b->s1top++];
    b->done |= (k >= n);
    return k <= n;
}

static inline bool _flt_takewhile(struct _flt_base* b, bool pred) {
    bool skip = (b->s2[b->s2top++] |= !pred);
    b->done |= skip;
    return !skip;
}

#endif
