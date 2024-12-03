% clear workspace, close open figures
clear all
close all
clc 

% load('result0');
% 
% f1 = figure(1);
% %f1.Position = [10 70 550 300]; 
% plot(x(1,:),x(2,:),'k','LineWidth',0.6), grid on, hold on,
% xlabel('state $x(1)$','FontSize',12,'Interpreter','latex');
% ylabel('state $x(2)$','FontSize',12,'Interpreter','latex');
% drawnow
%    
% f2 = figure(2);
% %f2.Position = [560 70 550 300]; 
% plot(u,'k','LineWidth',0.6), grid on, hold on,
% xlabel('MPC iteration','FontSize',12,'Interpreter','latex');
% ylabel('input $u$','FontSize',12,'Interpreter','latex');
% drawnow

% load('result0_noise');
% 
% f1 = figure(1);
% plot(x(1,:),x(2,:),'b--','LineWidth',0.6), grid on, hold on,
% % legend('closed-loop state when $Q=I, R=1$','closed-loop state when $Q=I, R=10^{-4}$','location','southeast');
% legend('closed-loop state when $Q=I, R=10^{-4}$','location','southeast');
% set(legend,'Interpreter','latex');
% set(legend,'FontSize',12);
% xlabel('state $x(1)$','FontSize',12,'Interpreter','latex');
% ylabel('state $x(2)$','FontSize',12,'Interpreter','latex');
% set(gca,'FontSize',12);
% drawnow
%    
% 
% u_max = 10*ones(1, 300);
% 
% f2 = figure(2);
% plot(u,'b--','LineWidth',0.6), grid on, hold on,
% plot(u_max,'r','LineWidth',0.6), grid on, hold on,
% % legend('closed-loop input when $Q=I, R=1$','closed-loop input when $Q=I, R=10^{-4}$','input constraint','location','east');
% legend('closed-loop input when $Q=I, R=10^{-4}$','input constraint','location','east');
% set(legend,'Interpreter','latex');
% set(legend,'FontSize',12);
% xlabel('MPC iteration','FontSize',12,'Interpreter','latex');
% ylabel('input $u$','FontSize',12,'Interpreter','latex');
% set(gca,'FontSize',12);
% ylim([-2 12]);
% drawnow

load('result_R10-4_tau');
x1=x;
u1=u;
load('result_R10-4');

f1 = figure(1);
plot(x1(1,:),x1(2,:),'k','LineWidth',0.6), grid on, hold on,
plot(x(1,:),x(2,:),'b--','LineWidth',0.6), grid on, hold on,
legend('closed-loop state when $Q=I, R=1$','closed-loop state when $Q=I, R=10^{-4}$','location','southeast');
set(legend,'Interpreter','latex');
set(legend,'FontSize',12);
xlabel('state $x(1)$','FontSize',12,'Interpreter','latex');
ylabel('state $x(2)$','FontSize',12,'Interpreter','latex');
set(gca,'FontSize',12);
drawnow
   

u_max = 10*ones(1, 300);

f2 = figure(2);
plot(u1,'k','LineWidth',0.6), grid on, hold on,
plot(u,'b--','LineWidth',0.6), grid on, hold on,
plot(u_max,'r','LineWidth',0.6), grid on, hold on,
% legend('closed-loop input when $Q=I, R=1$','closed-loop input when $Q=I, R=10^{-4}$','input constraint','location','east');
legend('closed-loop input when $Q=I, R=1$','closed-loop input when $Q=I, R=10^{-4}$','input constraint','location','east');
set(legend,'Interpreter','latex');
set(legend,'FontSize',12);
xlabel('MPC iteration','FontSize',12,'Interpreter','latex');
ylabel('input $u$','FontSize',12,'Interpreter','latex');
set(gca,'FontSize',12);
ylim([-2 12]);
drawnow
