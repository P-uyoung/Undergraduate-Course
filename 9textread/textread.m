[a,b,c] = textread('C:\Users\pc\Desktop\test.txt', '%u %u %c')

[A,B,C] = textread('C:\Users\pc\Desktop\test2.txt', '%u %u %c', ...
    'headerlines', 1, 'delimiter', ',')