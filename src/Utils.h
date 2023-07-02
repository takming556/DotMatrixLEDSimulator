#pragma once
#include <vector>
#include <filesystem>

using std::vector;
namespace fs = std::filesystem;


class Utils {
private:
	Utils() {}
public:
	vector<fs::path> OpenFiles();
};