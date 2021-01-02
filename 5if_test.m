a = 1

if a==1
    disp('a는 1입니다')
elseif a==0
    disp('a는 0입니다')
else
    disp('a는 1이 아니고 0도 아닙니다')    
end

A = [1 2 3 1 0 1];

for (i=1:1:length(A))   % 배열인덱스 1부터
    if A(i)==1
        disp('A는 1입니다')
    elseif A(i)==0
        disp('A는 0입니다')
    else
        disp('A는 1이 아니고 0도 아닙니다')
    end
end

%% (예제)학점 계산기
score = input('성적을 입력하시오>>');

if score >= 90
    disp('학점 = A')

elseif score >= 80
    disp('학점 = B')

elseif score >= 70
    disp('학점 = C')
    
else
    disp('학점 = D')
    
end

