#include <vector>
#include <map>
#include <iterator>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <msgpack.hpp>
#include <zlib.h>
#include "markov.h"

void seed(markov_dict &dict, std::string string)
{
	std::stringstream ss(string);
	std::string first;
	std::string word;
	std::string prev;

	while(std::getline(ss, word, ' '))
	{
		if(first != "" && prev != "")
		{
			std::cout << first << " " << prev << " => " << word << std::endl;
			dict[first + " " + prev][word]++;
		}
		first = prev;
		prev = word;
	}
}

markov_pair get_random_pair(markov_dict dict)
{
	markov_dict::iterator it = dict.begin();
	std::advance(it, std::rand() % dict.size());
	return *it;
}

std::string do_weighted_random(markov_state word)
{
	int total_weight = 0;
	markov_state::iterator it = word.begin();
	for(;it != word.end(); it++)
	{
		total_weight += it->second;
	}
	int val = rand() % total_weight;
	it = word.begin();
	for(; it != word.end(); it++)
	{
		if(val < it->second)
			return it->first;
		val -= it->second;
	}
	return "!!!!";
}

std::string generate(markov_dict dict, unsigned int len)
{
	markov_pair start = get_random_pair(dict);
	std::string context = start.first;
	int space = context.find(" ");
	std::string cur = context.substr(space+1, context.length() - space);
	std::string result = context;
	len-=2;

	std::string res;
	for(; len != 0; len--)
	{
		if(dict[context].size() == 0) break; // end of chain
		res = do_weighted_random(dict[context]);
		context = cur + " " + res;
		cur = res;
		result += " " + res;
	}
	return result;
}

void save(markov_dict dict, std::string filename)
{
	std::ostringstream os;
	msgpack::pack(os, dict);
	size_t orig_len = os.str().length();
	const unsigned char *uncompressed = (const unsigned char *)os.str().c_str();
	size_t compressed_len = (orig_len * 1.1) + 12;
	unsigned char *compressed = new unsigned char[compressed_len];
	int res = compress(compressed, &compressed_len, uncompressed, orig_len);
	if(res == Z_OK)
	{
		std::ofstream of(filename, std::ofstream::binary);
		of.write((const char*)&orig_len, sizeof(size_t));
		of.write((const char *)compressed, compressed_len);
		of.close();
	}
	else
	{
		std::cerr << "zlib error!" << std::endl;
		exit(1);
	}
}

markov_dict load(std::string filename)
{
	markov_dict dict;
	std::ifstream is(filename, std::ifstream::binary);
	if(is)
	{
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);
		unsigned char *buf = new unsigned char[length];
		is.read((char*)buf, length);
		is.close();

		size_t orig_len = *(size_t*) buf;
		size_t new_len;
		buf+= sizeof(size_t);
		unsigned char *decompressed = new unsigned char[orig_len];
		int res = uncompress(decompressed, &new_len, buf, orig_len);
		if(res == Z_OK)
		{
			msgpack::unpacked res = msgpack::unpack((const char *)decompressed, new_len);
			msgpack::object obj= res.get();
			obj.convert(&dict);
			//delete buf;
			delete decompressed;
		}
		else
		{
			std::cerr << "Zlib error!" << std::endl;
			exit(1);
		}
		return dict;
	}
}
