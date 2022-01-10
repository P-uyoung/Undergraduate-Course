x = linspace(0, 10*pi, 1000);
y = cos(x);
k = 10*pi/6;

for(i=1:1:6)
    subplot(2,3,i) % 2행3열의 i번째 plot
    plot(x(i*100:100*(i+1)),y(i*100:100*(i+1))) % 배열의 인덱스.. 1000개 중에
end
  