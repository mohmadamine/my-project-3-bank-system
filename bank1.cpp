#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>

using namespace std;

enum enmain_menue_options
{
	client_list = 1, add_clients = 2, delete_client = 3, update_client = 4, find_client = 5, transactions = 6,
	manage_users = 7, logout = 8
};
enum entransactions_menue_options
{
	deposit_amount = 1, withdraw_amount, total_balances, main_menue
};
enum enmanage_user_menue_options
{
	list_users = 1, add_new_user = 2, delete_user = 3, update_user = 4, find_user = 5, to_main_menue = 6
};

enum enmain_menue_permissions
{
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient
	= 4,
	pUpdateClients = 8, pFindClient = 16, pTranactions = 32,
	pManageUsers = 64
};

struct Stclients_data
{
	string account_number;
	string pin_code;
	string client_name;
	string client_phone;
	double account_balance;
	bool mark_client_for_delet = false;
};

struct stusers
{
	string username;
	string password;
	int permissions;
	bool mark_for_delete = false;
};

const string file_name = "clients.txt";
const string user_file_name = "users.txt";

stusers current_user;

void show_main_menue_screen();
void show_transactions_menue_screen();
void login();
//***************************** support & global function **********************************

Stclients_data change_client_info(string account_number)

{
	Stclients_data client_data;

	client_data.account_number = account_number;

	cout << "enter pin code : ";
	getline(cin >> ws, client_data.pin_code);

	cout << "enter name : ";
	getline(cin, client_data.client_name);

	cout << "enter phone : ";
	getline(cin, client_data.client_phone);

	cout << "enter account balance : ";
	cin >> client_data.account_balance;


	return client_data;
}

stusers change_user_info(string username)

{
	stusers user;

	user.username = username;

	cout << "enter password : ";
	getline(cin >> ws, user.password);


	user.permissions = read_permissions_to_set();

	return user;
}


vector <string> split_string(string S1, string delim)
{
	vector <string> Vworlds;

	string Sworld;
	short pos = 0;

	while ((pos = S1.find(delim)) != std::string::npos)
	{
		Sworld = S1.substr(0, pos);
		if (Sworld != "")
			Vworlds.push_back(Sworld);

		S1.erase(0, pos + delim.length());
	}
	if (S1 != "")
		Vworlds.push_back(S1);

	return Vworlds;
}

string convert_client_record_to_line(Stclients_data client, string delim = "#//#")
{
	string Sclient_record = "";
	Sclient_record += client.account_number + delim;
	Sclient_record += client.pin_code + delim;
	Sclient_record += client.client_name + delim;
	Sclient_record += client.client_phone + delim;
	Sclient_record += to_string(client.account_balance);
	return Sclient_record;

}

string convert_user_record_to_line(stusers user, string delim = "#//#")
{
	string user_line = "";
	user_line += user.username + delim;
	user_line += user.password + delim;
	user_line += to_string(user.permissions);
	return user_line;
}

Stclients_data convert_client_line_to_record(string line, string delim = "#//#")
{
	vector <string> Vdata = split_string(line, delim);
	Stclients_data client;

	client.account_number = Vdata[0];
	client.pin_code = Vdata[1];
	client.client_name = Vdata[2];
	client.client_phone = Vdata[3];
	client.account_balance = stod(Vdata[4]);

	return client;

}

stusers convert_user_line_to_record(string line, string delim = "#//#")
{
	vector <string> Vdata = split_string(line, delim);
	stusers users;

	users.username = Vdata[0];
	users.password = Vdata[1];
	users.permissions = stoi(Vdata[2]);

	return users;

}

void save_client_data_to_file(string file_name, vector <Stclients_data> Vclients_data)
{
	fstream my_file;
	string line;
	my_file.open(file_name, ios::out);
	if (my_file.is_open())
	{
		for (Stclients_data& C : Vclients_data)
		{
			if (C.mark_client_for_delet == false)
			{
				line = convert_client_record_to_line(C);
				my_file << line << endl;
			}

		}
		my_file.close();
	}
}

void save_users_to_file(string user_file_name, vector <stusers> Vusers)
{
	fstream my_file;
	string line;
	my_file.open(user_file_name, ios::out);
	if (my_file.is_open())
	{
		for (stusers &U : Vusers)
		{
			if (U.mark_for_delete == false)
			{
				line = convert_user_record_to_line(U);
				my_file << line << endl;
			}

		}
		my_file.close();
	}
}



vector <Stclients_data> load_clients_data_from_file(string file_name)
{
	vector <Stclients_data> Vclients_data;

	fstream my_file;
	my_file.open(file_name, ios::in);
	if (my_file.is_open())
	{
		string line;
		Stclients_data client;
		while (getline(my_file, line))
		{
			client = convert_client_line_to_record(line, "#//#");
			Vclients_data.push_back(client);
		}
		my_file.close();
	}

	return Vclients_data;

}

vector <stusers> load_users_from_file(string user_file_name)
{
	stusers user;
	vector <stusers> Vusers;
	fstream my_file;
	string line;
	my_file.open(user_file_name, ios::in);
	if (my_file.is_open())
	{
		while (getline(my_file,line))
		{
			user = convert_user_line_to_record(line);
			Vusers.push_back(user);
		}
		my_file.close();
	}
	
	return Vusers;
}

bool find_clients_by_account_number(vector <Stclients_data> Vclients_data, string account_number, Stclients_data& client_data)
{


	for (Stclients_data& c : Vclients_data)
	{
		if (c.account_number == account_number)
		{
			client_data = c;

			return true;
		}
	}
	return false;
}

bool find_user_by_username(string username,stusers &user)
{
	vector <stusers> Vusers = load_users_from_file(user_file_name);

	for (stusers& S : Vusers)
	{
		if (S.username == username)
		{
			user = S;
			return true;
		}
	}
	return false;

}




void print_client_information(Stclients_data client)
{
	cout << "\n\nthe following are the client details : \n\n";
	cout << "account number  : " << client.account_number << "\n";
	cout << "pin code        : " << client.pin_code << "\n";
	cout << "name            : " << client.client_name << "\n";
	cout << "phone           : " << client.client_phone << "\n";
	cout << "account balance : " << to_string(client.account_balance) << endl;
}

void print_user_short_information(stusers user)
{
	cout << "\n\nthe following are the user details : ";
	cout << "\n---------------------------------------------\n";
	cout << "account number  : " << user.username << "\n";
	cout << "pin code        : " << user.password << "\n";
	cout << "name            : " << user.permissions;
	cout << "\n---------------------------------------------\n";
}

void print_client_record_balance_line(Stclients_data client)
{
	cout << "| " << left << setw(16) << client.account_number
		<< "| " << left << setw(40) << client.client_name
		<< "| " << left << setw(40) << client.account_balance;
}

void print_user_record_line(stusers user)
{
	cout << "| " << left << setw(16) << user.username
		<< "| " << left << setw(40) << user.password
		<< "| " << left << setw(40) << user.permissions;
}

string read_account_number()
{
	string account_number;
	cout << "\nplease enter account number : ";
	getline(cin >> ws, account_number);

	return account_number;
}
string read_username()
{
	string username;
	cout << "\nplease enter username : ";
	getline(cin >> ws, username);

	return username;
}
bool client_exists_by_account_number(string account_number, string file_name)
{
	fstream my_file;
	my_file.open(file_name, ios::in);
	string line;
	if (my_file.is_open())
	{
		while (getline(my_file, line))
		{
			Stclients_data clients = convert_client_line_to_record(line, "#//#");
			if (clients.account_number == account_number)
			{
				my_file.close();
				return true;

			}
		}
		my_file.close();

	}
	return false;
}

bool user_exists_by_username(string username, string user_file_name)
{
	fstream my_file;
	my_file.open(user_file_name, ios::in);
	string line;
	if (my_file.is_open())
	{
		while (getline(my_file, line))
		{
			stusers user = convert_user_line_to_record(line, "#//#");
			if (user.username == username)
			{
				my_file.close();
				return true;

			}
		}
		my_file.close();

	}
	return false;
}

void add_data_line_to_file(string my_file_name, string line)
{
	fstream file;
	file.open(my_file_name, ios::out | ios::app);
	if (file.is_open())
	{
		file << line << endl;
		file.close();
	}

}

//*********************** program function ****************************************

bool update_user_information(vector <stusers> Vusers,string username)
{
	char option = 'Y';
	stusers user;
	if (find_user_by_username(username,user))
	{
		print_user_short_information(user);

		cout << "\n\nare you sure to want to update this user (y/n) ? \n";
		cin >> option;

		if (toupper(option) == 'Y')
		{
			for (stusers& U : Vusers)
			{
				if(U.username == username)
				{
					U = change_user_info(username);
					break;
				}
			}

			save_users_to_file(user_file_name, Vusers);
			cout << "\n\nthis user updated successfully ! " << endl;
			return true;
		}
	}

	cout << "user with username (" << username << ") is not found ! " << endl;
	return false;
}

void show_update_user_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tupdate user Screen";
	cout << "\n-----------------------------------\n";

	string username = read_username();
	vector <stusers> Vusers = load_users_from_file(user_file_name);
	update_user_information(Vusers, username);
	
}

bool mark_user_for_delete_by_username(vector <stusers> Vusers, string username)
{
	for (stusers& U : Vusers)
	{
		if(U.username == username)
		{
			U.mark_for_delete = true;
			return true;
		}
	}

	return false;
}

bool delete_user_by_username(vector <stusers> Vusers, string username)
{
	char option = 'Y';
	stusers user;
	if (find_user_by_username(username,user))
	{
		print_user_short_information(user);

		cout << "\n\ndo you want to delete this user (y/n) ? \n";
		cin >> option;

		if (toupper(option) == 'Y')
		{
			save_users_to_file(user_file_name, Vusers);
			Vusers = load_users_from_file(user_file_name);

			cout << "\n\nthis user deleted successfully !" << endl;
			return true;
		}
	}
	else
	{
		cout << "\n\nuser with username (" << username << ") not found! " << endl;
		return false;
	}
	
	
}

void show_delete_user_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tdelete user Screen";
	cout << "\n-----------------------------------\n";

	string username = read_username();
	vector <stusers> Vusers = load_users_from_file(user_file_name);
	delete_user_by_username(Vusers, username);
}

int read_permissions_to_set()
{
	int permissions;
	char option = 'Y';

	cout << "do you want to give full access (Y/N) ?\n ";
	cin >> option;
	if (toupper(option) == 'Y')
		return enmain_menue_permissions::eAll;
	
		cout << "\ndo you want to give access to : \n";

		cout << "\nshow client list (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pListClients;

		cout << "\nadd new client (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pAddNewClient;

		cout << "\ndelete client (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pDeleteClient;

		cout << "\nupdate client (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pUpdateClients;

		cout << "\nfind client (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pFindClient;

		cout << "\ntransactions (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pTranactions;

		cout << "\nmanage users (Y/N) : ";
		cin >> option;
		if (toupper(option) == 'Y')
			permissions += enmain_menue_permissions::pManageUsers;
	

	return permissions;
}

stusers read_new_user()
{
	stusers user;
	cout << "please enter username : ";
	getline(cin>>ws,user.username);

	while (user_exists_by_username(user.username, user_file_name))
	{
		cout << "user with [" << user.username << "] already exists, enter another username : ";
		getline(cin >> ws, user.username);
	}

	cout << "please enter password : ";
	getline(cin >> ws, user.password);

	user.permissions = read_permissions_to_set();

	return user;

}

void add_new_user()
{
	stusers user = read_new_user();
	add_data_line_to_file(user_file_name, convert_user_record_to_line(user));

}

void add_new_users()
{

	char option = 'Y';
	stusers user;
	do
	{
		cout << "adding new user : \n";
		add_new_user();
		
		cout << "this user added successfully, do you want to add more user (y/n) ?\n";
		cin >> option;

	} while (toupper(option) == 'Y');
}

void show_adding_users_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tadd new users screen ";
	cout << "\n-----------------------------------\n";

	add_new_users();
}

void show_list_users_screen()
{
	vector < stusers> Vusers = load_users_from_file(user_file_name);

	cout << "\t\t\t\tusers list (" << Vusers.size() << ") user(s) \n";

	cout << "\n---------------------------------------------------------------------------------------------------\n\n";
	cout << "| " << left << setw(16) << "user name "
		<< "| " << left << setw(40) << "password "
		<< "| " << left << setw(40) << "permissions " << endl;
	cout << "\n---------------------------------------------------------------------------------------------------\n\n";

	for (stusers& S : Vusers)
	{
		print_user_record_line(S);
		cout << endl;
	}
	cout << "\n---------------------------------------------------------------------------------------------------\n\n";


}

void go_back_to_manage_users_menue()
{
	cout << "\n\n\nPress any key to go back to manage users menue...";
	system("pause > 0");
	show_manage_users_menue_screen();

}

void perfrom_manage_users_menue(enmanage_user_menue_options manage_user_menue_options)
{
	switch (manage_user_menue_options)
	{
	case enmanage_user_menue_options::list_users:
		system("cls");
		show_list_users_screen();
		go_back_to_manage_users_menue();
		break;
	case enmanage_user_menue_options::add_new_user:
		system("cls");
		show_adding_users_screen();
		go_back_to_manage_users_menue();
		break;
	case enmanage_user_menue_options::delete_user:
		system("cls");
		show_delete_user_screen();
		go_back_to_manage_users_menue();
		break;
	case enmanage_user_menue_options::update_user:
		system("cls");
		show_update_user_screen();
		go_back_to_manage_users_menue();
		break;
	case enmanage_user_menue_options::find_user:
		system("cls");
		show_find_user_screen();
		go_back_to_manage_users_menue();
		break;
	case enmanage_user_menue_options::to_main_menue:
		system("cls");
		show_main_menue_screen();
		break;
	}
}


short read_manage_users_menue_options()
{
	short option;
	cout << "choose what do you want to do [1 To 6] ? \n";
	cin >> option;
	return option;
}

void show_manage_users_menue_screen()
{

	system("cls");
	cout << "\n======================================================================\n";
	cout << "\t\t\tmanage_users_menue_screen";
	cout << "\n======================================================================\n";
	cout << "\t[1] list users \n";
	cout << "\t[2] add new user  \n";
	cout << "\t[3] delete user  \n";
	cout << "\t[4] update user \n";
	cout << "\t[5] find user  \n";
	cout << "\t[6] main menue \n";
	cout << "\n======================================================================\n";

	perfrom_manage_users_menue(enmanage_user_menue_options(read_manage_users_menue_options()));
}

void show_total_balances_screen()
{
	vector <Stclients_data> Vclients_data = load_clients_data_from_file(file_name);

	cout << "\t\t\t\tbalances list (" << Vclients_data.size() << ") client(s) \n";
	cout << "\n---------------------------------------------------------------------------------------------------\n\n";
	cout << "| " << left << setw(16) << "account number "
		<< "| " << left << setw(40) << "client name "
		<< "| " << left << setw(40) << "balance " << endl;
	cout << "\n---------------------------------------------------------------------------------------------------\n\n";

	int total_balance = 0;
	if (Vclients_data.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
	{

		for (Stclients_data& C : Vclients_data)
		{
			print_client_record_balance_line(C);
			total_balance += C.account_balance;
			cout << endl;
		}

	}

	cout << "\n---------------------------------------------------------------------------------------------------\n\n";

	cout << "\t\t\t\t  total balance = " << total_balance << endl;
}










bool deposit_to_client_by_account_number(vector <Stclients_data> Vclients_data, string account_number, double amount)
{
	char option = 'Y';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? \n";
	cin >> option;

	if (toupper(option) == 'Y')
	{
		for (Stclients_data& C : Vclients_data)
		{
			if (C.account_number == account_number)
			{
				C.account_balance += amount;
				save_client_data_to_file(file_name, Vclients_data);
				cout << "\n\nDone Successfully. New balance is: " << C.account_balance;
				return true;
			}
		}
	}
	return false;
}
void show_withdraw_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\twithdraw screen ";
	cout << "\n-----------------------------------\n";
	vector <Stclients_data> Vclients_data = load_clients_data_from_file(file_name);
	string account_number = read_account_number();

	Stclients_data client;
	while (!find_clients_by_account_number(Vclients_data, account_number, client))
	{
		cout << "\nClient with [" << account_number << "] does not exist.\n";
		account_number = read_account_number();
	}
	print_client_information(client);

	double amount = 0;
	cout << "please enter amount to withdraw : ";
	cin >> amount;

	while (amount > client.account_balance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << client.account_balance << endl;
		cout << "Please enter another amount? ";
		cin >> amount;
	}

	deposit_to_client_by_account_number(Vclients_data, account_number, amount * -1);
}
void show_deposit_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tdeposit screen ";
	cout << "\n-----------------------------------\n";

	vector <Stclients_data> Vclients_data = load_clients_data_from_file(file_name);
	string account_number = read_account_number();

	Stclients_data client;
	while (!find_clients_by_account_number(Vclients_data, account_number, client))
	{
		cout << "\nClient with [" << account_number << "] does not exist.\n";
		account_number = read_account_number();
	}

	print_client_information(client);

	double amount = 0;
	cout << "please enter amount to deposit : ";
	cin >> amount;
	deposit_to_client_by_account_number(Vclients_data, account_number, amount);
}

void go_back_to_transactions_menue()
{
	cout << "\n\n\nPress any key to go back to transactions menue...";
	system("pause > 0");
	show_transactions_menue_screen();

}

void perfrom_transactions_menue(entransactions_menue_options transactions_menue_options)
{
	switch (transactions_menue_options)
	{
	case entransactions_menue_options::deposit_amount:
		system("cls");
		show_deposit_screen();
		go_back_to_transactions_menue();
		break;
	case entransactions_menue_options::withdraw_amount:
		system("cls");
		show_withdraw_screen();
		go_back_to_transactions_menue();
		break;
	case entransactions_menue_options::total_balances:
		system("cls");
		show_total_balances_screen();
		go_back_to_transactions_menue();
		break;
	case entransactions_menue_options::main_menue:
		system("cls");
		show_main_menue_screen();
	}
}


short read_transactions_menue_options()
{
	short option;
	cout << "choose what do you want to do [1 To 4] ? \n";
	cin >> option;
	return option;
}

void show_transactions_menue_screen()
{
	system("cls");
	cout << "\n======================================================================\n";
	cout << "\t\t\ttransactions menue screen";
	cout << "\n======================================================================\n";
	cout << "\t[1] deposit \n";
	cout << "\t[2] withdraw  \n";
	cout << "\t[3] total balance  \n";
	cout << "\t[4] main menue \n";
	cout << "\n======================================================================\n";

	perfrom_transactions_menue(entransactions_menue_options(read_transactions_menue_options()));

}

void show_find_client_information_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";
	string account_number = read_account_number();
	Stclients_data  client;
	vector <Stclients_data>  Vclients = load_clients_data_from_file(file_name);
	if (find_clients_by_account_number(Vclients, account_number, client))
	{
		print_client_information(client);

	} else
	{
		cout << "\n\n client with account number (" << account_number << ") is not found ! " << endl;
	}
}

bool update_client_by_account_number(string account_number, vector <Stclients_data>& Vclients_data)
{
	Stclients_data client;
	if (find_clients_by_account_number(Vclients_data, account_number, client))
	{
		print_client_information(client);

		char answer = 'Y';
		cout << "\n\nare you sure to update this client (y/Y) | (n/N) ? \n";
		cin >> answer;
		if (toupper(answer) == 'Y')
		{


			for (Stclients_data& C : Vclients_data)
			{
				if (C.account_number == account_number)
				{
					C = change_client_info(account_number);
					break;
				}
			}

			save_client_data_to_file(file_name, Vclients_data);

			cout << "\n\nthis client updated successfully" << endl;
			return true;
		}
	} else
	{
		cout << "\n\n client with account number (" << account_number << ") is not found ! " << endl;
		return false;
	}
}

void show_update_clients_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tupdate Client Screen";
	cout << "\n-----------------------------------\n";
	string account_number = read_account_number();
	vector <Stclients_data> Vclients = load_clients_data_from_file(file_name);
	update_client_by_account_number(account_number, Vclients);
}

bool mark_client_for_delete_by_account_number(vector <Stclients_data>& Vclients_data, string account_number)
{
	for (Stclients_data& C : Vclients_data)
	{
		if (C.account_number == account_number)
		{
			C.mark_client_for_delet = true;
			return true;
		}
	}
	return false;
}

bool delete_client_by_account_number(vector <Stclients_data>& Vclients_data, string account_number)
{
	Stclients_data client;
	char answer = 'Y';
	if (find_clients_by_account_number(Vclients_data, account_number, client))
	{
		print_client_information(client);

		cout << "\n\nare u sure to want delete this client  (Y/y) | (N/n) ? \n";
		cin >> answer;

		if (toupper(answer) == 'Y')
		{
			mark_client_for_delete_by_account_number(Vclients_data, account_number);
			save_client_data_to_file(file_name, Vclients_data);
			vector <Stclients_data> Vclients_data = load_clients_data_from_file(file_name);

			cout << "\n\nclient deleted successfully !" << endl;
			return true;
		}

	} else
	{
		cout << "\n\n client with account number (" << account_number << ") is not found ! " << endl;
		return false;
	}
}

void show_delete_clients_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tdelete Client Screen";
	cout << "\n-----------------------------------\n";

	string account_number = read_account_number();
	vector <Stclients_data> Vclients_data = load_clients_data_from_file(file_name);
	delete_client_by_account_number(Vclients_data, account_number);
}





Stclients_data read_client_data()

{
	Stclients_data client_data;

	cout << "enter account number : ";

	getline(cin >> ws, client_data.account_number);

	while (client_exists_by_account_number(client_data.account_number, file_name))
	{
		cout << "client with [" << client_data.account_number << "] already exists, entre another account number : ";
		cin >> client_data.account_number;
	}

	cout << "enter pin code : ";
	getline(cin >> ws, client_data.pin_code);

	cout << "enter name : ";
	getline(cin, client_data.client_name);

	cout << "enter phone : ";
	getline(cin, client_data.client_phone);

	cout << "enter account balance : ";
	cin >> client_data.account_balance;

	return client_data;
}

void add_new_client()
{
	Stclients_data client = read_client_data();
	add_data_line_to_file(file_name, convert_client_record_to_line(client));
}

void add_new_clients()
{
	char option = 'Y';
	do
	{
		cout << "\n\nadding new client : \n\n\n";

		add_new_client();


		cout << "client added successfully, do you want to add more clients : ";
		cin >> option;

	} while (toupper(option) == 'Y');
}

void show_add_new_clients_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tadd new clients Client Screen";
	cout << "\n-----------------------------------\n";

	add_new_clients();
}

void go_back_to_main_menue()
{
	cout << "\n\n\nPress any key to go back to Main Menu...";
	system("pause > 0");
	show_main_menue_screen();

}

void print_client_record_line(Stclients_data  client)
{
	cout << "| " << left << setw(16) << client.account_number
		<< "| " << left << setw(10) << client.pin_code
		<< "| " << left << setw(44) << client.client_name
		<< "| " << left << setw(16) << client.client_phone
		<< "| " << left << setw(12) << client.account_balance;

}

void print_all_clients_data(vector <Stclients_data> Vclients_data)
{
	cout << "\n\t\t\t\t\tclient list (" << Vclients_data.size() << ") client(s)\n";
	cout << "\n_______________________________________________________________________________________________________________\n\n";
	cout
		<< "| " << left << setw(16) << " Account number "
		<< "| " << left << setw(10) << " pin code "
		<< "| " << left << setw(44) << " client name"
		<< "| " << left << setw(16) << " phone"
		<< "| " << left << setw(12) << " balance";
	cout << "\n_______________________________________________________________________________________________________________\n\n";

	if (Vclients_data.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else
	{
		for (Stclients_data& client : Vclients_data)
		{
			print_client_record_line(client);
			cout << endl;
		}
	}

	cout << "\n_______________________________________________________________________________________________________________\n\n";
}

void show_all_client_screen()
{
	vector <Stclients_data> clients = load_clients_data_from_file(file_name);
	print_all_clients_data(clients);
}


void perfrom_main_menue_options(enmain_menue_options options)
{
	switch (options)
	{
	case enmain_menue_options::client_list:
		system("cls");
		show_all_client_screen();
		go_back_to_main_menue();
		break;
	case enmain_menue_options::add_clients:
		system("cls");
		show_add_new_clients_screen();
		go_back_to_main_menue();
		break;
	case enmain_menue_options::delete_client:
		system("cls");
		show_delete_clients_screen();
		go_back_to_main_menue();
		break;
	case enmain_menue_options::update_client:
		system("cls");
		show_update_clients_screen();
		go_back_to_main_menue();
		break;
	case enmain_menue_options::find_client:
		system("cls");
		show_find_client_information_screen();
		go_back_to_main_menue();
		break;
	case enmain_menue_options::transactions:
		system("cls");
		show_transactions_menue_screen();
		break;

	case enmain_menue_options::manage_users:
		system("cls");

	case enmain_menue_options::logout:
		system("cls");
		login();
		break;
	}
}

short read_main_menue_options()
{
	short option;
	cout << "choose what do you want to do [1 To 7] ? \n";
	cin >> option;

	return option;
}

void show_main_menue_screen()
{
	system("cls");
	cout << "\n======================================================================\n";
	cout << "\t\t\tmain menue screen";
	cout << "\n======================================================================\n";
	cout << "\t[1] show client list \n";
	cout << "\t[2] add new client  \n";
	cout << "\t[3] delete client  \n";
	cout << "\t[4] update client info \n";
	cout << "\t[5] find client \n";
	cout << "\t[6] transactions \n";
	cout << "\t[7] manage users \n";
	cout << "\t[8] logout ";
	cout << "\n======================================================================\n";

	perfrom_main_menue_options(enmain_menue_options(read_main_menue_options()));
}



bool load_current_user(string username, string password)
{
	
	if (find_user_by_username_and_password(username, password, current_user))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void login()
{
	bool user_faild = false;
	stusers user;
	do
	{
		
		if (user_faild)
		{
			cout << "invalid username/password\n";
		}

		cout << "enter username : ";
		getline(cin >> ws, user.username);

		cout << "enter username : ";
		getline(cin >> ws, user.password);

		user_faild = !load_current_user(user.username, user.username);

	} while (user_faild);

	show_main_menue_screen();
}

int main()
{
	login();
	system("pause>0");

}