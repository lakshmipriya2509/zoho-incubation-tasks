#include<stdio.h>
#include<malloc.h>


typedef struct variable{
	char var;
	int pow;
} VARIABLE;

typedef struct subEquation{
	int coeff;
	int var_count;
	VARIABLE* variables;
}SUBEQUATION;

typedef struct equation{
	int sub_count;
	SUBEQUATION** subEquation;
}EQUATION;


EQUATION* getEquation();
SUBEQUATION* getSubEquation();
void printEquation(EQUATION* eq);
void sortVariables(VARIABLE* var, int size);
void swapp(VARIABLE* var, int i);

EQUATION* multiply(EQUATION *eq1, EQUATION *eq2);
SUBEQUATION* multiplySubEquation(SUBEQUATION* sub1, SUBEQUATION* sub2);
EQUATION* simplify(EQUATION* eq);
int isequal(VARIABLE* v1, VARIABLE* v2, int size);

void main(){
	
	EQUATION *eq1, *eq2, *result;
	
	eq1 = getEquation();
	eq2 = getEquation();
	
	result = multiply(eq1, eq2);
	printEquation(result);
	
}


EQUATION* getEquation(){
	
	char c;
	int count = 0;
	EQUATION* equation = NULL;
	SUBEQUATION** sub = NULL;
	
	sub = (SUBEQUATION**) malloc(sizeof(SUBEQUATION*));
	
	while((c = getchar()) != '\n'){
		ungetc(c, stdin);
		count++;
		
		sub = (SUBEQUATION**) realloc(sub, count * sizeof(SUBEQUATION*));
		sub[count-1] = getSubEquation();
		
	}
	
	equation = (EQUATION*) malloc(sizeof(EQUATION));
	equation->sub_count = count;
	equation->subEquation = sub;
	
	return equation;
}


SUBEQUATION* getSubEquation(){
	
	char c;
	int coeff;
	int count = 0;
	SUBEQUATION* sub = NULL;
	VARIABLE* var = NULL;
	
	scanf("%d", &coeff);
	
	var = (VARIABLE*) malloc(sizeof(VARIABLE));
	
	c = getchar();
	
	while(isalpha(c)){
		
		count++;
		var = (VARIABLE*) realloc(var, count * sizeof(VARIABLE));
		
		var[count-1].var = c;
		scanf("%d", &var[count-1].pow);
		c = getchar();		
	}
	ungetc(c, stdin);
	
	sortVariables(var, count);
	
	sub = (SUBEQUATION*) malloc(sizeof(SUBEQUATION));
	
	sub->coeff = coeff;
	sub->var_count = count;
	sub->variables = var;
	
	return sub;
	
}


void printEquation(EQUATION* eq){
	
	int i, j;
	
	for(i=0; i<eq->sub_count; i++)
	{
		SUBEQUATION* sub = eq->subEquation[i];
		
		printf("%s", i ? " " : "");
		printf("%s%d", sub->coeff > 0 ? "+" : "", sub->coeff);
		
		for(j=0; j<sub->var_count; j++)
			printf("%c%d", sub->variables[j].var, sub->variables[j].pow);
	}
	
}


void sortVariables(VARIABLE* var, int size){
	
	int i;
	int swapped = 1;
	
	while(swapped){
		
		swapped = 0;
		
		for(i=0; i<size-1; i++){
			if(var[i].var > var[i+1].var){
				swapp(var, i);
				swapped = 1;
			}
		}
		
	}
	
}


void swapp(VARIABLE* var, int i){
	
	VARIABLE temp;
	
	temp = var[i];
	var[i] = var[i+1];
	var[i+1] = temp;
	
}


EQUATION* multiply(EQUATION *eq1, EQUATION *eq2){
	
	int i, j;
	int count = 0;
	EQUATION* result = NULL;
	SUBEQUATION** sub = NULL;
	
	sub = (SUBEQUATION**) malloc(sizeof(SUBEQUATION*));
	
	for(i=0; i<eq1->sub_count; i++){
		
		for(j=0; j<eq2->sub_count; j++){
			
			sub = (SUBEQUATION**) realloc(sub, (count+1) * sizeof(SUBEQUATION*));
			sub[count++] = multiplySubEquation(eq1->subEquation[i], eq2->subEquation[j]);
			
		}
		
	}
	
	result = (EQUATION*) malloc(sizeof(EQUATION));
	result->sub_count = count;
	result->subEquation = sub;
	
	return simplify(result);
	
}


SUBEQUATION* multiplySubEquation(SUBEQUATION* sub1, SUBEQUATION* sub2){
	
	int i, j;
	int count = 0;
	int coeff;
	SUBEQUATION* sub = NULL;
	VARIABLE* v1 = sub1->variables;
	VARIABLE* v2 = sub2->variables;
	VARIABLE* res_var = NULL;
	
	coeff = sub1->coeff * sub2->coeff;
	
	res_var = (VARIABLE*) malloc(sizeof(VARIABLE));
	
	i = j = 0;
	while(i < sub1->var_count && j < sub2->var_count){
		
		res_var = (VARIABLE*) realloc(res_var, (count+1) * sizeof(VARIABLE));
		
		if(v1[i].var < v2[j].var)
			res_var[count] = v1[i++];
		else if(v1[i].var > v2[j].var)
			res_var[count] = v2[j++];
		else{
			res_var[count].var = v1[i].var;
			res_var[count].pow = v1[i++].pow + v2[j++].pow;
		}
		
		count++;
		
	}
	
	while(i < sub1->var_count){
		
		res_var = (VARIABLE*) realloc(res_var, (count+1) * sizeof(VARIABLE));
		
		res_var[count++] = v1[i++];
		
	}
	
	while(j < sub2->var_count){
		
		res_var = (VARIABLE*) realloc(res_var, (count+1) * sizeof(VARIABLE));
		
		res_var[count++] = v2[j++];
		
	}
	
	sub = (SUBEQUATION*) malloc(sizeof(SUBEQUATION));
	sub->coeff = coeff;
	sub->var_count = count;
	sub->variables = res_var;
	
	return sub;
	
}


EQUATION* simplify(EQUATION* eq){
	
	int i, j;
	int count = 0;
	
	SUBEQUATION** sub = NULL;
	
	sub = (SUBEQUATION**) malloc(sizeof(SUBEQUATION));
	
	for(i=0; i<eq->sub_count; i++){
		
		if(eq->subEquation[i]->coeff == 0)	continue;
		
		int coeff = eq->subEquation[i]->coeff;
		
		for(j=i+1; j<eq->sub_count; j++){
			
			if(eq->subEquation[i]->var_count == eq->subEquation[j]->var_count &&
			isequal(eq->subEquation[i]->variables, eq->subEquation[j]->variables, eq->subEquation[i]->var_count)){
				coeff += eq->subEquation[j]->coeff;
				eq->subEquation[j]->coeff = 0;
				eq->subEquation[j]->variables = NULL;
			}
			
		}
		
		sub = (SUBEQUATION**) realloc(sub, (count+1) * sizeof(SUBEQUATION*));
		
		SUBEQUATION* subequ = (SUBEQUATION*) malloc(sizeof(SUBEQUATION));
		subequ->coeff = coeff;
		subequ->variables = eq->subEquation[i]->variables;
		subequ->var_count = eq->subEquation[i]->var_count;
		sub[count] = subequ;
		count++;
		
	}
	
	free(eq->subEquation);
	
	eq->subEquation = sub;
	eq->sub_count = count;
	
	return eq;
	
}


int isequal(VARIABLE* v1, VARIABLE* v2, int size){
	
	int i;
	
	for(i=0; i<size; i++)
		if(v1[i].var != v2[i].var || v1[i].pow != v2[i].pow)
			return 0;
			
	return 1;
	
}

