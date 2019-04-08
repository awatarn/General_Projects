function ydiff = Fitness_StretchedExpo(x)

global D LogRankNo

delta = x(1) - x(2)*abs(D).^x(3) - LogRankNo; % Row vector
ydiff = delta*transpose(delta);