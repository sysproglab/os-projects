#pragma once

#include <fstream>
#include <iostream>
#include "string"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"  

using namespace rapidjson;
using namespace std;

class LocalRepository// : public MemberRepository
{
public:
	LocalRepository();
	~LocalRepository();
	bool getFirstRuning();
	bool setFirstRuning(bool);

	bool setLicenseKey(string);
	string getLicenseKey();

	bool setActivateKey(string);
	string getActivateKey();

private:
	bool isFirstRuning;
	string licenseKey;
	string activeKey;

	bool serializeData();
	bool deserializeData();
};

class Data
{
public:
	Data(){}
	~Data(){}

	bool isFirstRuning;
	string licenseKey;
	string activeKey;

	template <typename Writer>
	void Serialize(Writer& writer) const {
		writer.StartObject();
		writer.String("isFirstRuning");
		writer.Bool(isFirstRuning);

		writer.String("licenseKey");
#if RAPIDJSON_HAS_STDSTRING
		writer.String(licenseKey);
#else
		writer.String(licenseKey.c_str(), static_cast<SizeType>(licenseKey.length()));
#endif

		writer.String("activeKey");
#if RAPIDJSON_HAS_STDSTRING
		writer.String(activeKey);
#else
		writer.String(activeKey.c_str(), static_cast<SizeType>(activeKey.length()));
#endif

		writer.EndObject();
	}
};