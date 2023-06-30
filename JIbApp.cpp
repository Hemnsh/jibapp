#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstring>
#include <cmath>
#include <iomanip>

using namespace std;

// Class representing a Currency
class Currency {
private:
    string name;
    int value;

public:
    Currency(const string& currencyName, int currencyValue) : name(currencyName), value(currencyValue) {}

    string getName() const {
        return name;
    }

    int getValue() const {
        return value;
    }
};

// Class representing a Balance
class Balance {
private:
    shared_ptr<Currency> currency;
    double amount;

public:
    Balance(shared_ptr<Currency> currencyPtr, double balanceAmount) : currency(currencyPtr), amount(balanceAmount) {}

    shared_ptr<Currency> getCurrency() const {
        return currency;
    }

    double getAmount() const {
        return amount;
    }

    bool deduct(double Amount) {
        // cout << "amount get: " << Amount << " amount had: " << amount << endl;
        if (Amount > amount) {
            return false;
        }
        else {
            amount -= Amount;
            // cout << "change deduct : " << amount << endl;
            return true;
        }
    }

    void add(double Amount) {
        amount += Amount;
        // cout << "change add : " << amount << endl;   
    }
};

class Account {
private:
    vector<Balance*> balances;
    string ID;

public:
    Account(const vector<double>& accountBalances, const string& accountID, const vector<shared_ptr<Currency>>& currencies)
        : ID(accountID) {
        // Create Balance objects for each currency in the account
        for (size_t i = 0; i < accountBalances.size(); i++) {
            balances.push_back(new Balance(currencies[i], accountBalances[i]));
        }
    }

    const string& getID() const {
        return ID;
    }

    const vector<Balance*>& getBalances() const {
        return balances;
    }

    Balance* findBalance(Currency* cur) {
        for (auto balance : balances) {
            if (balance->getCurrency().get() == cur) {
                return balance;
            }
        }
        return nullptr;
    }

    void withdraw(Currency* coin, double amount, string ID, Account* sarafi) {
        auto balance = findBalance(coin);
        auto sarafiBalance = sarafi->findBalance(coin);
        if (balance && sarafiBalance) {
            if (balance->deduct(amount)) {
                balance->deduct(amount);
                sarafiBalance->add(amount);
            }
            else {
                cout << "Insufficient Funds: " << ID << endl;
            }
        }
        else {
            cout << "Invalid currency or balance not found." << endl;
        }
    }

void deposit(Currency* coin, double amount, string ID, Account* sarafi) {
    auto balance = findBalance(coin);
    auto sarafiBalance = sarafi->findBalance(coin);
    if (balance && sarafiBalance) {
        if (sarafiBalance->deduct(amount) == true) {
            balance->add(amount);
        }
        else {
            cout << "Insufficient Funds: " << "SARAFI" << endl;
        }
    }
    else {
        cout << "Invalid currency or balance not found." << endl;
    }
}


    void transfer(Currency* coin, double amount, string IDfrom, Account* sarafi, double fee, Account* to) {
        auto balance = findBalance(coin);
        auto sarafiBalance = sarafi->findBalance(coin);
        auto toBalance = to->findBalance(coin);
        if (balance && sarafiBalance && toBalance) {
            if (balance->deduct(amount + fee)) {
                balance->deduct(amount);
                sarafiBalance->add(fee);
                toBalance->add(amount);
            }
            else {
                cout << "Insufficient Funds: " << IDfrom << endl;
            }
        }
        else {
            cout << "Invalid currency or balance not found." << endl;
        }
    }

    void exchange(Currency* coinFrom, Currency* coinTo, Account* sarafi, double amount, double fee, double cnvAmount, string IDex) {
        auto balanceFromUser = findBalance(coinFrom);
        auto balanceToUser = findBalance(coinTo);
        auto balanceFromSarafi = sarafi->findBalance(coinTo);
        auto balanceToSarafi = sarafi->findBalance(coinFrom);
        if (balanceFromUser && balanceToUser && balanceFromSarafi && balanceToSarafi) {
            if (balanceFromUser->deduct(amount + fee)) {
                if (balanceFromSarafi->deduct(cnvAmount)) {
                    // balanceFromUser->deduct(amount + fee);
                    balanceToSarafi->add(amount + fee);
                    balanceToUser->add(cnvAmount);
                    // balanceFromSarafi->deduct(cnvAmount);
                }
                else {
                    cout << "Insufficient Funds: " << "SARAFI" << endl;
                    balanceFromUser->add(amount + fee);
                }
            }
            else {
                cout << "Insufficient Funds: " << IDex << endl;
            }
        }
        else {
            cout << "Invalid currency or balance not found." << endl;
        }
    }
};

// Class representing a SARAFI account, inheriting from Account
class SARAFI : public Account {
public:
    SARAFI(const vector<double>& accountBalances, const vector<shared_ptr<Currency>>& currencies)
        : Account(accountBalances, "SARAFI", currencies) {}
};

// Class representing a Wallet
class Wallet {
private:
    vector<shared_ptr<Currency>> currencies;
    vector<Account*> accounts;

    vector<vector<int>> Codes;
    vector<vector<int>> calculatedCodes;
    vector<vector<char>> Commands;
    vector<vector<string>> Tokens;
    int phi, primeP, primeQ, modulus, publicKey, privateKey;

public:
    Wallet() {
        // Initialize values for encryption/decryption
        primeP = 19;
        primeQ = 101;
        modulus = primeP * primeQ;
        phi = (primeQ - 1) * (primeP - 1);
        publicKey = calculatePublicKey(phi);
        privateKey = calculatePrivateKey(publicKey, phi);
        
    }

void printBalances() {
    for (Account* account : accounts) {
        const vector<Balance*>& userBalances = account->getBalances();
        for (Balance* balance : userBalances) {
            cout << fixed << setprecision(2) << balance->getAmount() << " ";
        }
    cout << endl;
    }
}

    // Add a new currency to the wallet
    void addCurrency(const string& currencyName, int value) {
        currencies.push_back(make_shared<Currency>(currencyName, value));
    }

    // Add a new account to the wallet
    void addAccount(const vector<double>& accountBalances, const string& accountID) {
        if (accountID == "SARAFI") {
            accounts.push_back(new SARAFI(accountBalances, currencies));
        }
        else {
            accounts.push_back(new Account(accountBalances, accountID, currencies));
        }
    }

    const vector<Account*>& getAccounts() const {
        return accounts;
    }

    // Decrypt the commands
    void decrypting(int nCommands) {
        // Read and store the encrypted codes
        for (int i = 0; i < nCommands; i++) {
            int arraySize;
            cin >> arraySize;
            vector<int> v1;

            for (int j = 0; j < arraySize; j++) {
                int code;
                cin >> code;
                v1.push_back(code);
            }
            Codes.push_back(v1);
        }

        // Calculate the decrypted codes
        calculatingCodes(nCommands, modulus, privateKey, Codes);

        // Convert decrypted codes to ASCII characters
        getAsciiChar(nCommands, calculatedCodes);

        // Tokenize the decrypted commands
        Tokenizing(nCommands, Commands);

        executeCommands(nCommands, Tokens);
    }




private:


    // Calculate the public key based on Euler's totient function
    int calculatePublicKey(int phi) {
        for (int i = 2; i < phi; i++) {
            if (gcd(i, phi) == 1) {
                return i;
            }
        }
        return -1;
    }

    // Calculate the private key based on the extended Euclidean algorithm
    int calculatePrivateKey(int publicKey, int phi) {
        int t1 = 0, t2 = 1;
        int r1 = phi, r2 = publicKey;

        while (r2 > 0) {
            int q = r1 / r2;
            int temp = t2;
            t2 = t1 - q * t2;
            t1 = temp;

            temp = r2;
            r2 = r1 - q * r2;
            r1 = temp;
        }

        if (r1 == 1) {
            if (t1 < 0) {
                t1 += phi;
            }
            return t1;
        }

        return -1;
    }

    // Calculate the greatest common divisor of two numbers
    int gcd(int a, int b) {
        if (b == 0) {
            return a;
        }
        return gcd(b, a % b);
    }

    // Calculate the decrypted codes
    void calculatingCodes(int nCommands, int modulus, int privateKey, vector<vector<int>> codes) {
        for (int i = 0; i < nCommands; i++) {
            int arrayLength = codes[i].size();
            vector<int> v1;
            for (int j = 0; j < arrayLength; j++) {
                int res = 1;
                for (int k = 0; k < privateKey; k++) {
                    res = (res * codes[i][j]) % modulus;
                }
                v1.push_back(res);
            }
            calculatedCodes.push_back(v1);
        }

        for (int i = 0; i < nCommands; i++) {
            int arrayLength = codes[i].size();
            for (int j = 0; j < arrayLength; j++) {
                calculatedCodes[i][j] %= modulus;
            }
        }
    }

    // Convert decrypted codes to ASCII characters
    void getAsciiChar(int nCommands, vector<vector<int>> calculatedCodes) {
        for (int i = 0; i < nCommands; i++) {
            int arrayLength = calculatedCodes[i].size();
            vector<char> v1;
            for (int j = 0; j < arrayLength; j++) {
                char asciiChar = static_cast<char>(calculatedCodes[i][j]);
                v1.push_back(asciiChar);
            }
            Commands.push_back(v1);
        }
    }

    // Tokenize the decrypted commands
    void Tokenizing(int nCommands, vector<vector<char>> commands) {
        for (int i = 0; i < nCommands; i++) {
            string commandStr(commands[i].begin(), commands[i].end());
            char* commandCStr = strdup(commandStr.c_str());
            char* token = strtok(commandCStr, " ");
            vector<string> v1;

            while (token != NULL) {
                v1.push_back(string(token));
                token = strtok(NULL, " ");
            }
            Tokens.push_back(v1);
            free(commandCStr);
        }
    }


    Account* findID(string ID) {
        for (Account* account: accounts) {
            if (account->getID() == ID) {
                return account;
            }
        }
        return nullptr;
    }

    Currency* findCurrency(const string& currencyName) {
        for (const shared_ptr<Currency>& currency : currencies) {
            if (currency->getName() == currencyName) {
                return currency.get();  // Currency pointer found
            }
        }
        return nullptr;  // Currency not found
    }


    void WITHDRAW(string ID, string coinFrom, double amount) {
        if (findID(ID) == nullptr) {
            cout << "Invalid User: " << ID << endl;
        }
        else {
        Account* IdPtr = findID(ID);
        Currency* coinPtr = findCurrency(coinFrom);
        Account* sarafiPtr = findID("SARAFI");
        IdPtr->withdraw(coinPtr,amount,ID,sarafiPtr);
        // sarafiPtr->withdraw();
        }
    }

    void DEPOSIT(string ID, string coinFrom, double amount) {
        if (findID(ID) == nullptr) {
            cout << "Invalid User: " << ID << endl;
        }
        else {
        Account* IdPtr = findID(ID);
        Account* sarafiPtr = findID("SARAFI");
        Currency* coinPtr = findCurrency(coinFrom);
        IdPtr->deposit(coinPtr,amount,ID,sarafiPtr);
        }
    }

    void TRANSFER(string IDfrom, string IDto, string coin, double amount) {
        if (findID(IDfrom) == nullptr) {
            cout << "Invalid User: " << IDfrom << endl;
        }
        else if (findID(IDto) == nullptr) {
            cout << "Invalid User: " << IDto << endl;
        }
        else {
        Account* IdPtrFrom = findID(IDfrom);
        Account* IdPtrTo = findID(IDto);
        Currency* coinPtr = findCurrency(coin);
        Account* sarafiPtr = findID("SARAFI");
        Currency* jibPtr = findCurrency("jibCoin");
        double transactionFee = (amount > 100 ) ? (10.0 / coinPtr->getValue()) : (amount * 0.10);

        IdPtrFrom->transfer(coinPtr, amount, IDfrom, sarafiPtr, transactionFee, IdPtrTo);
        }
    }

    void EXCHANGE(string ID, string coinFrom, string coinTo, double amount) {
        if (findID(ID) == nullptr) {
            cout << "Invalid User: " << ID << endl;
        }
        else {
        Account* IdPtr = findID(ID);
        Account* sarafPtr = findID("SARAFI");
        Currency* coinFromPtr = findCurrency(coinFrom);
        Currency* coinToPtr = findCurrency(coinTo);
        Currency* jibPtr = findCurrency("jibCoin");
        double transactionFee = (amount > 1000) ? (10.0 / coinFromPtr->getValue()) : (amount * 0.01);
        double convertedAmount = (amount * coinFromPtr->getValue()) / coinToPtr->getValue();
        IdPtr->exchange(coinFromPtr, coinToPtr, sarafPtr, amount, transactionFee, convertedAmount,ID);
        }

    }

void executeCommands(int nCommands, vector<vector<string>> tokens) {
    for (int i = 0; i < nCommands; i++) {
        int arrayLength = tokens[i].size();
        string command = tokens[i][0];

        if (command == "WITHDRAW") {
            string ID = tokens[i][1];
            string coinFrom = tokens[i][2];
            double amount = stod(tokens[i][3]);
            WITHDRAW(ID, coinFrom, amount);
        }
        else if (command == "DEPOSIT") {
            string ID = tokens[i][1];
            string coinFrom = tokens[i][2];
            double amount = stod(tokens[i][3]);
            DEPOSIT(ID, coinFrom, amount);
        }
        else if (command == "TRANSFER") {
            string IDfrom = tokens[i][1];
            string IDto = tokens[i][2];
            string coin = tokens[i][3];
            double amount = stod(tokens[i][4]);
            TRANSFER(IDfrom, IDto, coin, amount);
        }
        else if (command == "EXCHANGE") {
            string ID = tokens[i][1];
            string coinFrom = tokens[i][2];
            string coinTo = tokens[i][3];
            double amount = stod(tokens[i][4]);
            EXCHANGE(ID, coinFrom, coinTo, amount);
        }
        else {
            cout << "Invalid command." << endl;
        }
    }
        printBalances();
}


};


int main() {
    Wallet wallet;

    int nCurrencies;
    cin >> nCurrencies;

    for (int i = 0; i < nCurrencies; i++) {
        string currencyName;
        int value;
        cin >> currencyName >> value;
        wallet.addCurrency(currencyName, value);
    }

    string sarafiID;
    cin >> sarafiID;

    vector<double> sarafiBalances;
    for (int i = 0; i < nCurrencies; i++) {
        double amount;
        cin >> amount;
        sarafiBalances.push_back(amount);
    }
    wallet.addAccount(sarafiBalances, sarafiID);

    int nAccounts;
    cin >> nAccounts;

    for (int i = 0; i < nAccounts; i++) {
        string ID;
        cin >> ID;

        vector<double> accountBalances;
        for (int j = 0; j < nCurrencies; j++) {
            double amount;
            cin >> amount;
            accountBalances.push_back(amount);
        }
        wallet.addAccount(accountBalances, ID);
    }

    int nCommands;
    cin >> nCommands;

    wallet.decrypting(nCommands);

    return 0;
}