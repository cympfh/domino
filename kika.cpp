float cross (float x1, float y1, float x2, float y2) {
	return x1 * y2 - x2 * y1;
}

int ccw (float x1, float y1, float x2, float y2, float x0, float y0) {
	return cross(x1-x0, y1-y0, x2-x0, y2-y0) > 0 ? 1 : -1;
}
