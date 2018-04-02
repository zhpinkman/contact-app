#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <cassert>
#include <fstream>
#define MAX_NUMBER 11
#define ASCII 48
#define ALLOWABLE_PARTS 2
#define FIRST_NUM '0'
#define LAST_NUM '9'
using namespace std;
struct Contact
{
	int id;
	bool is_contact;
	string phone_number;
	string name;
	string sur_name;
	string email;
	string address;
};
void test_string_to_int(); 
void test_is_command(); 
void test_is_email(); 
void test_is_number(); 
void test_compare_address(); 
void test_new_contact(); 
void test_get_command(); 
void test_part_address(); 
void test_is_info();
void test_ok_to_add();
void test_check_empty();
void test_valid_id();
void test_initialized();
void test_id_index();
bool is_email(string email); 
bool is_command(string command); 
bool is_info(string info, bool after_address); 
bool is_number(string number); 
bool compare(vector<Contact> contacts, Contact con, int id);
string get_command(string line); 
Contact initialized();  
Contact new_contact(string line); 
bool compare_address(string add1, string add2); 
bool ok_to_add(Contact NEW); 
bool check_empty(string x); 
vector<string> part_address(string address); 
void print_contacts(vector<Contact> contacts);
int string_to_int(string id); 
int id_index(vector<Contact> contacts, int id); 
Contact fill_contact(Contact NEW, Contact to_update);
void print_contact(Contact temp); 
bool valid_id(int id, vector<Contact> contacts); 
vector<int> search_index(vector<Contact>contacts, string text);
vector<Contact> add(string line, vector<Contact> contacts);
vector<Contact> update(string line, vector<Contact> contacts);
vector<Contact> Delete(string line, vector<Contact> contacts);
vector<Contact> read_file();
void write_file(vector<Contact> contacts);
bool search(string line, vector<Contact> contacts);
int main()
{
	vector<Contact> contacts;
	contacts = read_file();
	string line;
	string command;
	while(getline(cin, line))
	{
		command = get_command(line);
		if(!is_command(command))
		{
			cout << "Command Failed" << endl;
			continue;
		}
		if(command == "add")
		{
			contacts = add(line, contacts);
		}
		else if(command == "update")
		{
			contacts = update(line, contacts);
		}
		else if(command == "delete")
		{
			contacts = Delete(line, contacts);
		}
		else if(command == "search")
		{
			search(line, contacts);
		}
	}
	write_file(contacts);
	return 0;
}
bool search(string line, vector<Contact> contacts)
{
	string text;
	int parts = 0;
	stringstream input(line);
	while(input >> text)
		parts ++;
	if(parts != ALLOWABLE_PARTS)
		return false;
	vector<int> index = search_index(contacts, text);
	if(index.size() == 0)
		return false;
	for(int i = 0 ;i < index.size(); i++)
		print_contact(contacts[index[i]]);
	return true;
}
vector<Contact> Delete(string line, vector<Contact> contacts)
{
	vector<Contact> list(contacts);	
	int parts = 0;
	string part;
	int index;
	int id;
	stringstream input(line);
	while(input >> part)
		parts ++;
	if(parts != ALLOWABLE_PARTS)
	{
		cout << "Command Failed" << endl;
		return list;
	}
	id = string_to_int(part);
	if(!valid_id(id, contacts))
	{
		cout << "Command Failed" << endl;
		return list;
	}
	index = id_index(list, id);
	list.erase(list.begin() + index);
	cout << "Command Ok" << endl;
	return list;
}
vector<Contact> update(string line, vector<Contact> contacts)
{
	vector<Contact> list(contacts);
	int id;	
	string part;
	stringstream input(line);
	input >> part;
	input >> part;
	id = string_to_int(part);
	if(!valid_id(id, contacts))
	{
		cout << "Command Failed" << endl;
		return list;
	}
	getline(input, line);
	if(check_empty(line))
	{
		cout << "Command Failed" << endl;
		return list;
	}
	Contact to_update = contacts[id_index(contacts, id)];
	Contact NEW = new_contact(line);
	NEW = fill_contact(NEW, to_update);
	if(compare(contacts, NEW, id) && NEW.is_contact)
	{
		list[id_index(contacts, id)] = NEW;
		cout << "Command Ok" << endl;
	}
	else
		cout << "Command Failed" << endl;
	return list;
}
vector<Contact> add(string line, vector<Contact> contacts)
{
	vector<Contact> list(contacts);
	string part;
	stringstream input(line);
	input >> part;
	getline(input, line);
	Contact NEW = new_contact(line);
	if(compare(contacts, NEW, -1) && NEW.is_contact  && ok_to_add(NEW))
	{
		if(contacts.size() == 0)
			NEW.id = 0;
		else
			NEW.id = contacts.back().id + 1;
		list.push_back(NEW);
		cout << "Command Ok" << endl;
	}
	else
		cout << "Command Failed" << endl;	
	return list;
}
vector<int> search_index(vector<Contact>contacts, string text)
{
	vector<int> index;
	for(int i = 0 ;i < contacts.size(); i++)
	{
		if(contacts[i].name.find(text) != -1 || contacts[i].sur_name.find(text) != -1 || contacts[i].email.find(text) != -1 || contacts[i].phone_number.find(text) != -1)
			index.push_back(i);
		if(contacts[i].address.find(text) != -1)
			index.push_back(i);
	}
	return index;
}
void test_valid_id()
{
	vector<Contact> list;
	Contact x;
	Contact y;
	Contact z;
	x.id = 1;
	y.id = 2;
	z.id = -1;
	list.push_back(x);
	list.push_back(y);
	list.push_back(z);
	if(valid_id(1, list) == 1 && valid_id(-1, list) == 0 && valid_id(4, list) == 0)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
bool valid_id(int id, vector<Contact> contacts)
{
	if(id == -1)
		return false;
	for(int i = 0;i < contacts.size(); i++)
		if(contacts[i].id == id)
			return true;
	return false;
}
void print_contact(Contact temp)
{
	cout << temp.id << " " << temp.name << " " << temp.sur_name << " " << temp.phone_number << " " << temp.email << " " ;
	cout << temp.address << endl;
}
Contact fill_contact(Contact NEW, Contact to_update)
{
	Contact con = NEW;
	con.id = to_update.id;
	if(check_empty(NEW.name))
		con.name = to_update.name;
	if(check_empty(NEW.sur_name))
		con.sur_name = to_update.sur_name;
	if(check_empty(NEW.email))
		con.email = to_update.email;
	if(check_empty(NEW.phone_number))
		con.phone_number = to_update.phone_number;
	if(check_empty(NEW.address))
		con.address = to_update.address;
	return con;	
}
void test_id_index()
{
	vector<Contact> list;
	Contact x;
	Contact y;
	Contact z;
	x.id = 0;
	y.id = 1;
	z.id = 2;
	list.push_back(z);
	list.push_back(x);
	list.push_back(y);
	if(id_index(list, 2) == 0 && id_index(list, 1) == 2)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
int id_index(vector<Contact> contacts, int id)
{
	for(int i = 0;i < contacts.size(); i++)
		if(contacts[i].id == id)
			return i;
	return -1;	
}
void test_string_to_int()
{
	string x = "9876";
	string y = "987jj";
	string z = "-1234";
	if(string_to_int(x) == 9876 && string_to_int(y) == -1 && string_to_int(z) == -1)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
int string_to_int(string id)
{
	int result = 0;
	for(int i =0 ;i < id.size(); i++)
		if(id[i] > '9' || id[i] < '0')
			return -1;
	for(int i = 0;i < id.size();i ++)
		result += (id[i] - ASCII)*pow(10,id.size()-1-i);
	return result;
}
void test_part_address()
{
	string address = "saAdat abad,  kaj, behzad,   arqavan , 4,   20";
	vector<string> parts = part_address(address);
	if (parts[0] == "saAdat" && parts[1] == "abad" && parts[2] == "kaj" && parts[3] == "behzad" && parts[4] == "arqavan" && parts[5] == "4" && parts[6] == "20")
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
vector<string> part_address(string address)
{
	vector<string> parts;
	string part;
	stringstream from_address(address);
	while(getline(from_address, part, ','))
	{
		stringstream ss(part);
		while(ss >> part)
			parts.push_back(part);
	}
	return parts;
}
void test_compare_address()
{
	string add1 = "saAdat abad , kaj,    behzad , arqavan , 4, 20";
	string add2 = "saAdat abad,    kaj , behzad, arqavan, 4,    20";
	string add3 ="";
	if (compare_address(add1, add2) == 0 && compare_address(add2, add3) == 1 && compare_address(add3, add3) == 1)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
bool compare_address(string add1, string add2)
{
	vector<string> first = part_address(add1);
	vector<string> second = part_address(add2);
	if(first.size() == 0 || second.size() == 0)
		return true;
	if(first.size() != second.size())
		return true;
	for(int i = 0;i < first.size();i++)
		if(first[i] != second[i])
			return true;
	return false;
}
bool compare(vector<Contact> contacts, Contact con, int id )
{
	if(contacts.size() == 0)
		return true;
	for(int i = 0;i < contacts.size(); i++)
	{
		if((con.name == contacts[i].name && con.sur_name == contacts[i].sur_name )|| con.phone_number == contacts[i].phone_number || con.email == contacts[i].email)
		{
			if(id != i)
				return false;
		}
		if(!compare_address(contacts[i].address, con.address))
		{
			if(id != i)
				return false;
		}
	}
	return true;
}
void test_is_email()
{
	string email = "zhivarsourati@gmail.com";
	string email1 = "lfkas@jl";
	string email2 = "lfkd@.dfkl";
	string email3 = "lfjds.fld";
	if(is_email(email) == 1 && is_email(email1) == 0 && is_email(email2) == 0 && is_email(email3) == 0)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
void test_is_number()
{
	string phone = "09355102061";
	string phone1 = "0935dd";
	string phone2 = "0998423";
	string phone3 = "08738783212";
	if(is_number(phone) == 1 && is_number(phone1) == 0 && is_number(phone2) == 0 && is_number(phone3) == 0)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
void test_get_command()
{
	string line = "add -f ;lakdj -l ;alkdj -e f;lkadj";
	string line1 = "update 0 -f dkkd";
	if (get_command(line) == "add" && get_command(line1) == "update")
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
void test_is_command()
{
	string command = "add";
	string command1 = "update";
	string command2 = "lkafdj";
	if(is_command(command) == 1 && is_command(command1) == 1 && is_command(command2) == 0)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
string get_command(string line)
{
	stringstream part(line);
	string command;
	part >> command;
	return command;
}
bool is_command(string command)
{
	if(command == "add" || command == "update" || command == "delete" || command == "search")
		return true;
	else
		return false;
}
void test_is_info()
{
	string x = "-f";
	string y = "l";
	string z = "y";
	if(is_info(x, 0) == 1 && is_info(y, 1) == 1 && is_info(x, 1) == 0 && is_info(z, 1) == 0)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
bool is_info(string info, bool after_address)
{
	if(after_address == false)
	{
		if(info == "-f" || info == "-l" || info == "-e" || info == "-a" || info == "-p")
			return true;
	}
	else if(after_address == true)
	{
		if(info == "f" || info == "l" || info == "e" || info == "a" || info == "p")
			return true;
	}
	return false;
}
void test_ok_to_add()
{
	Contact x = initialized();
	Contact y;
	y.name = "fkljsd";
	y.sur_name = "kldjs";
	y.email = "lkd";
	y.phone_number = "987432";
	if(ok_to_add(x) == 0 && ok_to_add(y) == 1)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
bool ok_to_add(Contact NEW)
{
	if(NEW.name == "" || NEW.sur_name == "" || NEW.email == "" || NEW.phone_number == "")
		return false;
	else
		return true;
}
void test_new_contact()
{
	string line = " -f zhivar  -l sourati -e zhivarsourati@gmail.com -p 09355102061 -a saAdat abad, kaj, behzad, arqavan, 4, 20 ";
	Contact NEW = new_contact(line);
	cerr << NEW.name << "-" << NEW.sur_name << "-" << NEW.email << "-" << NEW.phone_number << "-" << NEW.is_contact<<  endl;
	cerr << NEW.address << endl;
}
void test_initialized()
{
	Contact x = initialized();
	if(x.is_contact == true && x.id == -1 && x.name == "")
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
Contact initialized()
{
	Contact NEW;
	NEW.id = -1;
	NEW.name = "";
	NEW.sur_name = "";
	NEW.email = "";
	NEW.phone_number = "";
	NEW.address = "";
	NEW.is_contact = true;
	return NEW;
}
Contact new_contact(string line)
{
	string info;
	string part;
	stringstream parse(line);
	Contact con = initialized();
	bool after_address = false;
	while(parse >> part)
	{
		info = part;
		if(!is_info(info, after_address))
		{
			con.is_contact = false;
			break;
		}
		if(info == "-f" ||  (after_address == true && info == "f") )
		{
			parse >> part;
			if(part == info)
			{
				con.is_contact = false;
				break;
			}
			con.name = part;
			after_address = false;
		}
		else if(info == "-l" || (after_address == true && info == "l") )
		{
			parse >> part;
			if(part == info)
			{
				con.is_contact = false;
				break;
			}
			con.sur_name = part;
			after_address = false;
		}
		else if(info == "-e" ||  (after_address == true && info == "e") )
		{
			parse >> part;
			if(part == info)
			{
				con.is_contact = false;
				break;
			}
			if(!is_email(part))
			{
				con.is_contact = false;
				break;
			}
			con.email = part;
			after_address = false;
		}
		else if(info == "-p" || (after_address == true && info == "p") )
		{
			parse >> part;
			if(part == info)
			{
				con.is_contact = false;
				break;
			}
			if(!is_number(part))
			{
				con.is_contact = false;
				break;
			}
			con.phone_number = part;
			after_address = false;
		}
		else if(info == "-a" ||  (after_address == true && info == "a") )
		{
			getline(parse, part, '-');
			if(part == info)
			{
				con.is_contact = false;
				break;
			}
			if(check_empty(part))
			{
				con.is_contact = false;
				break;
			}
			con.address	= part;
			after_address = true;
		}
		else
		{
			con.is_contact = false;
			break;
		}
	}
	return con;
}
bool is_email(string email)
{
	if(email.find('.') == -1 || email.find('@') == -1)
		return false;
	else if(email.find(',') != -1)
		return false;
	else if(email.find('.') == email.find('@')+1 )
		return false;
	else if(email.find('.',email.find('@')+1) == -1)
		return false;
	else
		return true;
}	
bool is_number(string number)
{
	if(number.size() != MAX_NUMBER)
	{
		return false;
	}
	for(int i = 0;i < number.size(); i++)
		if(number[i] > LAST_NUM || number[i] < FIRST_NUM)
		{
			return false;
		}
	if(number[0] != FIRST_NUM || number[1] != LAST_NUM)
	{
		
		return false;
	}
	return true;
}
void print_contacts(vector<Contact> contacts)
{
	for(int i = 0 ;i < contacts.size() ; i ++)
	{
		print_contact(contacts[i]);
	}
}
void test_check_empty()
{
	string x = "";
	string y = " ";
	string z = ";ld";
	if(check_empty(x) == 1 && check_empty(y) == 1 && check_empty(z) == 0)
		cerr << "ok" << endl;
	else
		cerr << "failed" << endl;
}
bool check_empty(string x)
{
	stringstream line(x);
	string input = "";
	line >> input;
	if(input == "")
	{
		return true;
	}
	else
		return false;
}
vector<Contact> read_file()
{
	vector<Contact> contacts;
	string line;
	Contact con;
	ifstream file("contacts.csv");
	if(file.fail())
		return contacts;
	while(getline(file, line))
	{
		con = initialized();
		stringstream input(line);
		getline(input, line, ',');
		con.id = string_to_int(line);
		getline(input, line, ',');
		con.name = line;
		getline(input, line, ',');
		con.sur_name = line;
		getline(input, line, ',');
		con.email = line;
		getline(input, line, ',');
		con.phone_number = line;
		getline(input, line);
		if(!check_empty(line))
		{
			con.address = line;
		}
		contacts.push_back(con);
	}
	return contacts;
}
void write_file(vector<Contact> contacts)
{
	ofstream file("contacts.csv");
	for(int i = 0;i < contacts.size() ;i++)
	{
		file <<contacts[i].id << "," << contacts[i].name << "," << contacts[i].sur_name << "," << contacts[i].email << "," << contacts[i].phone_number << "," << contacts[i].address << endl;
	}
}
