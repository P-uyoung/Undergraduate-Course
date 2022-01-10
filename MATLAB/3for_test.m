for (i=1:1:10) %1부터 10까지 1씩 증가
    i
end

% '+=' 연산자 존재하지 않음
% 세미콜론생략.. printf역할
total = 0;
for (i=1:1:100)
    total = total + i;
end
total

total2 = 0;
for (i=-1:-1:-100)
    total2 = total2 + i;
end
total2