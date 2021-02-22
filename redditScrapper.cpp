//
//redditScrapper.cpp
//
//Reddit's default api has some limitations so this program will be created to make some of these limitations less terrible

#include <fstream>
#include <iostream>
#include <ctime>
#include <chrono>
#include <string>

int main(int argc, char *argv[]){
	if(argc < 2){
		std::cout << "Usage: readReddit <subreddit name> <days>\n";
		exit(0);
	}
	std::string subreddit(argv[1]);
	int days = 1;
	if(argc > 2){
		days = std::atoi(argv[2]);
	}
	std::time_t result = std::time(nullptr);
	std::time_t target = result - (86400 * days);
	std::string after = "";
	while(1){
		std::string url = "https://www.reddit.com/r/" + subreddit + "/new.json?sort=new&limit=100" + after;
		std::string command = "curl -s -A \"A reddit Scrapper\" \"" + url + "\" | jq '.data.children | .[] | .data.created, .data.url' > .temp.txt";
		system(command.c_str());
		std::fstream fin;
		fin.open(".temp.txt", std::ios::in);
		std::string temp;
		while(fin >> temp){
			std::time_t current = std::stoi(temp);
			if(current < target){
				fin.close();
				system("rm .temp.txt");
				exit(0);
			}
			fin >> temp;
			std::string redditUrl = "www.reddit.com/r/";
			if(temp.find(redditUrl) != std::string::npos)
				std::cout << temp.substr(1, temp.length() - 3) << "\n";
		}
		command = "curl -s -A \"A reddit Scrapper\" \"" + url + "\" | jq '.data.after' > .temp.txt";
		system(command.c_str());
		fin.close();
		fin.open(".temp.txt", std::ios::in);
		fin >> temp;
		if(temp == "null")
			exit(0);
		after = "&after=" + temp;
	}
}
