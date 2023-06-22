//
// Created by Archishmaan Peyyety on 1/31/23.
//

#include "helpers.h"

std::vector<string> split(const string& s, const string& delimiter) {
	usize pos_start = 0, pos_end, delim_len = delimiter.length();
	string token;
	std::vector<string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}
