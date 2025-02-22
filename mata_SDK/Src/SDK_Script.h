#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "Library/tinyxml/tinyxml.h"
#include "Library/cryptopp/aes.h" 
#include "Library/cryptopp/filters.h"
#include "Library/cryptopp/modes.h"
#include "Library/cryptopp/osrng.h"
#include "Library/cryptopp/hex.h"
#include "SDK_Config.h"
#include "SDK_Header.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace CryptoPP;

// read only

namespace SDK {
	class Script {
	private:
		TiXmlDocument Doc{};
		TiXmlElement* Root{};
		bool          FileExist{};

		std::string   CategorySearch{};
		std::string   DataSearch{};

	public:
		Script() {}

		void Load(std::string FileName);
		void LoadSecure(std::string FileName);
		float LoadDigitData(std::string CategoryName, std::string DataName);
		std::string LoadStringData(std::string CategoryName, std::string DataName);
		std::wstring LoadWstringData(std::string CategoryName, std::string DataName);
		int CountCategory();
		SDK::DigitDataVec LoadCategoryDigitData(std::string CategoryName);
		SDK::StringDataVec LoadCategoryStringData(std::string CategoryName);
		void Release();


	private:
		TiXmlElement* FindCategory(std::string CategoryName);
		std::string Decrypt(const std::string& CipherText, const byte Key[], const byte IV[]);
		float GetDigitData(TiXmlElement* CategoryVar, std::string DataName);
		std::string GetStringData(TiXmlElement* CategoryVar, std::string DataName);
	};
}