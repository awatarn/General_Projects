function y = PredictedPrice(b,x,t)

A = b(1);
B = b(2);
C = b(3);
tc    = x(1);
phi   = x(2);
omega = x(3);
z     = x(4);
y = A + B*abs(tc-t).^z + C*cos(omega*log(abs(tc - t)) + phi);