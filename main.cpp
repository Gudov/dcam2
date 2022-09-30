#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string>
#include <functional>

using namespace std::placeholders;
using std::string;

std::tuple<void*, size_t> readFile(const std::string& file) {
	FILE* f = fopen(file.c_str(), "rb");
	fseek(f, 0, SEEK_END);
	size_t fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);
	void* buff = malloc(fileSize);
	fread(buff, 1, fileSize, f);
	fclose(f);

	return { buff, fileSize };
}

void writeFile(const std::string& file, const void* buff, size_t fileSize) {
	FILE* f = fopen(file.c_str(), "wb");
	fwrite(buff, 1, fileSize, f);
	fclose(f);
}

template<typename T>
T &find(T t, T *buff, size_t begin_pos) {
	T value = buff[begin_pos++];
	size_t seq = 0;
	while (buff[begin_pos] != t || seq < 10) {
		if (value < buff[begin_pos]) {
			seq++;
		} else {
			seq = 0;
		}
		begin_pos++;
	}

	return buff[begin_pos];
}

int main() {
	const std::string path = "E:\\SteamLibrary\\steamapps\\common\\dota 2 beta\\game\\dota\\bin\\win64\\client.dll";
	size_t fileSize;
	void* buff;
	std::tie(buff, fileSize) = readFile(path);

	size_t start_pos = 0x300'0000 / sizeof(float);
	auto findf = std::bind(find<float>, _1, (float*)buff, start_pos);

	findf(1200) = 1650;
	findf(60) = 89;

	writeFile(path, buff, fileSize);

	return 0;
}