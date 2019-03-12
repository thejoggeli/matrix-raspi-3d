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

string get_app_from_file(){
	std::string ret = "";
	ifstream file;
	file.open(LEDLIB_LAUNCHER_ARGS_PATH);
	if(!file.good()){
		cout << "[Launcher] Couldn't open file: " << endl;
		cout << LEDLIB_LAUNCHER_ARGS_PATH << endl;
	} else {
		std::string s;
		if(getline(file, s)){
			file >> s;
			stringstream ss(s);
			istream_iterator<string> begin(ss);
			istream_iterator<string> end;
			vector<string> vstrings(begin, end);
			ofstream ofile;
			ofile.open(LEDLIB_LAUNCHER_ARGS_PATH);
			ofile << "";
			ofile.close();
			if(vstrings.size() > 0){
				ret = vstrings[vstrings.size()-1];
			}
		}
	}
	file.close();
	return ret;
}

int main(int argc, char** argv){
	string app = "";
	if(argc > 1){
		app = argv[argc-1];
	}
	if(app == ""){
		cout << "[Launcher] No app specified. use \"launcher -app <appname>\"" << endl;
		return EXIT_FAILURE;
	}
	bool first = true;
	while(app != ""){
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
		system(ss.str().c_str());
		app = get_app_from_file();
		first = false;
	}
	return EXIT_SUCCESS;
}
