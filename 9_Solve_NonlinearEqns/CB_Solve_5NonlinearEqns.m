%% Clear
close all
clear
clc
%% Parameters
global gamma psi mi R sigma L A_kappa S kB eVtoK ee
gamma = 7.5;
a = 0.20;
R = 0.65;
q = 3.5;
L = pi*q*R;
psi = a/(q*R);
mi = 1.67e-27;
sigma = 1e-19; % Effective cross-section
A_kappa = 2000;
S = 4*pi*pi*a*R;
kB = 1.38e-23;
eVtoK = 11604.519;
ee = 1.6e-19;

%% Inputs
global P_heat d_SOL n_S D_perp chi_perp q_perp
d_SOL = 0.08; % [m]
D_perp = 1.0; % [m**2/s]
chi_perp = D_perp*3.0;
P_heat = 1e6; %[W]
q_perp = P_heat/S;
n_S = 1e-1; % [10**20 cm**-3]

%% MAIN PART (COMPUTATION ROUTINE)
% Initial guess [T_S, T_L, n_L, Gamma_LCMS, delta]
%               [  1,   2,   3,          4,     5]
T_S0 = 20.0;
T_L0 = 10.0;
n_L0 = 10e-1;
Gamma_LCMS0 = 1.0e1; % [x10^20 1/(m^2 s)]
delta0 = 0.25;
x0 = [T_S0, T_L0, n_L0, Gamma_LCMS0, delta0];

fun = @TwoPoint;
% options1 = optimoptions('fsolve',...
%     'MaxFunctionEvaluations',70000,...
%     'MaxIterations',4000);
options2 = optimoptions('fsolve','Display','iter-detailed',...
    'MaxFunctionEvaluations',200000,...
    'MaxIterations',50000);
x = fsolve(fun,x0,options2);
% x = fsolve(fun,x0);
temp1 = sprintf('T_S = %.4f eV',x(1));
disp(temp1)
temp3 = sprintf('n_S = %.4e m**-3',n_S);
disp(temp3)
temp2 = sprintf('T_L = %.4f eV',x(2));
disp(temp2)
temp3 = sprintf('n_L = %.4e m**-3',x(3));
disp(temp3)
temp4 = sprintf('Gamma_LCMS = %.4e',x(4));
disp(temp4)
temp5 = sprintf('delta   = %.4f m\n',x(5));
disp(temp5)

disp('Shows residuals:')
RR = Residual(x);
temp1 = sprintf('Residuals = %.4e %.4e %.4e %.4e %.4e',RR(1),RR(2),RR(3),RR(4),RR(5));
disp(temp1)
%% Plot E_ionization
% m=1:1:10;
% a=m.*0;
% T=100;
% for i =1:length(m)
%     a(i) = Eionization(m(i),T);
% end
% plot(m,a,'o')

%% FUNCTION IMPLEMENTATION
function F = TwoPoint(x)
    global P_heat gamma psi mi sigma d_SOL n_S A_kappa S R L kB eVtoK q_perp
    global D_perp chi_perp

    V_S = sqrt(kB*2.0*x(2)*eVtoK/mi);
    E_i = Eionization(x(3),x(2))*eVtoK*kB;
    T_C = q_perp/(5.0*x(4)*1e20*kB*eVtoK);

    F(1) = P_heat/1e6 - 4*pi*R*x(5)*(gamma*x(2)*eVtoK*kB + E_i)*x(3)*1e20*V_S*sin(psi)/1e6;
%     F(2) = x(4)*1e20 - 4*pi*R*x(5)*x(3)*1e20*V_S*sin(psi)*exp(-x(3)*1e20*sigma*d_SOL);
    F(2) = x(4)/x(3) - 4*pi*R*x(5)*V_S*sin(psi)*exp(-x(3)*1e20*sigma*d_SOL);    
    F(3) = 2.0*x(3)*x(2) - n_S*x(1);
%     F(4) = 0.5*(T_C^(2.5))*(log((sqrt(T_C)+sqrt(x(1)))/(sqrt(T_C)-sqrt(x(1)))) ...
%         - log((sqrt(T_C)+sqrt(x(2)))/(sqrt(T_C)-sqrt(x(2))))) ...
%         - ((x(1))^2.5 - (x(2))^2.5)/5.0 - T_C*((x(1))^1.5-(x(2))^1.5)/3.0 ...
%         - T_C^2*(sqrt(x(1))-sqrt(x(2))) ...
%         - (5.0*L^2/A_kappa/x(5)/4.0)*x(4)*kB;

    F(4) = 0.5*(T_C^(2.5))*(log((1+sqrt(x(1)/T_C))/(1-sqrt(x(1)/T_C))) - log((1+sqrt(x(2)/T_C))/(1-sqrt(x(2)/T_C)))) ...
        - ((x(1))^2.5 - (x(2))^2.5)/5.0 - T_C*((x(1))^1.5-(x(2))^1.5)/3.0 ...
        - T_C^2*(sqrt(x(1))-sqrt(x(2))) ...
        - (5.0*L^2/A_kappa/x(5)/4.0)*x(4)*kB;    
    
    F(5) = x(5)*x(5) - (2*L/V_S)*(chi_perp + 3*D_perp)/(gamma + (E_i/(kB*x(2)*eVtoK)));
end
%%

function Eioniz=Eionization(n,T)
    % [Eioniz] = eV
    % [T] = eV
    % [n] = 10**14 cm**-3
    n1 = 5;
    n2 = 13.7;
    Eioniz=(35 - 21.4*exp(-n1/n))*exp((5.45/T)*exp(-(n/n2)^0.26));
end

function F = Residual(x)
    global P_heat gamma psi mi sigma d_SOL n_S A_kappa S R L kB eVtoK q_perp
    global D_perp chi_perp

    V_S = sqrt(kB*2.0*x(2)*eVtoK/mi);
    E_i = Eionization(x(3),x(2))*eVtoK*kB;
    T_C = q_perp/(5.0*x(4)*1e20*kB*eVtoK);
    
    temp1 = sprintf('T_C = %.4e eV',T_C);
    disp(temp1)

    F(1) = P_heat/1e6 - 4*pi*R*x(5)*(gamma*x(2)*eVtoK*kB + E_i)*x(3)*1e20*V_S*sin(psi)/1e6;
%     F(2) = x(4)*1e20 - 4*pi*R*x(5)*x(3)*1e20*V_S*sin(psi)*exp(-x(3)*1e20*sigma*d_SOL);
    F(2) = x(4)/x(3) - 4*pi*R*x(5)*V_S*sin(psi)*exp(-x(3)*1e20*sigma*d_SOL);
    F(3) = 2.0*x(3)*x(2) - n_S*x(1);
%     F(4) = 0.5*(T_C^(2.5))*(log((sqrt(T_C)+sqrt(x(1)))/(sqrt(T_C)-sqrt(x(1)))) ...
%         - log((sqrt(T_C)+sqrt(x(2)))/(sqrt(T_C)-sqrt(x(2))))) ...
%         - ((x(1))^2.5 - (x(2))^2.5)/5.0 - T_C*((x(1))^1.5-(x(2))^1.5)/3.0 ...
%         - T_C^2*(sqrt(x(1))-sqrt(x(2))) ...
%         - (5.0*L^2/A_kappa/x(5)/4.0)*x(4)*kB;
    F(4) = 0.5*(T_C^(2.5))*(log((1+sqrt(x(1)/T_C))/(1-sqrt(x(1)/T_C))) - log((1+sqrt(x(2)/T_C))/(1-sqrt(x(2)/T_C)))) ...
        - ((x(1))^2.5 - (x(2))^2.5)/5.0 - T_C*((x(1))^1.5-(x(2))^1.5)/3.0 ...
        - T_C^2*(sqrt(x(1))-sqrt(x(2))) ...
        - (5.0*L^2/A_kappa/x(5)/4.0)*x(4)*kB;     
    
    F(5) = x(5)*x(5) - (2*L/V_S)*(chi_perp + 3*D_perp)/(gamma + (E_i/(kB*x(2)*eVtoK)));
%     display(F)
end

