/* oslab 실행파일이 eoslab 파일명으로도 접근 가능하도록 한다
하드링크: 서로 다른 파일명으로 접근 가능하며, 한 쪽에서 수정하면, 다른 쪽의 파일도 수정된다 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
        if (argc < 3) {
                fprintf(stderr, "usage : %s<file><file>\n", argv[0]);
                exit(1);
        }

        if (link(argv[1], argv[2]) == -1) {		//실행파일 oslab을 새로운 이름으로 접근하도록 새로운 링크 생성
                fprintf(stderr, "link error for %s\n", argv[1]);
                exit(1);
        }
        exit(0);
}
