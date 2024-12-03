x = [1 2 3 4 5 6]';
t = (0:0.02:2*pi)';
A = [sin(t) sin(2*t) sin(3*t) sin(4*t) sin(5*t) sin(6*t)];
e = (-4+8*rand(length(t),1));
e(100:115) = 30;
y = A*x+e;
plot(t,y);

xhat = sdpvar(6,1);

e = y-A*xhat;
bound = sdpvar(length(e),1);
Constraints = [-bound <= e <= bound];
optimize(Constraints,sum(bound));
x_L1 = value(xhat);
bound = sdpvar(1,1);
Constraints  = [-bound <= e <= bound];
optimize(Constraints,bound);
x_Linf = value(xhat);


% optimize([],e'*e);
% x_L2 = value(xhat);
% 
% 
% bound = sdpvar(length(e),1);
% Constraints = [-bound <= e <= bound];
% optimize(Constraints,e'*e + sum(bound));
% 
% plot(t,[y A*x_L1 A*x_L2 A*x_Linf]);
% optimize([],norm(e,1));
% optimize([],norm(e,inf));
% optimize([],e'*e + norm(e,1));
% optimize([],norm(residuals,2));
% s = sdpvar(length(e),1);
% optimize([s == e],s'*s);
% R = chol(Q);
% s = sdpvar(length(x),1);
% optimize([s == R*x],s'*s);
% R = my_smart_factorization(Q);
% s = sdpvar(size(R,2),1);
% optimize([s == R*x],s'*s);
% s = sdpvar(1);
% optimize([s == sum(x)],s^2);
% optimize([],norm(R*e)^2);
% optimize([],e'*R'*R*e);
