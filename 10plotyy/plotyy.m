[number, yy, mm, dd, hh, mmm, temper, psu] = textread('C:\Users\pc\Desktop\busan.txt','%u %u %u %u %u %u %f %f', 'headerlines', 6, 'delimiter', '/:-');

% 텍스트파일명 한글이면 오류
% M파일명 내장함수명과 동일하면 오류



%% 그래프 그리기

temper = flipud(temper);
plot(temper)
hold on

psu = flipud(psu);
plot(psu)
% 0값은 결측값을 나타냄
legend('temp', 'psu') % legend, 범례를 그래프에 추가



%% 오류나옴
% %% 두 그래프 보기 쉽게 비교하기(plotyy, 두 개의 y축)
% temper = flipud(temper);
% psu = flipud(psu);
% plotyy(1:3500, temper,1:3500, psu)
% legend('temper', 'psu')

