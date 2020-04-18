#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include "blank.h"

char datatype[DATATYPE_SIZE][MINLEN] = {"int", "char", "double", "float", "long"
			, "short", "ushort", "FILE", "DIR","pid"
			,"key_t", "ssize_t", "mode_t", "ino_t", "dev_t"
			, "nlink_t", "uid_t", "gid_t", "time_t", "blksize_t"
			, "blkcnt_t", "pid_t", "pthread_mutex_t", "pthread_cond_t", "pthread_t"
			, "void", "size_t", "unsigned", "sigset_t", "sigjmp_buf"
			, "rlim_t", "jmp_buf", "sig_atomic_t", "clock_t", "struct"};


operator_precedence operators[OPERATOR_CNT] = {
	{"(", 0}, {")", 0}
	,{"->", 1}	
	,{"*", 4}	,{"/", 3}	,{"%", 2}	
	,{"+", 6}	,{"-", 5}	
	,{"<", 7}	,{"<=", 7}	,{">", 7}	,{">=", 7}
	,{"==", 8}	,{"!=", 8}
	,{"&", 9}
	,{"^", 10}
	,{"|", 11}
	,{"&&", 12}
	,{"||", 13}
	,{"=", 14}	,{"+=", 14}	,{"-=", 14}	,{"&=", 14}	,{"|=", 14}
};

/*std_root, ans_root 노드를 시작으로 재귀적으로  노드가 가리키는 노드를 비교하여 같다면 
  result에 true를, 다르다면 result에 false 할당 */
void compare_tree(node *root1,  node *root2, int *result)	//root1 :std_root, root2 :ans_root
{
	node *tmp;
	int cnt1, cnt2;

	//root1 이나 root2 가 NULL 인 경우, false 처리
	if(root1 == NULL || root2 == NULL)	{
		*result = false;
		return;
	}

	//ans_root 노드의 name 부호방향 바꿈 (std_root 노드의 name이 비교연산자인 경우)
	if(!strcmp(root1->name, "<") || !strcmp(root1->name, ">") || !strcmp(root1->name, "<=") || !strcmp(root1->name, ">=")){
		if(strcmp(root1->name, root2->name) != 0){	//std_root 노드와 ans_root 노드의 name이 다를 때
			
			//ans_root 노드의 부호방향 바꾸기
			if(!strncmp(root2->name, "<", 1))
				strncpy(root2->name, ">", 1);

			else if(!strncmp(root2->name, ">", 1))
				strncpy(root2->name, "<", 1);

			else if(!strncmp(root2->name, "<=", 2))
				strncpy(root2->name, ">=", 2);

			else if(!strncmp(root2->name, ">=", 2))
				strncpy(root2->name, "<=", 2);

			root2 = change_sibling(root2);	//root2 의 부호방향을 바꿈
		}
	}
	
	//여전히 std_root 와 ans_root 노드의 name이 다른 경우, false 처리
	if(strcmp(root1->name, root2->name) != 0){
		*result = false;
		return;
	}

	//std_root, ans_root 노드 중에 하나라도 'child_head'가 널이면, false 처리
	if((root1->child_head != NULL && root2->child_head == NULL)
		|| (root1->child_head == NULL && root2->child_head != NULL)){
		*result = false;
		return;
	}

	//std_root 노드의 'child_head'가 널인 경우, 조건문에 따라 true/false 처리
	else if(root1->child_head != NULL){
		//std_root, ans_root 노드의 'child_head'의 형제노드 개수가 서로 다른 경우, false 처리
		if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){
			*result = false;
			return;
		}
		//std_root 노드의 name에 등호연산자가 있는 경우
		if(!strcmp(root1->name, "==") || !strcmp(root1->name, "!="))
		{
			compare_tree(root1->child_head, root2->child_head, result);	//재귀 :ans_root 노드의  'child_head'와 비교
			
			if(*result == false)	// 비교결과, 그 값이 다르면
			{
				//ans_root 의 부호방향을 바꿔서 다시한번 child_head 값 비교
				*result = true;
				root2 = change_sibling(root2);
				compare_tree(root1->child_head, root2->child_head, result);	//재귀
			}
		}
		//std_root 노드의 name에 해당 기호가 있는 경우
		else if(!strcmp(root1->name, "+") || !strcmp(root1->name, "*")
				|| !strcmp(root1->name, "|") || !strcmp(root1->name, "&")
				|| !strcmp(root1->name, "||") || !strcmp(root1->name, "&&"))
		{
			//std_root, ans_root 노드의 'child_head'의 형제노드 개수가 다른 경우, false 처리
			if(get_sibling_cnt(root1->child_head) != get_sibling_cnt(root2->child_head)){
				*result = false;
				return;
			}

			tmp = root2->child_head;

			while(tmp->prev != NULL)
				tmp = tmp->prev;	//root2 -> child_head ->prev

			while(tmp != NULL)
			{
				compare_tree(root1->child_head, tmp, result);	//재귀 :root1,2 비교하여 result
			
				if(*result == true)
					break;
				else{
					if(tmp->next != NULL)
						*result = true;
					tmp = tmp->next;	//root1 ->child_head ->next
				}
			}
		}
		else{
			compare_tree(root1->child_head, root2->child_head, result);	//재귀 :root1, root2 비교하여 result	
		}
	}	

	//root1 다음 노드가 널이 아닌 경우
	if(root1->next != NULL){

		if(get_sibling_cnt(root1) != get_sibling_cnt(root2)){	//root1, root2의 형제노드 수가 다를 때 
			*result = false;
			return;
		}

		if(*result == true)
		{
			tmp = get_operator(root1);	//temp :root1의 부모노드
	
			//부모노드가 해당 기호일 때,
			if(!strcmp(tmp->name, "+") || !strcmp(tmp->name, "*")
					|| !strcmp(tmp->name, "|") || !strcmp(tmp->name, "&")
					|| !strcmp(tmp->name, "||") || !strcmp(tmp->name, "&&"))
			{	
				tmp = root2;
	
				while(tmp->prev != NULL)	//root2의 이전 형제노드가 있다면,
					tmp = tmp->prev;	//그것을 tmp에 할당

				while(tmp != NULL)		//root2의 제일 처음에 오는 형제노드가 널이 아니라면
				{
					compare_tree(root1->next, tmp, result);		//그것과 root1을 비교

					if(*result == true)	
						break;
					else {						//비교 결과가 false인 경우,
						if(tmp->next != NULL)		//그 다음 형제노드가 0이 아니면, result는 true 
							*result = true;
						tmp = tmp->next;	//그것이 아니면, 마지막 형제노드로 이동
					}
				}
			}

			else
				compare_tree(root1->next, root2->next, result);	//root1,2  형제노드의 다음 노드 비교
		}
	}
}

//문제풀이 배열을 토큰화
int make_tokens(char *str, char tokens[TOKEN_CNT][MINLEN])	//str << s_answer, a_answer
{
	char *start, *end;		//문자열 탐색
	char tmp[BUFLEN];
	char str2[BUFLEN];
	char *op = "(),;><=!|&^/+-*\""; 	//opcode 기호들
	int row = 0;		//현재 토큰 위치
	int i;
 	int isPointer;		//포인터인가
	int lcount, rcount;		//괄호 개수
	int p_str;
	
	clear_tokens(tokens);		//tockens  0으로 초기화

	start = str;		//str 시작주소로 stat 할당
	
	if(is_typeStatement(str) == 0)		//str의 처음 문자열이 데이터타입이면, 오류 발생 
		return false;	
	
	while(1)
	{
		if((end = strpbrk(start, op)) == NULL)		//end : 처음 검색된 op 기호 문자의 포인터
			break;					//없다면 break

		//start 처음 문자가 op 기호일 경우
		if(start == end){
			
			//start 처음 두문자가 증감연산자인 경우
			if(!strncmp(start, "--", 2) || !strncmp(start, "++", 2)) {
				if(!strncmp(start, "++++", 4)||!strncmp(start,"----",4))	//잘못된 사용, 오류
					return false;
				
				//증감연산자 다음 문자가, 문자인 경우
				if(is_character(*ltrim(start + 2))){	
					if(row > 0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]))
						return false; 

					end = strpbrk(start + 2, op);	
					if(end == NULL)
						end = &str[strlen(str)];
					while(start < end) {
						if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1]))
							return false;
						else if(*start != ' ')
							strncat(tokens[row], start, 1);
						start++;	//tokens[row]에 저장한 문자, 다음
					}
				}
				
				//증감연산자 다음 문자가, 문자가 아닌 경우1
				else if(row>0 && is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){
					if(strstr(tokens[row - 1], "++") != NULL || strstr(tokens[row - 1], "--") != NULL)	
						return false;

					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);
					strcat(tokens[row - 1], tmp);
					start += 2;		//tokens[row-1]에 tmp로 적어준 것, 다음
					row--;
				}

				//증감연산자 다음 문자가, 문자가 아닌 경우2
				else{
					memset(tmp, 0, sizeof(tmp));
					strncpy(tmp, start, 2);
					strcat(tokens[row], tmp);		//tokens[]+(2바이트 기호)
					start += 2;		//tockens[row]에 tmp로 적어준 것, 다음
				}
			}
			
			//star 처음 두문자가 비교연산자인 경우
			else if(!strncmp(start, "==", 2) || !strncmp(start, "!=", 2) || !strncmp(start, "<=", 2)
				|| !strncmp(start, ">=", 2) || !strncmp(start, "||", 2) || !strncmp(start, "&&", 2) 
				|| !strncmp(start, "&=", 2) || !strncmp(start, "^=", 2) || !strncmp(start, "!=", 2) 
				|| !strncmp(start, "|=", 2) || !strncmp(start, "+=", 2)	|| !strncmp(start, "-=", 2) 
				|| !strncmp(start, "*=", 2) || !strncmp(start, "/=", 2)){

				strncpy(tokens[row], start, 2);
				start += 2;		//stat 오른쪽 2 이동
			}

			//star 처음 두문자가 구조체접근자인 경우
			else if(!strncmp(start, "->", 2))
			{
				end = strpbrk(start + 2, op);		//'->'기호 다음을, end로

				if(end == NULL)		//end가 초과하면
					end = &str[strlen(str)];

				while(start < end){
					if(*start != ' ')
						strncat(tokens[row - 1], start, 1);
					start++;
				}
				row--;		//토큰 감소
			}

			//처음 검색된 op 기호가 '&' 인 경우
			else if(*end == '&')
			{
				
				if(row == 0 || (strpbrk(tokens[row - 1], op) != NULL)){
					end = strpbrk(start + 1, op);
					if(end == NULL)
						end = &str[strlen(str)];
					
					strncat(tokens[row], start, 1);
					start++;	//1바이트 이동

					while(start < end){
						if(*(start - 1) == ' ' && tokens[row][strlen(tokens[row]) - 1] != '&')
							return false;
						else if(*start != ' ')
							strncat(tokens[row], start, 1);
						start++;	//1바이트 이동
					}
				}
				
				else{
					strncpy(tokens[row], start, 1);
					start += 1;	//1바이트 이동
				}
				
			}

			//처음 검색된 op 기호가 '*' 인 경우
		  	else if(*end == '*')
			{
				isPointer=0;

				if(row > 0)
				{
					
					for(i = 0; i < DATATYPE_SIZE; i++) {
						if(strstr(tokens[row - 1], datatype[i]) != NULL){
							strcat(tokens[row - 1], "*");
							start += 1;	
							isPointer = 1;
							break;
						}
					}
					if(isPointer == 1)
						continue;
					if(*(start+1) !=0)
						end = start + 1;

					
					if(row>1 && !strcmp(tokens[row - 2], "*") && (all_star(tokens[row - 1]) == 1)){
						strncat(tokens[row - 1], start, end - start);
						row--;
					}
					
					
					else if(is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1]) == 1){ 
						strncat(tokens[row], start, end - start);   
					}

					
					else if(strpbrk(tokens[row - 1], op) != NULL){		
						strncat(tokens[row] , start, end - start); 
							
					}
					else
						strncat(tokens[row], start, end - start);

					start += (end - start);
				}

			 	else if(row == 0)
				{
					if((end = strpbrk(start + 1, op)) == NULL){
						strncat(tokens[row], start, 1);
						start += 1;
					}
					else{
						while(start < end){
							if(*(start - 1) == ' ' && is_character(tokens[row][strlen(tokens[row]) - 1]))
								return false;
							else if(*start != ' ')
								strncat(tokens[row], start, 1);
							start++;	
						}
						if(all_star(tokens[row]))
							row--;
						
					}
				}
			}

			//처음 검색된 op 기호가 '('인 경우
			else if(*end == '(') 
			{
				lcount = 0;
				rcount = 0;
				if(row>0 && (strcmp(tokens[row - 1],"&") == 0 || strcmp(tokens[row - 1], "*") == 0)){
					while(*(end + lcount + 1) == '(')
						lcount++;
					start += lcount;

					end = strpbrk(start + 1, ")");

					if(end == NULL)
						return false;
					else{
						while(*(end + rcount +1) == ')')
							rcount++;
						end += rcount;

						if(lcount != rcount)
							return false;

						if( (row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1])) || row == 1){ 
							strncat(tokens[row - 1], start + 1, end - start - rcount - 1);
							row--;
							start = end + 1;
						}
						else{
							strncat(tokens[row], start, 1);
							start += 1;
						}
					}
						
				}
				else{
					strncat(tokens[row], start, 1);
					start += 1;
				}

			}

			//처음 검색된 op 기호가 '\' 인 경우
			else if(*end == '\"') 
			{
				end = strpbrk(start + 1, "\"");
				
				if(end == NULL)
					return false;

				else{
					strncat(tokens[row], start, end - start + 1);
					start = end + 1;
				}

			}

			else{
				
				if(row > 0 && !strcmp(tokens[row - 1], "++"))
					return false;

				
				if(row > 0 && !strcmp(tokens[row - 1], "--"))
					return false;
	
				strncat(tokens[row], start, 1);
				start += 1;
				
			
				if(!strcmp(tokens[row], "-") || !strcmp(tokens[row], "+") || !strcmp(tokens[row], "--") || !strcmp(tokens[row], "++")){


				
					if(row == 0)
						row--;

					
					else if(!is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])){
					
						if(strstr(tokens[row - 1], "++") == NULL && strstr(tokens[row - 1], "--") == NULL)
							row--;
					}
				}
			}
		}

		//star 처음 문자가 op 기호가 아닌 경우
		else{ 
			if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))   
				row--;				

			if(all_star(tokens[row - 1]) && row == 1)   
				row--;	

			for(i = 0; i < end - start; i++){
				if(i > 0 && *(start + i) == '.'){
					strncat(tokens[row], start + i, 1);

					while( *(start + i +1) == ' ' && i< end - start )
						i++; 
				}
				else if(start[i] == ' '){
					while(start[i] == ' ')
						i++;
					break;
				}
				else
					strncat(tokens[row], start + i, 1);
			}

			if(start[0] == ' '){
				start += i;
				continue;
			}
			start += i;
		}
			
		strcpy(tokens[row], ltrim(rtrim(tokens[row])));

		 if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1]) 
				&& (is_typeStatement(tokens[row - 1]) == 2 
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.' ) ){

			if(row > 1 && strcmp(tokens[row - 2],"(") == 0)
			{
				if(strcmp(tokens[row - 1], "struct") != 0 && strcmp(tokens[row - 1],"unsigned") != 0)
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)	
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)
					return false;
			}
			
		}

		if((row == 0 && !strcmp(tokens[row], "gcc")) ){
			clear_tokens(tokens);
			strcpy(tokens[0], str);	
			return 1;
		} 

		row++;
	}// while문 끝

	
	if(all_star(tokens[row - 1]) && row > 1 && !is_character(tokens[row - 2][strlen(tokens[row - 2]) - 1]))  
		row--;				
	if(all_star(tokens[row - 1]) && row == 1)   
		row--;	

	//start에 남은 문자처리
	for(i = 0; i < strlen(start); i++)   
	{
		if(start[i] == ' ')		//공백이면
		{
			while(start[i] == ' ')		
				i++;
			if(start[0]==' ') {
				start += i;
				i = 0;
			}
			else
				row++;
			
			i--;
		} 
		else
		{
			strncat(tokens[row], start + i, 1);
			if( start[i] == '.' && i<strlen(start)){
				while(start[i + 1] == ' ' && i < strlen(start))
					i++;

			}
		}
		strcpy(tokens[row], ltrim(rtrim(tokens[row])));

		if(!strcmp(tokens[row], "lpthread") && row > 0 && !strcmp(tokens[row - 1], "-")){ 	//-lpthread 옵션일 경우,
			strcat(tokens[row - 1], tokens[row]);		//이전 토큰과 결합
			memset(tokens[row], 0, sizeof(tokens[row]));
			row--;
		}
	 	else if(row > 0 && is_character(tokens[row][strlen(tokens[row]) - 1]) 
				&& (is_typeStatement(tokens[row - 1]) == 2 
					|| is_character(tokens[row - 1][strlen(tokens[row - 1]) - 1])
					|| tokens[row - 1][strlen(tokens[row - 1]) - 1] == '.') ){
			
			if(row > 1 && strcmp(tokens[row-2],"(") == 0)
			{
				if(strcmp(tokens[row-1], "struct") != 0 && strcmp(tokens[row-1], "unsigned") != 0)
					return false;
			}
			else if(row == 1 && is_character(tokens[row][strlen(tokens[row]) - 1])) {
				if(strcmp(tokens[0], "extern") != 0 && strcmp(tokens[0], "unsigned") != 0 && is_typeStatement(tokens[0]) != 2)	
					return false;
			}
			else if(row > 1 && is_typeStatement(tokens[row - 1]) == 2){
				if(strcmp(tokens[row - 2], "unsigned") != 0 && strcmp(tokens[row - 2], "extern") != 0)
					return false;
			}
		} 
	}


	if(row > 0)
	{

		
		if(strcmp(tokens[0], "#include") == 0 || strcmp(tokens[0], "include") == 0 || strcmp(tokens[0], "struct") == 0){ 
			clear_tokens(tokens); 
			strcpy(tokens[0], remove_extraspace(str)); 
		}
	}

	if(is_typeStatement(tokens[0]) == 2 || strstr(tokens[0], "extern") != NULL){
		for(i = 1; i < TOKEN_CNT; i++){
			if(strcmp(tokens[i],"") == 0)  
				break;		       

			if(i != TOKEN_CNT -1 )
				strcat(tokens[0], " ");
			strcat(tokens[0], tokens[i]);
			memset(tokens[i], 0, sizeof(tokens[i]));
		}
	}
	
	
	while((p_str = find_typeSpecifier(tokens)) != -1){ 
		if(!reset_tokens(p_str, tokens))
			return false;
	}

	
	while((p_str = find_typeSpecifier2(tokens)) != -1){  
		if(!reset_tokens(p_str, tokens))
			return false;
	}
	
	return true;
}

//인자노드의 트리 생성
node *make_tree(node *root, char (*tokens)[MINLEN], int *idx, int parentheses)
{
	node *cur = root;
	node *new;
	node *saved_operator;
	node *operator;
	int fstart;
	int i;

	while(1)	
	{
		if(strcmp(tokens[*idx], "") == 0)
			break;
	
		if(!strcmp(tokens[*idx], ")"))
			return get_root(cur);

		else if(!strcmp(tokens[*idx], ","))
			return get_root(cur);

		else if(!strcmp(tokens[*idx], "("))
		{
			
			if(*idx > 0 && !is_operator(tokens[*idx - 1]) && strcmp(tokens[*idx - 1], ",") != 0){
				fstart = true;

				while(1)
				{
					*idx += 1;

					if(!strcmp(tokens[*idx], ")"))
						break;
					
					new = make_tree(NULL, tokens, idx, parentheses + 1);
					
					if(new != NULL){
						if(fstart == true){
							cur->child_head = new;
							new->parent = cur;
	
							fstart = false;
						}
						else{
							cur->next = new;
							new->prev = cur;
						}

						cur = new;
					}

					if(!strcmp(tokens[*idx], ")"))
						break;
				}
			}
			else{
				*idx += 1;
	
				new = make_tree(NULL, tokens, idx, parentheses + 1);

				if(cur == NULL)
					cur = new;

				else if(!strcmp(new->name, cur->name)){
					if(!strcmp(new->name, "|") || !strcmp(new->name, "||") 
						|| !strcmp(new->name, "&") || !strcmp(new->name, "&&"))
					{
						cur = get_last_child(cur);

						if(new->child_head != NULL){
							new = new->child_head;

							new->parent->child_head = NULL;
							new->parent = NULL;
							new->prev = cur;
							cur->next = new;
						}
					}
					else if(!strcmp(new->name, "+") || !strcmp(new->name, "*"))
					{
						i = 0;

						while(1)
						{
							if(!strcmp(tokens[*idx + i], ""))
								break;

							if(is_operator(tokens[*idx + i]) && strcmp(tokens[*idx + i], ")") != 0)
								break;

							i++;
						}
						
						if(get_precedence(tokens[*idx + i]) < get_precedence(new->name))
						{
							cur = get_last_child(cur);
							cur->next = new;
							new->prev = cur;
							cur = new;
						}
						else
						{
							cur = get_last_child(cur);

							if(new->child_head != NULL){
								new = new->child_head;

								new->parent->child_head = NULL;
								new->parent = NULL;
								new->prev = cur;
								cur->next = new;
							}
						}
					}
					else{
						cur = get_last_child(cur);
						cur->next = new;
						new->prev = cur;
						cur = new;
					}
				}
	
				else
				{
					cur = get_last_child(cur);

					cur->next = new;
					new->prev = cur;
	
					cur = new;
				}
			}
		}
		else if(is_operator(tokens[*idx]))
		{
			if(!strcmp(tokens[*idx], "||") || !strcmp(tokens[*idx], "&&")
					|| !strcmp(tokens[*idx], "|") || !strcmp(tokens[*idx], "&") 
					|| !strcmp(tokens[*idx], "+") || !strcmp(tokens[*idx], "*"))
			{
				if(is_operator(cur->name) == true && !strcmp(cur->name, tokens[*idx]))
					operator = cur;
		
				else
				{
					new = create_node(tokens[*idx], parentheses);
					operator = get_most_high_precedence_node(cur, new);

					if(operator->parent == NULL && operator->prev == NULL){

						if(get_precedence(operator->name) < get_precedence(new->name)){
							cur = insert_node(operator, new);
						}

						else if(get_precedence(operator->name) > get_precedence(new->name))
						{
							if(operator->child_head != NULL){
								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}
						}
						else
						{
							operator = cur;
	
							while(1)
							{
								if(is_operator(operator->name) == true && !strcmp(operator->name, tokens[*idx]))
									break;
						
								if(operator->prev != NULL)
									operator = operator->prev;
								else
									break;
							}

							if(strcmp(operator->name, tokens[*idx]) != 0)
								operator = operator->parent;

							if(operator != NULL){
								if(!strcmp(operator->name, tokens[*idx]))
									cur = operator;
							}
						}
					}

					else
						cur = insert_node(operator, new);
				}

			}
			else
			{
				new = create_node(tokens[*idx], parentheses);

				if(cur == NULL)
					cur = new;

				else
				{
					operator = get_most_high_precedence_node(cur, new);

					if(operator->parentheses > new->parentheses)
						cur = insert_node(operator, new);

					else if(operator->parent == NULL && operator->prev ==  NULL){
					
						if(get_precedence(operator->name) > get_precedence(new->name))
						{
							if(operator->child_head != NULL){
	
								operator = get_last_child(operator);
								cur = insert_node(operator, new);
							}
						}
					
						else	
							cur = insert_node(operator, new);
					}
	
					else
						cur = insert_node(operator, new);
				}
			}
		}
		else 
		{
			new = create_node(tokens[*idx], parentheses);

			if(cur == NULL)
				cur = new;

			else if(cur->child_head == NULL){
				cur->child_head = new;
				new->parent = cur;

				cur = new;
			}
			else{

				cur = get_last_child(cur);

				cur->next = new;
				new->prev = cur;

				cur = new;
			}
		}

		*idx += 1;
	}

	return get_root(cur);
}

//형제노드 위치를 바꿈
node *change_sibling(node *parent)
{
	node *tmp;
	
	tmp = parent->child_head;

	parent->child_head = parent->child_head->next;
	parent->child_head->parent = parent;
	parent->child_head->prev = NULL;

	parent->child_head->next = tmp;
	parent->child_head->next->prev = parent->child_head;
	parent->child_head->next->next = NULL;
	parent->child_head->next->parent = NULL;		

	return parent;
}

//새로운 노드를 만들어줌
node *create_node(char *name, int parentheses)
{
	node *new;

	new = (node *)malloc(sizeof(node));
	new->name = (char *)malloc(sizeof(char) * (strlen(name) + 1));
	strcpy(new->name, name);

	new->parentheses = parentheses;
	new->parent = NULL;
	new->child_head = NULL;
	new->prev = NULL;
	new->next = NULL;

	return new;
}

//op 연산자의 우선순위 구함
int get_precedence(char *op)
{
	int i;

	for(i = 2; i < OPERATOR_CNT; i++){
		if(!strcmp(operators[i].operator, op))
			return operators[i].precedence;
	}
	return false;
}

//연산자 존재 체크
int is_operator(char *op)
{
	int i;

	for(i = 0; i < OPERATOR_CNT; i++)
	{
		if(operators[i].operator == NULL)
			break;
		if(!strcmp(operators[i].operator, op)){
			return true;
		}
	}

	return false;
}

//현재 노드와 현재노드가 가리키는 노드를 출력
void print(node *cur)
{
	if(cur->child_head != NULL){
		print(cur->child_head);
		printf("\n");
	}

	if(cur->next != NULL){
		print(cur->next);
		printf("\t");
	}
	printf("%s", cur->name);
}

//현재노드의 연산자(부모노드)를 리턴
node *get_operator(node *cur)
{
	if(cur == NULL)		//비워있는 노드일 때
		return cur;

	if(cur->prev != NULL)		//현재노드가 이전 형제노드가 있을 때
		while(cur->prev != NULL)
			cur = cur->prev;	//처음 형제노드로 이동

	return cur->parent;	//부모노드(연산자) 리턴
}

//현재노드의 루트노드를 현재노드에 저장후 리턴
node *get_root(node *cur)
{
	if(cur == NULL)
		return cur;

	while(cur->prev != NULL)
		cur = cur->prev;

	if(cur->parent != NULL)
		cur = get_root(cur->parent);

	return cur;
}

//두 인자의 노드의 우선순위 비교
node *get_high_precedence_node(node *cur, node *new)
{
	if(is_operator(cur->name))
		if(get_precedence(cur->name) < get_precedence(new->name))	//new노드의 우선순위가 높다면
			return cur;	//cur노드 반환

	if(cur->prev != NULL){		//현재노드가 이전노드가 있다면
		while(cur->prev != NULL){		//이전노드가 있을 때까지
			cur = cur->prev;		//이전노드로 이동
			
			return get_high_precedence_node(cur, new);		//형제노드중 처음노드(cur)와 new노드 최상위 우선순위 노드 리턴
		}


		if(cur->parent != NULL)		//현재노드가 부모노드가 있는 경우
			return get_high_precedence_node(cur->parent, new);		//부모노드와 new노드 비교하여 리턴
	}

	if(cur->parent == NULL)
		return cur;
}

//cur, new 노드 중 최상위 노드 리턴
node *get_most_high_precedence_node(node *cur, node *new)
{
	node *operator = get_high_precedence_node(cur, new);
	node *saved_operator = operator;

	while(1)
	{
		if(saved_operator->parent == NULL)
			break;

		if(saved_operator->prev != NULL)
			operator = get_high_precedence_node(saved_operator->prev, new);

		else if(saved_operator->parent != NULL)
			operator = get_high_precedence_node(saved_operator->parent, new);

		saved_operator = operator;
	}
	
	return saved_operator;
}

//새로운 노드 삽입
node *insert_node(node *old, node *new)
{
	if(old->prev != NULL){	//형이 있는 경우
		new->prev = old->prev; 		//old와 new의 형을 바꾼다
		old->prev->next = new;	
		old->prev = NULL;
	}

	new->child_head = old;		//새 노드의 자식을 old노드로 변경
	old->parent = new;		//기존 노드의 부모를 new노드로 변경

	return new;
}

//마지막 자식의 노드를 리턴
node *get_last_child(node *cur)		
{
	if(cur->child_head != NULL)
		cur = cur->child_head;

	while(cur->next != NULL)
		cur = cur->next;

	return cur;
}

//현재노드의 형제노드(pre,next) 개수를 구함
int get_sibling_cnt(node *cur)
{
	int i = 0;

	while(cur->prev != NULL)	//현재노드 이전에 실행된 형제노드가 있다면
		cur = cur->prev;	//이전 노드로 이동

	while(cur->next != NULL){	//현재노드에서 다음 노드가 있다면
		cur = cur->next;	//다음 노드로 이동 후
		i++;			//노드 카운트 함
	}

	return i;			//i :처음 인자로 받은 현재노드 형제노드 수(자신도 포함) 리턴
}

//노드 메모리 해제
void free_node(node *cur)
{
	if(cur->child_head != NULL)
		free_node(cur->child_head);	//자식노드 메모리부터 해제

	if(cur->next != NULL)
		free_node(cur->next);		//동생노드 메모리부터 해제

	if(cur != NULL){
		cur->prev = NULL;
		cur->next = NULL;
		cur->parent = NULL;
		cur->child_head = NULL;
		free(cur);
	}
}

//c가 char 형이 맞는지 확인
int is_character(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//str에 처음 문자열이 데이터타입일 경우
int is_typeStatement(char *str)
{ 
	char *start;			//str 문자열을 가리키는 포인터
	char str2[BUFLEN] = {0}; 	//strncpy, strcmp를 하기 위해, str 문자열을 할당할 배열
	char tmp[BUFLEN] = {0}; 	//배열 str2 을 strcpy 하기 위한 임시버퍼 
	char tmp2[BUFLEN] = {0}; 	//포인터 start 을 strcpy 하기 위한 임시버퍼
	int i;	 
	
	start = str;
	strncpy(str2,str,strlen(str));
	remove_space(str2);		//str2 :스페이스 공백문자가 제거된 str
	
	while (start[0] == ' ')			//start
		start += 1;

	if(strstr(str2, "gcc") != NULL)		//str2에 gcc 문자열이 있는 경우라면,
	{
		strncpy(tmp2, start, strlen("gcc"));
		if(strcmp(tmp2,"gcc") != 0)
			return 0;		//gcc 문자열이 처음에 나오면 오류
		else
			return 2;		
	}
	
	for(i = 0; i < DATATYPE_SIZE; i++)
	{
		if(strstr(str2,datatype[i]) != NULL)	//str2 문자열에 datatype[i]가 있을 때
		{	
			//해당 데이터타입 크기만큼 임시버퍼에 copy
			strncpy(tmp, str2, strlen(datatype[i]));
			strncpy(tmp2, start, strlen(datatype[i]));
			
			if(strcmp(tmp, datatype[i]) == 0)	//str2 배열에 데이터타입이 처음에 나오고
				if(strcmp(tmp, tmp2) != 0)	//start 포인터에도 동일하다면
					return 0;  		//오류
				else
					return 2;
		}

	}
	return 1;

}

//데이터타입 검색
int find_typeSpecifier(char tokens[TOKEN_CNT][MINLEN]) 
{
	int i, j;

	for(i = 0; i < TOKEN_CNT; i++)
	{
		for(j = 0; j < DATATYPE_SIZE; j++)
		{
			if(strstr(tokens[i], datatype[j]) != NULL && i > 0)
			{
				if(!strcmp(tokens[i - 1], "(") && !strcmp(tokens[i + 1], ")") 
						&& (tokens[i + 2][0] == '&' || tokens[i + 2][0] == '*' 
							|| tokens[i + 2][0] == ')' || tokens[i + 2][0] == '(' 
							|| tokens[i + 2][0] == '-' || tokens[i + 2][0] == '+' 
							|| is_character(tokens[i + 2][0])))  
					return i;
			}
		}
	}
	return -1;
}

//토큰의 타입이 struct인 경우 해당 토큰 위치를 리턴
int find_typeSpecifier2(char tokens[TOKEN_CNT][MINLEN]) 
{
    int i, j;

   
    for(i = 0; i < TOKEN_CNT; i++)
    {
        for(j = 0; j < DATATYPE_SIZE; j++)
        {
            if(!strcmp(tokens[i], "struct") && (i+1) <= TOKEN_CNT && is_character(tokens[i + 1][strlen(tokens[i + 1]) - 1]))  
                    return i;		//struct타입인 토큰의 위치
        }
    }
    return -1;
}

//str 문자열이 모두 '*'임
int all_star(char *str)
{
	int i;
	int length= strlen(str);
	
 	if(length == 0)		//널인 경우, 오류	
		return 0;
	
	for(i = 0; i < length; i++)
		if(str[i] != '*')	//str '*'가 아닌 문자가 오면, 오류
			return 0;
	return 1;

}

//인자의 str이 전부 문자인지(기호가 아닌지) 점검
int all_character(char *str)
{
	int i;

	for(i = 0; i < strlen(str); i++)
		if(is_character(str[i]))
			return 1;
	return 0;
	
}

//토큰 재정렬
int reset_tokens(int start, char tokens[TOKEN_CNT][MINLEN]) 
{
	int i;
	int j = start - 1;
	int lcount = 0, rcount = 0;
	int sub_lcount = 0, sub_rcount = 0;

	if(start > -1){		//현재토큰의 위치가 0이상 
		//현재토큰의 데이터타입이 구조체인 경우
		if(!strcmp(tokens[start], "struct")) {		
			strcat(tokens[start], " ");	//토큰에 공백을 붙임
			strcat(tokens[start], tokens[start+1]);	     //해당토큰에 다음토큰을 붙임

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);	//왼쪽으로 한 칸씩 토큰에 재정렬
				memset(tokens[i + 1], 0, sizeof(tokens[0]));	//i+1번째 토큰은 0으로 초기화
			}
		}

		//현재토큰이 "unsigned" 이면서, 다음 토큰이 '괄호닫기' 기호라면,
		else if(!strcmp(tokens[start], "unsigned") && strcmp(tokens[start+1], ")") != 0) {		
			strcat(tokens[start], " ");	//해당토큰에 
			strcat(tokens[start], tokens[start + 1]);	     
			strcat(tokens[start], tokens[start + 2]);

			for(i = start + 1; i < TOKEN_CNT - 1; i++){
				strcpy(tokens[i], tokens[i + 1]);		//토큰 재정렬
				memset(tokens[i + 1], 0, sizeof(tokens[0]));
			}
		}

     		j = start + 1;           
        	while(!strcmp(tokens[j], ")")){
                	rcount ++;
                	if(j==TOKEN_CNT)
                        	break;
                	j++;
        	}
	
		j = start - 1;
		while(!strcmp(tokens[j], "(")){
        	        lcount ++;
                	if(j == 0)
                        	break;
               		j--;
		}
		if( (j!=0 && is_character(tokens[j][strlen(tokens[j])-1]) ) || j==0)
			lcount = rcount;

		if(lcount != rcount )
			return false;

		if( (start - lcount) >0 && !strcmp(tokens[start - lcount - 1], "sizeof")){
			return true; 
		}
		
		else if((!strcmp(tokens[start], "unsigned") || !strcmp(tokens[start], "struct")) && strcmp(tokens[start+1], ")")) {		
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcpy(tokens[start - lcount + 1], tokens[start + rcount]);
		 
			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount; i++) {
				strcpy(tokens[i], tokens[i + lcount + rcount]);
				memset(tokens[i + lcount + rcount], 0, sizeof(tokens[0]));
			}


		}
 		else{
			if(tokens[start + 2][0] == '('){
				j = start + 2;
				while(!strcmp(tokens[j], "(")){
					sub_lcount++;
					j++;
				} 	
				if(!strcmp(tokens[j + 1],")")){
					j = j + 1;
					while(!strcmp(tokens[j], ")")){
						sub_rcount++;
						j++;
					}
				}
				else 
					return false;

				if(sub_lcount != sub_rcount)
					return false;
				
				strcpy(tokens[start + 2], tokens[start + 2 + sub_lcount]);	
				for(int i = start + 3; i<TOKEN_CNT; i++)
					memset(tokens[i], 0, sizeof(tokens[0]));

			}
			strcat(tokens[start - lcount], tokens[start]);
			strcat(tokens[start - lcount], tokens[start + 1]);
			strcat(tokens[start - lcount], tokens[start + rcount + 1]);
		 
			for(int i = start - lcount + 1; i < TOKEN_CNT - lcount -rcount -1; i++) {
				strcpy(tokens[i], tokens[i + lcount + rcount +1]);
				memset(tokens[i + lcount + rcount + 1], 0, sizeof(tokens[0]));

			}
		}
	}
	return true;
}

//tokens 배열 0으로 초기화
void clear_tokens(char tokens[TOKEN_CNT][MINLEN])
{
	int i;

	for(i = 0; i < TOKEN_CNT; i++)
		memset(tokens[i], 0, sizeof(tokens[i]));
}

//우측공백문자 제거
char *rtrim(char *_str)
{
	char tmp[BUFLEN];
	char *end;

	strcpy(tmp, _str);
	end = tmp + strlen(tmp) - 1;	//str의 마지막 주소
	while(end != _str && isspace(*end))
		--end;

	*(end + 1) = '\0'; 	//널문자로 변경경
	_str = tmp;
	return _str;
}

//좌측공백문자 제거
char *ltrim(char *_str)
{
	char *start = _str;

	while(*start != '\0' && isspace(*start))
		++start;
	_str = start;
	return _str;
}

//문자열의 공백문자 제거
char* remove_extraspace(char *str)
{
	int i;
	char *str2 = (char*)malloc(sizeof(char) * BUFLEN);	//str의 공백을 제거한 문자열
	char *start, *end;
	char temp[BUFLEN] = "";
	int position;

	if(strstr(str,"include<")!=NULL){
		start = str;
		end = strpbrk(str, "<");
		position = end - start;
	
		strncat(temp, str, position);
		strncat(temp, " ", 1);
		strncat(temp, str + position, strlen(str) - position + 1);

		str = temp;		
	}
	
	//공백을 제거한 문자열 -> str2 에 할당
	for(i = 0; i < strlen(str); i++)
	{
		if(str[i] ==' ')
		{
			if(i == 0 && str[0] ==' ')
				while(str[i + 1] == ' ')
					i++;	
			else{
				if(i > 0 && str[i - 1] != ' ')
					str2[strlen(str2)] = str[i];
				while(str[i + 1] == ' ')
					i++;
			} 
		}
		else
			str2[strlen(str2)] = str[i];
	}

	return str2;
}


//SPACE 공백문자 삭제
void remove_space(char *str)
{
	char* i = str;
	char* j = str;
	
	while(*j != 0)
	{
		*i = *j++;
		if(*i != ' ')
			i++;
	}
	*i = 0;			//SPACE 문자 삭제			//SPACE 문자 삭제됨됨
}

//'괄호열기' 개수와 '괄호닫기'개수가 같은지 확인
int check_brackets(char *str)		//str << (num번째)학생풀이 ;s_answer버퍼
{
	char *start = str;
	int lcount = 0, rcount = 0;		//lcount :'(' 개수, rcount : ')' 개수
	
	while(1){
		if((start = strpbrk(start, "()")) != NULL){	/* 프로토타입, char *strpbrk(const char *str, const char *key);
								  최초로 일치하는 문자를 가리킴, 일치하는 값이 없을 때 널문자 리턴 */
			//lcount, rcount : '(', ')'를 검색어로 str 에 들어간 개수를 각각 할당
			if(*(start) == '(')
				lcount++;
			else
				rcount++;

			start += 1; 		//검색한 문자, 그 다음부터 다시 검색하기 위함
		}
		else
			break;
	}

	if(lcount != rcount)		//'괄호열기' 수 만큼 '괄호닫기'를 하지 않은 상태 ->에러발생
		return 0;
	else 
		return 1;
}
//토큰의 개수를 구함
int get_token_cnt(char tokens[TOKEN_CNT][MINLEN])
{
	int i;
	
	for(i = 0; i < TOKEN_CNT; i++)
		if(!strcmp(tokens[i], ""))
			break;

	return i;
}
