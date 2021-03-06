% [x,y]=meshgrid(1:1:10,1:1:10);
% (10:1:20)
% dx =  (5*x.^4)/(1+x.^4) - x*y;
% dy =  (5*y.^4)/(1+y.^4) - x*y;
% quiver(x,y,dx,dy)

[x,y]=meshgrid(-3:.3:3,-3:.3:3);
dy =  - x*y;
dx = ones(size(y));
dyu = dy./sqrt(dx.^2+dy.^2);
dxu = dx./sqrt(dx.^2+dy.^2);
quiver(x,y,dxu,dyu)

