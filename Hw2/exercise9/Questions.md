
1. GL_LINES for polygon edges or our own functions? And if GL_LINES do we use our own interpolation method to figure out the starting color of the y-th line when filling? (might not be as accurate if handled diff by gllines).
2. Do we draw the points as the scanline is happening or do we scan first and then call GL_LINES?
3. Can we skip first scanline calls to find Active Edge Table and do it another way?
4. Do we need to perform min max somewhere? Because active edge list already takes care of it.