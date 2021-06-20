#include "Config.hpp"
#include "Util/Util.hpp"
#include <fstream>
#include <cstring>
#include <wordexp.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <filesystem>

using namespace std::literals;

std::map<std::string, Config::Var> Config::varList;
const static std::string ConfigDir = "$HOME/.config/Cunnyware";

static bool FileExists(const char* path)
{
	struct stat st;
	return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static bool DirExists(const char* path)
{
	struct stat st;
	return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

static std::string Resolve(std::string path)
{
	for (std::size_t i = 0; i < path.size(); ++i)
	{
		if (path[i] == ' ')
		{
			path.insert(i, 1, '\\');
			++i;
		}
	}

	wordexp_t p;
	if (wordexp(path.c_str(), &p, 0) != 0)
		throw Exception("Resolve({}) wordexp failed", path);
	std::string expanded = "";
	for (std::size_t i = 0; i < p.we_wordc; ++i)
		expanded += p.we_wordv[i];
	wordfree(&p);

	return expanded;
}

static std::vector<std::string> Split(const std::string& path, char delim)
{
	std::vector<std::string> words;

	std::size_t pos, lastpos = 0;
	pos = path.find(delim, lastpos);
	while (pos != std::string::npos)
	{
		words.push_back(path.substr(pos, lastpos-pos));

		lastpos = pos + 1;
		pos = path.find(delim, lastpos);
	}

	return words;
}

static std::ofstream CreateFileR(const std::string& path, const std::string& filename)
{
	const auto rpath = Resolve(path);
	try
	{
		if (!std::filesystem::exists(rpath))
			std::filesystem::create_directories(rpath);
	}
	catch (std::filesystem::filesystem_error& e)
	{
		throw Exception("CreateFileR({}, {}) `{}`", path, filename, e.what());
	}

	return std::ofstream(rpath + "/" + filename, std::ios::binary);
}

static std::ifstream OpenFile(const std::string& path, const std::string& filename)
{
	const auto file = Resolve(path) + "/" + filename;

	if (!std::filesystem::exists(file))
		throw Exception("OpenFile(): Tried to open non existing file '{}'", file);

	return std::ifstream(file);
}

std::vector<std::string> Config::GetConfigs()
{
	std::vector<std::string> configs;
	const auto path = Resolve(ConfigDir);
	
	const auto dir = opendir(path.c_str());
	if (dir == NULL)
		throw Exception("Config::GetConfigs() Could not open `{}`", path);
	struct dirent64* pdir;
	while ((pdir = readdir64(dir)))
	{
		if (pdir->d_type != DT_REG)
			continue;

		configs.push_back(pdir->d_name);
	}

	return configs;
}

void Config::Load(const std::string& name)
{
	auto file = OpenFile(ConfigDir, name);
	const std::string content(
		(std::istreambuf_iterator<char>(file)),
		(std::istreambuf_iterator<char>()));
	file.close();

	std::size_t i = 0;
	while (i != content.size())
	{
		std::size_t ilen; // Identifier length
		std::size_t dlen; // Data length
		std::string identifier;
		// Type info
		{
			std::size_t pos = content.find(' ', i);
			if (pos == std::string::npos || pos-i == 0)
				throw Exception("Config::Load(): Could not get type info for file '{}' at position '{}'", name, i);
			std::string type = content.substr(i, pos-i);

			if (type == "B")
				dlen = sizeof(bool);
			else if (type == "I")
				dlen = sizeof(i32);
			else if (type == "F")
				dlen = sizeof(f32);
			else if (type == "IV4")
				dlen = sizeof(ImVec4);
			else if (type == "S")
			{
				try
				{
					dlen = std::stoi(content.substr(i+1, i-pos-1));
				}
				catch (std::invalid_argument& e)
				{
					throw Exception("Config::Load(): Could not get string length for file '{}' at position '{}' (invalid argument) '{}'", name, i, e.what());
				}
				catch (std::out_of_range& e)
				{
					throw Exception("Config::Load(): Could not get string length for file '{}' at position '{}' (out of range) '{}'", name, i, e.what());
				}
			}
			else
				throw Exception("Config::Load() Unhandled primitive '{}'.", type);

			i = pos+1;
		}
		//Identifier length
		{
			std::size_t pos = content.find(' ', i);
			if (pos == std::string::npos || pos-i == 0)
				throw Exception("Config::Load(): Could not get identifier length for file '{}' at position '{}'", name, i);
			std::string type = content.substr(i, pos-i);

			try
			{
				ilen = std::stoi(content.substr(i, pos-i));
			}
			catch (std::invalid_argument& e)
			{
				throw Exception("Config::Load(): Could not get identifier length for file '{}' at position '{}' (invalid argument) '{}'", name, i, e.what());
			}
			catch (std::out_of_range& e)
			{
				throw Exception("Config::Load(): Could not get identifier length for file '{}' at position '{}' (out of range) '{}'", name, i, e.what());
			}

			i = pos+1;
		}
		//Identifier
		{
			if (i+ilen+1 >= content.size())
				throw Exception("Config::Load(): Could not get identifier for file '{}' at position '{}'", name, i);

			identifier = content.substr(i, ilen);

			i += ilen+1;
		}
		//Data
		{
			if (i+dlen >= content.size())
				throw Exception("Config::Load(): Could not get data for file '{}' at position '{}'", name, i);

			const auto it = varList.find(identifier);
			if (it == varList.end())
				fmt::print("Config::Load() Cound not find variable '{}' in '{}', was it removed?", identifier, name);
			else
				std::memcpy(it->second.p, reinterpret_cast<const void*>(&content.data()[i]), dlen);

			i += dlen;
		}

		++i;
	}
}

void Config::Save(const std::string& name)
{
	/*
	 * <type>.<Nid><value>
	 * eg: B 17 misc.bhop.enabled 1 // for 'bool Settings::BHop::enabled = true'
	 * S9 12 clantag.name MyClanTag // '9' is the length of the string
	*/

	auto GetType = [&](const Var& v) -> std::string
	{
		const static std::array<std::string, Var::Type::size> typeArray
		{
			"B"s, // bool
			"I"s, // i32
			"F"s, // f32
			"IV4"s, // ImVec4
			"S"s, // strings (size, buffer)
		};

		if (v.type == Var::STRING)
			return typeArray[v.type] + std::to_string(strlen(reinterpret_cast<const char*>(v.p)));

		return typeArray[v.type];
	};

	if (name.empty() || name.find('/') != std::string::npos)
		throw Exception("Configs::Save({}) invalid filename", name);

	auto file = CreateFileR(Resolve(ConfigDir), name);
	if (!file.is_open() || !file.good())
		throw Exception("Configs::Save({}) file not open", name);

	for (const auto& e : varList)
	{
		const auto type = GetType(e.second);
		file.write(type.data(), type.size()); // type info
		file.put(' ');
		file << e.first.size(); // size of identifier
		file.put(' ');
		file.write(e.first.data(), e.first.size()); // identifier
		file.put(' ');
		switch (e.second.type)
		{
			case Var::BOOL:
				file.put(*reinterpret_cast<const char(&)[sizeof(bool)]>(e.second.p));
				break;
			case Var::I32:
				file.write(reinterpret_cast<const char(&)[sizeof(i32)]>(e.second.p), sizeof(i32));
				break;
			case Var::F32:
				file.write(reinterpret_cast<const char(&)[sizeof(f32)]>(e.second.p), sizeof(f32));
				break;
			case Var::IMVEC4:
				file.write(reinterpret_cast<const char(&)[sizeof(ImVec4)]>(e.second.p), sizeof(ImVec4));
				break;
			case Var::STRING:
				file.write(reinterpret_cast<const char*>(e.second.p), strlen(reinterpret_cast<const char*>(e.second.p)));
				break;
			default:
				break;
		}
		file.put('\n');
	}

	file.close();
}

void Config::Delete(const std::string& name)
{
	const auto path = Resolve(ConfigDir) + "/" + name;

	try
	{
		if (!std::filesystem::exists(path))
			throw Exception("Config::Delete({}) File does not exists", name);

		std::filesystem::remove(path);
	}
	catch (std::filesystem::filesystem_error& e)
	{
		throw Exception("Config::Delete({}) {}", name, e.what());
	}
}
