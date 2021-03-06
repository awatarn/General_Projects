function y = simple_fitness(x)
global t yexp

y = transpose(x(1) - x(2)*(x(4)-t).^x(7) + ...
    x(3)*((x(4) - t).^x(7)).*cos(x(6)*log10(x(4) - t) + x(5)) - yexp)*...
    (x(1) - x(2)*(x(4)-t).^x(7) + ...
    x(3)*((x(4) - t).^x(7)).*cos(x(6)*log10(x(4) - t) + x(5)) - yexp);

