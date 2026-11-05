/*
 * @brief ファイル関連のものをまとめたもの
 */
#pragma once
#ifndef _MYFILE_H_
#define _MYFILE_H_

#include <string>
#include <vector>

struct FileData {
	std::string fileName;
	std::string filePath;
};

class MyFile {
public:
	/*
	 * @brief 指定のフォルダ階層以下の指定の拡張子のすべてのファイルのファイルパスと名前を取得
	 * @param[in] _startFilePath  読み込むフォルダの開始階層
	 * @param[in] _extension      読み込むファイルの拡張子(.から始める)
	 * @return ファイルパスとファイルの名前をまとめたvector配列
	 */
	static std::vector<FileData> GetFileData(const std::string& _startFilePath,const std::string& _extension);

};

#endif