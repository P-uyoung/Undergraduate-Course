#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "ssu_score.h"
#include "blank.h"

extern struct ssu_scoreTable score_table[QNUM];
extern char id_table[SNUM][10];

struct ssu_scoreTable score_table[QNUM];
char id_table[SNUM][10];

char stuDir[BUFLEN];
char ansDir[BUFLEN];
char errorDir[BUFLEN];
char threadFiles[ARGNUM][FILELEN];	//-t option, 문제파일명
char iIDs[ARGNUM][FILELEN];		//-i option, 학번

//옵션 플래그
int eOption = false;
int tOption = false;
int mOption = false;
//int iOption = false;

//채점 프로그램 시작
void ssu_score(int argc, char *argv[])
{
	char saved_path[BUFLEN];		//작업 디렉토리 문자열을 담을 버퍼
	int i;

	for(i = 0; i < argc; i++){	//-h 옵션이면,
		if(!strcmp(argv[i], "-h")){	//usage 찍어줌
			print_usage();
			return;
		}
	}

	memset(saved_path, 0, BUFLEN);		// 0으로 초기화
	if(argc >= 3) {
		strcpy(stuDir, argv[1]);
		strcpy(ansDir, argv[2]);
	}

	if(!check_option(argc, argv))	//옵션 체크
		exit(1);

	//stuDir 존재하는지 확인
	getcwd(saved_path, BUFLEN);		//chdir 하기 전, 현재 작업 디렉토리 saved_path 버퍼에 저장

	if(chdir(stuDir) < 0){			//stuDir로 작업 디렉토리를 변경
		fprintf(stderr, "%s doesn't exist\n", stuDir);
		return;
	}
	getcwd(stuDir, BUFLEN);

	chdir(saved_path);			//원래대로 돌리기

	//ansDir 존재하는지 확인
	if(chdir(ansDir) < 0){
		fprintf(stderr, "%s doesn't exist\n", ansDir);
		return;
	}
	getcwd(ansDir, BUFLEN);

	chdir(saved_path);			//원래대로 돌리기

	set_scoreTable(ansDir);			//점수테이블 있는지 확인 후, 없으면 생성
	set_idTable(stuDir);			//학번테이블 생성

	i = 0;

	printf("grading student's test papers..\n");
	score_students();		//채점

/*	if(iOption != false)	//-i 옵션이 다른 옵션과 함께 나올 때
		do_iOption();
*/
	return;
	
}

int check_option(int argc, char *argv[])
{
	int i, j;
	int c;			//getopt() 리턴값

	while((c = getopt(argc, argv, "e:thmi")) != -1)		//옵션종류 :e,t,h,m,i
	{
		switch(c){
			case 'e':	//옵션 e인 경우 (에러메세지 출력)
				eOption = true;
				strcpy(errorDir, optarg);	//optarg -옵션 뒤 파라미터인 error를 문자열로 저장

				if(access(errorDir, F_OK) < 0)	//프로세스 접근 전, 파일이 존재하는지 확인
					mkdir(errorDir, 0755);	//존재하지 않는다면, 755모드로 디렉토리 만듦
				else{
					rmdirs(errorDir);	//존재하면, 삭제하고, 755모드로 다시 만듦
					mkdir(errorDir, 0755);
				}
				break;
			case 't':	//옵션 t인 경우
				tOption = true;
				i = optind;			//optind -다음 처리할 옵션의 인덱스
				j = 0;

				while(i < argc && argv[i][0] != '-'){	//옵션문자가 아닌(-로 시작하지 않는) 문자(열)

					if(j >= ARGNUM)		//인자 5개 초과시
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);
					else
						strcpy(threadFiles[j], argv[i]);
					i++; 
					j++;
				}
				break;
	/*		case 'i':
				iOption = true;
				i = optind;
				j = 0;

				while(i < argc && argv[i][0] != '-'){	//인자 5개 초과시

					if(j >= ARGNUM)
						printf("Maximum Number of Argument Exceeded.  :: %s\n", argv[i]);
					else
						strcpy(iIDs[j], argv[i]);
					i++; 
					j++;
				}
				if(!strcmp(argv[1], "-i")) {	//-i 옵션 단독
					do_iOption();
					return false;
				}
				break;*/
			case 'm' :
				mOption = true;
				break;
			case '?':
				printf("Unkown option %c\n", optopt);
				return false;
		}
	}

	return true;
}
void do_mOption(char *path)
{
	FILE *fp;
	int fd;
	int fsize, offset, now = 0; 	//파일 오프셋 관련 변수
	char ch_qname[FILELEN], ch_score[BUFLEN];		//점수를 바꿀 파일명, 바꿀 점수
	char *head, *tail;
	double tmp;
	

	while(1) {
		if((fp = fopen(path, "r")) == NULL) {
			fprintf(stderr, "file open error for %s\n", path);
			return;
		}

		fseek(fp, 0, SEEK_END);
		fsize = ftell(fp);	//파일사이즈 측정
		fseek(fp, 0, SEEK_SET);	//맨 앞으로 이동

		printf("Inuput question's number to modify >> ");
		scanf("%s", ch_qname);

		if(!strcmp(ch_qname, "no"))	//"no"를 입력받으면
			break;		
	}
	
}	
/*
void do_iOption(void)
{
	FILE *fp;
	char tmp[BUFLEN];
	char *qname;		//각 문제의 파일명
	char qlist[QNUM][FILELEN];		//문제파일명 리스트를 담음
	int qnum = 0;			//문제 개수
	int i, j = 0;
	int exist; 		//틀린문제를 출력할 학생의 학번이 존재하는 학번인가
	char *id, *score;
	double d_score;
	int size = sizeof(iIDs) / sizeof(iIDs[0]);

	if((fp = fopen("score.csv", "r")) == NULL){		//score.csv 파일을 읽기모드로 열기
		fprintf(stderr, "file open error for score.csv\n");
		return;
	}

	fscanf(fp, "%s\n", tmp);		//score.csv 파일 제목쉘 문자열 tmp배열에 저장
	qname = strtok(tmp, ",");		//문제파일명 차례대로 qname에 저장
	
	while(strcmp(qname, "sum")) 		//sum 전, 문제파일명 나올때까지만
	{
		strcpy(qlist[qnum++], qname);	
		qname = strtok(NULL, ",");
	}//문제파일명 qlist에 저장	

		
	//score.csv에서, 한 학생씩 read
	while(strcmp(iIDs[i], "") && i < 5)  {
		while (fscanf(fp, "%s\n", tmp) != EOF) {
		 	id = strtok(tmp, ",");	//tmp에 저장된 문자열을 id에
			 
		 	if(!is_exist(iIDs, tmp))
				 continue;

		 	printf("%s's wrong answer :\n", tmp);
		 	score = strtok (NULL, ","); 	//학번 다음, 문제당 맞은 점수 저장

			//틀린문제 
		 	while (qnum--) {
			 	d_score = atof(score);
			 	if(d_score == 0) 
			       		printf("%s ", qlist[j++]);
				score = strtok(NULL,",");	//점수 마저읽기
			}
				 
			printf("\n");
			break;	
		}
		fseek(fp, 0, SEEK_SET);		//파일의 맨 앞으로 커서 이동	
		fscanf(fp, "%s\n", tmp); 	//제목 쉘 다음으로
		i++;
	}

	fclose(fp);
}
*/

//학생(학번)이 존재하는지 판단하는 함수
int is_exist(char (*src)[FILELEN], char *target)
{
	int i = 0;

	while(1)
	{
		if(i >= ARGNUM)		// main에서 받은 인자가 0일 때
			return false;
		else if(!strcmp(src[i], ""))	//ids의 첫문자열이 " " 일 때
			return false;
		else if(!strcmp(src[i++], target))	//ids의 문자열이 tmp의 문자열과 같을때 
			return true;
	}
	return false;
}

//점수테이블 "score_talbe.csv" 생성
void set_scoreTable(char *ansDir)
{
	char filename[FILELEN];
	
	sprintf(filename, "%s", "score_table.csv");

	if(access(filename, F_OK) == 0)		//"scoretable.csv" 파일이 존재한다면, read_() 함수
		read_scoreTable(filename);
	else{
		make_scoreTable(ansDir);	//없다면, make_() 로 만들고, write()
		write_scoreTable(filename);
	}

	if(mOption) {
		do_mOption(filename);
	}
}

//기존의 점수테이블에서 문제파일명과 배점을 읽음
void read_scoreTable(char *path)
{
	FILE *fp;
	//ssu_scoreTable 구조체 필드변수
	char qname[FILELEN];	
	char score[BUFLEN];
	int idx = 0;		//학생(id)의 인덱스

	if((fp = fopen(path, "r")) == NULL){	//읽기모드로 filename 열기
		fprintf(stderr, "file open error for %s\n", path);
		return ;
	}

	while(fscanf(fp, "%[^,],%s\n", qname, score) != EOF){  /* filename에서 서식지정자에 맞게 qname, score 읽어옴 
								  (','전까지의 문자열)qname, score를 저장 */
		strcpy(score_table[idx].qname, qname);		//filename에서 읽은 qname을 구조체 필드에 할
		score_table[idx++].score = atof(score);		//문자열로 지정된 score를 숫자값으로 변경 후 할당 > idx + 1
	}

	fclose(fp);
}

//ansDir 인자로 받아, 문제당_배점 테이블 만든다
void make_scoreTable(char *ansDir)
{
	int type, num;
	double score, bscore, pscore;		// ,blank score, program score
	struct dirent *dirp;		//dirent 구조체 
	DIR *dp;
	char tmp[BUFLEN];
	int idx = 0;				//score_table의 인덱스
	int i;

	num = get_create_type();	//select type : 1 / 2

	if(num == 1)	//같은 문제형식은 같은 배점
	{
		printf("Input value of blank question : ");
		scanf("%lf", &bscore);		//빈칸문제 배점
		printf("Input value of program question : ");
		scanf("%lf", &pscore);		//프로그램 배점
	}

	//score_table 의 qname 할당하기 위한 작업
	if((dp = opendir(ansDir)) == NULL){		//DIR * 형 변수 dp :ansDir 디렉토리를 가리킴
		fprintf(stderr, "open dir error for %s\n", ansDir);
		return;
	}	

	while((dirp = readdir(dp)) != NULL)		//dirent 구조체 dirp :dp가 가리키는 디렉토리의 정보를  가져옴 
	{
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))	//dp 가 가리키는 디렉토리가 현재디렉토리 혹은 부모디렉토리에 있지 않을 때
			continue;	//바로 readdir(dp) 다른 파일 혹은 서브디렉토리 읽는다
		

		if((type = get_file_type(dirp->d_name)) < 0)		//확장자명 .txt / .c  둘 중에 하나여야 함
				continue;

		strcpy(score_table[idx++].qname, dirp->d_name);		//dirent 파일명 필드를 score_table[idx]에 해당 문제이름 할당

	}
	
	closedir(dp);		//score_table[].qname 차례대로 할당 한 후 temp 닫기
	sort_scoreTable(idx);		//문제이름 오름차순으로 정렬
	


	//각 qname의 타입에 맞는 score 값(배점) 할당
	for(i = 0; i < idx; i++)
	{
		type = get_file_type(score_table[i].qname);

		if(num == 1)	//select type 1 인 경우(각각의 빈칸/프로그램 문제 배점 동일)
		{
			if(type == TEXTFILE)
				score = bscore;
			else if(type == CFILE)
				score = pscore;
		}
		else if(num == 2)	//select type 2 인 경우 (각 문제별로 배점이 다름)
		{
			printf("Input of %s: ", score_table[i].qname);
			scanf("%lf", &score);		//사용자에게 각 문제당 배점 입력받음
		}

		score_table[i].score = score;		//score_table[].score :문제별 배점 입력
	}
}

//filename 파일명을 만들어, score_talbe 필드를 temp 버퍼로 write
void write_scoreTable(char *filename)		//filename = ssu_table.csv
{
	int fd;
	char tmp[BUFLEN];
	int i;
	int num = sizeof(score_table) / sizeof(score_table[0]);		//score_table 인덱스 수

	if((fd = creat(filename, 0666)) < 0){			//filename 파일 만듦
		fprintf(stderr, "creat error for %s\n", filename);
		return;
	}

	for(i = 0; i < num; i++)
	{
		if(score_table[i].score == 0)		//배점 0 인 경우, break 
			break;

		sprintf(tmp, "%s,%.2f\n", score_table[i].qname, score_table[i].score);
		write(fd, tmp, strlen(tmp));		//filename 파일에 tmp 버퍼 출력(쓰기)
	}						/* e) 1-1.txt,0.5
							      1-2.txt,0.5 ... */		
	close(fd);
}

//stuDir 에서 2차 배열 id_table(학생들 학번) 할당 
void set_idTable(char *stuDir)
{
	struct stat statbuf;	//stuDir 디렉토리 내부, 학생 파일의 종류 확인하기 위함
	struct dirent *dirp;
	DIR *dp;
	char tmp[BUFLEN];
	int num = 0;

	if((dp = opendir(stuDir)) == NULL){		//DIR * 변수 dp :stuDir 디렉토리를 가리킴
		fprintf(stderr, "opendir error for %s\n", stuDir);
		exit(1);
	}

	while((dirp = readdir(dp)) != NULL){		//dirent 구조체 dirp :dp 가 가리키는 디렉토리(stuDir)의 정보를 담음 
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))	//현재디렉토리 혹은 부모디렉토리 안에 없으면 다음 항목 읽음
			continue;

		sprintf(tmp, "%s/%s", stuDir, dirp->d_name);		//tmp <- (studir 주소)/서브디렉토리명
		stat(tmp, &statbuf);					//tmp 파일의 stat구조체 참조

		if(S_ISDIR(statbuf.st_mode))			//"stuDir/학생파일" 파일종류가 디렉토리여야 함
			strcpy(id_table[num++], dirp->d_name);		//맞다면, id_table에 파일명(학번)을 할당
		else
			continue;		//아니라면, 다음 학생파일 readdir 
	}

	sort_idTable(num);			//학번 오름차순 정렬
}

//id(학번) 오름차순으로 정렬
void sort_idTable(int size)
{
	int i, j;
	char tmp[10];

	//exchange sort
	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 -i; j++){
			if(strcmp(id_table[j], id_table[j+1]) > 0){	//id_talbe[j] 가 [j+1] 보다 클 경우
				//temp(배열)로 [j] <-> [j+1]
				strcpy(tmp, id_table[j]);
				strcpy(id_table[j], id_table[j+1]);
				strcpy(id_table[j+1], tmp);
			}
		}
	}
}

//qname(문제) 오름차순으로 정렬
void sort_scoreTable(int size)	//size <- score_table 의 인덱스
{
	int i, j;
	struct ssu_scoreTable tmp;
	int num1_1, num1_2;		//score_table[j].qname 의 대분류, 중분류
	int num2_1, num2_2;		//score_table[j+1].qname 의 대분류, 중분류

	for(i = 0; i < size - 1; i++){
		for(j = 0; j < size - 1 - i; j++){

			//qname을 "-" 기준으로 쪼개기
			get_qname_number(score_table[j].qname, &num1_1, &num1_2);
			get_qname_number(score_table[j+1].qname, &num2_1, &num2_2);
			
			if((num1_1 > num2_1) || ((num1_1 == num2_1) && (num1_2 > num2_2))){
				
				//tmp(ssu_scoreTable 구조체)로 [j] <-> [j+1]
				memcpy(&tmp, &score_table[j], sizeof(score_table[0]));	//프로토타입 void *memcpy(void *dest, const void *src, size_t num);
				memcpy(&score_table[j], &score_table[j+1], sizeof(score_table[0]));
				memcpy(&score_table[j+1], &tmp, sizeof(score_table[0]));
			}
		}
	}
}

//문제번호 대분류-중분류 쪼개기(int 값으로 할당)
void get_qname_number(char *qname, int *num1, int *num2)	//e) qname = "1-2" 일 때, num1 = 1, num2 = 2
	
{
	char *p;
	char dup[FILELEN];		//qname을 복사할 버퍼

	strncpy(dup, qname, strlen(qname));
	*num1 = atoi(strtok(dup, "-."));	//delim : -, .
	
	p = strtok(NULL, "-.");			//나머지 문자열 구분자로 tokenize
	if(p == NULL)
		*num2 = 0;
	else
		*num2 = atoi(p);		
}

//문제번호와 점수 설정타입 선택받기
int get_create_type()
{
	int num;

	while(1)
	{	//select type 사용자입력 받기
		printf("score_table.csv file doesn't exist!\n");
		printf("1. input blank question and program question's score. ex) 0.5 1\n");
		printf("2. input all question's score. ex) Input value of 1-1: 0.1\n");
		printf("select type >> ");
		scanf("%d", &num);

		if(num != 1 && num != 2)
			printf("not correct number!\n");
		else
			break;
	}

	return num;
}

//"score.csv" 파일에 내용셀을 채움 (=학생들의 학번과 점수,총점을 입력받음)
void score_students()
{
	double score = 0;
	int num;		//id_table의 인덱스 수 ;학생 수
	int fd;
	char tmp[BUFLEN];
	int size = sizeof(id_table) / sizeof(id_table[0]);	//size -> 학생수

	if((fd = creat("score.csv", 0666)) < 0){		//학생들의 성적 테이블 파일
		fprintf(stderr, "creat error for score.csv");
		return;
	}
	write_first_row(fd);		//"score_csv" 파일에, 제목 셀 write

	for(num = 0; num < size; num++)
	{
		if(!strcmp(id_table[num], ""))	//id_table[num] 값이 존재하는지 확인
			break;
		//(num번째) 학번을 tmp 버퍼로 "score.csv"에 출력(쓰기) 
		sprintf(tmp, "%s,", id_table[num]);	
		write(fd, tmp, strlen(tmp)); 

		score += score_student(fd, id_table[num]);	
		//(num)명의 학생들의 점수 테이블을 "score.csv" 파일에 순서대로 입력받고,
		//학생들의 총점을 score 에 할당	
	}

	printf("Total average : %.2f\n", score / num);

	close(fd);
}

// (num번째) 학생의 문제당 점수와 총점을 "score_csv" 파일에 입력
double score_student(int fd, char *id)	//fd :"score_csv" 파일의 파일디스크립터, id :(num번째) 학번
{
	int type;		//해당문제 qname 이 .txt, .c 인지 구분값
	double result;		//해당문제 맞았는지, 틀렸는지, 감점인지 구분값
	double score = 0;	//sum 항목에 들어갈 값
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);	//size :문제수

	// 1) (num번째) 학생의 각 문제당 점수를 "score.csv" 파일에 입력
	for(i = 0; i < size ; i++)
	{
		if(score_table[i].score == 0)	//배점이 있는 경우
			break;

		sprintf(tmp, "%s/%s/%s", stuDir, id, score_table[i].qname);	//tmp <- (stuDir주소)/(num번째)학번/(i번째)문제파일명

		if(access(tmp, F_OK) < 0)		//tmp가 제대로 생성되었는지 확인
			result = false;
		else
		{
			if((type = get_file_type(score_table[i].qname)) < 0)	//qname이 .txt 인지 .c인지 구분
				continue;
			
			if(type == TEXTFILE)	//.txt 파일인 경우 
				result = score_blank(id, score_table[i].qname);
			else if(type == CFILE)		//.c 파일인 경우
				result = score_program(id, score_table[i].qname);
		}

		//학생풀이가 틀렸을 경우,
		if(result == false)	 
			write(fd, "0,", 2);	//해당 문제의 학생 점수 0
		//맞았을 경우,
		else{
			if(result == true){	//감점없이 맞았을 경우
				score += score_table[i].score;		//학생의 sum 값을 구하기 위함
				sprintf(tmp, "%.2f,", score_table[i].score);		//tmp 배열 <- 맞춘 문제 배점
			}
			else if(result < 0){	//감점있는 경우
				score = score + score_table[i].score + result;	//학생의 sum 값을 구하기 위함
				sprintf(tmp, "%.2f,", score_table[i].score + result);		//tmp 배열 <-맞춘 문제 배점(감점포함)
			}
			write(fd, tmp, strlen(tmp));	//"score.csv" 파일에 해당문제의 학생 점수 입력
		}
	}

	printf("%s is finished. score : %.2f\n", id, score); 

	// 2) 학생의 마지막 열에, 총점(score) 입력
	sprintf(tmp, "%.2f\n", score);
	write(fd, tmp, strlen(tmp));

	return score;
}

//first_row(제목 셀) ->"score.csv" 파일에 쓰기
void write_first_row(int fd)	//fd :"score.csv" 의 파일디스크립터
{
	int i;
	char tmp[BUFLEN];
	int size = sizeof(score_table) / sizeof(score_table[0]);	//score_table 인덱스 수 ;문제 수

	write(fd, ",", 1);		//"score.csv" 파일에 쉼표 write

	for(i = 0; i < size; i++){
		if(score_table[i].score == 0)	//배점이 0인 경우, break
			break;
		//문제명을 tmp 버퍼를 통해 차례대로 "score.csv"에 출력(쓰기)
		sprintf(tmp, "%s,", score_table[i].qname);	
		write(fd, tmp, strlen(tmp));
	}
	write(fd, "sum\n", 4);		//"score.csv"에 모든 문제명을 입력한 후, 합계 항목 입력
}

//학생 풀이파일에서 풀이를 result 배열에 담음
char *get_answer(int fd, char *result)		//fd :학생의 풀이파일 파일디스크립터, result :학생의 풀이를 담을 버퍼(<- s_answer[])
{
	char c;
	int idx = 0;

	memset(result, 0, BUFLEN);	//result 포인터 초기화
	while(read(fd, &c, 1) > 0)	//학생의 풀이파일 문자 하나씩 읽기
	{
		if(c == ':')		//':' 나오면 그만 읽기
			break;
		
		result[idx++] = c;	//학생풀이 버퍼에 하나씩 담기
	}
	if(result[strlen(result) - 1] == '\n')	//마지막에 개행문자 들어갔으면
		result[strlen(result) - 1] = '\0';	//종료문자인 널문자로 바꾸기

	return result;
}

//빈칸문제 채점 :s_answer, a_answer 을 토큰화하고, 서로의 트리 비교
int score_blank(char *id, char *filename)	//<- (num번째)학번, (i번째)문제파일명
{
	char tokens[TOKEN_CNT][MINLEN];
	node *std_root = NULL, *ans_root = NULL;
	int idx, start;			//idx :tokens[] 인덱스
	char tmp[BUFLEN];
	char s_answer[BUFLEN], a_answer[BUFLEN];
	char qname[FILELEN];
	int fd_std, fd_ans;	//해당 문제의 학생, 정답 파일의 파일디스크립터
	int result = true;
	int has_semicolon = false;	//s_answer, a_answer  마지막 문자가 세미콜론인가

	memset(qname, 0, sizeof(qname));	//qname 배열 0으로 초기화
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));	//qname <- 확장자명을 뺀 문제파일명 문자열

	sprintf(tmp, "%s/%s/%s", stuDir, id, filename);		//tmp <- (stuDir주소)/(num번째)학번/문제파일명
	fd_std = open(tmp, O_RDONLY);			//즉, 학생의 풀이파일 열기
	strcpy(s_answer, get_answer(fd_std, s_answer));		//s_answer <-get_answer() 리턴값인 result 배열

	// 1) s_answer 토큰화하고 트리 만들기	
	
	if(!strcmp(s_answer, "")){	//s_answer 에 풀이가 없을 때 false 리턴
		close(fd_std);
		return false;
	}

	if(!check_brackets(s_answer)){		//괄호 열기-닫기 개수 맞는지 확인
		close(fd_std);
		return false;
	}

	strcpy(s_answer, ltrim(rtrim(s_answer)));	//문자열의 좌우 공백 지우고, s_answer 재할당

	if(s_answer[strlen(s_answer) - 1] == ';'){	//s_answer 마지막 문자가 ';'인 경우
		has_semicolon = true;
		s_answer[strlen(s_answer) - 1] = '\0';		//널문자로 변경
	}

	if(!make_tokens(s_answer, tokens)){		//s_answer 토큰화하기
		close(fd_std);
		return false;
	}

	idx = 0;
	std_root = make_tree(std_root, tokens, &idx, 0);	//std_root :s_answer 토큰화 한 트리의 root 노드 할당

	sprintf(tmp, "%s/%s", ansDir, filename);		//tmp <- (ansDir 주소)/문제파일명
	fd_ans = open(tmp, O_RDONLY);		//즉, 답안파일 열기

	while(1)
	{
	// 2) a_answer 토큰화하고 트리 만들기
	
		ans_root = NULL;	
		result = true;

		for(idx = 0; idx < TOKEN_CNT; idx++)		//tockens[] 널문자로 초기화 
			memset(tokens[idx], 0, sizeof(tokens[idx]));

		strcpy(a_answer, get_answer(fd_ans, a_answer));		//a_answer <-get_answer() 리턴값인 result 배열

		if(!strcmp(a_answer, ""))		//a_answer 에 답이 없을 때, break
			break;

		strcpy(a_answer, ltrim(rtrim(a_answer)));	//문자열의 좌우 공백 지우고, a_answer 재할당

		if(has_semicolon == false){		//s_answer 마지막 문자가 ';' 아닌데,
			if(a_answer[strlen(a_answer) -1] == ';')	//a_answer 마지막 문자가 ';'인 경우
				continue;
		}

		else if(has_semicolon == true)		//s_answer 마지막 문자가 ';' 인데,
		{
			if(a_answer[strlen(a_answer) - 1] != ';')	//a_answer 마지막 문자가 ';'인 경우
				continue;
			else
				a_answer[strlen(a_answer) - 1] = '\0';	//아닌 경우,
		}

		//s_answer ';' 있는데, a_answer 에 ';' 없을 때
		
		if(!make_tokens(a_answer, tokens))	//a_answer 토큰화하기
			continue;

		idx = 0;
		ans_root = make_tree(ans_root, tokens, &idx, 0);	//ans_root :a_answer 토큰화 한 트리의 root 노드 할당


	// 3) std_root, ans_root 부터 재귀적으로 비교하여, 같으면 result = true
	
		compare_tree(std_root, ans_root, &result);		//노드를 비교를 통해, 해당문제의 학생풀이와 답안풀이를 비교하여 
									//같다면 result 에 true를, 다르면 false 할당

		//노드비교를 통해, 학생풀이가 맞았다면
		if(result == true){
			close(fd_std);		//해당문제의 학생파일과
			close(fd_ans);		//답안파일의 파일디스크립터로 파일을 닫음

			//노드를 끊고 true 리턴
			if(std_root != NULL)	
				free_node(std_root);
			if(ans_root != NULL)
				free_node(ans_root);
			return true;

		}
	}
	
	//a_answer 에 답이 없어 break 된 경우
	close(fd_std);		//해당문제의 학생파일과
	close(fd_ans);		//답안파일의 파일디스크립터로 파일을 닫음

	//노드를 끊고 false 리턴
	if(std_root != NULL)
		free_node(std_root);
	if(ans_root != NULL)
		free_node(ans_root);

	return false;
}

//프로그램 문제 채점 :
double score_program(char *id, char *filename)	//id :(num번째) 학번, filename :해당문제명
{
	double compile;		//compile_program 의 result 값
	int result;		//execute_program 의 result 값

	compile = compile_program(id, filename);	//컴파일 후 감점사항 리턴

	if(compile == ERROR || compile == false)
		return false;
	
	result = execute_program(id, filename);		//프로그램 실행한 결과가 맞는지

	if(!result)		//틀렸으면
		return false;

	if(compile < 0)		//감점이 있으면
		return compile;

	return true;		//정답이면
}

//스레드 파일인지 판단
int is_thread(char *qname)
{
	int i;
	int size = sizeof(threadFiles) / sizeof(threadFiles[0]);	//threadFiles :ssu_score() 에서, tOption = true 일때, 입력받은 이차원배열

	for(i = 0; i < size; i++){
		if(!strcmp(threadFiles[i], qname))			//qname이 threadFiles에 있는 경우
			return true;
	}
	return false;
}

//소스코드 컴파일
double compile_program(char *id, char *filename)	//'(num번째)학번 학생의 해당문제' 에 대한 
{							//(참고. filename 은 score_table.name[i] 이다)
	int fd;
	char tmp_f[BUFLEN], tmp_e[BUFLEN];
	char command[BUFLEN];
	char qname[FILELEN];
	int isthread;
	off_t size;
	double result;

	memset(qname, 0, sizeof(qname));	//0으로 초기화
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));	//qname <- 해당문제명(확장자명을 제외한)
	
	isthread = is_thread(qname);		

	//답안파일에서 해당문제가 threadFiles에 속하는 지 
	sprintf(tmp_f, "%s/%s", ansDir, filename);
	sprintf(tmp_e, "%s/%s.exe", ansDir, qname);

	if(tOption && isthread)		//-t 옵션이 켜진 경우, 해당 문제가 threadFiles 에 속한 경우
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);	

	sprintf(tmp_e, "%s/%s_error.txt", ansDir, qname);
	fd = creat(tmp_e, 0666);

	redirection(command, fd, STDERR);	//fd를 STDERR와 동일시되며, command
	size = lseek(fd, 0, SEEK_END);		//파일 사이즈 측정
	close(fd);
	unlink(tmp_e);			//(정답파일)에러메세지 삭제

	if(size > 0)		//학생 풀이파일에 에러가 있을 경우
		return false;

	//학생실행파일 생성
	sprintf(tmp_f, "%s/%s/%s", stuDir, id, filename);	
	sprintf(tmp_e, "%s/%s/%s.stdexe", stuDir, id, qname);

	if(tOption && isthread)		//해당문제가 threadFIles 속한 문제이고, 사용자가 -t 옵션 켰을 때
		sprintf(command, "gcc -o %s %s -lpthread", tmp_e, tmp_f);
	else
		sprintf(command, "gcc -o %s %s", tmp_e, tmp_f);

	sprintf(tmp_f, "%s/%s/%s_error.txt", stuDir, id, qname);
	fd = creat(tmp_f, 0666);

	redirection(command, fd, STDERR);	//STDERR 을 fd에 dup하고, system(command)
	size = lseek(fd, 0, SEEK_END);		//fd 파일의 사이즈 저장
	close(fd);

	if(size > 0){		//학생 풀이파일에 컴파일에러가 있을 경우
		if(eOption)		//-e 옵션이 있는 경우 
		{
			sprintf(tmp_e, "%s/%s", errorDir, id);		//tmp_e <- 설정한 에러디렉토리 경로
			if(access(tmp_e, F_OK) < 0)
				mkdir(tmp_e, 0755);

			sprintf(tmp_e, "%s/%s/%s_error.txt", errorDir, id, qname);
			rename(tmp_f, tmp_e);

			result = check_error_warning(tmp_e);	//감점체크
		}
		else{ 
			result = check_error_warning(tmp_f);	//감점체크
			unlink(tmp_f);		//컴파일에러 출력 텍스트파일 삭제
		}

		return result;
	}

	unlink(tmp_f);		//에러 출력 텍스트파일 삭제
	return true;
}

//실행프로그램 감점하는 함수
double check_error_warning(char *filename)
{
	FILE *fp;
	char tmp[BUFLEN];
	double warning = 0;

	if((fp = fopen(filename, "r")) == NULL){		//읽기전용 파일로 열기
		fprintf(stderr, "fopen error for %s\n", filename);
		return false;
	}

	while(fscanf(fp, "%s", tmp) > 0){
		if(!strcmp(tmp, "error:"))
			return ERROR;
		else if(!strcmp(tmp, "warning:"))
			warning += WARNING;
	}

	return warning;
}

//(num번째) 학생의 해당 문제풀이 실행
int execute_program(char *id, char *filename)
{
	char std_fname[BUFLEN], ans_fname[BUFLEN];
	char tmp[BUFLEN];
	char qname[FILELEN];
	time_t start, end;
	pid_t pid;
	int fd;

	memset(qname, 0, sizeof(qname));
	memcpy(qname, filename, strlen(filename) - strlen(strrchr(filename, '.')));

	sprintf(ans_fname, "%s/%s.stdout", ansDir, qname);
	fd = creat(ans_fname, 0666);

	sprintf(tmp, "%s/%s.exe", ansDir, qname);
	redirection(tmp, fd, STDOUT);
	close(fd);

	sprintf(std_fname, "%s/%s/%s.stdout", stuDir, id, qname);
	fd = creat(std_fname, 0666);

	sprintf(tmp, "%s/%s/%s.stdexe &", stuDir, id, qname);

	start = time(NULL);
	redirection(tmp, fd, STDOUT);
	
	sprintf(tmp, "%s.stdexe", qname);
	while((pid = inBackground(tmp)) > 0){
		end = time(NULL);

		if(difftime(end, start) > OVER){
			kill(pid, SIGKILL);
			close(fd);
			return false;
		}
	}

	close(fd);

	return compare_resultfile(std_fname, ans_fname);	//컴파일 후 풀이결과 비교
}

//현재프로세스의 pid을 구함
pid_t inBackground(char *name)
{
	pid_t pid;
	char command[64];
	char tmp[64];
	int fd;
	off_t size;
	
	memset(tmp, 0, sizeof(tmp));
	fd = open("background.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);	//backgournd 파일 생성

	sprintf(command, "ps | grep %s", name);
	redirection(command, fd, STDOUT);	//STDOUT가 fd의 파일을 공유하고, command 명령어 실행 (그 후 제자리)

	lseek(fd, 0, SEEK_SET);
	read(fd, tmp, sizeof(tmp));

	if(!strcmp(tmp, "")){
		unlink("background.txt");
		close(fd);
		return 0;
	}

	pid = atoi(strtok(tmp, " "));
	close(fd);

	unlink("background.txt");
	return pid;
}

//컴파일 후 결과 비교
int compare_resultfile(char *file1, char *file2)	//file1 :학생풀이, file2 :답안풀이
{
	int fd1, fd2;
	char c1, c2;
	int len1, len2;

	fd1 = open(file1, O_RDONLY);
	fd2 = open(file2, O_RDONLY);

	while(1)
	{
		while((len1 = read(fd1, &c1, 1)) > 0){
			if(c1 == ' ') 
				continue;
			else 
				break;
		}
		while((len2 = read(fd2, &c2, 1)) > 0){
			if(c2 == ' ') 
				continue;
			else 
				break;
		}
		
		if(len1 == 0 && len2 == 0)
			break;

		to_lower_case(&c1);
		to_lower_case(&c2);

		if(c1 != c2){
			close(fd1);
			close(fd2);
			return false;
		}
	}
	close(fd1);
	close(fd2);
	return true;
}

//파일디스크립트를 dup 했다가, system으로 명령어 실행 후, 되돌리기
void redirection(char *command, int new, int old)
{
	int saved;

	saved = dup(old);
	dup2(new, old);

	system(command);

	dup2(saved, old);
	close(saved);
}
//파일 확장자명 받기 (빈칸문제 .txt, 프로그램문제 .c)
int get_file_type(char *filename)
{
	char *extension = strrchr(filename, '.');	//extension :확장자명 문자의 포인터 할당

	if(!strcmp(extension, ".txt"))			//빈칸문제인 경우
		return TEXTFILE;
	else if (!strcmp(extension, ".c"))		//프로그램문제인 경우
		return CFILE;
	else
		return -1;
}

//errorDir 있는 경우, 삭제 (->mkdir 로 만듬)
void rmdirs(const char *path)
{
	struct dirent *dirp;
	struct stat statbuf;
	DIR *dp;
	char tmp[BUFLEN];
	
	if((dp = opendir(path)) == NULL)
		return;

	while((dirp = readdir(dp)) != NULL)
	{
		if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
			continue;

		sprintf(tmp, "%s/%s", path, dirp->d_name);

		if(lstat(tmp, &statbuf) == -1)
			continue;

		if(S_ISDIR(statbuf.st_mode))
			rmdirs(tmp);
		else
			unlink(tmp);
	}

	closedir(dp);
	rmdir(path);
}
//대문자를 소문자로 변경
void to_lower_case(char *c)
{
	if(*c >= 'A' && *c <= 'Z')	//*c 가 알파벳 대문자인 경우
		*c = *c + 32;		//ASCII 코드에 의해 +32, 소문자로 변경
}

//-h 옵션 입력받으면, 옵션 용도 출력
void print_usage()
{
	printf("Usage : ssu_score <STUDENTDIR> <TRUEDIR> [OPTION]\n");
	printf("Option : \n");
	printf(" -m                modify question's score\n");
	printf(" -e <DIRNAME>      print error on 'DIRNAME/ID/qname_error.txt' file \n");
	printf(" -t <QNAMES>       compile QNAME.c with -lpthread option\n");
	printf(" -i <IDS>          print ID's wrong qestions\n");
	printf(" -h                print usage\n");
}
