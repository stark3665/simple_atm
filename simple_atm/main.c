// For simulating account transaction data,
// binary file read/write manner is adopted.

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

unsigned long deposit, withdraw;
int choice, input_pin, k;
char insert_card = 'n';
char transaction = 'y';

typedef struct AccountInfo
{
	int pin;
	unsigned long balance[2]; // 0 : check account  1 : saving account, should be long overflow case.
} AccountInfo;

void check_cardreader()
{
	while (insert_card != 'y')
	{
		printf("PLEASE PUT YOUR DEBIT CARD INTO THE ATM. (y/n)");
		scanf("%s", &insert_card);
		if (insert_card != 'y')
			printf("PLEASE INSERT CARD AGAIN OR PROPERLY \n");
	}
}

void write_acc(AccountInfo* info)
{
	FILE* binInfo = fopen("info.bin", "wb");

	if (binInfo == NULL) {
		printf("error: failed to getting info from bank system \n");
		return;
	}

	fwrite(info, sizeof(AccountInfo), 1, binInfo);
	fclose(binInfo);
	//free(info);
}

void update_acc(AccountInfo* info)
{
	write_acc(info);
}

void write_acc_debug()
{	
	AccountInfo* info = (AccountInfo*)(malloc(sizeof(AccountInfo)));
	info->pin = 1234;
	info->balance[0] = 1300;
	info->balance[1] = 5500;

	FILE* binInfo = fopen("info.bin", "wb");

	if (binInfo == NULL) {
		printf("error: unable to open \n");
		return;
	}

	fwrite(info, sizeof(AccountInfo), 1, binInfo);
	fclose(binInfo);
	free(info);
}

AccountInfo* auth_pin()
{
	// read user pin info from file : 
	// It was originally supposed to be obtained from the bank, 
	// but has been temporarily replaced by reading a text file.
	FILE* fp = fopen("info.bin", "rb");

	if (fp == NULL)
	{
		printf("error: failed to getting info from bank system \n");
		return NULL;
	}

	AccountInfo* binInfo = (AccountInfo*)(malloc(sizeof(AccountInfo)));

	fread(binInfo, sizeof(AccountInfo), 1, fp);
	fclose(fp);

	while (input_pin != binInfo->pin)
	{
		printf("ENTER YOUR SECRET PIN NUMBER (4 DIGIT):");
		scanf("%d", &input_pin);
		if (input_pin != binInfo->pin)
			printf("PLEASE ENTER VALID PASSWORD\n");
	}

	return binInfo;
}

int select_acc()
{
	int indexAcc = -1;
		
	while (indexAcc == -1) 
	{
		printf("Type of account\nEnter '0' for Checking\nEnter '1' for Savings\n");
		scanf("%d", &indexAcc);

		switch (indexAcc)
		{
		case 0:
			indexAcc = 0;
			break;
		case 1:
			indexAcc = 1;
			break;
		default:
			printf("Please select proper account type\n");
			break;
		}		
	};

	return indexAcc;
}

bool close_cashbin()
{
	bool ret = true;
	// routine for checking cash bin hardware
	return ret;
}

bool open_cashbin()
{
	bool ret = true;
	// routine for checking cash bin hardware
	ret = close_cashbin();
	return ret;
}

void repeat_menu(char* transaction)
{
	printf("\n\n\n DO U WISH TO HAVE ANOTHER TRANSCATION?(y/n): \n");
	scanf("%s", transaction);
}

void main()
{
	int account_index = 0;
	printf("********Welcome to ATM Service**************\n");
	//write_acc_debug();// check : 1300  saving : 5500
	check_cardreader();
	AccountInfo* accInfo = auth_pin();
	account_index = select_acc();

	do
	{
		printf("1. Check Balance\n");
		printf("2. Withdraw Cash\n");
		printf("3. Deposit Cash\n");
		printf("4. Quit\n");
		printf("*******************************************\n\n");
		printf("Enter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1://check
			printf("\n YOUR BALANCE IS : %lu ", accInfo->balance[account_index]);
			break;
		case 2://withdraw
			printf("\n ENTER THE AMOUNT TO WITHDRAW: ");
			scanf("%lu", &withdraw);
			if (withdraw > accInfo->balance[account_index])
			{
				printf("\n INSUFFICENT BALANCE");
			}
			else
			{
				accInfo->balance[account_index] = accInfo->balance[account_index] - withdraw;
				if (open_cashbin())
				{
					printf("\n YOUR CURRENT BALANCE IS: %lu", accInfo->balance[account_index]);
					printf("\n\n PLEASE COLLECT CASH");
					update_acc(accInfo);
				}
			}
			break;
		case 3://deposit
			printf("\n ENTER THE AMOUNT TO DEPOSIT");
			if (open_cashbin())
			{
				scanf("%lu", &deposit);
				accInfo->balance[account_index] = accInfo->balance[account_index] + deposit;
				printf("YOUR BALANCE IS: %lu", accInfo->balance[account_index]);
				update_acc(accInfo);
			}
			break;
		case 4:
			printf("\n THANK U USING ATM");
			goto EXIT;
		default:
			printf("\n INVALID CHOICE");
		}
		printf("\n");

		repeat_menu(&transaction);
	} while (transaction != 'N' && transaction != 'n');
EXIT:
	printf("\n\n THANKS FOR USING OUR ATM SERVICE");
	free(accInfo);
}