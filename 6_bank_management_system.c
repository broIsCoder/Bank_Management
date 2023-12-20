#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define max_acc 10
//  1:half day(2pm~) 2: full day 3:half day(2pm)
typedef struct birthdate
{
    int year;
    int month;
    int day;
} birth;

typedef struct acc_info
{
    int id;     
    char name[15];
    char type[10];
    birth date;
    char password[10];
    int balance;
} acc;

void save(FILE *file, int countacc, acc accounts[]);        // save to file
void load(FILE *file, int *countacc, acc accounts[]);          // load to array from file
void create_acc(FILE *file, int *countacc, acc accounts[]);        // promt acc info , save in array from file
void login_acc(int countacc, acc accounts[]);                      // verify from array
void currentaccinfo(int index, acc accounts[]);                  // show from array
void listacc(int countacc, acc accounts[]);                     // show from array
void delete(FILE *file, int *countacc, acc accounts[]);             // delete from array and from file
void transcation(FILE *file, int countacc, acc accounts[]);             // increment or decrement of balance
void update(FILE *file,int countacc ,acc accounts[]);
void fordelay(int n);                           // animation for loading screen

int main()
{   
    system("cls");               //clear console
    int verify = 9870 ;
    int password;
    printf("enter a password:\n");
    scanf("%d",&password);
    if (password == verify){
        printf("\n\nPassword Match!\nLOADING");
        for(int i=0;i<=6;i++)
        {
            fordelay(100000000);
            printf(".");
        }
        system("cls");               //clear console

        FILE *file;
        file = fopen("bank.dat", "r"); 
        acc accounts[max_acc];
        
        printf("\t\t\t********Welcome**********\n");
        int countacc = 0;
        load(file, &countacc, accounts);                        // loading accounts from file to array

        while (1)
        {
            int choice;

            printf("Enter 1. Create an account \n2. Login \n3. List of accounts \n4. Delete an account \n5. Transaction \n6.Update_account \n0. Exit\n\n>>>");
            scanf("%d", &choice);
            getchar();

            if (choice == 1)
            {
                create_acc(file, &countacc, accounts);
            }
            else if (choice == 2)
            {
                login_acc(countacc, accounts);
            }
            else if (choice == 3)
            {
                listacc(countacc, accounts);
            }
            else if (choice == 4)
            {
                delete(file, &countacc, accounts);
            }
            else if (choice == 5)
            {
                transcation(file, countacc, accounts);
            }
            else if (choice == 6)
            {
                update(file, countacc, accounts);
            }
            else if (choice == 0)
            {
                printf("Goodbye\n");
                break;
            }
            else
            {
                printf("Invalid choice. Please try again.\n");
            }
        }
    }
    else {
        printf("INCORRECT PASSWORD !\n");
    }

    return 0;
}

void update(FILE *file,int countacc ,acc accounts[]){
    int acc;

    int verify =0 ;
    int ind ;

    while (verify == 0)
    {
        printf("Select account id to update:");
        scanf("%d",&acc);
        getchar();
        
        for (int i = 0; i < countacc; i++)
        {
            if (accounts[i].id == acc)          // if id found 
            {
                verify = 1;
                ind = i;
                break;
            }
        }

        if (verify == 0){                  // if id not found
            printf("Invalid Id\n");
            printf("0.Exit 1.Try again\n");
            int opt;
            scanf("%d",&opt);
            if (opt == 0){
                break;
            }
        }
    }

    if(verify == 1)
    {      
        char password[10];
        printf("enter password:");
        fgets(password,sizeof(password) ,stdin);
        password[strcspn(password,"\n")] = '\0';     

        if (strcmp(password,accounts[ind].password)== 0)
        {
            int attributes ;
            printf("Balance: $%d\n",accounts[ind].balance);
            printf("UPDate:  1.name 2.type 3.date 4.password 6.exit\n");
            scanf("%d",&attributes);
            getchar();

            if(attributes == 1){
                char newname[10];
                printf("Your current name :%s\n",accounts[ind].name);
                printf("change your name (9 char):");
                fgets(accounts[ind].name, sizeof(accounts[ind].name), stdin);
                accounts[ind].name[strcspn(accounts[ind].name, "\n")] = '\0';     
            }
            else if (attributes == 2){
                int newtype;
                printf("Your current type :%s\n", accounts[ind].type);
                printf("change your type: 1.fixed or 2.interest");
                scanf("%d",&newtype);
                getchar();                                          // remove '/n'

                if (newtype == 1)
                {
                    strcpy(accounts[ind].type, "fixed");
                }
                else if (newtype == 2)
                {
                    strcpy(accounts[ind].type,"interest");
                }
                else{
                    printf("Invalid choice\n");
                }
            }

            else if( attributes == 3){
                 
                printf(" your current birthdate :%d/%d/%d \n",accounts[ind].date.year ,accounts[ind].date.month ,accounts[ind].date.day);
                printf("change your new year: ");
                scanf("%d", &accounts[ind].date.year);
                getchar();                                                   // remove '/n'
                printf("change your new month: ");
                scanf("%d", &accounts[ind].date.month);
                getchar();                                                 // remove '/n'
                printf("change your new day: ");
                scanf("%d", &accounts[ind].date.day);
                getchar();                      

                printf("Your new DOB: %d/%d/%d\n",accounts[ind].date.year ,accounts[ind].date.month ,accounts[ind].date.day);
                
            }

            else if (attributes == 4){
                printf("Your current password : %s",accounts[ind].password);
                
                printf("change your new password (9 char): ");
                fgets(accounts[ind].password,sizeof(accounts[ind].password),stdin) ;
                accounts[ind].password[strcspn(accounts[ind].password,"\n")] = '\0';          // remove '\n'
            }

            else if (attributes == 5)
                 printf("Invalid choice .\n");
            }

            save(file,countacc,accounts);
            

        }
            
        else{
            printf("Password incorrect . Safety Alert !\n");
        }
        
     
        
}

 

void save(FILE *file, int countacc, acc accounts[])                  // save in file
{
    file = fopen("bank.dat", "w");               // will write again and again from  scratch to get all modification as well
    
    for (int i = 0; i < countacc; i++)
    {
        fprintf(file, "%d %9s %9s %d/%d/%d $%d %9s\n",
                accounts[i].id, accounts[i].name, accounts[i].type,
                accounts[i].date.year, accounts[i].date.month, accounts[i].date.day,
                accounts[i].balance, accounts[i].password);
    }

    fclose(file);
}



// loading accounts from file to array
void load(FILE *file, int *countacc, acc accounts[])
{
    file = fopen("bank.dat", "r");

    if (file == NULL)
    {
        printf("Error opening file for reading.\n");
        return;
    }

    *countacc = 0;
    // update array with file content
    while (*countacc < max_acc && fscanf(file, "%d %9s %9s %d/%d/%d $%d %9s",       // will read maximim of 9ch of string
                  &accounts[*countacc].id,& accounts[*countacc].name, &accounts[*countacc].type,
                  &accounts[*countacc].date.year, &accounts[*countacc].date.month, &accounts[*countacc].date.day,
                  &accounts[*countacc].balance, &accounts[*countacc].password) == 8){
                                                                                    // untill there is 8 element in each line
        (*countacc)++;            /// increament of accounts
        }
     
    

    fclose(file);
}

void fordelay(int n) {          // animation for loading screen     // delay
    int i, j;
    for (i = 0; i < n; i++){
        j = i;
    }
}



void create_acc(FILE *file, int *countacc, acc accounts[])           // prompt user ,check id uniqueness , save in array , remove '\n'
{
    if (*countacc >= max_acc)             
    {
        printf("Maximum number of accounts reached.\n");
        return;
    }


    // Storing in array
    // id
    int id;
    int unique = 0;

    if (*countacc > 0 ){               //  if there is any accounts
        while(unique ==0)            // check uniqueness
        {
            printf("Enter ID: ");
            scanf("%d", &id);
            getchar();                // remove '/n'

            for(int i=0 ; i < *countacc ; i++){
                if (id == accounts[i].id)
                {
                    unique = 0;
                    printf("Id taken.\n");
                    break;
                }
                if (accounts == NULL){        // if array and file are empty
                    unique = 1 ;
                }
                else{
                    unique = 1;
                }  
            }

            if (unique == 0)
            {       
                int cho;
                printf("0.Exit 1.Try again\n");
                scanf("%d",&cho);
                if (cho == 0){           //  if exit
                    break;
                }
            }
            
        }
    }

    else {             //  if there is no accounts before
        printf("Enter ID :");
        scanf("%d",&id);
        getchar ();
        unique = 1;
    }

    if (unique == 1){               // if id is unique
        accounts[*countacc].id = id ;
    
        // name
        printf("Enter name (9 char): ");
        fgets(accounts[*countacc].name, sizeof(accounts[*countacc].name), stdin);
        accounts[*countacc].name[strcspn(accounts[*countacc].name, "\n")] = '\0';              // remove '/n'


        // account type
        int type;
        char type1[] = "fixed";
        char type2[] = "interest";
        printf("Enter 1. Interest 2. Fixed: ");
        scanf("%d", &type);
        getchar();                                          // remove '/n'
        if (type == 1)
        {
            strcpy(accounts[*countacc].type, type1);
        }
        else if (type == 2)
        {
            strcpy(accounts[*countacc].type, type2);
        }


        // birthday
        printf("Enter your birthdate:\n");
        printf("Enter your year: ");
        scanf("%d", &accounts[*countacc].date.year);
        getchar();                                                   // remove '/n'
        printf("Enter your month: ");
        scanf("%d", &accounts[*countacc].date.month);
        getchar();                                                 // remove '/n'
        printf("Enter your day: ");
        scanf("%d", &accounts[*countacc].date.day);
        getchar();                                                     // remove '/n'

        // password
        printf("Enter password(9 char ): ");
        fgets(accounts[*countacc].password, sizeof(accounts[*countacc].password), stdin);
        accounts[*countacc].password[strcspn(accounts[*countacc].password, "\n")] = '\0';                // remove '/n'

        // balance
        accounts[*countacc].balance = 0;              // initilize balance of accounts as $0

        // Storing info in array
        save(file, *countacc +1, accounts);               // save in file                  // will save in next line in file
        (*countacc)++;                         // Increment for the next account

        printf("New account successfully created.\n");
        Sleep(3000); // Delay for 3 seconds
        system("cls");
    }

    else{                 // if exit
        return ; 
    }
}



void login_acc(int countacc, acc accounts[])                 // validation of id and password
{
    int id;
    int found = 0;
    int index = -1;

    if (countacc == 0)
    {
        printf("No account is created. Create one now!\n");
        return;
    }

    while (found != 1)
    {
        printf("Enter ID: ");
        scanf("%d", &id);
        getchar();                              // remove '/n'

        for (int i = 0; i < countacc; i++)
        {
            if (id == accounts[i].id)               // validation of id
            {
                found = 1;
                index = i;
                break;
            }
        }

        if (found != 1)              //  if id not found
        {
            printf("Invalid ID!\n");
            printf("\n0. Exit or 1. Try again: ");
            int command;
            scanf("%d", &command);
            getchar();
            if (command == 0)
            {
                break;
            }
        }
    }
 
    if (found == 1)                    // if id found
    {
        int correct = 0;
        while (correct == 0)
        {
            char password[10];
            printf("Enter password for ID %d: ", id);
            fgets(password, sizeof(password), stdin);
            password[strcspn(password, "\n")] = '\0';

            if (strcmp(password, accounts[index].password) == 0)               // if password is correct
            {
                printf("Login successful!\n");
                correct = 1;
                currentaccinfo(index, accounts);
            }
            else                                                  // if password is wrong
            {
                printf("Invalid password.\n");
                int try;
                printf("0. Exit 1. Try again\n");
                scanf("%d", &try);
                getchar();
                if (try == 0)
                {
                    break;
                }
            }
        }
    }
}

void currentaccinfo(int index, acc accounts[])                // info of current logged in account
{
    printf("\nID: %d\nName: %s\nType of Account: %s\nDOB: %d/%d/%d\nBalance: $%d\nPassword: %s\n",
           accounts[index].id, accounts[index].name, accounts[index].type,
           accounts[index].date.year, accounts[index].date.month, accounts[index].date.day,
           accounts[index].balance, accounts[index].password);
}

void listacc(int countacc, acc accounts[])                 // info of all accounts
{
    if (countacc <= 0)
    {
        printf("Nothing to show.\n");
        return;
    }
    printf("\nID | NAME | TYPE | DOB |$BALANCE  | PASSWORD\n");

    for (int i = 0; i < countacc; i++)
    {
        printf("ID: %d \nName: %s \nType: %s \nDOB: %d/%d/%d \nBalance: $%d \nPassword: %s\n\n",
               accounts[i].id, accounts[i].name, accounts[i].type,
               accounts[i].date.year, accounts[i].date.month, accounts[i].date.day,
               accounts[i].balance, accounts[i].password);
    }
}



void delete(FILE *file, int *countacc, acc accounts[])        // shift next acc to deleting account .....and save in file
{
    if (*countacc <= 0)
    {
        printf("Nothing to delete\n");
        return;
    }

    int delId;
    printf("Enter ID to delete: ");
    scanf("%d", &delId);

    int found = 0;
    for (int i = 0; i < *countacc; i++)
    {
        if (accounts[i].id == delId)
        {
            int j = i;
            while (j < *countacc - 1)          // shift next acc to deleting account
            {
                accounts[j] = accounts[j + 1];
                j++;
            }
        }
        found = 1;
        break;
    }

    if (found)
    {
        printf("ID %d has  been deleted\n", delId);
        (*countacc)--;                                   // decrement of account
    }
    else
    {
        printf("ID %d not found\n", delId);
    }

    save(file, *countacc, accounts);               // update with changes
} 

// increment or decrement of balance 
void transcation(FILE *file, int countacc, acc accounts[]){
    int acc;

    int verify =0 ;
    int ind ;

    while (verify == 0)
    {
        printf("Select account id:");
        scanf("%d",&acc);
        getchar();
        
        for (int i = 0; i < countacc; i++)
        {
            if (accounts[i].id == acc)          // if id found 
            {
                verify = 1;
                ind = i;
                break;
            }
        }

        if (verify == 0){                  // if id not found
            printf("Invalid Id\n");
            printf("0.Exit 1.Try again\n");
            int opt;
            scanf("%d",&opt);
            if (opt == 0){
                break;
            }
        }
    }

    if(verify == 1)
    {
        int transaction ;
        printf("Balance: $%d\n",accounts[ind].balance);
        printf("TRANSACTION: 1.Deposit 2.Withdraw 3.Exit\n");
        scanf("%d",&transaction);
        
        int amount ;
        if (transaction == 1)
        {   
            printf("enter amount to deposit: ");
            scanf("%d",&amount);
            getchar();

            accounts[ind].balance += amount;
            printf("$%d is deposited\n",amount);
            save(file,countacc,accounts);
        }

        else if (transaction ==2 )
        {
            printf("enter amount to withdrawn: ");
            scanf("%d",&amount);
            getchar();

            if(accounts[ind].balance >= amount){
            accounts[ind].balance-=amount;
            printf("$%d is withdrawn. %d remains\n",amount,accounts[ind].balance);
            save(file,countacc,accounts);
            }

            else {
            printf("Not enough amount in your balance. &%d only\n",accounts[ind].balance);
            
            }
        }
    }
}