// BankingSysytem.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;


class Account
{

	static int id;
	string first_name;
	string last_name;
	int accno, balance;
	bool id_flag = false;

public:
	void insert_record(string, string, int);
	void print_values(Account);
	void write_acc_info(Account);
	int set_acc_no_start();
	void print_all_acounts(int);
	void change_balance(int, int, bool);
	void delete_record(int);
};

int Account::id = 0;

void Account::print_all_acounts(int accno=0) {

	ifstream file("banking_info.txt");
	if (file.is_open()) {
		string line;
		int temp_accno;
		while (std::getline(file, line)) {
			
			istringstream iss(line.c_str());
			string s;
			bool flag = false;
			int c = 0;
			while (getline(iss, s, ' ')) {
				if (accno == 0) {
					printf("%s\n", s.c_str());
				}
				else {
					if (!flag and c == 0) {
						c += 1;
						temp_accno = stoi(s.c_str());
						if (temp_accno == accno) {
							printf("%s\n", s.c_str());
							flag = true;
							continue;
						}
					}
					if (flag) {
						printf("%s\n", s.c_str());
					}
					
				}
				
			}
			if (flag) break;
			cout << endl;
		}
		file.close();
	}
}

void Account::insert_record(string f, string l, int b)
{
	 
	 if (!id_flag) {
		 accno = set_acc_no_start();
		 id_flag = true;
	 }
	 else {
		 accno = ++id;
	 }
	
	 first_name = f;
	 last_name = l;
	 balance = b;
}

int Account::set_acc_no_start() {
	ifstream fin("banking_info.txt", ios::in);

	if (fin.peek() == ifstream::traits_type::eof())
	{
		id = 0;
	}
	else {
		if (fin.is_open()) {   //checking whether the file is open
			fin.seekg(-1, std::ios_base::end);
			if (fin.peek() == '\n')
			{
				fin.seekg(-1, std::ios_base::cur);
				int i = fin.tellg();
				for (i; i > 0; i--)
				{
					if (fin.peek() == '\n')
					{
						//Found
						fin.get();
						break;
					}
					fin.seekg(i, std::ios_base::beg);
				}
			}
			string lastline;
			getline(fin, lastline);

			istringstream iss(lastline);
			string s;
			while (getline(iss, s, ' ')) {
				id = stoi(s.c_str());
				break;
			}
		}
		
	}
	fin.close();

	return ++id;
}

void Account::change_balance(int accno, int money, bool add) {
	std::fstream file("banking_info.txt", std::ios::in | std::ios::out);
	std::string user, first_name, last_name, balance;
	int new_bal;
	while (file >> user >> first_name >> last_name) {
		if (stoi(user) == accno) {
			auto pos = file.tellg();
			file >> balance;
			file.seekp(pos);
			if (add) {
				new_bal = stoi(balance) + money;
			}
			else {
				if (stoi(balance) - money < 500) {
					cout << "Cannot withdraw less than the minimum balance of 500" << endl;
					new_bal = stoi(balance);
				}
				else {
					new_bal = stoi(balance) - money;
				}
			}
			file << " " << new_bal;
			break;
		}
		else {
			file >> balance;
		}
	}
	file.close();
}

void Account::delete_record(int accno) {
	string line;

	ifstream fin;
	fin.open("banking_info.txt", std::ios::in);
	ofstream temp;
	temp.open("temp.txt", std::ios::out);
	string deleteline = to_string(accno);

	while (getline(fin, line))
	{
		//line.replace(line.find(deleteline), deleteline.length(), "");
		if (line.rfind(deleteline, 0) == 0) {
			continue;
		}
		temp << line << endl;
	}

	temp.close();
	fin.close();
	remove("banking_info.txt");
	rename("temp.txt", "banking_info.txt");
}

void Account::print_values(Account acc) {
	cout << acc.accno << " " << acc.first_name << " " << acc.last_name << " " << acc.balance << "\n";
}

void Account::write_acc_info(Account acc) {
	ofstream fout("banking_info.txt", ios::app);

	fout << acc.accno << " " << acc.first_name << " " << acc.last_name << " " << acc.balance << "\n";
	fout.close();
}

int main()
{
	Account acc;
	int option, balance, accno, money;
	string first_name, last_name;

    
	do {
		std::cout << "***Banking System***\n";
		std::cout << "\n\tSelect an option below:\n";
		std::cout << "\n\t1 Open an Account";
		std::cout << "\n\t2 Balance Enquiry";
		std::cout << "\n\t3 Deposit";
		std::cout << "\n\t4 Withdrawal";
		std::cout << "\n\t5 Close an Account";
		std::cout << "\n\t6 Show All Accounts";
		std::cout << "\n\t7 Quit";
		std::cout << "\nEnter your option: ";
		std::cin >> option;

		switch (option)
		{
			case 1: {
				std::cout << "Enter First Name: ";
				cin >> first_name;
				cout << "Enter Last Name: ";
				cin >> last_name;
				cout << "Enter Balance: ";
				cin >> balance;
				acc.insert_record(first_name, last_name, balance);
				cout << "Account Created!"<<endl;
				acc.write_acc_info(acc);
				break;
			}
			case 2: {
				cout << "Enter Account No: ";
				cin >> accno;
				acc.print_all_acounts(accno);
				break;
			}
			case 3: {
				cout << "Enter Account No: ";
				cin >> accno;
				cout << "Enter Deposit: ";
				cin >> money;
				acc.change_balance(accno, money, true);
				break;
			}
			case 4: {
				cout << "Enter Account No: ";
				cin >> accno;
				cout << "Enter Withdrawal: ";
				cin >> money;
				acc.change_balance(accno, money, false);
				break;
			}
			case 5: {
				cout << "Enter Account No: ";
				cin >> accno;
				acc.delete_record(accno);
				break;
			}
			case 6: {
				acc.print_all_acounts();
				break;
			}

		default:
			break;
		}
	} while (option != 7);
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
