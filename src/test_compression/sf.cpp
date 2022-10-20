
#include <string.h>
#include "../Compression/shannon_fano.h"


int main(int argc, char* argv[])
{
	
	char buff[256]{};
	strcpy(buff, "aabbabcaaaaa");
	
	
	Compression::ShannonFano::Compress(Compression::CStringToData(buff));

}