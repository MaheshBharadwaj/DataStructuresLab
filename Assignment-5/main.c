#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "stack.h"
#include "Functions.h"

int main(){
	DataType dt;
	char infix[20] = {0};

	Stack opStack = createEmptyStack(),
		  postfix = createEmptyStack();

	int val;

	printf("Enter the infix expression: ");
	scanf("%[^\n]",infix);

	for(int i = 0 ; infix[i] ; i++){

		if(isdigit( infix[i]) ){
			
			//Converting character to integer
			val = infix[i] - '0';

			for(int j = i + 1 ; isdigit(infix[j]) ; j++){
				val = val * 10 + (infix[j] - '0');
				i = j + 1;
			}
			push(&postfix,Operand,(void*)&val);
		}
		
		else if(isOpenBracket(infix[i])) 
			push(&opStack,Operator,(void*)&infix[i]);
		
		else if(isOperator(infix[i])){
			if(isEmpty(opStack))
				push(&opStack,Operator,(void*)&infix[i]);
			else{
				Data tmp = peek(opStack);
				while(getPriority( *((char*)tmp.ptr) ) >= getPriority(infix[i]) ){
					push(&postfix,Operator,(void*)tmp.ptr);
					pop(&opStack);
					if(!isEmpty(opStack))
						tmp = peek(opStack);
					else 
						break;
				}
				push(&opStack,Operator,(void*)&infix[i]);
			}	
		}
		
		else if(isClosingBracket(infix[i]) ){
			if(isEmpty(opStack)){
				printf("Error in expression: Unbalanced brackets!\n");
				return 1;
			}

			Data tmp = peek(opStack);
			while(!isOpenBracket( *((char*)tmp.ptr) ) ){
				push(&postfix,Operator,(void*)tmp.ptr);
				pop(&opStack);
				
				if(isEmpty(opStack)){
					printf("Error in expression: Unbalanced brackets!\n");
					return 1;
				}

				tmp = peek(opStack);
			}
			pop(&opStack);

		}
		else continue; //It is a whitespace character			
	}
	
	while(!isEmpty(opStack)){
		Data tmp = pop(&opStack);
		if(isOpenBracket( *((char*)tmp.ptr) ) ){
			printf("Error in expression: Unbalanced brackets!\n");
			return 1;
		}
		push(&postfix,tmp.dt,(void*)tmp.ptr);
	}

	display(postfix);

}

