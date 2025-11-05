x_kentro = 0;
y_kentro = r;
x_shmeio = x0;
y_shmeio = y0;
error = 3 - 2r;

while (x_kentro <= y_kentro) { 
    draw(x_shmeio + x_kentro, y_shmeio + y_kentro);      // (x,y)
    draw(x_shmeio + y_kentro, y_shmeio + x_kentro);     // (y,x) 

    draw(x_shmeio - x_kentro, y_shmeio + y_kentro);     // (-x, y)
    draw(x_shmeio + y_kentro, y_shmeio - x_kentro);     // (y, -x)

    draw(x_shmeio - x_kentro, y_shmeio - y_kentro);     // (-x, -y)
    draw(x_shmeio - y_kentro, y_shmeio - x_kentro);     // (-y, -x)

    draw(x_shmeio + x_kentro, y_shmeio - y_kentro);     // (x, -y)
    draw(x_shmeio - y_kentro, y_shmeio + x_kentro);     // (-y, x)
        
    x_kentro += 1;
    if (error >= 0){
        y_kentro -= 1;
        error = error - (4 * y_kentro);
    }   
    error = error + (4*x_kentro) + 2;
}
