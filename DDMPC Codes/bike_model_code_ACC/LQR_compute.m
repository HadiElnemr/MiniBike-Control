% This code is for testing the LQR Controller and getting its parameters.
% 1. Generate a sequence of data for noisy system 
% x_{t+1} = A_s x_t + B_s u_t + w_t, w_t satisfies instantaneous constraint
    
% clear workspace, close open figures
clear all
close all
clc

%% Parameters of the system

% state dimension and input dimension 
n = 3; 
m = 1; 

% Bike data
g = 9.8;
h = 0.088; % m (hieght of the centre of mass)
v = 0.634; % m/s or v = 0.634
a = 0.055; % m (distance between rear wheel and centre of gravity projection)
% b = a; %Test
w = 0.167; % m (Distance between front and rear wheels and ground contact points)
lambda =  75/180 * pi; % in rad = 70 degrees  (fork angle)
wheel_radius = 0.0375; % m (diameter is around 7.5cm)
r_tau = wheel_radius * tan(pi/2 - lambda); %(distance between front wheel and intersection of fork with ground)
l = w; % length

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% No Fork Model
% A_21 = g/h;
% A_23 = -v^2/(w*h);
% B_21 = -(b*v)/(w*h);
% A_c = [0 1 0;
%        A_21 0 A_23;
%        0 0 0];
% B_c = [0;
%        B_21;
%        1];
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Fork Model
A23 = -(v^2*h-a*r_tau*g) * sin(lambda) / (h^2*l); % added a negative

% true continuous system matrices
A_c = [0   1   0;
      g/h  0  A23;
       0   0   0];
B_c = [0 ; 
       - a*v*sin(lambda)/(h*l) ; % Try with negative: probably no
       1];

% K = -103.4593   -9.7400    8.7622
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

C = [1 0 0;
     0 0 1];
D = 0;

% sysc = ss(A_c,B_c, [1 0 0], 0);
% sysd = c2d(sysc, 0.01);
% A_s = sysd.A;
% B_s = sysd.B;
% C = [1 0 0];

Ts = 0.020;
t = 0:Ts:4;
u = zeros(size(t));
[G,H]= c2d(A_c, B_c, Ts); % [A_d, B_d] 
x0 = [0.0873; 0; 0];

Tc = ctrb(G,H);

if (rank(Tc)==3)
    fprintf('This system is controllable! \n');
    
    % Q matrix
    Q = [300 0 0; 0 0 0; 0 0 300];
    
    % R matrix
    R = 1;
    
    % Calculate state feedback coefficients (see calculated values for yourself)
    K = dlqr(G,H,Q,R);
    G2 = G-H*K;

    y = dlsim(G2,H,C,D,u,x0);
    subplot(2,1,1)
    plot(t,y(:,1),'b.-','LineWidth',1.5);
    xlabel('Time(s)');
    ylabel('\phi(rad)');
    grid on
    subplot(2,1,2)
    plot(t,y(:,2),'b.-','LineWidth',1.5);
    xlabel('Time(s)');
    ylabel('\delta(rad)');
    grid on
end
