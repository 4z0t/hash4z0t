

#include <string>
#include "../BitsAndBytes/BitsAndBytes.h"
#include "../FileData.h"

using namespace std;




int main(int argc, char** argv)
{
	using namespace H4z0t;
	using namespace BitsAndBytes;
	if (argc < 4) {
		cout << "Wrong arguments!" << endl;
		return 0;
	}

	string input_path = argv[1];
	string key_path = argv[2];
	string output_path = argv[3];


	File key_file(key_path);
	
	BytesVector key;

	for (size_t i = 0; i < key_file.GetSize(); i++)
	{
		key.push_back(key_file.Get());
	}

	cout << "Key size is " << key.size() << endl;


	


	return 0;
}