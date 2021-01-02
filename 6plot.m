% 직선함수
plot([2,5,7],[1,2,3])

% cos함수
x = linspace(0,4*pi,1000); % pi, 정의된 상수
y = cos(x);
plot(x,y)   % plot(x, y, 'r:', 'LineWidth', 5) 빨간색 점선,굵게

xlabel('x축')    % fontsize : 20
ylabel('y축')
title('xy 그래프','fontsize', 20)

hold on % 그래프 같이 그리고 싶을 때

y2 = sin(x);
plot(x,y2)
 
hold off
xlim([0, 4*pi]) % x축 제한