
#include <string.h>
#include "../Compression/shannon_fano.h"


int main(int argc, char* argv[])
{
	
	char buff[64]{};
	strcpy(buff, "aaaaaaaaaaaaaaabac");
	
	
	Compressions::ShannonFano::Compress(Compressions::CStringToData(buff));

}