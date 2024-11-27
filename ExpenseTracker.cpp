#include <iostream> 
#include <mysql_driver.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string> 
#include <openssl/sha.h>
#include <openssl/evp.h>
using namespace std;

std::string hashPassword(std::string& password){
	unsigned char hash[SHA256_DIGEST_LENGTH]; 
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
	cout << newPassword; 
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
void logIn(sql::Connection* conn)

{	
	string query = "select * from UserManagement 
	string username , password, hashPassword, actualPassword; 
	cout << "Please insert your username: " << endl; 
	cin >> username; 

	do{
		cout << "Please inserty our password: " << endl ;
		cin >> password; 
		hashpassword 		
	
	}while (hashPassword == actualPassword); 

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
