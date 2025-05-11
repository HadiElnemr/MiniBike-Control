clear all
close all
clc

y_theta_75 = load('lqr_output_75_cost=341.7198_normalQandR.mat');
y_theta_90 = load('lqr_output_90_cost=273.1727_normalQandR.mat');
y_theta_Q300 = load('lqr_output_90_cost=546.1975_Q300x3_andnormalR.mat');
y_theta_R100 = load('lqr_output_90_cost=5044.2769_normalQandR100.mat');
y_theta_QR = load('lqr_output_90_cost=5130.1350_Q300x3_andR100.mat');

fig1 = figure;

Ts = 0.020;
t = 0:Ts:3;


subplot(2,1,1)
hold on
plot(t(1:end-1), y_theta_75.y_theta(:,1), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_90.y_theta(:,1), 'b.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\phi (degrees)');
grid on
legend('\lambda = 75^\circ', '\lambda = 90^\circ');

subplot(2,1,2)
hold on
plot(t(1:end-1), y_theta_75.y_theta(:,2), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_90.y_theta(:,2), 'b.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\delta (degrees)');
grid on
legend('\lambda = 75^\circ', '\lambda = 90^\circ');
drawnow;

% exportgraphics(fig1, 'LQR_compare_models.pdf', 'ContentType', 'vector');
% print(fig1, '-dmeta', 'LQR_compare_models.emf');
% exportgraphics(fig1, 'LQR_compare_models.svg')
% set(fig1, 'Renderer', 'painters');
print(fig1, 'LQR_compare_models.svg', '-dsvg')

% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

fig2 = figure;

subplot(2,1,1)
hold on
plot(t(1:end-1), y_theta_90.y_theta(:,1), 'b.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_R100.y_theta(:,1), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_Q300.y_theta(:,1), 'g.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_QR.y_theta(:,1), 'k.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\phi (degrees)');
grid on
legend(...
    'Q = diag(300,0,300), R = 1', ...
    'Q = diag(300,0,300), R = 100', ...
    'Q = diag(300,300,300), R = 1', ...
    'Q = diag(300,300,300), R = 100');

subplot(2,1,2)
hold on
plot(t(1:end-1), y_theta_90.y_theta(:,2), 'b.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_R100.y_theta(:,2), 'r.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_Q300.y_theta(:,2), 'g.-', 'LineWidth', 1.5);
plot(t(1:end-1), y_theta_QR.y_theta(:,2), 'k.-', 'LineWidth', 1.5);
xlabel('Time (s)');
ylabel('\delta (degrees)');
grid on
legend(...
    'Q = diag(300,0,300), R = 1', ...
    'Q = diag(300,0,300), R = 100', ...
    'Q = diag(300,300,300), R = 1', ...
    'Q = diag(300,300,300), R = 100');

% exportgraphics(fig2, 'LQR_compare_QR.pdf', 'ContentType', 'vector');
% print(gcf, '-dmeta', 'LQR_compare_QR.emf');
print(fig2, 'LQR_compare_QR.svg', '-dsvg')
