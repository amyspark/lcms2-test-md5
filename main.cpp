#define CMS_NO_REGISTER_KEYWORD

#include <lcms2.h>
#include <algorithm>
#include <iostream>
#include <vector>

constexpr char hex_digit(const uint8_t value)
{
	char digit = static_cast<char>(value);
	if (value > 9U)
		digit += 'A' - '0' - 10U;
	digit += '0';
	return digit;
}

std::string hexify(const void *const buf, const size_t size)
{
	std::string hex(size * 2, '\0');
	char *dst{hex.data()};
	const uint8_t *const src = static_cast<const uint8_t *>(buf);

	for (size_t idx = 0; idx < size; ++idx) {
		*dst++ = hex_digit(src[idx] >> 4U);
		*dst++ = hex_digit(src[idx] & 0xfU);
	}
	*dst = '\0';

	return hex;
}

int main(int argc, const char** argv)
{
	if (argc <= 1) {
		std::cerr << "Please supply the path to the profile" << std::endl;
		return 1;
	}

	cmsHPROFILE profile = cmsOpenProfileFromFile(argv[1], "r");

	if (!profile) {
		std::cout << "Failed to open profile " << argv[1] << std::endl;
		return 1;
	}

	std::vector<uint8_t> id(16, 0x0);

	cmsGetHeaderProfileID(profile, id.data());

	bool isNull = std::all_of(id.begin(),
								id.end(),
								[](char c) {return c == 0;});

	if (isNull) {
		std::cout << "The profile doesn't have a MD5, overriding it now" << std::endl;
		if (cmsMD5computeID(profile)) {
			cmsGetHeaderProfileID(profile, id.data());
			isNull = false;
		}
	}

	if (isNull) {
		std::cerr << "Unable to get the MD5 checksum of the profile " << argv[1] << std::endl;
		return 2;
	}

	std::cout << hexify(id.data(), id.size()) << " " << argv[1] << std::endl;
	return 0;
}
