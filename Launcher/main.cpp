#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <fstream>
#include <string>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

std::string app = "";
std::string args = "";

vector<string> Split(const string &str, const string &delimiter){
	vector<string> split;
	size_t len = delimiter.length();
	size_t pos = 0;
	string s = str;
	string token;
	while((pos = s.find(delimiter)) != string::npos){
		token = s.substr(0, pos);
		split.push_back(token);
		s.erase(0, pos + len);
	}
	split.push_back(s);
	return split;
}

void get_app_from_file(){
	app = "";
	args = "";
	ifstream file;
	file.open(LEDLIB_LAUNCHER_ARGS_PATH);
	if(!file.good()){
		cout << "[Launcher] Couldn't open file: " << endl;
		cout << LEDLIB_LAUNCHER_ARGS_PATH << endl;
	} else {
		std::string s;
		getline(file, s);
		vector<string> split = Split(s, " ");
		app = split[0];
		for(int i = 1; i < split.size(); i++){
			args += split[i];
		}
		ofstream ofile;
		ofile.open(LEDLIB_LAUNCHER_ARGS_PATH);
		ofile << "";
		ofile.close();
	}
	file.close();
}

int main(int argc, char** argv){
	if(argc == 1){
		// cout << "[Launcher] No app specified. use \"Launch <appname> [options]\"" << endl;
		// return EXIT_FAILURE;
		app = "Welcome";
	} else {
		app = argv[1];
	}
	for(int i = 2; i < argc; i++){
		args += argv[i];
	}
	bool quit = false;
	bool first = true;
	while(!quit){
		if(!first){
			struct winsize size;
			ioctl(STDOUT_FILENO, TIOCGWINSZ,&size);
			std::string buffer;
			for(int i = 0; i < size.ws_col; i++){
				buffer += "=";
			}
			cout << buffer << endl;
		}
		cout << "[Launcher] Launching app: " << app << endl;
		stringstream ss;
		ss << LEDLIB_APPS_PATH << "/" << app << "/" << app;
		if(args != "") ss << " " << args;
		system(ss.str().c_str());
		get_app_from_file();
		first = false;
		if(app == ""){
			app = "Welcome";
			// cout << "[Launcher] Quitting: app string is empty" << endl;
			// quit = true;
		} else if(app == "quit_matrix"){
			cout << "[Launcher] Quitting: quit_matrix requested" << endl;
			quit = true;
		}
	}
	return EXIT_SUCCESS;
}
