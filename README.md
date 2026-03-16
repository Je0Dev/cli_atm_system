# C - A Command-Line Atm System

## ✨ Key Features
---
- **🔐 Secure Customer Authentication** : Robust login system requiring a unique account number and a 4-digit PIN.
- **💰 Check Balance**: View your current account balance.
- **💸 Withdraw Funds**: Withdraw your cash with built-in checks for sufficient funds.
- **📥 Deposit Funds**: Deposit your money into your account(or in *one* of them).
- **➕ Multiple Accounts Creation**: You can open new bank accounts on the fly.
- **➖ Multiple Accounts Deletion**: Remove old accounts (as long as it's *not* the only one).
- **↔️ Switch Accounts**: Seamlessly switch between your own multiple accounts.
- **🛡️ PIN Management Capabilities**: Change your PIN securely after verifying your current one.
- **💾 Data Persistence**: All customer and account data can be saved to `customers.dat` and `accounts.dat` when you exit the app and automatically reloaded on the next launch.
- **🕵️ Administrator Mode**: A special hardcoded account for a potential administrator to view the complete list of all customers and their associated accounts.
  - *Account Number: 1*
  - *Password: admin!!!$##0?17*
  - *PIN: 1111*

### 🚀 Getting Started
```bash
gcc main.c system.c ui.c customer.c account.c utils.c -o atm.exe #or make
./atm #Linux,MacOS
./atm.exe #Windows
make clean #remove .o files 
```
