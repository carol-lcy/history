#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define BYTE unsigned char

int mov[3] = {0,0,0};
int cal[3] = {0,0,1};
int pop[3] = {0,1,0};
int ret[3] = {0,1,1};
int add[3] = {1,0,0};
int and[3] = {1,0,1};
int not[3] = {1,1,0};
int equ[3] = {1,1,1};

int value[2] = {0,0};
int reg_add[2] = {0,1};
int sta_add[2] = {1,0};
int poi_add[2] = {1,1};



int get_opcode(int ***a, int i,int k, int j)
{
	if(a[i][k][j] == mov[2] && a[i][k][j+1] == mov[1] && a[i][k][j+2] == mov[0])
	{
		return 0;
	}
	else if(a[i][k][j] == cal[2] && a[i][k][j+1] == cal[1] && a[i][k][j+2] == cal[0])
	{
		return 1;
	}
	else if(a[i][k][j] == pop[2] && a[i][k][j+1] == pop[1] && a[i][k][j+2] == pop[0])
	{
		return 2;
	}
	else if(a[i][k][j] == ret[2] && a[i][k][j+1] == ret[1] && a[i][k][j+2] == ret[0])
	{
		return 3;
	}
	else if(a[i][k][j] == add[2] && a[i][k][j+1] == add[1] && a[i][k][j+2] == add[0])
	{
		return 4;
	}
	else if(a[i][k][j] == and[2] && a[i][k][j+1] == and[1] && a[i][k][j+2] == and[0])
	{
		return 5;
	}
	else if(a[i][k][j] == not[2] && a[i][k][j+1] == not[1] && a[i][k][j+2] == not[0])
	{
		return 6;
	}
	else if(a[i][k][j] == equ[2] && a[i][k][j+1] == equ[1] && a[i][k][j+2] == equ[0])
	{
		return 7;
	}else{}
	return 0;
}

int get_adtype(int ***a, int i,int k, int j)
{
	if(a[i][k][j] == value[1] && a[i][k][j+1] == value[0])
	{
		return 0;
	}
	else if(a[i][k][j] == reg_add[1] && a[i][k][j+1] == reg_add[0])
	{
		return 1;
	}
	else if(a[i][k][j] == sta_add[1] && a[i][k][j+1] == sta_add[0])
	{
		return 2;
	}
	else if(a[i][k][j] == poi_add[1] && a[i][k][j+1] == poi_add[0])
	{
		return 3;
	}else{}
	return 0;
}

void MOV (int *arr,int arr_pointer,int value)
{
	arr[arr_pointer] = value;
}

void ADD (int *a, int a1,int *b, int b1)
{
	a[a1] = a[a1]+b[b1];
	if(a[a1]>255)
	{
		a[a1] = a[a1]-256;
	}
}

void AND (int *a, int a1,int *b, int b1)
{
	int remainder = 0;
	int *arr1 = (int *)calloc(8,sizeof(int));
	int *arr2 = (int *)calloc(8,sizeof(int));
	for ( int i = 0; i < 8 ; i++)
	{
		if(a[a1] == 0)
		{
			break;
		}
		remainder = a[a1]%2;
		a[a1] /= 2;
		arr1[i] = remainder;
	}
	remainder = 0;
	for ( int i = 0; i < 8 ; i++)
	{
		if(b[b1] == 0)
		{
			break;
		}
		remainder = b[b1]%2;
		b[b1] /= 2;
		arr2[i] = remainder;
	}
	for ( int i = 0; i < 8 ; i++)
	{
		if(arr1[i] == 1 && arr2[i] == 1)
		{
			arr1[i] = 1;
		}
		else
		{
			arr1[i] = 0;
		}
	}
	int number  = 0;
	for ( int i = 0; i < 8 ; i++)
	{
		number += arr1[i]*pow(2,i);
	}
	a[a1] = number;
	free(arr1);
	free(arr2);
}

void NOT (int *a, int a1)
{
	int remainder = 0;
	int *arr1 = (int *)calloc(8,sizeof(int));
	for ( int i = 0; i < 8 ; i++)
	{
		if(a[a1] == 0)
		{
			break;
		}
		remainder = a[a1]%2;
		a[a1] /= 2;
		arr1[i] = remainder;
	}
	for ( int i = 0; i < 8 ; i++)
	{
		if(arr1[i] == 1)
		{
			arr1[i] = 0;
		}
		else
		{
			arr1[i] = 1;
		}
	}
	int number  = 0;
	for ( int i = 0; i < 8 ; i++)
	{
		number += arr1[i]*pow(2,i);
	}
	a[a1] = number;
	free(arr1);
}

void EQU (int *a, int a1)
{
	if(a[a1] == 0)
	{
		a[a1] = 1;
	}
	else
	{
		a[a1] = 0;
	}
}

/*
 * main
 * Scaffold code for the COMP2017/9017 assignment 1
 * This code will take a filename and print the numeric value associated
 * with each byte in the file
 * :: int n_args :: The number of arguments passed to the file
 * :: char** vargs :: Stores the arguments passed to this program as cstrings
 * Returns 0
 */
int main(int n_args, char** vargs)
{

    // For readability
    char* filename = vargs[1];
    // A pro-tip to prevent seg-faults for yourself might be to check
    // if this file actually exists, and if an argument is actually passed...

    // Create a 1 byte buffer. You might want more than one byte yourself
    BYTE buffer;

    // Open our file in 'read binary' mode,
    // I sure do hope that this file actually exists
    FILE *file_ptr = fopen(filename,"rb");

    // Skip to the end of the file
    fseek(file_ptr, 0, SEEK_END);

    // Find the length of the file
    size_t file_length = ftell(file_ptr);

    // Rewind to the start of the file so we can actually read it
    rewind(file_ptr);
 	
	BYTE bitmask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

	int *array = (int *)malloc(sizeof(int) * file_length * 8);
	int stack[10][130];
	for(int i = 0; i<10; i++)
	{
		stack[i][0] = 0;
		stack[i][1] = 3;
		stack[i][2] = 0;
	}
	
	int *regi = (int *)malloc(sizeof(int) * 32);
	int stack_point = 0;
	int regi_point = 0;
	int value_number = 0;
	int cal_result[10];
	int cal_instruction;
	int cal_stack_point;
	int pc_counter;
	
    // For each byte in the file, first write the byte to our buffer
    // then print the integer representation of the byte
    for (int i = 0; i < file_length; i++)
    {
       	fread(&buffer, 1, 1, file_ptr); // read 1 byte
       	// printf("%d ", (int)buffer);

		for (int j = 0; j < 8; j ++)
		{
			array[(file_length - i - 1) * 8 + j] = (bitmask[j] & buffer)?1:0;	
			
		}
    }
	
	// printf("\n");
	for (int i = file_length * 8 - 1; i >= 0;i--)
	{
		// printf("%d", array[i]);
		// if (i % 8  == 0)printf("|");
	}
	
	// printf("\n");
	for (int i = 0; i <file_length * 8;i++)
	{
		if (i % 8  == 0)printf("|");
		printf("%d", array[i]);
		
	}
	printf("\n");
	int ***instr = (int ***)malloc(sizeof(int**) * 10);
	int count_arr = 0;
	int instruction[10][1];
	for(int i = 0; i<10; i++)
	{
		instruction[i][0] = 0;
	}
	int function_number[10];
	int op;
	int ad;
	for(int i = 9; i >= 0 ; i--)
	{
		if((file_length * 8 - count_arr) <= 8)
		{
			instr[i] = (int **)malloc(sizeof(int*) * 2);
			for (int j = 0; j < 2; j++)
			{
				instr[i][j] = (int*)malloc(sizeof(int) * 22);
			}
			continue;
		}
		//get instruction
		instruction[i][0] = 0;
		for (int j = 0; j<8 ; j++)
		{
		instruction[i][0] += array[count_arr + j]*pow(2,j);
		}
		count_arr = count_arr + 8;
		instr[i] = (int **)malloc(sizeof(int*) * (instruction[i][0]+2));
		for (int j = 0; j < instruction[i][0]+2; j++)
		{
			instr[i][j] = (int*)malloc(sizeof(int) * 22);
		}
		for(int j = instruction[i][0]+1; j >= 2 ; j--)
		{
			int count_ins = 0;//pointer to instr, to know the position
			int count_tot = 0;
			for(int k = 0; k < 3 ; k++)
			{
				instr[i][j][count_ins] = array[count_arr + k];
				count_ins++;
			}
			op = get_opcode(instr,i,j,count_tot);
			count_arr = count_arr + 3;
			count_tot = count_tot + 3;
			if(op == 3)
			{
				// printf("return---------------\n");
				continue;
			}
			//POP stack
			else if (op == 2)
			{
				for(int k = 0; k < 9 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				ad = get_adtype(instr,i,j,count_tot);
				count_arr = count_arr + 9;
				count_tot = count_tot + 9;
				continue;
			}
			//MOV
			else if (op == 0)
			{
				for(int k = 0; k < 2 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				ad = get_adtype(instr,i,j,count_tot);
				count_arr = count_arr + 2;
				count_tot = count_tot + 2;

				//stack
				if(ad == 2)
				{
					for(int k = 0; k < 7 ; k++)
					{
						instr[i][j][count_ins] = array[count_arr + k];
						count_ins++;
					}
					count_arr = count_arr + 7;
					count_tot = count_tot + 7;

					//look for address type
					for(int k = 0; k < 2 ; k++)
					{
						instr[i][j][count_ins] = array[count_arr + k];
						count_ins++;
					}
					ad = get_adtype(instr,i,j,count_tot);
					count_arr = count_arr + 2;
					count_tot = count_tot + 2;
					//value
					if(ad == 0)
					{
						for(int k = 0; k < 8 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 8;
						count_tot = count_tot + 8;
					}
					//register
					else if(ad == 1)
					{
						for(int k = 0; k < 3 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 3;
						count_tot = count_tot + 3;
						continue;
					}
					//stack
					else if(ad == 2)
					{
						for(int k = 0; k < 7 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 7;
						count_tot = count_tot + 7;
						continue;
					}
					else{}	
				}
				//register
				else if(ad == 1)
				{
					for(int k = 0; k < 3 ; k++)
					{
						instr[i][j][count_ins] = array[count_arr + k];
						count_ins++;
					}
					count_arr = count_arr + 3;
					count_tot = count_tot + 3;
					for(int k = 0; k < 2 ; k++)
					{
						instr[i][j][count_ins] = array[count_arr + k];
						count_ins++;
					}
					ad = get_adtype(instr,i,j,count_tot);
					count_arr = count_arr + 2;
					count_tot = count_tot + 2;

					if(ad == 0)
					{
						for(int k = 0; k < 8 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 8;
						count_tot = count_tot + 8;
						continue;
					}

					else if(ad == 2)
					{
						for(int k = 0; k < 7 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 7;
						count_tot = count_tot + 7;
						continue;
					}
					else{}	
				}
				//pointer
				else if(ad == 3)
				{
					for(int k = 0; k < 7 ; k++)
					{
						instr[i][j][count_ins] = array[count_arr + k];
						count_ins++;
					}
					count_arr = count_arr + 7;
					count_tot = count_tot + 7;
					for(int k = 0; k < 2 ; k++)
					{
						instr[i][j][count_ins] = array[count_arr + k];
						count_ins++;
					}
					ad = get_adtype(instr,i,j,count_tot);
					count_arr = count_arr + 2;
					count_tot = count_tot + 2;

					if(ad == 0)
					{
						for(int k = 0; k < 8 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 8;
						count_tot = count_tot + 8;
						continue;
					}
					//stack
					else if(ad == 2)
					{
						for(int k = 0; k < 7 ; k++)
						{
							instr[i][j][count_ins] = array[count_arr + k];
							count_ins++;
						}
						count_arr = count_arr + 7;
						count_tot = count_tot + 7;
						continue;
					}
				}
			}
			//ADD register to register
			else if (op == 4)
			{
				for(int k = 0; k < 10 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				count_arr = count_arr + 10;
				count_tot = count_tot + 10;
				continue;		
			}
			//AND register to register
			else if (op == 5)
			{
				for(int k = 0; k < 10 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				count_arr = count_arr + 10;
				count_tot = count_tot + 10;
				continue;
			}
			//NOT register
			else if (op == 6)
			{
				for(int k = 0; k < 5 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				count_arr = count_arr + 5;
				count_tot = count_tot + 5;
				continue;
			}
			//EQU register
			else if (op == 7)
			{
				for(int k = 0; k < 5 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				count_arr = count_arr + 5;
				count_tot = count_tot + 5;
				continue;
			}
			//CAL register
			else if (op == 1)
			{
				for(int k = 0; k < 19 ; k++)
				{
					instr[i][j][count_ins] = array[count_arr + k];
					count_ins++;
				}
				count_arr = count_arr + 19;
				count_tot = count_tot + 19;
				continue;
			}
		}
		//look for argument 
		instr[i][1][0] = 0;
		for(int j = 0;j < 4; j++)
		{
			instr[i][1][0] += array[count_arr + j]*pow(2,j);
		}
		count_arr = count_arr + 4;
		//function number
		instr[i][0][0] = 0;
		for(int j = 0;j < 4; j++)
		{
			instr[i][0][0] += array[count_arr + j]*pow(2,j);
		}
		count_arr = count_arr + 4;
		function_number[instr[i][0][0]] = i;
		continue;
	}

	int times = 0; //判断直接退出，在overflow 和 main 的 return
	int copy = 0; //判断该段已经被复制过
	int main = 0; //判断谁发出cal
	int help = 0; //判断cal谁
	for(int i = 0; i < 10 ; i++)
	{
		if(times == 1)
		{
			break;
		}
		if(instruction[i][0] == 0)
		{
			continue;
		}
		if(instr[i][0][0] != 0)
		{
			continue;
		}
		int j = 2;
		while(j < instruction[i][0]+2)
		{
			if(instr[i][0][0] == 0 && instr[i][1][0] == 0)
			{
				cal_instruction = 0;
			}
			else if(instr[i][0][0] != 0 && instr[i][1][0] != 0 && copy == 0)
			{
				for(int x = 0; x < instr[i][1][0]; x++)
				{
					stack[help][3 + x] = stack[main][cal_stack_point+x];
				}
				copy = 1;
			}else{}	
			if (stack[i][1] > 128)
			{
				printf("Stack Overflow!\n");
				times = 1;
				break;
			}
			if(stack[i][2]+2 > j)
			{
				j = stack[i][2]+2;
			}
			else if(stack[i][2]+2 < j)
			{
				j = stack[i][2]+2;
			}
			for(int k = 0; k < 22 ; k++)
			{
				int count_tot = 0;
				op = get_opcode(instr,i,j,count_tot);
				count_tot = count_tot + 3;
				//MOV
				if (op == 0)
				{
					ad = get_adtype(instr,i,j,count_tot);
					int newad;
					count_tot = count_tot + 2;
					if(ad == 2)
					{
						stack_point = 0;
						for(int k = 0; k < 7 ; k++)
						{
							stack_point += instr[i][j][count_tot + k]*pow(2,k);
						}
						count_tot = count_tot + 7;
						newad = get_adtype(instr,i,j,count_tot);
						count_tot = count_tot + 2;
						if(newad == 0)
						{
							value_number = 0;
							for(int k = 0; k < 7 ; k++)
							{
								value_number += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(stack[i],stack_point,value_number);
							stack[i][2]++;
							stack[i][1] = stack_point + 1;
							j++;
							break;
						}
						else if(newad == 1)
						{
							regi_point = 0;
							for(int k = 0; k < 3 ; k++)
							{
								regi_point += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(stack[i],stack_point,regi[regi_point]);
							stack[i][stack_point] = regi[regi_point];
							stack[i][2]++;
							stack[i][1] = stack_point + 1;
							j++;
							break;
						}
						else if(newad == 2)
						{
							int new_stack_point = 0;
							for(int k = 0; k < 7 ; k++)
							{
								new_stack_point += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(stack[i],stack_point,stack[i][new_stack_point]);
							stack[i][2]++;
							j++;
							break;
						}

					}
					else if(ad == 1)
					{
						regi_point = 0;
						for(int k = 0; k < 3 ; k++)
						{
							regi_point += instr[i][j][count_tot + k]*pow(2,k);
						}
						count_tot = count_tot + 3;//the number of regsiter

						newad = get_adtype(instr,i,j,count_tot);
						count_tot = count_tot + 2;//number of new ad
						if(newad == 0)
						{	
							value_number = 0;
							for(int k = 0; k < 8 ; k++)
							{
								value_number += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(regi,regi_point,value_number);
							stack[i][2]++;
							j++;
							break;
						}
						if(newad == 2)
						{	
							stack_point = 0;
							for(int k = 0; k < 7 ; k++)
							{
								stack_point += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(regi,regi_point,stack[i][stack_point]);
							stack[i][2]++;
							j++;
							break;
						}
					}
					else if(ad == 3)
					{
						stack_point = 0;
						for(int k = 0; k < 7 ; k++)
						{
							stack_point += instr[i][j][count_tot + k]*pow(2,k);
						}
						count_tot = count_tot + 7;//the number of regsiter

						newad = get_adtype(instr,i,j,count_tot);
						count_tot = count_tot + 2;//number of new ad
						if(newad == 0)
						{	
							value_number = 0;
							for(int k = 0; k < 8 ; k++)
							{
								value_number += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(stack[i],stack[i][stack_point],value_number);
							stack[i][1] = stack[i][stack_point] + 1;
							stack[i][2]++;
							j++;
							break;
						}
						//stack
						else if(newad == 2)
						{	
							int new_stack_point = 0;
							for(int k = 0; k < 7 ; k++)
							{
								new_stack_point += instr[i][j][count_tot + k]*pow(2,k);
							}
							MOV(stack[i],stack[i][stack_point],stack[i][new_stack_point]);
							stack[i][1] = stack[i][stack_point] + 1;
							stack[i][2]++;
							j++;
							break;
						}

					}
				}
				//CAL
				else if (op == 1)
				{
					count_tot = count_tot + 2;
					stack_point = 0;
					for(int k = 0; k < 7 ; k++)
					{
						stack_point += instr[i][j][count_tot + k]*pow(2,k);
					}
					count_tot = count_tot + 7;

					count_tot = count_tot + 2;//number of value
					value_number = 0;
					for(int k = 0; k < 8 ; k++)
					{
						value_number += instr[i][j][count_tot + k]*pow(2,k);
					}
					copy = 0;
					stack[i][2]++;
					pc_counter = stack[i][2];
					stack[i][2] = 0;
					main = i;
					i = function_number[value_number]; // 第几个function
					help = i;
					cal_stack_point = stack_point; // stack从哪里复制
					cal_instruction = j; // 这一行的行数
					j = 2;
					break;
				}
				//ADD
				else if (op == 4)
				{
					int regi1 = 0,regi2 = 0;
					count_tot = count_tot + 2;
					for(int k = 0; k < 3 ; k++)
					{
						regi1 += instr[i][j][count_tot + k]*pow(2,k);
					}
					count_tot = count_tot + 3;

					count_tot = count_tot + 2;//number of new ad
					for(int k = 0; k < 3 ; k++)
					{
						regi2 += instr[i][j][count_tot + k]*pow(2,k);
					}
					ADD(regi,regi2,regi,regi1);
					stack[i][2]++;
					j++;
					break;
				}
				//AND
				else if (op == 5)
				{
					count_tot = count_tot + 2;
					int regi1 = 0,regi2 = 0;
					for(int k = 0; k < 3 ; k++)
					{
						regi1 += instr[i][j][count_tot + k]*pow(2,k);
					}
					count_tot = count_tot + 3;//the number of regsiter
					count_tot = count_tot + 2;//number of new ad
					for(int k = 0; k < 3 ; k++)
					{
						regi2 += instr[i][j][count_tot + k]*pow(2,k);
					}
					AND(regi,regi2,regi,regi1);
					stack[i][2]++;
					j++;
					break;
				}
				//NOT
				else if (op == 6)
				{
					count_tot = count_tot + 2;
					regi_point = 0;
					for(int k = 0; k < 3 ; k++)
					{
						regi_point += instr[i][j][count_tot + k]*pow(2,k);
					}
					count_tot = count_tot + 3;//the number of regsiter
					NOT(regi,regi_point);
					stack[i][2]++;
					j++;
					break;
				}
				//EQU
				else if (op == 7)
				{
					count_tot = count_tot + 2;
					regi_point = 0;
					for(int k = 0; k < 3 ; k++)
					{
						regi_point += instr[i][j][count_tot + k]*pow(2,k);
					}
					count_tot = count_tot + 3;//the number of regsiter
					EQU(regi,regi_point);
					stack[i][2]++;
					j++;
					break;
				}
				//POP
				else if (op == 2)
				{
					count_tot = count_tot + 2;
					stack_point = 0;
					for(int k = 0; k < 7 ; k++)
					{
						stack_point += instr[i][j][count_tot + k]*pow(2,k);
					}
					if(instr[i][0][0] == 0)
					{
						printf("%d\n",stack[i][stack_point]);
					}
					else
					{
						cal_result[0] = stack[i][stack_point];
					}
					stack[i][stack_point] = 0;
					stack[i][1] = stack_point;
					j++;
					stack[i][2]++;
					break;
				}
				//return
				else if (op == 3)
				{
					if(instr[i][0][0] == 0)
					{
						stack[i][2]++;
						j++;
						times = 1;
						break;
					}
					else
					{
						stack[main][stack[main][1]] = cal_result[0];
						stack[i][2]++;
						i = main;
						j = cal_instruction+1;
						stack[main][2] = cal_instruction-1;
						break;
					}
				}
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		for(int j = 0; j < (instruction[i][0]+2); j++)
		{
			free(instr[i][j]);
		}
	}
	for (int i = 0; i < 10; i++)
	{
		free(instr[i]);
	}
	free(instr);
	free(array);
	free(regi);
    return 0;
}

