#include "MyFile.h"
#include <filesystem>

std::vector<FileData> MyFile::GetFileData(const std::string& _startFilePath, const std::string& _extension) {
	std::vector<FileData> array;
	for (const auto& entry : std::filesystem::recursive_directory_iterator(_startFilePath)) {
		// ファイル以外は無視
		if (!entry.is_regular_file())
			continue;

		auto& path = entry.path();

		// 指定の拡張子のみ読み込む
		if (path.extension() != _extension)
			continue;

		std::string filepath = path.string();
		std::string name = path.stem().string();

		FileData data = {name,filepath};

		array.emplace_back(data);
	}
	return array;
}
