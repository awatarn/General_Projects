function Rsq = RSquare(y1, y2)

y = y1;
yCalc = y2;

Rsq = 1 - sum((y - yCalc).^2)/sum((y - mean(y)).^2);
end