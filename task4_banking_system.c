#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define FILENAME "accounts.dat"

// Structure for bank account
struct Account {
    int accountNumber;
    char accountHolder[50];
    float balance;
    char accountType[20];  // Savings/Current
    int isActive;  // 1 for active, 0 for closed
};

// Structure for transaction history
struct Transaction {
    int accountNumber;
    char type[20];  // Deposit/Withdraw/Transfer
    float amount;
    char date[30];
    float balanceAfter;
};

// Function prototypes
void displayMainMenu();
void createAccount();
void deposit();
void withdraw();
void balanceEnquiry();
void displayAllAccounts();
void deleteAccount();
void transferFunds();
void accountStatement();
void loadAccounts();
void saveAccounts();
void recordTransaction(int accNum, char *type, float amount, float newBalance);
int findAccount(int accountNumber);
void getCurrentDateTime(char *buffer);

// Global variables
struct Account accounts[MAX_ACCOUNTS];
int accountCount = 0;
int nextAccountNumber = 1001;

int main() {
    int choice;
    
    // Load existing accounts
    loadAccounts();
    
    printf("========================================\n");
    printf("   WELCOME TO BANK MANAGEMENT SYSTEM   \n");
    printf("========================================\n\n");
    
    do {
        displayMainMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                createAccount();
                break;
            case 2:
                deposit();
                break;
            case 3:
                withdraw();
                break;
            case 4:
                balanceEnquiry();
                break;
            case 5:
                transferFunds();
                break;
            case 6:
                accountStatement();
                break;
            case 7:
                displayAllAccounts();
                break;
            case 8:
                deleteAccount();
                break;
            case 9:
                printf("\n========================================\n");
                printf("  Thank you for using our services!    \n");
                printf("  Saving data and exiting...           \n");
                printf("========================================\n");
                saveAccounts();
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    } while(choice != 9);
    
    return 0;
}

// Display main menu
void displayMainMenu() {
    printf("\n========================================\n");
    printf("          MAIN MENU                     \n");
    printf("========================================\n");
    printf("1. Create New Account\n");
    printf("2. Deposit Money\n");
    printf("3. Withdraw Money\n");
    printf("4. Balance Enquiry\n");
    printf("5. Transfer Funds\n");
    printf("6. Account Statement\n");
    printf("7. Display All Accounts\n");
    printf("8. Close Account\n");
    printf("9. Exit\n");
    printf("========================================\n");
}

// Create new account
void createAccount() {
    if(accountCount >= MAX_ACCOUNTS) {
        printf("\nError: Maximum account limit reached!\n");
        return;
    }
    
    struct Account newAccount;
    
    printf("\n========================================\n");
    printf("        CREATE NEW ACCOUNT              \n");
    printf("========================================\n");
    
    newAccount.accountNumber = nextAccountNumber++;
    
    printf("Account Number: %d (Auto-generated)\n", newAccount.accountNumber);
    
    printf("Enter Account Holder Name: ");
    scanf(" %[^\n]", newAccount.accountHolder);
    
    printf("Enter Account Type (Savings/Current): ");
    scanf(" %[^\n]", newAccount.accountType);
    
    printf("Enter Initial Deposit Amount: Rs. ");
    scanf("%f", &newAccount.balance);
    
    if(newAccount.balance < 500) {
        printf("\nError: Minimum initial deposit is Rs. 500!\n");
        nextAccountNumber--;
        return;
    }
    
    newAccount.isActive = 1;
    
    accounts[accountCount] = newAccount;
    accountCount++;
    
    printf("\n========================================\n");
    printf("  Account Created Successfully!         \n");
    printf("  Account Number: %d                    \n", newAccount.accountNumber);
    printf("  Account Holder: %s                    \n", newAccount.accountHolder);
    printf("  Initial Balance: Rs. %.2f             \n", newAccount.balance);
    printf("========================================\n");
    
    // Record transaction
    recordTransaction(newAccount.accountNumber, "Account Opening", newAccount.balance, newAccount.balance);
    saveAccounts();
}

// Deposit money
void deposit() {
    int accNum;
    float amount;
    
    printf("\n========================================\n");
    printf("           DEPOSIT MONEY                \n");
    printf("========================================\n");
    
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    if(accounts[index].isActive == 0) {
        printf("\nError: Account is closed!\n");
        return;
    }
    
    printf("Current Balance: Rs. %.2f\n", accounts[index].balance);
    printf("Enter amount to deposit: Rs. ");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\nError: Invalid amount!\n");
        return;
    }
    
    accounts[index].balance += amount;
    
    printf("\n========================================\n");
    printf("  Deposit Successful!                   \n");
    printf("  Amount Deposited: Rs. %.2f            \n", amount);
    printf("  New Balance: Rs. %.2f                 \n", accounts[index].balance);
    printf("========================================\n");
    
    recordTransaction(accNum, "Deposit", amount, accounts[index].balance);
    saveAccounts();
}

// Withdraw money
void withdraw() {
    int accNum;
    float amount;
    
    printf("\n========================================\n");
    printf("          WITHDRAW MONEY                \n");
    printf("========================================\n");
    
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    if(accounts[index].isActive == 0) {
        printf("\nError: Account is closed!\n");
        return;
    }
    
    printf("Current Balance: Rs. %.2f\n", accounts[index].balance);
    printf("Enter amount to withdraw: Rs. ");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\nError: Invalid amount!\n");
        return;
    }
    
    if(amount > accounts[index].balance) {
        printf("\nError: Insufficient balance!\n");
        return;
    }
    
    if(accounts[index].balance - amount < 500) {
        printf("\nError: Minimum balance of Rs. 500 must be maintained!\n");
        return;
    }
    
    accounts[index].balance -= amount;
    
    printf("\n========================================\n");
    printf("  Withdrawal Successful!                \n");
    printf("  Amount Withdrawn: Rs. %.2f            \n", amount);
    printf("  New Balance: Rs. %.2f                 \n", accounts[index].balance);
    printf("========================================\n");
    
    recordTransaction(accNum, "Withdrawal", amount, accounts[index].balance);
    saveAccounts();
}

// Balance enquiry
void balanceEnquiry() {
    int accNum;
    
    printf("\n========================================\n");
    printf("         BALANCE ENQUIRY                \n");
    printf("========================================\n");
    
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("  Account Number: %d                    \n", accounts[index].accountNumber);
    printf("  Account Holder: %s                    \n", accounts[index].accountHolder);
    printf("  Account Type: %s                      \n", accounts[index].accountType);
    printf("  Current Balance: Rs. %.2f             \n", accounts[index].balance);
    printf("  Status: %s                            \n", accounts[index].isActive ? "Active" : "Closed");
    printf("========================================\n");
}

// Transfer funds
void transferFunds() {
    int fromAcc, toAcc;
    float amount;
    
    printf("\n========================================\n");
    printf("          TRANSFER FUNDS                \n");
    printf("========================================\n");
    
    printf("Enter Source Account Number: ");
    scanf("%d", &fromAcc);
    
    int fromIndex = findAccount(fromAcc);
    
    if(fromIndex == -1) {
        printf("\nError: Source account not found!\n");
        return;
    }
    
    if(accounts[fromIndex].isActive == 0) {
        printf("\nError: Source account is closed!\n");
        return;
    }
    
    printf("Enter Destination Account Number: ");
    scanf("%d", &toAcc);
    
    int toIndex = findAccount(toAcc);
    
    if(toIndex == -1) {
        printf("\nError: Destination account not found!\n");
        return;
    }
    
    if(accounts[toIndex].isActive == 0) {
        printf("\nError: Destination account is closed!\n");
        return;
    }
    
    printf("Current Balance (Source): Rs. %.2f\n", accounts[fromIndex].balance);
    printf("Enter amount to transfer: Rs. ");
    scanf("%f", &amount);
    
    if(amount <= 0) {
        printf("\nError: Invalid amount!\n");
        return;
    }
    
    if(amount > accounts[fromIndex].balance) {
        printf("\nError: Insufficient balance!\n");
        return;
    }
    
    if(accounts[fromIndex].balance - amount < 500) {
        printf("\nError: Minimum balance of Rs. 500 must be maintained!\n");
        return;
    }
    
    // Perform transfer
    accounts[fromIndex].balance -= amount;
    accounts[toIndex].balance += amount;
    
    printf("\n========================================\n");
    printf("  Transfer Successful!                  \n");
    printf("  Amount Transferred: Rs. %.2f          \n", amount);
    printf("  From Account: %d                      \n", fromAcc);
    printf("  To Account: %d                        \n", toAcc);
    printf("  New Balance (Source): Rs. %.2f        \n", accounts[fromIndex].balance);
    printf("========================================\n");
    
    // Record transactions
    recordTransaction(fromAcc, "Transfer Out", amount, accounts[fromIndex].balance);
    recordTransaction(toAcc, "Transfer In", amount, accounts[toIndex].balance);
    saveAccounts();
}

// Account statement
void accountStatement() {
    int accNum;
    
    printf("\n========================================\n");
    printf("        ACCOUNT STATEMENT               \n");
    printf("========================================\n");
    
    printf("Enter Account Number: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("  Account Number: %d                    \n", accounts[index].accountNumber);
    printf("  Account Holder: %s                    \n", accounts[index].accountHolder);
    printf("  Account Type: %s                      \n", accounts[index].accountType);
    printf("  Current Balance: Rs. %.2f             \n", accounts[index].balance);
    printf("========================================\n");
    
    // Read and display transaction history
    FILE *file = fopen("transactions.dat", "rb");
    if(file == NULL) {
        printf("\nNo transaction history available.\n");
        return;
    }
    
    struct Transaction trans;
    int found = 0;
    
    printf("\nRecent Transactions:\n");
    printf("%-15s %-15s %-12s %-20s %-12s\n", "Date", "Type", "Amount", "", "Balance");
    printf("--------------------------------------------------------------------------------\n");
    
    while(fread(&trans, sizeof(struct Transaction), 1, file)) {
        if(trans.accountNumber == accNum) {
            printf("%-15s %-15s Rs. %-8.2f %20s Rs. %-8.2f\n", 
                   trans.date, trans.type, trans.amount, "", trans.balanceAfter);
            found = 1;
        }
    }
    
    if(!found) {
        printf("No transactions found.\n");
    }
    
    fclose(file);
}

// Display all accounts
void displayAllAccounts() {
    if(accountCount == 0) {
        printf("\nNo accounts found!\n");
        return;
    }
    
    printf("\n========================================\n");
    printf("          ALL ACCOUNTS                  \n");
    printf("========================================\n");
    printf("%-10s %-25s %-15s %-12s %-10s\n", "Acc No", "Holder Name", "Type", "Balance", "Status");
    printf("--------------------------------------------------------------------------------\n");
    
    for(int i = 0; i < accountCount; i++) {
        printf("%-10d %-25s %-15s Rs. %-8.2f %-10s\n",
               accounts[i].accountNumber,
               accounts[i].accountHolder,
               accounts[i].accountType,
               accounts[i].balance,
               accounts[i].isActive ? "Active" : "Closed");
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("Total Accounts: %d\n", accountCount);
}

// Delete/Close account
void deleteAccount() {
    int accNum;
    
    printf("\n========================================\n");
    printf("           CLOSE ACCOUNT                \n");
    printf("========================================\n");
    
    printf("Enter Account Number to close: ");
    scanf("%d", &accNum);
    
    int index = findAccount(accNum);
    
    if(index == -1) {
        printf("\nError: Account not found!\n");
        return;
    }
    
    if(accounts[index].isActive == 0) {
        printf("\nError: Account is already closed!\n");
        return;
    }
    
    printf("\nAccount Details:\n");
    printf("Account Holder: %s\n", accounts[index].accountHolder);
    printf("Current Balance: Rs. %.2f\n", accounts[index].balance);
    
    char confirm;
    printf("\nAre you sure you want to close this account? (y/n): ");
    scanf(" %c", &confirm);
    
    if(confirm == 'y' || confirm == 'Y') {
        accounts[index].isActive = 0;
        printf("\n========================================\n");
        printf("  Account Closed Successfully!          \n");
        printf("  Closing Balance: Rs. %.2f             \n", accounts[index].balance);
        printf("========================================\n");
        
        recordTransaction(accNum, "Account Closed", 0, accounts[index].balance);
        saveAccounts();
    } else {
        printf("\nAccount closure cancelled.\n");
    }
}

// Find account by account number
int findAccount(int accountNumber) {
    for(int i = 0; i < accountCount; i++) {
        if(accounts[i].accountNumber == accountNumber) {
            return i;
        }
    }
    return -1;
}

// Load accounts from file
void loadAccounts() {
    FILE *file = fopen(FILENAME, "rb");
    
    if(file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    
    fread(&accountCount, sizeof(int), 1, file);
    fread(&nextAccountNumber, sizeof(int), 1, file);
    fread(accounts, sizeof(struct Account), accountCount, file);
    
    fclose(file);
    printf("Data loaded successfully. Total accounts: %d\n\n", accountCount);
}

// Save accounts to file
void saveAccounts() {
    FILE *file = fopen(FILENAME, "wb");
    
    if(file == NULL) {
        printf("Error: Unable to save data!\n");
        return;
    }
    
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(&nextAccountNumber, sizeof(int), 1, file);
    fwrite(accounts, sizeof(struct Account), accountCount, file);
    
    fclose(file);
}

// Record transaction
void recordTransaction(int accNum, char *type, float amount, float newBalance) {
    FILE *file = fopen("transactions.dat", "ab");
    
    if(file == NULL) {
        return;
    }
    
    struct Transaction trans;
    trans.accountNumber = accNum;
    strcpy(trans.type, type);
    trans.amount = amount;
    trans.balanceAfter = newBalance;
    getCurrentDateTime(trans.date);
    
    fwrite(&trans, sizeof(struct Transaction), 1, file);
    fclose(file);
}

// Get current date and time
void getCurrentDateTime(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 30, "%Y-%m-%d %H:%M", t);
}
