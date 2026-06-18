# A Command-Line Atm System
---
 > Made for a University Lesson. I made one or two additions but nothing too much since then.

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
- **🕵️ Administrator Mode(Here just to check if the project works correctly)**: A special hardcoded account for a potential administrator to view the complete list of all customers and their associated accounts.
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
### Licence
- *MIT* Licence

Copyright © 2026 Je0Dev

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### Contributions
- If someone wants to suggest something, I am open to here!
