#include <iostream> 
#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string> 
#include <openssl/sha.h>
#include <openssl/evp.h>
using namespace std;
std::string hashPassword(std::string& password){ unsigned char hash[SHA256_DIGEST_LENGTH]; 
	EVP_MD_CTX* ctx = EVP_MD_CTX_new(); //initialize ctx
	EVP_DigestInit_ex(ctx, EVP_sha256(),nullptr);
	EVP_DigestUpdate(ctx, password.c_str() , password.size()); 
	EVP_DigestFinal_ex(ctx, hash, nullptr); 
	EVP_MD_CTX_free(ctx);

	string newPassword;
	for (int i = 0 ; i < SHA256_DIGEST_LENGTH; i++){
		char buf[3];
		sprintf(buf,"%02x",hash[i]); 

		newPassword.append(buf);

	
	
	}
	return newPassword; 
}


sql::Connection* connectDatabase(){
	try{
		sql::mysql::MySQL_Driver *driver;
		sql::Connection *con; 
	       	driver = sql::mysql::get_mysql_driver_instance(); 	
		con = driver->connect("tcp://127.0.0.1:3306","root","netacad");
		con->setSchema("ExpenseTracker"); 
		return con; 	
	 
	} catch (sql::SQLException &e) {
		cout << "Error connecting Databae" << e.what() << endl; 
		return nullptr; 
	}
}
void viewTransaction(sql::Connection* con, string username){
	string queryOne = " select * from Income where username = ? ; " ; 
	string queryTwo = "select * from Expense where username = ? ; " ; 
	sql::PreparedStatement* pstmt1 = con ->prepareStatement(queryOne); 
	sql::PreparedStatement* pstmt2 = con->prepareStatement(queryTwo); 
	pstmt1->setString(1,username); 
	pstmt2->setString(1,username); 
	sql::ResultSet* res1 = pstmt1->executeQuery();
	sql::ResultSet* res2 = pstmt2->executeQuery(); 
	cout << "Income: " << endl;
	while (res1->next()){
		cout << res1->getString("source") << ": " << res1->getString("amount") << endl;  
	}
	string oneSum = " select SUM(amount) as sum from Income where username = ? ; " ; 
	sql::PreparedStatement* pstmt3 = con->prepareStatement(oneSum);
	pstmt3->setString(1,username); 
	sql::ResultSet* sumOne = pstmt3->executeQuery();
       	while (sumOne->next()){
		cout << "Total Income: " << sumOne->getString("sum") << endl; 	
	}	

	cout << "Expense: " << endl; 
	while (res2->next()){
		cout << res2->getString("source") << ": " << res2->getString("amount") << endl ; 
	}

	string twoSum = " select SUM(amount) as sum from Expense where username = ? ; " ; 
	sql::PreparedStatement* pstmt4 = con->prepareStatement(twoSum);
	pstmt4->setString(1,username); 
	sql::ResultSet* sumTwo = pstmt4->executeQuery();
       	while (sumTwo->next()){
		cout << "Total Expense: " << sumTwo->getString("sum") << endl; 	
	}	
	


}

string incomeExpense (){
	int userInput; 
	cout << "What would you like to change? " << endl; 
	cout << "1. Income " <<endl; 
	cout << "2. Expense " << endl;
        cin >> userInput; 	
	return (userInput == 1 ) ? "Income" : "Expense" ; 
}
void addTransaction(sql::Connection* con, string username){
	int amount ;
	string source ; 
	string track = incomeExpense(); 
	cout << "Please insert the source: " << endl; 
	cin >> source ; 
	cout << "Pleasee insert the amount: " << endl; 
	cin >> amount; 
	string query = "insert into " + track + " (username,source,amount) values (?,?,?);"; 
	sql::PreparedStatement* pstmt = con->prepareStatement(query); 
	pstmt->setString(1,username); 
	pstmt->setString(2,source); 
	pstmt->setInt(3,amount); 
	pstmt->executeUpdate(); 
	cout << "Transaction added" << endl; 
}

void deleteTransaction(sql::Connection* con, string username){
	int amount ;
       	string source ;
	string track = incomeExpense(); 
	cout << "Please inert the source that you would like to delete: " << endl; 
	cin >> source; 
	string query = "delete from " + track + " where username = ?  and source = ? ; "; 
	sql::PreparedStatement* pstmt = con->prepareStatement(query); 
	pstmt->setString(1,username);
	pstmt->setString(2,source); 
	pstmt->executeUpdate();
	cout << "Transaction deleted" << endl; 

	

			
}

void editTransaction(sql::Connection* con, string username){
	int amount ;
	string source, newSource; 
	string track = incomeExpense();
	cout << "Please insert the source that you would like to update: " << endl; 
	cin  >> source; 
	cout << "Please insert the new source name: " << endl;
	cin >> newSource; 
	cout << "Please insert the new amount: " << endl; 
	cin >> amount; 
	string query = "Update " + track + " set source = ? , amount = ? where source = ? and username = ? ; " ; 
	sql::PreparedStatement* pstmt = con->prepareStatement(query); 
	pstmt->setString(1,newSource);
	pstmt->setInt(2,amount);
	pstmt->setString(3,source);
	pstmt->setString(4,username);
	pstmt->executeUpdate();
	cout << "Transaction Update" << endl; 
}


void expenseTracker(sql::Connection* con, string username) {
	int userInput; 
	do
	{
		cout << "1. Add a transaction" << endl;
		cout << "2. Delete a transaction" << endl; 
		cout << "3. Edit a transaction" << endl; 
		cout << "4. Go back" << endl ;
		cin >> userInput; 
		switch (userInput){
			case 1: 
				addTransaction(con,username);
				break; 
			case 2: 
				deleteTransaction(con,username);
				break;
			case 3: 
				editTransaction(con,username);
				break;
			case 4: 
				return; 
			default : 
				cout << "Invalid Input" << endl; 
		}
	
	}while(userInput != 4); 	
}
void logInMenu(sql::Connection* con, string username)

{
	int userInput; 
	do {
		cout << "Hello " << username << endl; 
		cout << "1. View Transactions " << endl ; 
		cout << "2. Edit Transactions " << endl  ; 
		cout << "3. Exit " << endl; 	
		cin >> userInput; 
		switch (userInput){ 
			case 1: 
				viewTransaction(con,username); 
				break;
			case 2: 
				expenseTracker(con,username);
				break;
			case 3:
				cout << "Logging Out" << endl; 
				return; 		
			default:
			       cout << "Inavlid input " ; 
		       		       
		
		}			
	}while (userInput !=3); 

} 

void logIn(sql::Connection* con)

{	
	string query = "select * from UserManagement where username = ?" ; 
	string username , password, hashedPassword, actualPassword; 
	cout << "Please insert your username: " << endl; 
	cin >> username; 
	sql::PreparedStatement* pstmt = con->prepareStatement(query); 
	pstmt->setString(1,username); 
	sql::ResultSet* res = pstmt->executeQuery(); 

	if (!res->next()){
		cout << "Invaliid username" << endl; 
		return ;
	}

	actualPassword = res->getString("password"); 
	
	do{
		cout << "Please inserty our password: " << endl ;
		cin >> password; 
		hashedPassword = hashPassword(password);  		
		if (hashedPassword != actualPassword){
			cout << "Password incorrect" << endl ; 
			
		}


	}while (hashedPassword != actualPassword); 
	cout << "Log in successful" << endl ; 
	logInMenu(con,username);	


}


void registerAccount(sql::Connection* con){
	string username , password , confirmPassword, hashedPassword; 
	cout << "Please enter your username: " << endl; 
	cin >> username; 
	do {
		cout << "Please enter your password: " << endl; 
		cin >> password; 
		cout << "Please confirm your password: " << endl; 
		cin >> confirmPassword ; 
		if (password != confirmPassword) {
			cout << "The passwords do not match " << endl ;
		}

	} while (password != confirmPassword); 

	string query = "Insert into UserManagement (username,password) VALUES (?,?)"; 
	hashedPassword = hashPassword(password); 
	sql::PreparedStatement* pstmt = con->prepareStatement(query); 
	pstmt->setString(1,username); 
	pstmt->setString(2,hashedPassword); 
	pstmt->executeUpdate(); 
	cout << "User added" << endl ; 	

}


int main () {
	sql::Connection* con = connectDatabase(); 
	int userInput; 
	do 
	{
		cout << "Main Menu" << endl ;
		cout << "1. Log In" << endl; 
		cout << "2. Register" << endl ;
		cout << "3. Exit" << endl; 
		cin >> userInput; 
		switch(userInput){
			case 1: 
				logIn(con); 
				break; 
			case 2:
				registerAccount(con);
				break; 
		
			case 3:
				return 0 ;
			
			default: 
				cout << "Invalid input" << endl ;
		}
	
	
	
	
	
	}while(userInput != 3); 
	
}
