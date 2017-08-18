#include <stdio.h>
#include <string.h>
//Sarah Carroll-G00330821
#include <conio.h>
#include <stdlib.h>

typedef struct login {
	char username[10];
	char password[10];
	int options;
	struct login *next;
} LOGIN;

LOGIN * loginptr = NULL;


void load_login_file() {
	FILE * lfp;
	LOGIN * lptr;
	LOGIN * temp = NULL;
	int first = 1;
	int result;

	lfp = fopen("login.txt", "r");

	if (lfp != NULL) {
		while (!feof(lfp)) {
			lptr = (LOGIN *)malloc(sizeof(LOGIN));
			result = fscanf(lfp, "%s %s", lptr->username, lptr->password);
			if (result == 0)
				printf("Opps");
			lptr->options = 0;				// default options to zero - future use!
			lptr->next = NULL;				// make next pointer = NULL
			if (temp != NULL)					// have we previous entry
				temp->next = lptr;			// yes, make it point at this entry
			temp = lptr;					// set temp to current entry	
			if (first == 1) {
				loginptr = lptr;			// first time thru? Yes, set head of link list
				first = 0;
			}
			//printf("%p %p %s %s %p\n\n", temp,lptr, lptr->username, lptr->password,lptr->next);

		}
	}
	else {
		printf("\nCannot open login.txt file.");
		printf("\nProgram Terminating ...");
		getch();
		exit(-1);
	}

	fclose(lfp);

}

void getpass(char password[])
{
	char c;
	int i = 0;

	while (i <= 9) {
		c = getch();
		if (c == 13)		// is it enter/newline?
			break;
		if (c == 8) {		// is it backspace?
			if (i > 0) {
				i--;
				printf("%c", c);
			}
			continue;		// back for next character
		}
		password[i] = c;	// store in password char array
		printf("*");		// print * to screen
		i++;				// increment index
	}
	password[i] = '\0';		// add trailing NULL character to string
	return;
}

void logon() {

	char username[10];
	char password[10];
	LOGIN * templ = loginptr;

	printf("\nPlease enter Username: ");
	scanf("%s", username);

	printf("\nPlease enter Password: ");
	// get masked password here - call function
	getpass(password);
	//printf("password =%s", password);
	//scanf("%s", password);

	while (templ != NULL) {
		//printf("%p %s %s %p\n\n", templ, templ->username, templ->password, templ->next);

		if ((stricmp(username, templ->username) == 0) &&
			(stricmp(password, templ->password) == 0))
			break;
		else
			templ = templ->next;
	}

	if (templ == NULL) {
		printf("\nInvalid Username//Password combination entered.");
		printf("\nProgram Terminating ...");
		getch();
		exit(-1);
	}

	return;
}
