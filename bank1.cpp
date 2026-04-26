#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<fstream>

using namespace std;

enum enmain_menue_options
{
	client_list = 1, add_clients, delete_client, update_client, find_client, transactions, Exit
};
enum entransactions_menue_options
{
	deposit_amount = 1, withdraw_amount, total_balances, main_menue
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

const string file_name = "clients.txt";

void show_main_menue_screen();
void show_transactions_menue_screen();
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

string convert_record_to_line(Stclients_data client, string delim = "#//#")
{
	string Sclient_record = "";
	Sclient_record += client.account_number + delim;
	Sclient_record += client.pin_code + delim;
	Sclient_record += client.client_name + delim;
	Sclient_record += client.client_phone + delim;
	Sclient_record += to_string(client.account_balance);
	return Sclient_record;

}

Stclients_data convert_line_to_record(string line, string delim = "#//#")
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
				line = convert_record_to_line(C);
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
			client = convert_line_to_record(line, "#//#");
			Vclients_data.push_back(client);
		}
		my_file.close();
	}

	return Vclients_data;

}

bool find_clients_by_account_number(vector <Stclients_data> Vclients_data, string account_number, Stclients_data& client_data)
{


	for (Stclients_data &c : Vclients_data)
	{
		if (c.account_number == account_number)
		{
			client_data = c;

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

void print_client_record_balance_line(Stclients_data client)
{
	cout << "| " << left << setw(16) << client.account_number
		 << "| " << left << setw(40) << client.client_name
		 << "| " << left << setw(40) << client.account_balance;
}

string read_account_number()
{
	string account_number;
	cout << "\nplease enter account number : ";
	getline(cin >> ws, account_number);

	return account_number;
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
			Stclients_data clients = convert_line_to_record(line, "#//#");
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

//*********************** program function ****************************************


void show_ends_screen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tends screen :-)";
	cout << "\n-----------------------------------\n";
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










bool deposit_to_client_by_account_number(vector <Stclients_data> Vclients_data, string account_number,double amount)
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

	deposit_to_client_by_account_number( Vclients_data, account_number,  amount * -1);
}
void show_deposit_screen()
{
   cout << "\n-----------------------------------\n";
   cout << "\tdeposit screen ";
   cout << "\n-----------------------------------\n";

   vector <Stclients_data> Vclients_data = load_clients_data_from_file( file_name);
   string account_number = read_account_number();

   Stclients_data client;
   while (!find_clients_by_account_number( Vclients_data,  account_number, client))
   {
	   cout << "\nClient with [" << account_number << "] does not exist.\n";
	   account_number = read_account_number();
   }

   print_client_information(client); 

   double amount = 0;
   cout << "please enter amount to deposit : ";
   cin >> amount;
   deposit_to_client_by_account_number(Vclients_data, account_number,amount);
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
	add_data_line_to_file(file_name, convert_record_to_line(client));
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
	case enmain_menue_options::Exit:
		system("cls");
		show_ends_screen();
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
	cout << "\t[7] exit ";
	cout << "\n======================================================================\n";

	perfrom_main_menue_options(enmain_menue_options(read_main_menue_options()));
}

int main()
{
	show_main_menue_screen();
	system("pause>0");

}