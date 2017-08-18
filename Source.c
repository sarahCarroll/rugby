//Sarah Carroll-G00330821
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "login.h"

//global variables
struct node
{
	int  irfuNumber;
	char firstName[30];
	char secondName[30];
	int  age;
	int  height;
	int  weight;
	char club[20];
	char emailAddress[40];
	int  playerPosition;
	int  tackles;
	int  metresRan;

	struct node* NEXT;
};
struct node* head_ptr = NULL;

//pointer to an array of strings
char* playerPositions[] = { "","Prop","Hooker","Second Row","Back Row","Half Back","Centres","Wingers/FullBack" };
char* playerTackles[] = { "","None                           ",
"Less than three times per match",
"Less than five times per match",
"More than five times per match" };
char* playerMetres[] = { "","None               ",
"Less than 10 metres",
"Less than 20 metres",
"More than 20 metres" };

int change = 0;

//function prototypes
void addPlayer(int );
void updatePlayer(struct node *);
struct node* search(int irfuSearch);
void printPlayers();
int  displayMenu();
int  getPlayerPosition();
int  getPlayerTackles();
int  getPlayerMetres();
int  validateEmail(char[]);
void printPlayer();
void deletePlayer(int irfunum);
void saveFile();
void readFile();
void writeToReport();
void printPositionStats(int);
void printWeightStats(int);
void printPositionByHeight(int);
int cmpHeight(const void *, const void *);


void main()
{
	int option = 0;
	int position;
	int result;
	int irfu = 0;
	int choice;
	int statsNo;
	int pos = -1;
	int statWeight;
	struct node* x;


	//load loginFile and log in user
	load_login_file();

	logon();

	printf("\nLogon successfull!");

	readFile();				// Read in contents of Rugby.txt, if it exists ...


	while (option != 9)
	{
		option = displayMenu();

		if (option == 1)				// Add New Player
		{
			printf("\nPlease enter the IRFU number of the new player : ");
			scanf("%d", &irfu);
			x = search(irfu);

			if (x == NULL)
			{
				addPlayer(irfu);
			}
			else
			{
				printf("\nThis IRFU number is already in use ...");
				printf("\nHit any key to return to menu ...");
				getch();
			}

			continue;//to go around loop again

		}//endif

		if (option == 2) {				// Print all Player details
			printPlayers();
			continue;
		}//end option2 if

		if (option == 3) {				// Print details of specific Player (by IRFU # or Name)
			printPlayer();
			printf("\nHit any key to return to menu ...");
			getch();
		}

		if (option == 4)				// Update Player Stats
		{
			printf("\nPlease enter the IRFU number of the Player to Update: ");
			scanf("%d", &irfu);
			x = search(irfu);

			if (x != NULL)
			{
				updatePlayer(x);
			}
			else
			{
				printf("\nThis IRFU number is invalid ...");
				printf("\nHit any key to return to menu ...");
				getch();
			}

		}//endif

		if (option == 5) {				// Delete a Player
			printf("\nPlease enter the IRFU number to be deleted : ");
			scanf("%d", &irfu);

			x = search(irfu);

			if (x == NULL)
			{
				printf("\nThis IRFU number does not exist!");
				printf("\nHit any key to return to menu ...");
				getch();
			}
			else
			{

				deletePlayer(irfu);
				change = 1;
				printf("Player %d Deleted!", irfu);
				printf("\nHit any key to return to menu ...");
				getch();

			}
		}

		if (option == 6) {
			printf("\n\t Eneter 1 to get statistics by position");
			printf("\n\t Eneter 2 to get statistics by weight");
			printf("\n\n Enter Option: ");
			scanf("%d", &statsNo);

			if (statsNo == 1) {
				pos=getPlayerPosition();
				printPositionStats(pos);
				printf("\n\nHit any key to return to menu ...");
			}
			else if (statsNo == 2) {
				printf("Enter Weight you wish to get stats of and above:");
				scanf("%d", &statWeight);
				printWeightStats(statWeight);
				printf("\n\nHit any key to return to menu ...");
			}
			
			

			getch();
		}

		if (option == 7) {
			printf("\nEntered!");
			writeToReport();
			printf("\n You have successfully printed all players to a report file");
			printf("\nHit any key to return to menu ...");
			getch();
		}
		if (option == 8) {
			printPositionByHeight(3);    // Second Row
			printPositionByHeight(4);    // Back Row
			printf("\n\nHit any key to return to menu ...");

			getch();
		}

	}//end while

	if (change != 0) {
		saveFile();					// Save changes, if any, to Rugby.txt
	}

	printf("\n Program Complete.....");
	getch();
}

void addPlayer(int irfu) {

	int goodEmail;

	struct node *temp;
	struct node *prev;
	struct node *newNode;

	newNode = (struct node*)malloc(sizeof(struct node));

	newNode->irfuNumber = irfu;

	printf("\n Enter New players First Name   : ");
	scanf("%s", newNode->firstName);

	printf("\n Enter New players Surname      : ");
	scanf("%s", newNode->secondName);

	printf("\n Enter new players age in years : ");
	scanf("%d", &newNode->age);

	printf("\n Enter new players Height (cm)  : ");
	scanf("%d", &newNode->height);

	printf("\n Enter new players Weight (Kg)  : ");
	scanf("%d", &newNode->weight);

	printf("\n Enter new players Rugby Club   : ");
	scanf("%s", newNode->club);

	goodEmail = -1;
	while (goodEmail == -1) {
		printf("\n Enter new players Email address: ");
		scanf("%s", newNode->emailAddress);

		goodEmail = validateEmail(newNode->emailAddress);
		if (goodEmail < 0)
			printf("\nInvalid email address! Try again ...");
	}

	newNode->playerPosition = getPlayerPosition();

	newNode->tackles = getPlayerTackles();

	newNode->metresRan = getPlayerMetres();



	if (head_ptr == NULL) {			// first entry in linklist
		newNode->NEXT = NULL;
		head_ptr = newNode;
	}
	else {							// find correct location to insert entry
									// printf("EHBJr");
		temp = head_ptr;
		prev = NULL;
		while (temp != NULL) {
			if (irfu < temp->irfuNumber) {
				// insert new node here
				newNode->NEXT = temp;

				if (prev == NULL) {			// insert at start of linklist
					head_ptr = newNode;
				}
				else {
					prev->NEXT = newNode;
				}
				break;						// inserted - break here
			}
			prev = temp;
			temp = temp->NEXT;
		}

		if (temp == NULL) {					// reached end of list?
			prev->NEXT = newNode;			// add to end of list
			newNode->NEXT = NULL;			// end of list
		}
	}
	change = 1;
	printf("\nPlayer %d added!", irfu);
	printf("\nHit any key to return to menu ...");
	getch();

}//end addplayer

struct node* search(int irfuSearch)
{
	struct node *temp;

	temp = head_ptr;
	while (temp != NULL)
	{
		if (irfuSearch == temp->irfuNumber) {
			return temp;
		}

		temp = temp->NEXT;
	}

	return NULL;

}

void printPlayers() {
	int data;

	struct node *temp;

	temp = head_ptr;

	printf("\nNumber of Players in database = %d", count());
	while (temp != NULL) // go to the last node
	{

		///Display the contents

		printf("\n\nIRFU #  %d", temp->irfuNumber);
		printf("\n\tName     : %s, %s", temp->secondName, temp->firstName);
		printf("\n\tAge      : %d", temp->age);
		printf("\n\tHeight   : %d cm", temp->height);
		printf("\n\tWeight   : %d Kg", temp->weight);
		printf("\n\tClub     : %s", temp->club);
		printf("\n\tEmail    : %s", temp->emailAddress);
		printf("\n\tPosition : %s", playerPositions[temp->playerPosition]);
		printf("\n\tTackles  : %s", playerTackles[temp->tackles]);
		printf("\n\tMetres   : %s", playerMetres[temp->metresRan]);
		temp = temp->NEXT;
	}

	if (head_ptr == NULL) {
		printf("\n No Players in database to display");
	}

	printf("\nHit any key to return to menu ...");
	getch();
}

void printPlayer() {
	int choice, irfu;
	struct node* temp;
	char surname[30];

	do {
		printf("\n Search for an individual player by:");
		printf("\n\t1: IRFU Number");
		printf("\n\t2: Player Surname");
		printf("\n\nEnter Option: ");
		scanf("%d", &choice);
		printf("\nchoice = %d!", choice);
	} while (choice < 1 || choice > 2);

	if (choice == 1) {
		printf("\nPlease enter the IRFU number of the player : ");
		scanf("%d", &irfu);
		temp = search(irfu);
		if (temp == NULL) {
			printf("\n Sorry that IRFU Number does not exist");
			return;
		}
	}
	else {
		printf("\nPlease enter the surname of the player : ");
		scanf("%s", surname);

		temp = head_ptr;
		while (temp != NULL) {
			//stricmp=case insensitive search
			if (stricmp(surname, temp->secondName) == 0)
				break;
			temp = temp->NEXT;
			if (temp == NULL) {
				printf("\n Sorry this surname was not found");
				return;
			}
		}
	}

	printf("\n\nIRFU #  %d", temp->irfuNumber);
	printf("\n\tName     : %s, %s", temp->secondName, temp->firstName);
	printf("\n\tAge      : %d", temp->age);
	printf("\n\tHeight   : %d cm", temp->height);
	printf("\n\tWeight   : %d Kg", temp->weight);
	printf("\n\tClub     : %s", temp->club);
	printf("\n\tEmail    : %s", temp->emailAddress);
	printf("\n\tPosition : %s", playerPositions[temp->playerPosition]);
	printf("\n\tTackles  : %s", playerTackles[temp->tackles]);
	printf("\n\tMetres   : %s", playerMetres[temp->metresRan]);

}//end of printplayer()

 //display main menu and await user input
int displayMenu() {
	int choice = 0;
	//Sarah Carroll-g00330821
	printf("\n\n Main Menu - Rugby Performance Metric Ltd.");
	printf("\n =========================================");
	printf("\n 1. Add a Player");
	printf("\n 2. Display all Players");
	printf("\n 3. Display Player details");
	printf("\n 4. Update a Player's statistics");
	printf("\n 5. Delete a Player");
	printf("\n 6. Generate statistics based on user selected Player Position/Weight");
	printf("\n 7. Print all players details to a report file");
	printf("\n 8. List  all players of the following categories in order of height");
	printf("\n    - Second Row");
	printf("\n    - Back Row");
	printf("\n 9. Logout");
	printf("\n\n Enter Option: ");
	scanf("%d", &choice);

	return choice;

}
int getPlayerPosition() {
	int pos = -1;
	do {
		printf("\nEnter new Players position:");
		printf("\n\t1: Prop");
		printf("\n\t2: Hooker");
		printf("\n\t3: Second Row");
		printf("\n\t4: Back Row");
		printf("\n\t5: Half Back");
		printf("\n\t6: Centres");
		printf("\n\t7: Wingers/Full Back");
		printf("\n\nEnter Option: ");
		scanf("%d", &pos);
	} while (pos < 1 || pos > 7);
	return pos;
}

int getPlayerTackles() {
	int tackles = -1;

	do {
		printf("\nEnter Number of tackles per match by this Player: ");
		printf("\n\t1: None                           ");
		printf("\n\t2: Less than three times per match");
		printf("\n\t3: Less	than five times	per	match");
		printf("\n\t4: More	than five times	per	match");
		printf("\n\nEnter Option: ");
		scanf("%d", &tackles);
	} while (tackles < 1 || tackles > 4);

	return tackles;
}

int getPlayerMetres() {
	int metres = -1;
	do {
		printf("\nEnter Number of metres per match by this Player: ");
		printf("\n\t1: None               ");
		printf("\n\t2: Less	than 10	metres");
		printf("\n\t3: Less	than 20	metres");
		printf("\n\t4: More	than 20	metres");
		printf("\n\nEnter Option: ");
		scanf("%d", &metres);
	} while (metres < 1 || metres > 4);

	return metres;
}

void deletePlayer(int irfunum)
{
	struct node *temp, *prev;
	temp = head_ptr;

	while (temp != NULL)
	{
		if (temp->irfuNumber == irfunum)
		{
			if (temp == head_ptr)
			{
				head_ptr = temp->NEXT;
				free(temp);
				return;
			}
			else
			{
				prev->NEXT = temp->NEXT;
				free(temp);
				return;
			}
		}
		else
		{
			prev = temp;
			temp = temp->NEXT;
		}
	}
	return;
}

int count() {
	struct node *temp;
	temp = head_ptr;
	int i = 0;

	while (temp != NULL)
	{
		i++;
		temp = temp->NEXT;
	}
	return i;
}
void saveFile() {
	FILE* filep;
	filep = fopen("Rugby.txt", "w");

	struct node *temp;
	temp = head_ptr;

	while (temp != NULL)
	{
		fprintf(filep, "%d %s %s %d %d %d %s %s %d %d %d\n",
			temp->irfuNumber,
			temp->firstName,
			temp->secondName,
			temp->age,
			temp->height,
			temp->weight,
			temp->club,
			temp->emailAddress,
			temp->playerPosition,
			temp->tackles,
			temp->metresRan);
		temp = temp->NEXT;
	}
	fclose(filep);
}

// Read Rugby.txt file into Linklist

void readFile() {
	FILE* filep;
	struct node *prev;
	struct node *newNode;
	int result;


	filep = fopen("Rugby.txt", "r");

	//If file exists
	if (filep != NULL)
	{
		//Read to the end of the files
		while (!feof(filep))
		{
			newNode = (struct node*)malloc(sizeof(struct node));
			//Read in structure parameters
			result = fscanf(filep, "%d %s %s %d %d %d %s %s %d %d %d",
				&newNode->irfuNumber,
				newNode->firstName,
				newNode->secondName,
				&newNode->age,
				&newNode->height,
				&newNode->weight,
				newNode->club,
				newNode->emailAddress,
				&newNode->playerPosition,
				&newNode->tackles,
				&newNode->metresRan);

			if (result == 11) {
				if (head_ptr == NULL) {			// first entry in linklist
					newNode->NEXT = NULL;
					head_ptr = newNode;
					prev = head_ptr;
				}
				else {							// find correct location to insert entry
					prev->NEXT = newNode;			// add to end of list
					prev = newNode;
					newNode->NEXT = NULL;			// end of list
				}
			}
		}
	}

	// Close the file
	if (filep != NULL)
		fclose(filep);
}

int validateEmail(char email[]) {

	char * temp = email;
	char * start;

	start = strstr(temp, "@");			// does email address contain @
	if (start == NULL) return -1;		// NO!
	if (start == temp) return -1;		// @ cannot be at start of email address

	start = strstr(temp, ".com");		// does email address contain ".com"
	if (start == NULL) return -1;		// NO!

	return 0;							// Good email found
}

void writeToReport() {
	FILE* outp;

	struct node *temp;
	temp = head_ptr;

	outp = fopen("Report.txt", "w");

	//If file exists
	if (outp == NULL)
	{
		printf("Error opening file!\n");
		getch();
		return;
	}
	while (temp != NULL) {

		fprintf(outp, "\n\nIRFU #  %d", temp->irfuNumber);
		fprintf(outp, "\n\tName     : %s, %s", temp->secondName, temp->firstName);
		fprintf(outp, "\n\tAge      : %d", temp->age);
		fprintf(outp, "\n\tHeight   : %d cm", temp->height);
		fprintf(outp, "\n\tWeight   : %d Kg", temp->weight);
		fprintf(outp, "\n\tClub     : %s", temp->club);
		fprintf(outp, "\n\tEmail    : %s", temp->emailAddress);
		fprintf(outp, "\n\tPosition : %s", playerPositions[temp->playerPosition]);
		fprintf(outp, "\n\tTackles  : %s", playerTackles[temp->tackles]);
		fprintf(outp, "\n\tMetres   : %s", playerMetres[temp->metresRan]);

		temp = temp->NEXT;
	}
	fclose(outp);
}

void printPositionStats(int pos) {

	int i, j, pcount;
	int posStatsA[5];
	int posStatsB[5];

	struct node* temp;

	for (i = 0; i<5; i++) {						// Initialise arrays to zero
		posStatsA[i] = posStatsB[i] = 0;
	}

	temp = head_ptr;
	pcount = 0;
	while (temp != NULL) {

		if (temp->playerPosition == pos) {
			pcount++;							// just count players in this POSITION category
			posStatsA[temp->tackles]++;			// increment appropriate bucket
			posStatsB[temp->metresRan]++;		// depending on value in linklist
		}
		temp = temp->NEXT;
	}

	// pcount = count();						// all players?
	printf("\nNumber of players selected %d:", pcount);

	if (pcount == 0) pcount = 1;				//avoid division by zero!

	
	printf("\nPosition : %s", playerPositions[pos]);
	printf("\n\n\tTackles Made");
	printf("\n\t------------");
	for (j = 1; j<5; j++) {
		//printf("\n\tTackles %d:",posStatsA[j]); 
		printf("\n\t%s\t: %05.2f%%", playerTackles[j], ((float)posStatsA[j] / (float)pcount) * 100.00);
	}
	printf("\n\n\tMetres Run");
	printf("\n\t----------");
	for (j = 1; j<5; j++) {
		//printf("\n\tMetres %d:",posStatsB[j]);
		printf("\n\t%s\t: %05.2f%%", playerMetres[j], ((float)posStatsB[j] / (float)pcount) * 100.00);
	}


}//end of printPositionStats()

void printWeightStats(int wkg) {

	int i, j, pcount;
	int posStatsA[5];
	int posStatsB[5];

	struct node* temp;

	for (i = 0; i<5; i++) {						// Initialise arrays to zero
		posStatsA[i] = posStatsB[i] = 0;
	}

	temp = head_ptr;
	pcount = 0;
	while (temp != NULL) {

		if (temp->weight >= wkg) {
			pcount++;							// just count players in this WEIGHT category
			posStatsA[temp->tackles]++;			// increment appropriate bucket
			posStatsB[temp->metresRan]++;		// depending on value in linklist
		}
		temp = temp->NEXT;
	}

	//pcount = count();						// all players?

	printf("\nNumber of players selected %d:", pcount);

	if (pcount == 0) pcount = 1;				//avoid division by zero!

	

	printf("\nWeight >= %d Kg", wkg);
	printf("\n\n\tTackles Made");
	printf("\n\t------------");
	for (j = 1; j<5; j++) {
		//printf("\n\tTackles %d:",posStatsA[j]); 
		printf("\n\t%s\t: %05.2f%%", playerTackles[j], ((float)posStatsA[j] / (float)pcount) * 100.00);
	}
	printf("\n\n\tMetres Run");
	printf("\n\t----------");
	for (j = 1; j<5; j++) {
		//printf("\n\tMetres %d:",posStatsB[j]);
		printf("\n\t%s\t: %05.2f%%", playerMetres[j], ((float)posStatsB[j] / (float)pcount) * 100.00);
	}


}//end of printWeightStats()

int cmpHeight(const void *a, const void *b) {
	return ((const int *)a)[0] - ((const int *)b)[0];
}

void printPositionByHeight(int pos) {

	int i;
	int pcount = count();								// get number of items in linklist

	int list[50][2];								// set max array size-first col holds heights other hold irfu
		
	struct node* temp;

	for (i = 0; i<50; i++) {						// Initialise array to zero
		list[i][0] = list[i][1] = 0;
	}

	temp = head_ptr;
	pcount = 0;
	while (temp != NULL) {

		if (temp->playerPosition == pos) {
			list[pcount][0] = temp->height;		// insert height in first array element
			list[pcount][1] = temp->irfuNumber;	// insert irfu # in second array element
			pcount++;							// just count players in this POSITION category
		}
		temp = temp->NEXT;
	}

	//printf("Before sorting\n");
	//for(i=0; i<pcount; i++){ 
	//    printf ("list[%d][0] = %d list[%d][1] = %d\n", i, list[i][0], i, list[i][1]);
	//}
	//printf("AFTER sorting\n");

	qsort(list, pcount, 2 * sizeof(int), cmpHeight);

	printf("\n\nNumber of Players Selected: %d", pcount);

	printf("\n\nIRFU #  Name                    Height  Position");
	printf("\n------  ----------------------  ------  ------------------------");

	for (i = 0; i<pcount; i++) {
		// printf ("list[%d][0] = %d list[%d][1] = %d\n", i, list[i][0], i, list[i][1]);
		temp = search(list[i][1]);
		printf("\n%6d  %-10s,%-10s   %6d  %s",
			temp->irfuNumber,
			temp->secondName, temp->firstName,
			temp->height,
			playerPositions[pos]);
	}


}//end of printPositionByHeight()
void updatePlayer(struct node *updNode) {

	updNode->tackles = getPlayerTackles();

	updNode->metresRan = getPlayerMetres();

	change = 1;									//mark as changed so it will be saved.

	printf("\n\nPlayer %d (%s,%s)updated!",
		updNode->irfuNumber,
		updNode->secondName,
		updNode->firstName);

	printf("\nHit any key to return to menu ...");
	getch();

}//end updatePlayer()