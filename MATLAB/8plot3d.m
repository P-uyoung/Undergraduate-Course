% linspace는 콜론 연산자 ":"과 유사하지만, 
% 점 개수를 직접 제어할 수 있으며 항상 끝점을 포함함
% 이름 "linspace"의 "lin"은 선형 간격 값을 생성하는 것을 의미
x = 1:10;
y = 1:10;

%% meshgrid 란? -->mesh/pcolor/contour
[X,Y] = meshgrid(x, y); 
% 벡터 x 및 y에 포함된 좌표를 바탕으로 2차원 그리드 좌표를 반환
% X는 각 행이 x의 복사본인 행렬이고, Y는 각 열이 y의 복사본인 행렬
% 좌표 X와 Y로 표현되는 그리드에는 length(y)개의 행과 length(x)개의 열이 있다

Z = X+Y;

mesh(X,Y,Z)

xlabel('x')
ylabel('y')
zlabel('z')


%% mesh
x = linspace(0, 2*pi, 100);
y = linspace(0, 2*pi, 100);

[X,Y] = meshgrid(x,y);

Z = cos(X) + sin(Y);

mesh(X,Y,Z)

xlabel('x')
ylabel('y')
zlabel('z')


%% pcolor
pcolor(X,Y,Z)

xlabel('x')
ylabel('y')
zlabel('z')

colorbar;
shading interp;


%% contour.. 등고선그래프
contour(X,Y,Z)	% contour(X,Y,Z,50).. 등고선 개수 50개
		% contourf().. 색 입힌 등고선 그래프
xlabel('x')
ylabel('y')
zlabel('z')

colorbar;