// Denis Gantman 213913809

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COMMANDSIZE 80
#define MAXNAMESIZE 12
#define MAXMATRIXCOUNT 2

// Global variable of stringsplitter function
char* ss_buffer = NULL;

/*
 Item 0: Pointer to name char*
 Item 1: Pointer to N int*
 Item 2: Pointer to M int*
 Item 3: Pointer to array int**
*/
void** MatrixStructureArray[MAXMATRIXCOUNT];

// Change from string to int
int str2int(const char* str) {
	if (str == NULL) {
		return 0;
	}

	int sign = 1;
	int res = 0;
	int i = 0;

	// Handle leading white spaces
	while (str[i] == ' ') {
		i++;
	}

	// Handle sign
	if (str[i] == '-') {
		sign = -1;
		i++;
	}
	else if (str[i] == '+') {
		i++;
	}

	// Process digits
	while (str[i] != '\0') {
		if (str[i] >= '0' && str[i] <= '9') {
			res = res * 10 + (str[i] - '0');
			i++;
		}
		else {
			break;
		}
	}

	return sign * res;
}

// Validate integer string
int isStringInteger(char* str)
{
	// Check for the NULL poiner
	if (str == NULL)
		return 0;

	int res = 1;
	for (int i = 0; i < strlen(str); i++)
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			res = 0;
	}

	return res;
}

int isMatrixNameValid(char* str)
{
	int res = 1;

	for (int i = 0; i < strlen(str); i++)
	{
		if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')))
			res = 0;
	};

	if (strlen(str) > 12)
		res = 0;
	return res;
};

// Allocate memory for new Matrix Structure
void** AllocateMatrixMemory(int n, int m)
{
	void** res = (void**)calloc(sizeof(void*), 4);
	if (res == NULL) return NULL;

	res[0] = calloc(sizeof(char), MAXNAMESIZE);
	res[1] = calloc(sizeof(int), 1);
	res[2] = calloc(sizeof(int), 1);

	int** matrix = (int**)calloc(n, sizeof(int*));

	if (matrix == NULL) return NULL;
	for (int i = 0; i < n; i++)
	{
		matrix[i] = (int*)calloc(m, sizeof(int));
	};
	res[3] = matrix;

	return res;
};

// Get Matrix Name (variable name) from the data structure
char* getmName(void** data)
{
	char* res = NULL;
	res = (char*)data[0];
	return res;
};

// Set matrix name into data structure
void setmName(void** data, char* name)
{
	if (name == NULL)
		return;

	// We have a limit for the max name size. Need to be checked. But we have made an assumption that we got the correct string lenght
	strcpy_s((char*)data[0], MAXNAMESIZE, name);
};

// Get Matrix N dimention from the data structure
int getmN(void** data)
{
	int res = 0;
	res = *(int*)data[1];
	return res;
};

// Set N dimention for the matrix into data structure
void setmN(void** data, int value)
{
	*(int*)data[1] = value;
};

// Get Matrix M dimention from the data structure
int getmM(void** data)
{
	int res = 0;
	res = *(int*)data[2];
	return res;
};

// Set M dimention for the matrix into data structure
void setmM(void** data, int value)
{
	*(int*)data[2] = value;
};

// Get Matrix data from the data structure
int** getmData(void** data)
{
	int** res = 0;
	res = (int**)data[3];
	return res;
};

// Free Matrix Memory that was allocated by the function AllocateMatrixMemory
void FreeMatrixMemory(void** data)
{
	// Free memory for the Matrix Elements
	int n = getmN(data);
	int** matrix = getmData(data);
	for (int i = 0; i < n; i++)
		free(matrix[i]);

	// Free Varibles
	free(data[0]); // name
	free(data[1]); // N
	free(data[2]); // M

	// Free container
	free(data);
};

// That function performs registration of new matrix by name and dimentions
void** RegisterMatrix(char* name, int n, int m)
{

	void** data = NULL;

	// Allocate memory for the matrix data structure
	data = AllocateMatrixMemory(n, m);

	// Set name, N, M values
	setmName(data, name);
	setmN(data, n);
	setmM(data, m);

	// Looking for the free slot in MatrixStructureArray
	for (int i = 0; i < MAXMATRIXCOUNT; i++)
	{
		if (MatrixStructureArray[i] == NULL)
		{
			MatrixStructureArray[i] = data;
			return data;
		};
	};

	// We was not able to register new matrix structure. So, we need to free allocated memory :(
	FreeMatrixMemory(data);
	return NULL;
};

// Return the number of MatrixStructures that have been regitered
int getMatrixStructureCount()
{
	int res = 0;
	// Read array of MAtrix Structures and count non NULL values.
	for (int i = 0; i < MAXMATRIXCOUNT; i++)
	{
		if (MatrixStructureArray[i] != NULL)
			res++;
	}

	return res;
};

// Returns the pointer to a Matrix Structure by name. That function enumerate all Matrix Structures and find the match by given name.
void** findMatrixStructureByName(char* name)
{
	void** res = NULL;

	for (int i = 0; i < MAXMATRIXCOUNT; i++)
	{
		void** data = MatrixStructureArray[i];
		if (data != NULL)
		{
			char* targetname = getmName(data);
			if (!strcmp(name, targetname))
				return data;
		};
	};

	return res;
};

// Check is Matrix with provided name is already registered in the Matrix Structure Array
int isMatrixRegistered(char* name)
{
	int res = 0;
	// Try to find the Matrix with the provided name
	void** data = findMatrixStructureByName(name);
	if (data != NULL)
		res = 1;
	return res;
};

// String splitter
char* strsplitter(char* str)
{
	if (str != NULL)
	{
		ss_buffer = str;
	}

	if (ss_buffer == NULL || *ss_buffer == '\0')
	{
		return NULL;
	}

	// Skip leading delimiters
	while (*ss_buffer == ' ' || *ss_buffer == '\n' || *ss_buffer == '\t')
	{
		ss_buffer++;
	}

	char* token_start = ss_buffer;
	char* token_end = ss_buffer;

	// Find token end or end of string
	while (*token_end != ' ' && *token_end != '\n' && *token_end != '\t' && *token_end != '\0')
	{
		token_end++;
	}

	if (*token_end != '\0')
	{
		*token_end = '\0';
		ss_buffer = token_end + 1;
	}
	else
	{
		ss_buffer += strlen(ss_buffer);
	}

	return token_start;
};

// The function that that split that string by " ", "\t", "n"
char* getNextToken(char* string)
{
	char* res = NULL;
	res = strsplitter(string);
	return res;
};

// Performs right shift for one demension
void shiftRight(int arr[], int n)
{
	if (n <= 1)
	{
		return; // No need to shift if array size is 0 or 1
	}
	int last = arr[n - 1];

	for (int i = n - 1; i > 0; i--)
	{
		arr[i] = arr[i - 1];
	};

	arr[0] = last;
};

// Performs left shift for one demension
void shiftLeft(int arr[], int n)
{
	if (n <= 1)
	{
		return; 
	}

	int first = arr[0];

	for (int i = 0; i < n - 1; i++)
	{
		arr[i] = arr[i + 1];
	};

	arr[n - 1] = first;
};

// Performs matrix shift operation
void shiftMatrix(void** data, int shift)
{
	int n = getmN(data);
	int m = getmM(data);
	int** matrix = getmData(data);
	int shiftDirection = 1;

	// Detect shift direction
	if (shift < 0)
	{
		shiftDirection = -1;
		shift = shift * -1;
	}

	// Performs shift for each matrix line
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < shift; j++)
		{
			if (shiftDirection == 1)
			{
				shiftRight(matrix[i], m);
			}
			else
			{
				shiftLeft(matrix[i], m);
			}
		};
	};
};

// This funciton parse the command line line and tryes to execute command
int executeCommandLine(char* input)
{
	char* token = NULL;

	// We need to have a copy of the input line for the getNextToken function
	char copy[COMMANDSIZE];
	strcpy_s(copy, COMMANDSIZE, input);

	token = getNextToken(copy);

	if (token == NULL)
		return 0;

	if (strcmp(token, "zeros") == 0)
	{
		// Parameter: name
		char* msName = getNextToken(NULL);
		// Parameter: N
		char* msN = getNextToken(NULL);
		// Parameter: M
		char* msM = getNextToken(NULL);
		// Checking for the Extra Parameters
		char* extPar = getNextToken(NULL);

		if (!((msName != NULL) && (msM != NULL) && (msN != NULL) && (extPar == NULL)))
			return -2;

		// Validate string parameters for N and M.
		if (!isStringInteger(msM))
		{
			printf("Error: invalid dimension!\n");
			return 1;
		}
		if (!isStringInteger(msN))
		{
			printf("Error: invalid dimension!\n");
			return 1;
		}
		int miM = str2int(msM);
		int miN = str2int(msN);

		if (getMatrixStructureCount() == MAXMATRIXCOUNT)
		{
			printf("Error: zeros cannot save more than %d variables!\n", MAXMATRIXCOUNT);
			return 1;
		};

		if ((miM < 1) || (miN < 1))
		{
			printf("Error: invalid dimension!\n");
			return 1;
		}

		if (!(isMatrixNameValid(msName)))
		{
			printf("Error: '%s' - invalid variable name!\n", msName);
			return 1;
		};

		if (isMatrixRegistered(msName))
		{
			printf("Error: variable name '%s' is already in use!\n", msName);
			return 1;
		}

		RegisterMatrix(msName, miN, miM);

		return 1;
	}
	else if (strcmp(token, "set") == 0)
	{

		// Read matrix name
		char* name = getNextToken(NULL);
		void** data = findMatrixStructureByName(name);
		if (data == NULL)
		{
			printf("Error: '%s' - unknown variable!\n", name);
			return 1;
		};

		// What we should do if user provided wrong matrix name
		if (data == NULL)
		{

			printf("Variable %s does not exist\n", name);
			return 1;
		};

		int n = getmN(data);
		int m = getmM(data);
		int** matrix = getmData(data);

		// Enumerate matrix element and ask for user input
		char input[COMMANDSIZE];
		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < m; j++)
			{
				// Read value from the user input
				fgets(input, sizeof(input), stdin);
				matrix[i][j] = str2int(input);
			}
		}

		return 1;
	}
	else if (strcmp(token, "print") == 0)
	{
		// Read matrix name
		char* name = getNextToken(NULL);
		void** data = findMatrixStructureByName(name);
		if (data == NULL)
		{
			printf("Error: '%s' - unknown variable!\n", name);
			return 1;
		};

		// Read Matrix Data structure
		char* x_name = getmName(data);
		int x_n = getmN(data);
		int x_m = getmM(data);
		int** x_d = getmData(data);

		// Print Matrix Data
		for (int i = 0; i < x_n; i++)
		{
			for (int j = 0; j < x_m; j++)
			{
				// Print Value
				printf("%d", x_d[i][j]);
				// Print delimeter between values. We dont need to have delimeter on the last item in row
				if (j != x_m - 1)
					printf(" ");
			}
			// Print new line
			printf("\n");
		};

		return 1;
	}
	else if (strcmp(token, "isSquare") == 0)
	{
		// Read matrix name
		char* name = getNextToken(NULL);
		void** data = findMatrixStructureByName(name);
		if (data == NULL)
		{
			printf("Error: '%s' - unknown variable!\n", name);
			return 1;
		}

		// Read Matrix dimetions
		int n = getmN(data);
		int m = getmM(data);

		if (n != m)
		{
			printf("Matrix '%s' isn't square with size %dx%d\n", name, n, m);
		}
		else
		{
			printf("Matrix '%s' is square with size %dx%d\n", name, n, m);
		};

		return 1;
	}
	else if (strcmp(token, "shift") == 0)
	{
		// Read matrix name
		char* name = getNextToken(NULL);
		void** data = findMatrixStructureByName(name);
		if (data == NULL)
		{
			printf("Error: '%s' - unknown variable!\n", name);
			return 1;
		};

		// Read shift number
		char* sShift = getNextToken(NULL);

		if (sShift == NULL)
		{
			return -2;
		};

		int shift = str2int(sShift);
		shiftMatrix(data, shift);

		return 1;
	}
	else if (strcmp(token, "exit") == 0)
	{
		return -1;
	}

	return -2;
}

int main()
{

	// Zero Initialization of the matrix container array
	char input[COMMANDSIZE];

	// Loop that reads the user input in command line
	while (1)
	{
		// Print user prompt
		printf("$ ");

		// Read command line from the user input
		fgets(input, sizeof(input), stdin);

		
		// Try to Execute the command line that was provided by user input
		int res = executeCommandLine(input);

		// Analyse the execution result
		if (res == -2)
		{
			// illegal command
			printf("Error: illegal command!\n");
		}
		else if (res == -1)
		{
			// exit command
			break;
		}
	}

	// Before Exit we need to free all memory that was allocated for the Matrix Structures
	for (int i = 0; i < MAXMATRIXCOUNT; i++)
	{
		void** data = MatrixStructureArray[i];
		if (data != NULL)
			FreeMatrixMemory(data);
	}

	// Say Goodbye for the user
	printf("Goodbye.\n");
};