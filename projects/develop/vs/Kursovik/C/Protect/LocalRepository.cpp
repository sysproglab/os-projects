#include "LocalRepository.h"

LocalRepository::LocalRepository()
{
	deserializeData();
}

LocalRepository::~LocalRepository()
{
	serializeData();
}

bool LocalRepository::getFirstRuning()
{
	if (!deserializeData())
		return NULL;

	return isFirstRuning;
}

bool LocalRepository::setFirstRuning(bool k)
{
	isFirstRuning = k;	
	return serializeData();
}


bool LocalRepository::setLicenseKey(string key)
{
	licenseKey = key;
	return serializeData();
}

string LocalRepository::getLicenseKey()
{
	if (!deserializeData())
		return NULL;

	return licenseKey;
}

bool LocalRepository::setActivateKey(string key)
{
	activeKey = key;
	return serializeData();
}

string LocalRepository::getActivateKey()
{
	if (!deserializeData())
		return NULL;

	return activeKey;
}

bool LocalRepository::serializeData()
{
	Data data = Data();
	data.activeKey = activeKey;
	data.isFirstRuning = isFirstRuning;
	data.licenseKey = licenseKey;
	StringBuffer sb;
	PrettyWriter<StringBuffer> writer(sb);

	data.Serialize(writer);

	ofstream fout("data.txt");
	if (!fout.is_open())
		cout << "file not opened" << endl;
	else
	{
		fout << sb.GetString() << endl;
		fout.close();
	}
	return true;
}

bool LocalRepository::deserializeData()
{
	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, "data.txt", "rb"); // non-Windows use "r"
	if (err != 0)
	{
		cout << "file not opened" << endl;
		return false;
	}

	char readBuffer[65536];
	FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	Document document;
	document.ParseStream(is);

	if (!document.IsObject());

	assert(document.HasMember("isFirstRuning"));
	assert(document["isFirstRuning"].IsBool());
	isFirstRuning = document["isFirstRuning"].GetBool();

	assert(document.HasMember("licenseKey"));
	assert(document["licenseKey"].IsString());
	licenseKey = document["licenseKey"].GetString();

	assert(document.HasMember("activeKey"));
	assert(document["activeKey"].IsString());
	activeKey = document["activeKey"].GetString();
	
	fclose(fp);
	return true;
}
