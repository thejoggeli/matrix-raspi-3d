#include "ServerMessage.h"
#include <iostream>
#include <regex>

namespace Ledlib {

ServerMessage::ServerMessage(std::string message) {
	AddParam("type", message);
}
std::string& ServerMessage::GetJsonString(){
	json = "{";
	std::string delim_1 = "";
	std::regex reg("^(?=.)([+-]?([0-9]*)(\\.([0-9]+))?)$");
	for(auto const& key: params){
		json += delim_1 + "\"" + key.first + "\":";
		if(key.second.size() == 1){
			if(std::regex_match(key.second[0], reg)){
				// string needs no quotes (numbers, true, false)
				json += key.second[0];
			} else {
				// string needs quotes
				json += "\""+key.second[0]+"\"";
			}
		} else {
			json += "[";
			std::string delim_2 = "";
			for(auto const& val: key.second){
				if(std::regex_match(val, reg)){
					// string needs no quotes (numbers, true, false)
					json += delim_2 + val;
				} else {
					// string needs quotes
					json += delim_2 + "\""+val+"\"";
				}
				delim_2 = ",";
			}
			json += "]";
		}
		delim_1 = ",";
	}
	json += "}";
	return json;
}

}
