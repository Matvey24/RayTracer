#include "Segments.h"
Segments::Segments() {
	s_cap = 10;
	s = (Segment_p*)malloc(sizeof(Segment_p) * s_cap);
	s_len = 0;
}
Segments::~Segments() {
	free(s);
}
bool func_union(bool a, bool b) {
	return a || b;
}
bool func_intersect(bool a, bool b) {
	return a && b;
}
bool func_subtract(bool a, bool b) {
	return a && !b;
}
void Segments::unio(Segments& oth, Segments& saveTo) {
	makeOperation(oth, func_union, saveTo);
}
void Segments::intersect(Segments& oth, Segments& saveTo) {
    if (oth.s_len == 0 || s_len == 0)
        return;
	makeOperation(oth, func_intersect, saveTo);
}
void Segments::subtract(Segments& oth, Segments& saveTo) {
    if (s_len == 0)
        return;
    if (oth.s_len == 0) {
        for (size_t i = 0; i < s_len; ++i) {
            saveTo.add(s[i]);
        }
        return;
    }
	makeOperation(oth, func_subtract, saveTo);
}
void Segments::clear() {
	s_len = 0;
}
bool Segments::isEmpty() {
	return s_len == 0;
}
void Segments::makeOperation(Segments& oth, bool func(bool, bool), Segments& saveTo) {
    SPoint *pos;
    SPoint *last_pos = 0;
    SPoint *n_pos, *n_pos_2;
    int nx1 = 0, nx2 = 0;
    bool seg_active = false;
    bool tsa = false, osa = false;
    bool n_end = false, n_end_2 = false;
    bool hnt = true, hno = true;
    bool next_cycle = true;
    do {
        n_pos = hnt ? &(n_end ? s[nx1].e : s[nx1].s) : NULL;
        n_pos_2 = hno ? &(n_end_2 ? oth.s[nx2].e : oth.s[nx2].s) : NULL;
        if (!hno || hnt && n_pos->pos < n_pos_2->pos) {
            if (n_end) {
                nx1++;
                hnt = nx1 != s_len;
                next_cycle = hnt || nx2 != oth.s_len;
            }
            n_end = !n_end;
            tsa = n_end;
            pos = n_pos;
        }
        else {
            if (n_end_2) {
                nx2++;
                hno = nx2 != oth.s_len;
                next_cycle = nx1 != s_len || hno;
            }
            n_end_2 = !n_end_2;
            osa = n_end_2;
            pos = n_pos_2;
        }
        if (func(tsa, osa) && next_cycle) {
            if (!seg_active) {
                seg_active = true;
                last_pos = pos;
            }
        }
        else if (seg_active) {
            seg_active = false;
            Segment_p seg(*last_pos, *pos);
            saveTo.add(seg);
        }
    } while (next_cycle);
}
void Segments::add(Segment_p& p) {
    if (s_len == s_cap) {
        Segment_p* m = (Segment_p*)realloc(s, sizeof(Segment_p) * s_cap * 2);
        if(m == NULL){
            m = (Segment_p*)realloc(s, sizeof(Segment_p) * (s_cap + 1));
            if (m == NULL) {
                exit(1);
                return;
            }
            s_cap++;
        }
        else {
            s_cap *= 2;
        }
        s = m;
    }
    s[s_len++] = Segment_p(p);
}
SPoint *Segments::getFirstActive() {
    for (size_t i = 0; i < s_len; ++i) {
        Segment_p &seg = s[i];
        if (seg.s.pos > 0)
            return &seg.s;
        if (seg.e.pos > 0)
            return &seg.e;
    }
    return NULL;
}