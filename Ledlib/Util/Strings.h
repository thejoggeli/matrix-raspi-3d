#include <string>
#include <vector>

namespace Ledlib {
namespace Strings {

std::vector<std::string> Split(const std::string &str, const std::string &delimiter);
std::string Trim(const std::string &str);
bool StartsWith(const std::string &str, const std::string &with);
std::string GetExecDir();
std::string GetAppName();
std::string GetLedlibConfigPath(const std::string& filename);
std::string GetAppConfigPath(const std::string& filename);
std::string RoundToFixed(float number, int decimals);

}
};


