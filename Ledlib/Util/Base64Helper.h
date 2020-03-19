#pragma once

#include <string>
#include <vector>

namespace Ledlib {
namespace Base64Helper {

void base64_encode(std::string& out, unsigned char const* bytes_to_encode, unsigned int in_len);
void base64_decode(std::vector<unsigned char>& out, std::string const& encoded_string);

}
}

