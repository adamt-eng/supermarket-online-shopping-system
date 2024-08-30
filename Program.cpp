#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <fstream>
#include <vector>

using namespace std;

struct user { string username, password, id, name, phone_number, location; };
user active_user;
vector<user> users;

struct product { string sku, name, category, production_date, expiration_date, price, ingredients, note; int quantity = 1; };
vector<product> products;

struct order { string user_id; int id = 0; double total = 0; vector<product> products; };
order active_order;

vector<product> beverages, dairy, poultry, laundry, personal_hygiene, hair_care;
vector<vector<product>> categories = { beverages, dairy, poultry, laundry, personal_hygiene, hair_care };

void clear_terminal() { system("CLS"); }

// Prototypes
void view_main_menu(bool), logout(),
validate_input(string& input, const string& field, int),
Register(), view_login_menu(), view_and_edit_information(bool),
view_product_menu(), view_product_details(int), confirm_purchase(), view_order(), view_order_options(), edit_order();

bool login();
int generate_id();
double total_price();

string data_path, products_path;

int main()
{
	data_path = R"(C:\Users\Public\Documents\Data.csv)";
	products_path = R"(C:\Users\Public\Documents\Products.csv)";

	// Check if Products.csv exists
	// If it doesn't exist, create it and write the products' information to it
	ifstream products_check(products_path);
	if (!products_check.good())
	{
		ofstream create_products(products_path);
		if (create_products.is_open())
		{
			create_products << "1,Beyti Tropicana Cocktail Juice - 235ml,Beverages,2023-09-05,2024-09-05,7.60,-Natural Guava Pulp-Hibiscus Extract-Natural & Natural Identical Flavour (Cocktail Fruits) Antioxidant (Ascorbic Acid)-Stabilizer (Cellulose Gum),Store in a cool and dry place.\n"
				<< "2,Spiro Spathis,Beverages,2023-10-11,2024-10-11,8.0,-Apple flavour-Carbonated water-Sugar,Store in cold and dry place.\n"
				<< "3,Full Fat Milk,Dairy,2023-01-01,2024-01-01,40.0,-Cream(3% fat)-Fresh cow's milk,Store in cold and dry place.\n"
				<< "4,Chicken Thighs,Poultry,2023-03-17,2024-03-17,85.0,-Chicken,Store in a cool and dry place.\n"
				<< "5,Tide Gel,Laundry,2023-09-01,2025-12-31,124.95,-Acrylamide-Sodium carbonate-Water,Do not consume this product.\n"
				<< "6,Cold Stone,Ice Cream,2023-12-01,2023-12-31,100.0,-Cream-Skimmed Milk-Sugar,Store in a cold and dry place.\n"
				<< "7,Sensodyne Toothpaste,Personal Hygiene,2023-01-01,2023-12-31,69.9,-Fluoride-Calcium Carbonate-Triclosan,Do not consume this product\n"
				<< "8,L'Oreal Elvive,Hair Care,2023-01-01,2026-12-31,79.9,Sodium Laureth-Sulfate-Sodium Chloride-Benzyl alcohol, Do not consume this product\n"
				<< "9,Masdam Holand,Cheese,2023-01-01,Never,88.5,-Pasteurized cow's milk-Salt-Cheese cultures,Keep away from heat\n"
				<< "10,Almarai Greek Yogurt,Dairy,2023-12-06,2023-12-26,24.5,-Fresh milk-Modified corn starch-1% of natural vanilla flavor,Store in a cool and dry place\n"
				<< "11,Juhayna Classic Apple Juice 1L,Beverages,2023-10-11,2024-10-11,25.00,-Apple Juice,XXX\n"
				<< "12,Lamar Drink Red Cocktail 1L,Beverages,2023-10-11,2024-10-11,25.00,-Cherry-Cranberry-Pomegranate-Sweeteners,XXX\n"
				<< "13,Green Mojito 1L,Beverages,2023-10-11,2024-10-11,40.00,-Lime Juice-Mint Leaves-Sugar-Soda Water,XXX\n"
				<< "14,Shahd Whole Chicken 1Kg,Poultry,2023-03-17,2024-03-17,164.95,-Whole Chicken,XXX\n"
				<< "15,Mafaza Chicken Fillet 1 Kg,Poultry,2023-03-17,2024-03-17,226.95,-Chicken Fillet,XXX\n"
				<< "16,Shahd Chicken Liver 500 gm,Poultry,2023-03-17,2024-03-17,69.95,-Chicken Liver,XXX\n"
				<< "17,Shahd Chicken Shish 400 gm,Poultry,2023-03-17,2024-03-17,103.95,-Chicken Shish,XXX\n"
				<< "18,Frida Fabric Softener Spring Breeze 4L,Laundry,2023-09-01,2025-12-31,105.19,-Fabric Softener,XXX\n"
				<< "19,Ariel Automatic Liquid lavender detergent 2.5 KG,Laundry,2023-09-01,2025-12-31,149.94,-Detergent with Lavender Scent,XXX\n"
				<< "20,Velveta Gel For Automatic 1.5L,Laundry,2023-09-01,2025-12-31,117.95,-Laundry Detergent,XXX\n"
				<< "21,Gel Oxi 900g,Laundry,2023-09-01,2025-12-31,76.00,-Oxi Action Crystal White Laundry Gel,XXX\n"
				<< "22,Listerine Cool Mint 250Ml,Personal Hygiene,2023-01-01,2023-12-31,111.45,-Cool Mint Mouthwash,XXX\n"
				<< "23,Crest Toothpaste 75 ml,Personal Hygiene,2023-01-01,2023-12-31,181.45,-Toothpaste,XXX\n"
				<< "24,Rexona Charcoal 150ML,Personal Hygiene,2023-01-01,2023-12-31,100.95,-Aerosol Deodorant with Charcoal,XXX\n"
				<< "25,Gillette Razors 5 Per Pack,Personal Hygiene,2023-01-01,2023-12-31,509.45,-Razor Blades,XXX\n"
				<< "26,Dove Shampoo 350ML,Hair Care,2023-01-01,2026-12-31,69.95,-Shampoo for Intensive Hair Repair,XXX\n"
				<< "27,Eva Hair Shampoo 230Ml,Hair Care,2023-01-01,2026-12-31,104.94,-Shampoo for Curls,XXX\n"
				<< "28,Tresemme Shampoo 400 ml,Hair Care,2023-01-01,2026-12-31,89.95,-Shampoo with Keratin,XXX\n"
				<< "29,Pantene 2 in 1 Shampoo 600ml,Hair Care,2023-01-01,2026-12-31,196.45,-2in1 Shampoo with Daily Care Formula,XXX\n"
				<< "30,Redbull 250ml,Beverages,2023-10-11,2024-10-11,25.00,-Taurine-Caffeine,XXX\n"
				<< "31,Almarai Cooking Cream 1L,Dairy,2023-01-01,2024-01-01,156.50,-Fresh Cream-Fresh Cow's Milk-Milk Solids-Stabilizers-Vitamin D3,XXX\n"
				<< "32,Juhayna Sour Cream 500 Gm,Dairy,2023-06-06,2023-07-06,94.95,-Cultured Pasteurized Grade A Milk and Cream-Milk Protein Concentrate.-Vitamin A Palmitate.-Enzyme,XXX\n"
				<< "33,Saluti Old Gouda Cheese 250g,Dairy,2023-03-17,2024-03-17,199.95,-Milk-Salt-Bacterial Culture-Anatto (natural carotennoid colour)-Rennet,XXX\n";
			create_products.close();
		}
		else
		{
			cout << "\033[1;31m";
			cout << "Could not write system data (Products).";
			cout << "\033[0m";
			exit(1);
		}
	}

	// Read Products.csv
	ifstream products_csv(products_path);
	if (products_csv.is_open())
	{
		string line;
		while (getline(products_csv, line))
		{
			product product;

			product.sku = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.name = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.category = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.production_date = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.expiration_date = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.price = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.ingredients = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			product.note = line;

			products.push_back(product);

			int category = 0;

			if (product.category == "Beverages")
			{
				category = 0;
			}
			else if (product.category == "Dairy")
			{
				category = 1;
			}
			else if (product.category == "Poultry")
			{
				category = 2;
			}
			else if (product.category == "Laundry")
			{
				category = 3;
			}
			else if (product.category == "Personal Hygiene")
			{
				category = 4;
			}
			else if (product.category == "Hair Care")
			{
				category = 5;
			}

			categories[category].push_back(product);
		}
		products_csv.close();
	}
	else
	{
		cout << "\033[1;31m" << "Could not read system data (Products)." << "\033[0m";
		exit(1);
	}

	view_login_menu();
}

void view_main_menu(bool invalid_input = false)
{
	// If ViewMainMenu() is being called due to invalid input (InvalidInput = true),
	// it won't clear the terminal to allow the user
	// to see that their input was invalid.
	if (!invalid_input)
	{
		clear_terminal();
	}

	string choice;
	cout << "\033[1;32m";
	cout << "Main Menu:\n";
	cout << "\033[0m";
	cout << "[1] View/Edit Your Information\n"
		<< "[2] View Product Menu\n"
		<< "[3] View/Edit Order\n"
		<< "[4] Logout\n"
		<< "\nEnter your choice: ";
	cin >> choice;

	clear_terminal();

	if (choice == "1")
	{
		view_and_edit_information(false);
	}
	else if (choice == "2")
	{
		view_product_menu();
	}
	else if (choice == "3")
	{
		view_order_options();
	}
	else if (choice == "4")
	{
		cout << "Successfully logged out.\n\n";
		logout();
	}
	else
	{
		// Re-displays main menu without clearing the terminal ViewMainMenu(InvalidInput = true)
		// to show the user that their input was invalid.
		cout << "\033[1;31m";
		cout << "Invalid input!\n\n";
		cout << "\033[0m";

		view_main_menu(true);
	}
}

void logout()
{
	string choice;
	cout << "[1] Login\n[2] Exit\n\nEnter your choice: ";
	cin >> choice;

	clear_terminal();

	if (choice == "1" && login())
	{
		view_main_menu();
	}
	else if (choice == "2")
	{
		exit(0);
	}
	else
	{
		cout << "\033[1;31m";
		cout << "Invalid input!\n\n";
		cout << "\033[0m";

		logout();
	}
}

bool login()
{
	string username, password;

	while (true)
	{
		cout << "\033[1;32m" << "Welcome to our Supermarket!" << "\033[0m" << "\n\nUsername: ";
		cin >> username;

		cout << "Password: ";
		cin >> password;

		clear_terminal();

		for (const user& user : users)
		{
			if (user.username == username)
			{
				active_user = user;
				active_order.user_id = user.id;

				if (password == active_user.password) return true;
			}
		}

		// This part is only reached if the password or username were incorrect
		cout << "\033[1;31m";
		cout << "Invalid information!\n\n";
		cout << "\033[0m";
	}
}

void validate_input(string& input, const string& field, int max_length = 100)
{
	cout << field << ": ";
	getline(cin, input);

	if (input.length() > max_length)
	{
		cout << field << "\033[1;31m" << " can't exceed " << max_length << " characters." << "\033[0m" << "\n";
		validate_input(input, field, max_length);
	}
	else if (input.find(',') != -1)
	{

		cout << field << "\033[1;31m" << " can't contain a comma." << "\033[0m" << "\n";
		validate_input(input, field, max_length);
	}
}

void Register()
{
	user registrar;
	string first_name, last_name;

	cout << "\033[1;32m";
	cout << "Register\n";
	cout << "\033[0m";

	cin.ignore();
	validate_input(first_name, "First Name", 24);
	validate_input(last_name, "Last Name", 24);
	registrar.name = first_name + " " + last_name;
	validate_input(registrar.username, "Username", 12);
	validate_input(registrar.password, "Password", 32);
	validate_input(registrar.phone_number, "Phone Number", 16);
	validate_input(registrar.location, "Location");

	ofstream Register(data_path, ios::app);
	if (Register.is_open())
	{
		Register << "\n"
			<< registrar.username << ","
			<< registrar.password << ","
			<< generate_id() << ","
			<< registrar.name << ","
			<< registrar.phone_number << ","
			<< registrar.location;

		Register.close();

		clear_terminal();

		cout << "\033[1;32m";
		cout << "User successfully registered!\n\n";
		cout << "\033[0m";

		view_login_menu();
	}
	else
	{
		cout << "\033[1;31m";
		cout << "Could not register user.";
		cout << "\033[0m";

		exit(1);
	}
}

void view_login_menu()
{
	cout << "\033[1;32m";
	cout << "Welcome to our Supermarket!\n\n";
	cout << "\033[0m";

	// Check if Data.csv exists
	// If it doesn't exist, create it and write the data to it
	ifstream data_check(data_path);
	if (!data_check.good())
	{
		ofstream create_data(data_path);
		if (create_data.is_open())
		{
			create_data << "adamx,adampass,001,Adam Tamer,01093859555,First Settlement\nabdoggx,abdopass,002,Abdo Adel,01158561339,Third Settlement\nmeo,meopass,003,Mohamed Ahmed,01025600641,El Sherouq\nsaraswelam,sarapass,004,Sara Ahmed,01211669691,El Mokattam\nbeloanoos,belalpass,005,Belal Anas,01121114666,Fifth Settlement\ntotos,mostafapass,006,Mostafa Amr,01063005955,Fifth Settlement";
			create_data.close();
		}
		else
		{
			cout << "\033[1;31m";
			cout << "Could not write system data (Users).";
			cout << "\033[0m";
			exit(1);
		}
	}

	// Read Data.csv
	ifstream data_csv(data_path);
	if (!data_csv.is_open())
	{
		cout << "\033[1;31m";
		cout << "Could not read system data (Users).";
		cout << "\033[0m";
		exit(1);
	}
	else
	{
		string line;
		while (getline(data_csv, line))
		{
			user user;

			user.username = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			user.password = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			user.id = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			user.name = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			user.phone_number = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);

			user.location = line;

			users.push_back(user);
		}
		data_csv.close();
	}

	string choice;
	cout << "[1] Login\n[2] Register\n\nEnter your choice: ";
	cin >> choice;

	if (choice == "1")
	{
		clear_terminal();
		if (login())
		{
			view_main_menu();
		}
	}
	else if (choice == "2")
	{
		clear_terminal();
		Register();
	}
	else
	{
		clear_terminal();
		cout << "\033[1;31m";
		cout << "Invalid input!\n\n";
		cout << "\033[0m";

		view_login_menu();
	}
}

void view_and_edit_information(bool modified = false)
{
	if (modified) cout << "Updated information successfully!\n";

	string prompt;

	// Display Customer Information
	cout << "Customer Information:" <<
		"\n[1] ID: " << active_user.id <<
		"\n[2] Name: " << active_user.name <<
		"\n[3] Phone Number: " << active_user.phone_number <<
		"\n[4] Location: " << active_user.location <<
		"\n[5] Username: " << active_user.username <<
		"\n[6] Password: ***************" <<
		"\n\nTo update a field; type it's number to select it.\n\n[R] Return To Main Menu.\n\nEnter your choice: ";

	cin >> prompt;

	if (prompt == "R" || prompt == "r") view_main_menu();
	else if (prompt == "2" || prompt == "3" || prompt == "4" || prompt == "5" || prompt == "6")
	{
		// Fix input
		cin.ignore();

		cout << "\nEnter updated value: ";

		string updated_value;

		getline(cin, updated_value);

		// Find current user & update their information
		for (user& user : users)
		{
			if (user.username == active_user.username)
			{
				if (prompt == "2") user.name = active_user.name = updated_value;
				else if (prompt == "3") user.phone_number = active_user.phone_number = updated_value;
				else if (prompt == "4") user.location = active_user.location = updated_value;
				else if (prompt == "5") user.username = active_user.username = updated_value;
				else if (prompt == "6") user.password = active_user.password = updated_value;
			}
		}

		// Save modified information to the CSV file
		ofstream out_file(data_path);

		if (out_file.is_open())
		{
			for (user& user : users) out_file << user.username << ',' << user.password << ',' << user.id << ',' << user.name << ',' << user.phone_number << ',' << user.location << '\n';
			out_file.close();
		}

		clear_terminal();
		view_and_edit_information(true);
	}
	else
	{
		clear_terminal();

		if (prompt == "1") cout << "You can't modify your ID!\n\n";
		else
		{
			cout << "\033[1;31m";
			cout << "Invalid input!\n\n";
			cout << "\033[0m";
		}

		view_and_edit_information();
	}
}

void update_width(int& field_w, const string& field, int add)
{
	int field_l = field.length() + add;
	if (field_l > field_w)
	{
		field_w = field_l;
	}
}

void view_product_menu()
{
	string choice_1;
	cout << "\033[1;32m" << "Category Menu:\n\033[0m"
		<< "[1] Beverages\n"
		<< "[2] Dairy\n"
		<< "[3] Poultry\n"
		<< "[4] Laundry\n"
		<< "[5] Personal Hygiene\n"
		<< "[6] Hair Care\n"
		<< "[R] Return To Main Menu\n"
		<< "\nEnter your choice: ";
	cin >> choice_1;

	bool all_digits_1 = true;
	for (const char ch : choice_1)
	{
		if (!isdigit(ch))
		{
			all_digits_1 = false;
		}
	}

	if (all_digits_1 && choice_1.length() == 1)
	{
		clear_terminal();

		int sku_w = 0, name_w = 0, production_date_w = 0, expiration_date_w = 0, price_w = 0;

		for (const product &product : categories[stod(choice_1) - 1])
		{
			update_width(sku_w, product.sku, 3);
			update_width(name_w, product.name, 5);
			update_width(production_date_w, product.production_date, 10);
			update_width(expiration_date_w, product.expiration_date, 10);
			update_width(price_w, product.price, 5);
		}

		// Green Header
		cout << "\033[1;32m";
		cout << categories[stod(choice_1) - 1][0].category << " Menu\n\n";
		cout << "\033[0m";
		cout << left << setw(sku_w) << "SKU"
			<< setw(name_w) << "Name"
			<< setw(production_date_w) << "Production Date"
			<< setw(expiration_date_w) << "Expiration Date"
			<< setw(price_w) << "Price"
			<< "\n";

		for (const product& product : categories[stod(choice_1) - 1])
		{
			cout << left
				<< setw(sku_w) << product.sku
				<< setw(name_w) << product.name
				<< setw(production_date_w) << product.production_date
				<< setw(expiration_date_w) << product.expiration_date
				<< setw(price_w) << product.price
				<< "\n";
		}

		string choice_2;
		cout << "\nTo view details for a product; enter it's SKU.\n\n[R] Return To Main Menu\n\nEnter your choice: ";
		cin >> choice_2;

		if (choice_2 == "R" || choice_2 == "r")
		{
			view_main_menu();
		}
		else
		{
			bool all_digits_2 = true;
			for (const char ch : choice_2) if (!isdigit(ch)) all_digits_2 = false;

			if (all_digits_2 && stoi(choice_2) <= size(products) && stoi(choice_2) >= 0)
			{
				view_product_details(stoi(choice_2) - 1);
			}
			else
			{
				clear_terminal();
				cout << "\033[1;31m";
				cout << "Invalid input!\n\n";
				cout << "\033[0m";
				view_product_menu();
			}
		}
	}
	else
	{
		if (choice_1 == "R" || choice_1 == "r")
		{
			view_main_menu();
		}
		else
		{
			clear_terminal();
			cout << "\033[1;31m";
			cout << "Invalid input!\n\n";
			cout << "\033[0m";
			view_product_menu();
		}
	}
}

void view_product_details(int i)
{
	clear_terminal();

	cout << "Product: " << products[i].name << "\n";
	cout << "\nCategory: " << products[i].category << "\n";
	cout << "\nPrice: " << products[i].price << "\n";
	cout << "\nIngredients: ";

	// Replace dashes with new lines
	for (char ch : products[i].ingredients)
	{
		ch != '-' ? cout << ch : cout << "\n";
	}

	cout << "\n\nNote: " << products[i].note << "\n";
	cout << "\nProduction Date: " << products[i].production_date << "\n";
	cout << "\nExpiration Date: " << products[i].expiration_date;

	string choice;
	cout << "\n\n[1] Add product to shopping cart.";
	cout << "\n[R] Return To Product Menu.\n\nEnter your choice: ";
	cin >> choice;

	clear_terminal();

	if (choice == "1")
	{
		bool found = false;

		for (product& product : active_order.products)
		{
			if (product.sku == products[i].sku)
			{
				found = true;
				product.quantity++;
				break;
			}
		}

		if (!found)
		{
			active_order.products.push_back(products[i]);
		}

		cout << "Added \"" << products[i].name << "\" successfully to shopping cart!\n\n";

		view_product_menu();
	}
	else if (choice == "R" || choice == "r")
	{
		view_product_menu();
	}
	else
	{
		view_product_details(i);
	}
}

void confirm_purchase()
{
	cout << "Order ID: #" << active_order.id << "\n";
	cout << "Customer ID: #" << active_order.user_id << "\n";
	cout << "Phone Number: " << active_user.phone_number << "\n";
	cout << "Shipping Address: " << active_user.location << "\n";
	cout << "Total Price: " << total_price();

	string confirmation;
	cout << "\n\nType CONFIRM to confirm your payment: ";
	cin >> confirmation;

	if (confirmation == "CONFIRM")
	{
		cout << "\nPayment confirmed! Your order will be shipped soon!";
	}
	else
	{
		cout << "\nAre you sure you want to cancel your order? (Y/N): ";
		cin >> confirmation;

		if (confirmation == "N")
		{
			cout << "\nPayment confirmed! Order will be shipped soon.";
		}
		else if (confirmation == "Y")
		{
			cout << "\nPayment cancelled.";
		}
		else
		{
			clear_terminal();
			cout << "\033[1;31m";
			cout << "Invalid input!\n\n";
			cout << "\033[0m";
			confirm_purchase();
		}
	}

	cin.ignore();
	cout << "\n\nPress enter to exit program.";
	cin.get();
}

int generate_id()
{
	srand(time(nullptr));

	int id = 0;

	for (int i = 0; i < 5; i++)
	{
		id = id * 5 + rand() % 5;
	}

	return id;
}

double total_price()
{
	active_order.total = 0;

	for (const product& product : active_order.products)
	{
		active_order.total += stod(product.price) * product.quantity;
	}

	return active_order.total;
}

void view_order()
{
	for (const product& product : active_order.products)
	{
		if (product.quantity != 0)
		{
			cout << "Product: " << product.name << "\n";
			cout << "SKU: " << product.sku << "\n";
			cout << "Quantity: " << product.quantity << "\n";
			cout << "Price: " << product.price << " x " << product.quantity << " = ";
			cout << fixed << setprecision(2) << stod(product.price) * product.quantity << "\n\n";
		}
	}

	cout << "Total Price: " << fixed << total_price();
}

void view_order_options()
{
	if (total_price() == 0)
	{
		cout << "Your order is empty. Add some products!\n\n";
		view_product_menu();
	}
	else
	{
		view_order();

		string choice;
		cout << "\n\n[1] Edit Order\n[2] Finalize Purchase\n\nEnter your choice: ";
		cin >> choice;

		if (choice == "1")
		{
			edit_order();
		}
		else if (choice == "2")
		{
			clear_terminal();
			active_order.id = generate_id();
			confirm_purchase();
		}
		else
		{
			clear_terminal();
			view_order_options();
		}
	}
}

void edit_order()
{
	clear_terminal();

	string choice_1;
	cout << "[R] Return To Order\n[1] View Product Menu\n[2] Select Products To Remove\n\nEnter your choice: ";
	cin >> choice_1;

	clear_terminal();

	if (choice_1 == "R" || choice_1 == "r")
	{
		view_order_options();
	}
	else if (choice_1 == "1")
	{
		view_product_menu();
	}
	else if (choice_1 == "2")
	{
		view_order();

		string choice_2;
		cout << "\n\nType the SKU of the product you'd like to remove from your order: ";
		cin >> choice_2;

		for (product& product : active_order.products)
		{
			if (product.sku == choice_2)
			{
				product.quantity--;
				break;
			}
		}

		clear_terminal();
		view_order_options();
	}
	else
	{
		edit_order();
	}
}
