#include "DevelopmentKit.h"
DevelopmentKit::DevelopmentKit() {
	seg_cap = 10;
	seg_stack = (Segments**)malloc(sizeof(Segments*) * seg_cap);
	seg_pos = 0;
}
DevelopmentKit::~DevelopmentKit() {
	for (size_t i = 0; i < seg_pos; ++i)
		delete seg_stack[i];
	free(seg_stack);
}
Segments* DevelopmentKit::getSegments() {
	if (seg_pos == 0)
		return new Segments();
	return seg_stack[--seg_pos];
}
void DevelopmentKit::pushSegment(Segments* seg) {
	if (seg_pos == seg_cap) {
		Segments** m = (Segments**)realloc(seg_stack, sizeof(Segments*) * seg_cap * 2);
		if (m == NULL) {
			exit(1);
			return;
		}
		seg_stack = m;
		seg_cap *= 2;
	}
	seg->clear();
	seg_stack[seg_pos++] = seg;
}