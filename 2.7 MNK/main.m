x = [1 0.95 0.9 0.8 0.7 0.6 0.5 0.4 0.3 0.12 0.08 0.05 0.01];
y = [3.374 2.42 1.737 0.898 0.458 0.243 0.126 0.068 0.034 0.009 0.006 0.002 0];

m = length(x);   % points number
n = 5;           % polynom deg

plot(x, y, '.');

% approximation
Sy = sum(y);
Sxy = sum(x.*y);
Sx2y = sum(x.^2.*y);
Sx3y = sum(x.^3.*y);
Sx4y = sum(x.^4.*y);
Sx5y = sum(x.^5.*y);

Vector = [Sx5y; Sx4y; Sx3y; Sx2y; Sxy; Sy];

Sx = sum(x);
Sx2 = sum(x.^2);
Sx3 = sum(x.^3);
Sx4 = sum(x.^4);
Sx5 = sum(x.^5);
Sx6 = sum(x.^6);
Sx7 = sum(x.^7);
Sx8 = sum(x.^8);
Sx9 = sum(x.^9);
Sx10 = sum(x.^10);

Matrix = [
    Sx10, Sx9, Sx8, Sx7, Sx6, Sx5;
    Sx9, Sx8, Sx7, Sx6, Sx5, Sx4;
    Sx8, Sx7, Sx6, Sx5, Sx4, Sx3;
    Sx7, Sx6, Sx5, Sx4, Sx3, Sx2;
    Sx6, Sx5, Sx4, Sx3, Sx2, Sx;
    Sx5, Sx4, Sx3, Sx2, Sx, m
];

% now we should solve SLAU
% Matrix * A = Vector  =>  A = (Matrix)^(-1) * Vector

A = Matrix \ Vector;
a5 = A(1);
a4 = A(2);
a3 = A(3);
a2 = A(4);
a1 = A(5);
a0 = A(6);

xApprox = min(x):0.01:max(x)
P = a5*xApprox.^5 + a4*xApprox.^4 + a3*xApprox.^3 + a2*xApprox.^2 + a1*xApprox + a0

hold on
plot(xApprox, P)

