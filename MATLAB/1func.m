a = [1 2 3; 4 5 6; 7 8 9; 10 11 12] % A 만들기

%reshape.. 세로로 읽어들임
b = reshape(a,1,12) % A행렬을 1x12행렬로 변환

%flipud.. 상하반전
d = flipud(a)

%fliplr.. 좌우반전
e = fliplr(a)

% '     ..행열반전(전치행렬)
f = a'

%size, length
g = size(a) % 4행3열
h = length(a) % a에서 가장 큰 배열 차원의 길이 반환

help length

% [1 2 3 4 5 6 7 8 9 10 11 12] 변환하기
i=reshape(a', 1, 12)