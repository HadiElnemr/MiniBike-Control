clear all
close all
clc

y_theta_FSB_75 = load('DDSFB_output_75_cost=341.7198.mat');
y_theta_FSB_90 = load('DDSFB_output_90_cost=273.1727.mat');
y_theta_MPC_75 = load('DDMPC_output_75_cost=341.8561.mat');
y_theta_MPC_90 = load('DDMPC_output_90_cost=273.2988.mat');
% y_theta_QR = load('');
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
y_theta1 = y_theta_FSB_90.y_theta;
y_theta2 = y_theta_MPC_90.y_theta;
m = 0
for i = 1:150
    m = max(m,y_theta1(i,1) - y_theta2(1,i))
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
fig1 = figure;

Ts = 0.020;
t = 0:Ts:3;


subplot(2,1,1)
hold on
plot(t(1:end-1), y_theta_FSB_75.y_theta(:,1), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_FSB_90.y_theta(:,1), 'b.-', 'LineWidth', 1.5);

xlabel('Time (s)');
ylabel('\phi (degrees)');
grid on
legend('\lambda = 75^\circ', '\lambda = 90^\circ');

subplot(2,1,2)
hold on
plot(t(1:end-1), y_theta_FSB_75.y_theta(:,2), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_FSB_90.y_theta(:,2), 'b.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\delta (degrees)');
grid on
legend('\lambda = 75^\circ', '\lambda = 90^\circ');
% drawnow;

% exportgraphics(fig1, 'DDFSB_compare_models.pdf', 'ContentType', 'vector');
print(fig1, 'DDFSB_compare_models.svg', '-dsvg')

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fig2 = figure;

subplot(2,1,1)
hold on
plot(t(1:end-1), y_theta_MPC_75.y_theta(1,:), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_MPC_90.y_theta(1,:), 'b.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\phi (degrees)');
grid on
legend('\lambda = 75^\circ', '\lambda = 90^\circ');

subplot(2,1,2)
hold on
plot(t(1:end-1), y_theta_MPC_75.y_theta(3,:), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_MPC_90.y_theta(3,:), 'b.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\delta (degrees)');
grid on
legend('\lambda = 75^\circ', '\lambda = 90^\circ');

% exportgraphics(fig2, 'DDMPC_compare_models.pdf', 'ContentType', 'vector');
print(fig2, 'DDMPC_compare_models.svg', '-dsvg')