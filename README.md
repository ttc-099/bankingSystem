# COMP1312 Coursework Sem 1 - Simple Banking System

This is a **text-based banking system** implemented in **C** that manages bank accounts through **file I/O operations**.  
The application supports:

- Account creation  
- Account deletion  
- Deposits  
- Withdrawals  
- Remittances  
- Proper authentication and transaction logging  

---

## Contents

1. [File Structure](#file-structure)  
2. [Setup Guide](#setup-guide)  
3. [Features Implemented](#features-implemented)  
4. [Expected Output per Function](#expected-output-per-function)  

---

## File Structure

```
SEM 1 PROJECT/
│
├── vscode/
│   ├── settings.json
│   └── tasks.json
│
├── Banking System/
│   │
│   ├── database/
│   │   ├── 100009774.txt
│   │   ├── 100010841.txt
│   │   ├── index.txt
│   │   └── transaction.log
│   │
│   ├── createAccount.h
│   ├── deleteAccount.h
│   ├── deposit.h
│   ├── helpers_CreateDelete.h
│   ├── helpers_DepositWithdraw.h
│   ├── helpers_Remittance.h
│   ├── helpers2.h
│   ├── helpers3.h
│   ├── main.c
│   ├── main.exe
│   ├── remittance.h
│   ├── withdraw.h
│   └── README.md
```

---

## Setup Guide

1. **Navigate to the project folder containing `main.c`.**

```bash
cd Banking System
```

2. **Compile the program.**

```bash
gcc main.c -o main
```

3. **Run the program.**

```bash
./main.exe
```

---

## Features Implemented

### Core Operations
- **Account Creation:** Create new bank accounts (Savings or Current)  
- **Account Deletion:** Securely remove accounts with multi-factor authentication  
- **Deposits:** Add funds to existing accounts (RM0 - RM50,000 per transaction)  
- **Withdrawals:** Withdraw funds with balance validation  
- **Remittances:** Transfer funds between accounts with transaction fees:
  - Savings → Current: 2% fee  
  - Current → Savings: 3% fee  

### Security Features
- 4-digit PIN authentication for all sensitive operations  
- Unique 7-9 digit account number generation  
- Multi-factor confirmation for account deletion  
- Input validation and sanitization  

### Data Management
- Persistent storage in text files  
- Transaction logging with timestamps  
- Automatic directory and file creation  
- Unique account number enforcement  

---

## Expected Output per Function

Upon starting the program, the user is greeted and prompted to choose an option.  
Users can enter either the **numeric input** or a **keyword** corresponding to the function:

| Function         | Numeric Input | Keyword  |
|-----------------|---------------|----------|
| Create Account  | 1             | create   |
| Delete Account  | 2             | delete   |
| Deposit         | 3             | deposit  |
| Withdrawal      | 4             | withdraw |
| Remittance      | 5             | remit    |
| Quit            | 6             | quit     |

---

## Usage Note 

Then, follow what the prompts request for.