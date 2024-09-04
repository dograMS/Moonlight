#ifndef MOONLIGHT_HPP
#define MOONLIGHT_HPP

#include "client.hpp"
//#include "conio.h"

using namespace sun;

class MoonLight
{
public:
	MoonLight(std::string addr)
	: cl()
	{
		std::cout<<"connecting...\n";
		cl.connect(addr, 60000);
	}
	
	
	
	void LogClientScheme()
	{	
		std::cout <<"\n";
		system("clear");
		
		std::cout <<"\t...Log Page...\n"
					" 1. (/login)  Login\n"
					" 2. (/signup) Signup\n"
					" 3. (/logout) Logout\n";
		
		std::string input, cmd;
		bool wrong_input = false;
		do
		{
			wrong_input = false;
			std::cout << ">> ";
			GetLine(input);
			cmd = ToLower(GetFirstWord(input));
		
			if	  (cmd == "1" || cmd == "/login")  LoginScheme();
			else if(cmd == "2" || cmd == "/signup") SignupScheme();
			else if(cmd == "3" || cmd == "/logout") cl.Logout();
			else
			{
				std::cout <<"\t wrong Input!!";
				wrong_input = true;
			}
		}while(wrong_input);
	}
	
private:
	void LoginScheme()
	{
		std::cout <<"\n";
		system("clear");
		
		std::string input, uid, pass;
		
		std::cout << "\t...Login...\n"
		
		" user_id - ";
		do
		{
			GetLine(input);
			uid = GetFirstWord(input);
			input.resize(0);
		}while(uid.empty());
		
		std::cout << " Password - ";
		do
		{
			GetLine(input);
			pass = GetFirstWord(input);
			input.resize(0);
		}while(pass.empty());
		
		cl.SendLoginPacket(uid, pass);
		
		if(!cl.isLogged()) LoginScheme();
	
		HomePageScheme();
		
	}
	
	void SignupScheme()
	{
		std::cout <<"\n";
		system("clear");
		
		std::string input, uid, name, pass;
		
		std::cout << "\t...Signup...\n"
		
		" user_id - ";
		do
		{
			GetLine(input);
			uid = GetFirstWord(input);
			input.resize(0);
		}while(uid.empty());
		
		std::cout << " Name - ";
		GetLine(name);
		
		std::cout << " Password - ";
		do
		{
			GetLine(input);
			pass = GetFirstWord(input);
			input.resize(0);
		}while(pass.empty());
		
		cl.SendSignUpPacket(uid, name, pass);
		
		if(!cl.isLogged()) SignupScheme();
	
		HomePageScheme();
	}
	
	void HomePageScheme()
	{
		std::cout <<"\n";
		system("clear");
		
		std::cout <<"\t...MoonLight Home Page...\n"
					" 1. (/chat)    Enter Room\n"
					" 2. (/add)     Add Friend\n"
					" 3. (/list)    Friend list\n"
					" 4. (/inlist)  Incoming Requests\n"
					" 5. (/outlist) Outgoing Requests\n"
					" 6. (/find)    Find Friend\n"
					" 7. (/accept)  Accept Request\n"
					" 8. (/exit)    Exit Home\n";
					
		std::string input, cmd;
		bool exit = false;
		while(!exit)
		{
			exit = false;
			std::cout << ">> ";
			GetLine(input);
			cmd = ToLower(GetFirstWord(input));
			
			if(cmd == "1" || cmd == "/chat")
			{
				if(cl.EnterRoom(GetFirstWord(input)))
					ServeCurrentRoom();
			}
			else if(cmd == "2" || cmd == "/add")
			{
				cl.SendRoomRequestTo(GetFirstWord(input));
			}
			else if(cmd == "3" || cmd == "/list")
			{
				std::cout << "\t Friend list...\n";
				for(const std::string& d : cl.GetRoomList())
				{
					std::cout << "\t •" << d << "\n";
				}
			}
			else if(cmd == "4" || cmd == "/inlist")
			{
				std::cout << "\t Received Request list...\n";
				for(const std::string& d : cl.GetRecvRoomRequestList())
				{
					std::cout << "\t •" << d << "\n";
				}
			}
			else if(cmd == "5" || cmd == "/outlist")
			{
				std::cout << "\t Sent Request list...\n";
				for(const std::string& d : cl.GetSendRoomRequestList())
				{
					std::cout << "\t •" << d << "\n";
				}
			}
			else if(cmd == "6" || cmd == "/find")
			{
				std::cout << "\t Query Result list...\n";
				for(const std::string& d : cl.GetQueryResultFor(GetFirstWord(input)))
				{
					std::cout << "\t •" << d << "\n";
				}
			}
			else if(cmd == "7" || cmd == "/accept")
			{
				cl.AcceptRoomRequestOf(GetFirstWord(input));
			}
			else if(cmd == "8" || cmd == "/exit")
			{
				std::cout <<"\n";
				system("clear");
				return;
			}
			else
			{
				std::cout <<"\t wrong Input!!\n";
			}
		}
		
	}
	
	void ServeCurrentRoom()
	{
		system("clear");
		
		std::atomic<bool> exit_chat = false;
		
		m_printThread = std::thread(
			[this, &exit_chat]()
			{
				std::shared_ptr<ChatMessage> mesg;
				while(!exit_chat)
				{
					mesg = cl.CurrentRoomNextMessage();
					
					if(!mesg) continue;
					else if(mesg->status)
						std::cout <<"( "<< cl.CurrentRoomID() <<" ) - "<< mesg->message <<"\n";
					else
						std::cout <<"( "<< cl.MyID() <<" ) - "<< mesg->message <<"\n";
				}
			});
		
		
		std::cout <<"\t user (/exit) or (/e) to Exit chat room\n";
		
		std::string input;
		while(!exit_chat)
		{
			GetLine(input);
			
			if(input.at(0) == '/')
			{
				std::string cmd = ToLower(GetFirstWord(input));
				if(cmd == "/exit" || cmd == "/e")
				{
					exit_chat = true;
					cl.ExitRoom();
					
					std::cout <<"\n";
					system("clear");
					
					break;
				}
				
			}
			
			cl.SendTextPacket(input);
			input.clear();
		}
		
		
		if(m_printThread.joinable())
		{
			m_printThread.join();
		}
	}	
	
	void GetLine(std::string& input)
	{
		
		while(input.empty())
		{
			std::getline(std::cin, input);
			trim(input);
		}
	}
	
	std::string GetFirstWord(std::string& str)
	{
		auto end = std::find_if(str.begin(), str.end(),
			[](const char& c)
			{
				return (c == ' ');
			});
		
		std::string word(str.begin(), end);
		
		str.erase(str.begin(), end);
		trim(str);
		return word;
	}
	
	void trim(std::string& str)
	{
		str.erase(str.begin(), std::find_if(str.begin(),str.end(),
			[](const char c)
			{
				return !(c == ' ');
			}));
			
		str.erase(std::find_if(str.rbegin(),str.rend(),
			[](const char c)
			{
				return !(c == ' ');
			}).base(), str.end());
	}
	
	std::string ToLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), 
    	[](unsigned char c)
    	{
    		return std::tolower(c);
    	});
    return str;
}
	
private:
	client cl;
	std::thread m_printThread;
};


#endif 