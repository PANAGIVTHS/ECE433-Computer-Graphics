Enum coords x = 0, y = 0;

dx = abs(points[1][x] - points[0][x]);
dy = abs(points[1][y] - points[0][y]);


// If p0 = p1
if (!(dx && dy)) {
	draw(points[0][x], points[0][y]);
	return;
}

num_pixels = max(dx, dy);
condSlope = dx >= dy;

if (condSlope) {
	coord1 = points[0][x];
	coord2 = points[0][y];
	endPointCoord = points[1][x];
	signCoord1 = sign(points[1][x] - points[0][x]);
	signCoord2 = sign(points[1][y] - points[0][y]);
	dcoord1 = dx;
	dcoord2 = dy;
	draw(coord1, coord2);
} else {
	coord1 = points[0][y];
	coord2 = points[0][x];
	endPointCoord = points[1][y];
	signCoord1 = sign(points[1][y] - points[0][y]);
	signCoord2 = sign(points[1][x] - points[0][x]);
	dcoord1 = dy;
	dcoord2 = dx;
	draw(coord2, coord1);
}

error = dcoord2 * 2 - dcoord1;

while (coord1 != endPointCoord) {
	error += 2 * dcoord2;
	coord1 += signCoord1;

	// Blepe video abdul gia na doume ti shmainei otan p >= 0
	// TODO: ejigise oti to special case dx = dy litourgei
	if (error >= 0) {
		error -= 2 * dcoord1;
		coord2 += signCoord2;
	}

	// Depending on m decide draw
	condSlope ? draw(coord1, coord2) : draw(coord2, coord1);
}