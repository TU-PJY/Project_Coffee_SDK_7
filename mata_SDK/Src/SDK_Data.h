#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "SDK_Config.h"
#include "Library/tinyxml/tinyxml.h"
#include "Library/cryptopp/aes.h"
#include "Library/cryptopp/filters.h"
#include "Library/cryptopp/modes.h"
#include "Library/cryptopp/osrng.h"
#include "Library/cryptopp/hex.h"
#include "SDK_Header.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <cctype>
using namespace CryptoPP;

// write and read
namespace SDK {
	class Data {
	private:
		std::string   FilePath{};
		TiXmlDocument Doc{};
		TiXmlElement* Root{};

		DataFormat    DataFormatInfo{};

		bool          FileExist{};

		std::string   CategorySearch{};
		std::string   DataSearch{};

	public:
		Data() {}

		void Load(std::string FileName, DataFormat Fmt);
		void UpdateDigitData(std::string CategoryName, std::string DataName, float Value);
		void UpdateStringData(std::string CategoryName, std::string DataName, std::string Value);
		float LoadDigitData(std::string CategoryName, std::string DataName);
		std::string LoadStringData(std::string CategoryName, std::string DataName);
		std::wstring LoadWStringData(std::string Categoryname, std::string DataName);
		SDK::DigitDataVec LoadCategoryDigitData(std::string CategoryName);
		SDK::StringDataVec LoadCategoryStringData(std::string CategoryName);
		void ResetData();
		void Release();

	private:
		void SetupData();
		void CheckDataVersion();
		void UpdateDataVersion(float VersionValue);
		void CreateDec(float VersionValue);
		void AddRoot(std::string RootName);
		void AddCategory(std::string CategoryName);
		void AddDigitData(std::string CategoryName, std::string DataName, float Value);
		void AddStringData(std::string CategoryName, std::string DataName, std::string Value);
		void WriteDigitData(TiXmlElement* CategoryVar, std::string DataName, float Value);
		void WriteStringData(TiXmlElement* CategoryVar, std::string DataName, std::string Value);
		float GetDigitData(TiXmlElement* CategoryVar, std::string DataName);
		std::string GetStringData(TiXmlElement* CategoryVar, std::string DataName);
		TiXmlElement* FindRoot();
		TiXmlElement* FindCategory(std::string CategoryName);
		bool CheckCategoryExist(std::string CategoryName);
		bool Data::CheckDataExist(std::string CategoryName, std::string DataName);
		bool LoadDataFile(std::string FileName);
		void UpdateDataFile();
		std::string Encrypt(const std::string& PlainText, const byte Key[], const byte IV[]);
		std::string Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]);
		std::string GetFileName(const std::string& FileDirectory);
		std::string GetFolderPath(const std::string& FileDirectory, const std::string& RemoveString);
	};
}