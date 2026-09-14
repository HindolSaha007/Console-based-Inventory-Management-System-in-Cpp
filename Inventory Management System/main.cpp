#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

class Customer
{
	protected:
		int PIN;
		char name_of_customer[20];
		char phone_number[11];
		
	public:
		int login_pin, current_login_pin;
		
		//Getting customer's name
		char* get_customer_name()
		{
			return name_of_customer;
		}
		
		//Getting customer's PIN
		int get_customer_pin()
		{
			return PIN;
		}
		
		//Getting customer's number
		char* get_customer_number()
		{
			return phone_number;
		}
		
		//Checking if PIN is avl
		int is_pin_avl(int x)
		{
			Customer c;
			ifstream read("Customer.bin", ios::binary);
			
			while(read.read((char*)&c, sizeof(Customer)))
			{
				if (c.PIN == x)
				{
					return 0; //pin already avl
				}
			}
			read.close();
			return 1;
		}
		
		//Customer Login
		int customer_login()
		{
			Customer c;
			ifstream read("Customer.bin", ios::binary);
			int v;
			

			cout<<"\nEnter your PIN to login and continue shopping: ";
			cin>>login_pin;
			current_login_pin = login_pin;
			v = is_pin_avl(login_pin);
			
			if(v == 0)
			{
				while(read.read((char*)&c, sizeof(Customer)))
				{
					if (login_pin == c.get_customer_pin())
					{
						read.close();
						cout<<"\n\n\t\t\t\t\tWelcome "<<c.get_customer_name()<<endl;
						cout<<"\t\t\t\t+===============================+";
						remove("Cart.bin");
						return 1;
					}
				}
			}
			read.close();
			cout<<"\nWrong PIN....\n";
			return 0;
		}
		
		//Creating customer acc
		void create_acc()
		{
			char temp_name[20];
			
			ofstream write("Customer.bin", ios::binary | ios::app);
			int temp, v;
			
			cout<<"Enter your name: ";
			cin.getline(name_of_customer, 20);
			strcpy(temp_name, name_of_customer);
			
			cout<<"Enter your 10-digit mobile number (in this case not the real one): ";
			cin.getline(phone_number, 11);
			
			cout<<"Set your 6-digit PIN for Login: ";
			cin>>temp;
			cin.ignore();
			v = is_pin_avl(temp);
			
			if (v == 0)
			{
				cout<<"\nPIN not available. Try using another one.";
				return;
			}
			
			else
			{
				PIN = temp;
			}
			cout<<"\nWelcome "<<temp_name<<"\nYour account has been created successfully";
			write.write((char*)this, sizeof(Customer));
			write.close();
		}
};


//Product Class
class Product : public Customer
{
	protected:
		char name_of_product[30];
		char category[20];
		float price;
		int quantity;
		
	public:
		
		//Getting product's name
		char* get_product_name()
		{
			return name_of_product;
		}
		
		//Getting product's quantity
		int get_quantity()
		{
			return quantity;
		}
		
		void set_qty(int q)
		{
			quantity = q;
		}
		
		//Getting product's category
		char* get_product_category()
		{
			return category;
		}
		
		//Getting Product's price
		float get_product_price()
		{
			return price;
		}
		
		//Add new Product
		void add_new_product()
		{
			ofstream write("Products.bin", ios::binary | ios::app);
			
			cout<<"Enter the name of the product: ";
			cin.getline(name_of_product, 30);
			
			cout<<"Enter the category to which the product belong: ";
			cin.getline(category, 20);
			
			cout<<"Enter the selling price of the product: ";
			cin>>price;
			cin.ignore();
			
			cout<<"Enter the quantity which is to be added: ";
			cin>>quantity;
			cin.ignore();
			
			write.write((char*)this, sizeof(Product));
			cout<<"\nProduct added successfully";
		}
		
		//Checking if product is avl
		int is_product_avl(char x[30])
		{
			ifstream read("Products.bin", ios::binary);
			Product p;
			
			while(read.read((char*)&p, sizeof(Product)))
			{
				if (strcmp(x, p.name_of_product)==0)
				{
					return 1; //product exists;
				}
			}
			read.close();
			return 0;
		}
		
		//Checking if the category is avl
		int is_category_avl(char x[20])
		{
			Product p;
			ifstream read("Products.bin", ios::binary);
			
			while(read.read((char*)&p, sizeof(Product)))
			{
				if (strcmp(x,p.get_product_category())==0)
				{
					return 1;
				}
			}
			read.close();
			return 0;
		}
		
		//Restock Product
		void restock_product()
		{
			char restocked_product[30];
			int v, restock_qty;
			
			cout<<"Enter the name of the product which is to be restocked: ";
			cin.getline(restocked_product, 30);
			
			v = is_product_avl(restocked_product);
			if(v == 0)
			{
				cout<<"\nIt seems that there is no such product added. Try adding it as a new product\n";
				return;
			}
			
			else
			{
				cout<<"Enter the restock quantity: ";
				cin>>restock_qty;
				cin.ignore();
				
				ifstream read("Products.bin", ios::binary);
				ofstream write("Temp_Products.bin", ios::binary);
				
				while(read.read((char*)this, sizeof(Product)))
				{
					if(strcmp(restocked_product, name_of_product)!=0)
					{
						write.write((char*)this, sizeof(Product));
					}
					
					else
					{
						quantity = quantity+restock_qty;
						write.write((char*)this, sizeof(Product));
					}
				}
				
				read.close();
				write.close();
				cout<<"\nStock added successfully.\n";
				
				remove("Products.bin");
				rename("Temp_Products.bin", "Products.bin");
			}
		}
		
		//Display Products
		void display_product()
		{
			ifstream read("Products.bin", ios::binary);
			
			while(read.read((char*)this, sizeof(Product)))
			{
				cout<<"Name: "<<name_of_product<<"\n";
				cout<<"Categoty: "<<category<<"\n";
				cout<<"Price: Rs."<<price<<"\n";
				cout<<"Quantity: "<<quantity<<"\n";
				cout<<"+=========================+\n\n";
			}
		}
		
		//Search product by product name
		void search_product_by_name()
		{
			char search[30];
			int v;
			ifstream read("Products.bin", ios::binary);
			cout<<"Enter the product name to search: ";
			cin.getline(search, 30);
			v = is_product_avl(search);
			
			if(v==1)
			{
				while(read.read((char*)this, sizeof(Product)))
				{
					if (strcmp(search, name_of_product)==0)
					{
						cout<<"\n+========================================+";
						cout<<"\nName of the Product: "<<name_of_product<<"\n";
						cout<<"Category of the Product: "<<category<<"\n";
						cout<<"Price: "<<price<<"\n";
						cout<<"Quantity: "<<quantity<<"\n";
						cout<<"+=========================================+\n";
					}
				}
			}
			else
			{
				cout<<"\nProduct not available.\n";
			}
			read.close();
		}
		
		//Search product by category
		void search_product_by_category()
		{
			char src[20];
			int v;
			ifstream read("Products.bin", ios::binary);
			
			cout<<"Enter the category name to search: ";
			cin.getline(src, 20);
			v = is_category_avl(src);
			
			if (v==1)
			{
				while(read.read((char*)this, sizeof(Product)))
				{
					if (strcmp(src, category)==0)
					{
						cout<<"\n+========================================+";
						cout<<"\nName of the Product: "<<name_of_product<<"\n";
						cout<<"Category of the Product: "<<category<<"\n";
						cout<<"Price: "<<price<<"\n";
						cout<<"Quantity: "<<quantity<<"\n";
						cout<<"+========================================+"<<endl;
					}
				}
			}
			else
			{
				cout<<"\nCategory not available\n";
			}
			read.close();
			
		}
	
};

class Shopping_cart : public Product
{
	private:
		char name[30];
		int qty;
		
	public:
			
		//Add to cart
		void add_to_cart()
		{
			Product p;
			Customer c;
			
			char temp_name[30];
			int cart_qty, val;
			
			cout<<"Enter the product you want: ";
			cin.getline(name, 30);
			strcpy(temp_name, name);
			cout<<"Enter the quantity you want: ";
			cin>>cart_qty;
			
			val = is_product_avl(name);
			strcpy(name, temp_name);
			
			if (cart_qty < 1)
			{
				cout<<"\nQuantity cannot be lesser than 1\n";
				return;
			}
			else
			{
				qty = cart_qty;
				
				if (val == 1)
				{
					ofstream write("Cart.bin", ios::binary | ios::app);
					ifstream read("Products.bin", ios::binary);
					
					while(read.read((char*)&p, sizeof(Product)))
					{
						if(strcmp(name, p.get_product_name())==0)
						{
							if (qty <= p.get_quantity())
							{
								write.write((char*)this, sizeof(Shopping_cart));
								cout<<"\nProduct: "<<name<<" Quantity: "<<qty<<" added to cart\n";
								return;
							}
							else
							{
								cout<<"\nOnly "<<p.get_quantity()<<" is available\n";
							}
						}
					}
					read.close();
					write.close();
				}
				else
				{
					cout<<"\nProduct not found. Try searching with the proper name\n";
				}
			}
		}
			
		//Is the product in cart avl
		int is_product_avl_in_cart(char x[30])
		{
			ifstream read("Cart.bin", ios::binary);
			
			while(read.read((char*)this, sizeof(Shopping_cart)))
			{
				if (strcmp(x, name)==0)
				{
					return 1; //product exists;
				}
			}
			
			return 0;
			read.close();
		}
			
		//Display Cart
		void display_cart()
		{
			ifstream read("Cart.bin", ios::binary);
				
			if(read==NULL)
			{
				cout<<"There's no item in your shopping cart.\nTry adding some.";
			}
			else
			{
				
				while(read.read((char*)this, sizeof(Shopping_cart)))
				{
					cout<<"Name of the product: "<<name<<endl;
					cout<<"Quantity added: "<<qty<<endl;
					cout<<"+==============================+\n"<<endl;
				}
				read.close();
			}
		}
			
		//Adjust items from cart
		void adjust_cart()
		{
			ifstream read("Cart.bin", ios::binary);
			char adjust_prod[30];
			int v;
				
			cout<<"Enter the name of the product to adjust: ";
			cin.getline(adjust_prod, 30);
			v = is_product_avl_in_cart(adjust_prod);
			if(v == 1)
			{
				ofstream write("Temp_cart.bin", ios::binary);
				while(read.read((char*)this, sizeof(Shopping_cart)))
				{
					if (strcmp(adjust_prod, name)==0)
					{
						cout<<"Enter the adjusted quantity: ";
						cin>>qty;
						write.write((char*)this, sizeof(Shopping_cart));
					}
					else
					{
						write.write((char*)this, sizeof(Shopping_cart));
					}
				}
				cout<<"\nItem adjusted successfully\n";
				write.close();
			}
			else
			{
				cout<<"\nItem not in your cart. Check and try again\n";
				return;
			}
			read.close();
				
			remove("Cart.bin");
			rename("Temp_cart.bin", "Cart.bin");
		}
			
		//Remove item from cart
		void remove_from_cart()
		{
			char rem_prod[30];
			int v;
			cout<<"Enter the product which you want to remove from your cart: ";
			cin.getline(rem_prod, 30);
			v = is_product_avl_in_cart(rem_prod);
				
			ifstream read("Cart.bin", ios::binary);
				
			if (v == 1)
			{
				ofstream write("Temp_cart.bin", ios::binary);
				while(read.read((char*)this, sizeof(Shopping_cart)))
				{
					if (strcmp(name, rem_prod)!=0)
					{
						write.write((char*)this, sizeof(Shopping_cart));
					}
				}
				cout<<"\n"<<rem_prod<<" removed from your cart.\n";
				write.close();
			}
				
			else
			{
				cout<<"\nItem not in your cart. Check and try again\n";
				return;
			}
			read.close();
				
			remove("Cart.bin");
			rename("Temp_cart.bin", "Cart.bin");
		}
			
		//Cart check out
		void cart_checkout_and_billing()
		{
			Customer c;
			Product p;
			Shopping_cart cart;
			
			int check_pin, v;
			float total_amt = 0;
				
			cout<<"Enter your PIN to continue buying process: ";
			cin>>check_pin;
			cin.ignore();
			v = is_pin_avl(check_pin);
			
			ifstream read_cart("Cart.bin", ios::binary);
			ifstream read_cus("Customer.bin", ios::binary);
			ifstream read_price("Products.bin", ios::binary);
			
			if(!read_cart)
			{
				cout<<"\nOops it seems your cart is empty\n";
			}
			else
			{
				if(v == 0)
				{
					if(check_pin == current_login_pin)
					{
					
						while(read_cus.read((char*)&c, sizeof(Customer)))
						{
							if(check_pin == c.get_customer_pin())
							{
								cout<<"Checking out "<<c.get_customer_name()<<"'s Cart.....\n";
								cout<<"\n\nBill Summary\n+=====================+\n";
								cout<<"\nName of the buyer: "<<c.get_customer_name();
								cout<<"\n\nContact Details: "<<c.get_customer_number();
								cout<<"\n\nItems in the Cart......."<<endl;
								while(read_cart.read((char*)&cart, sizeof(Shopping_cart)))
								{
									read_price.clear();
									read_price.seekg(0, ios::beg);
									while(read_price.read((char*)&p, sizeof(Product)))
									{
										if(strcmp(cart.name, p.get_product_name())==0)
										{
											cout<<"\nProduct: "<<p.get_product_name()<<"  Quantity: "<<cart.qty<<"  Total Price: "<<p.get_product_price()*cart.qty<<endl;
											total_amt = total_amt + (p.get_product_price()*cart.qty);
										}
									}
								}
							}
						}
						cout<<"\n\n\t\t\t\t=================";
						cout<<"\n\t\t\t\tTotal amount: "<<total_amt;
						remove("Cart.bin");
						read_price.close();
						read_cart.close();
						read_cus.close();
						
						//Updating the inventory stock
						ifstream read_prod("Products.bin", ios::binary);
						
						ofstream wr_update("Temp_update.bin", ios::binary);
						
						while(read_prod.read((char*)&p, sizeof(Product)))
						{
							ifstream read_cart("Cart.bin", ios::binary);

								
							while(read_cart.read((char*)&cart, sizeof(Shopping_cart)))
							{
								
								if (strcmp(cart.name, p.get_product_name())==0)
								{
									p.set_qty(p.get_quantity() - cart.qty);
									break;
								}
							}
							read_cart.close();
							wr_update.write((char*)&p, sizeof(Product));
						}
						wr_update.close();
						read_prod.close();
						read_cus.close();
						
						remove("Products.bin");
						rename("Temp_update.bin", "Products.bin");
						
						//Storing the products from the cart to Sales history
						ifstream rd_cart("Cart.bin", ios::binary);
						ofstream write_history("Sales_history.bin", ios::binary | ios::app);
						
						while(rd_cart.read((char*)&cart, sizeof(Shopping_cart)))
						{
							write_history.write((char*)&cart, sizeof(Shopping_cart));
						}
						rd_cart.close();
						write_history.close();
						
					}
					else
					{
						cout<<"\n\nPIN doesn't match";
						return;
					}
				}
				else
				{
					cout<<"\nIncorrect PIN\n";
					return;
				}
			}
		}
		
		//Printing the sales history
		void sales_history()
		{
			Shopping_cart cart;
			ifstream sales("Sales_history.bin", ios::binary);
			
			cout<<"\n\tSales History"<<endl;
			cout<<"\t+=================+"<<endl;
			while(sales.read((char*)&cart, sizeof(Shopping_cart)))
			{
				cout<<"\n\tName: "<<cart.name;
				cout<<"\n\tQuantity bought: "<<cart.qty;
				cout<<"\n\t+==============================+";
			}
			sales.close();
		}
};

int main()
{
	class Shopping_cart crt;
	int ch1, chv, ch2, ch3;
	
	while(1)
	{
		cout<<"\n\n\n\t\t\t\t\tINVENTORY MANAGEMENT SYSTEM"<<endl;
		cout<<"\t\t\t\t  +=====================================+"<<endl;
		cout<<"\t\t\t\t   1. For Vendors"<<endl;
		cout<<"\t\t\t\t   2. For Customers"<<endl;
		cout<<"\n\t\t\t\t   Enter your choice (1 or 2): ";
		cin>>ch1;
		cin.ignore();
		switch (ch1)
		{
			case 1:
				{
					while(1)
					{
						cout<<"\n\n\t\t\t\t\tVENDOR'S MAIN MENU"<<endl;
						cout<<"\t\t\t\t  +===================================+"<<endl;
						cout<<"\t\t\t\t   1. Add a new product."<<endl;
						cout<<"\t\t\t\t   2. Display products."<<endl;
						cout<<"\t\t\t\t   3. Search product's by name."<<endl;
						cout<<"\t\t\t\t   4. Search categories of products."<<endl;
						cout<<"\t\t\t\t   5. Restock a product."<<endl;
						cout<<"\t\t\t\t   6. Check Sales History."<<endl;
						cout<<"\t\t\t\t   7. Exit."<<endl;
						cout<<"\n\t\t\t\t   Enter your choice (1 - 7): ";
						cin>>chv;
						cin.ignore();
						
						switch (chv)
						{
							case 1:
								{
									cout<<"\n\n";
									crt.add_new_product();
								}
								break;
								
							case 2:
								{
									cout<<"\n\n";
									crt.display_product();
								}
								break;
								
							case 3:
								{
									cout<<"\n\n";
									crt.search_product_by_name();
								}
								break;
								
							case 4:
								{
									cout<<"\n\n";
									crt.search_product_by_category();
								}
								break;
								
							case 5:
								{
									cout<<"\n\n";
									crt.restock_product();
								}
								break;
								
							case 6:
								{
									cout<<"\n\n";
									crt.sales_history();
								}
								break;
								
							case 7:
								{
									exit(1);
								}
								break;
								
							default:
								{
									cout<<"\nChoose from option 1 - 6"<<endl;
								}
								continue;
						}
					}
				}
				break;
				
			case 2:
				{
					cout<<"\n\n\t\t\t\t\tCUSTOMER'S MAIN MENU"<<endl;
					cout<<"\t\t\t\t  +=============================+"<<endl;
					cout<<"\t\t\t\t   1. New sign in."<<endl;
					cout<<"\t\t\t\t   2. Already have an account."<<endl;
					cout<<"\n\t\t\t\t   Enter your choice (1 or 2): ";
					cin>>ch2;
					cin.ignore();
					
					switch (ch2)
					{
						case 1:
							{
								cout<<"\n\n";
								crt.create_acc();
							}
							break;
							
						case 2:
							{
								cout<<"\n\n";
								if (crt.customer_login() == 1)
								{
									while(1)
									{
										
										cout<<"\n\n\t\t\t\t\tCUSTOMER'S MAIN MENU"<<endl;
										cout<<"\t\t\t\t  +============================+"<<endl;
										cout<<"\t\t\t\t   1. Search for products."<<endl;
										cout<<"\t\t\t\t   2. Search Categories."<<endl;
										cout<<"\t\t\t\t   3. Add Products to Cart."<<endl;
										cout<<"\t\t\t\t   4. Display your Cart."<<endl;
										cout<<"\t\t\t\t   5. Adjust Product in cart."<<endl;
										cout<<"\t\t\t\t   6. Remove product from cart."<<endl;
										cout<<"\t\t\t\t   7. Cart checkout and billing."<<endl;
										cout<<"\t\t\t\t   8. Exit."<<endl;
										cout<<"\n\t\t\t\t   Enter your choice (1 - 8): ";
										cin>>ch3;
										cin.ignore();
										switch (ch3)
										{
											case 1:
												{
													cout<<"\n\n";
													crt.search_product_by_name();
												}
												continue;
												
											case 2:
												{
													cout<<"\n\n";
													crt.search_product_by_category();
												}
												continue;
												
											case 3:
												{
													cout<<"\n\n";
													crt.add_to_cart();
												}
												continue;
												
											case 4:
												{
													cout<<"\n\n";
													crt.display_cart();
												}
												continue;
												
											case 5:
												{
													cout<<"\n\n";
													crt.adjust_cart();
												}
												continue;
												
											case 6:
												{
													cout<<"\n\n";
													crt.remove_from_cart();
												}
												continue;
												
											case 7:
												{
													cout<<"\n\n";
													crt.cart_checkout_and_billing();
												}
												break;
												
											case 8:
												{
													exit(1);
												}
												break;
												
											default:
												{
													cout<<"\n\nChoose option from 1 - 8";
												}
												continue;
										}
										break;
									}
								}
								else
								{
									cout<<"\nWrong PIN. Customer doesn't exists\n";
								}
							}
							break;
							
						default:
							{
								cout<<"\n\nChoose from option 1 or 2"<<endl;
							}
							break;
					}
				}
				break;
				
			default:
				{
					cout<<"\n\nChoose from option 1 or 2"<<endl;
				}
				continue;
		}
	}
	return 0;
}
