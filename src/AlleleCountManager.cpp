#include "AlleleCountManager.h"
#include "AlleleCountFile.h"

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <iterator>

namespace AlleleCount
{
	AlleleCountManager* AlleleCountManager::s_instance = NULL;

	AlleleCountManager* AlleleCountManager::Instance()
	{
		if (AlleleCountManager::s_instance == NULL)
		{
			s_instance = new AlleleCountManager();
		}
		return s_instance;
	}

	AlleleCountManager::AlleleCountManager()
	{
	}

	AlleleCountManager::~AlleleCountManager()
	{
		while(!this->m_allele_files.empty())
		{
			AlleleCountFile* file = this->m_allele_files.back();
			delete file;
			this->m_allele_files.pop_back();
		}
	}

	void AlleleCountManager::addAlleleCountFile(const std::string& file_name)
	{
		AlleleCountFile* allele_count_file = new AlleleCountFile(file_name);
		allele_count_file->init();
		this->m_allele_files.push_back(allele_count_file);
	}

	void AlleleCountManager::run()
	{

		unsigned int count_sum = 0;
		unsigned int kmer_size = this->m_allele_files.front()->getKMerSize();
		unsigned int kmer_size_after_space = kmer_size + 1;
		std::vector<char> kmer(kmer_size + 100); // get a large enough buffer
		kmer[kmer_size] = ' ';
		std::vector<AlleleCountFile*>::iterator iter;
		std::string count_sum_string;
		unsigned int line_length;
		size_t count = 0;
		bool match = false;

		while (this->m_allele_files.size() > 1)
		{
			std::sort(this->m_allele_files.begin(), this->m_allele_files.end(), &AlleleCountFile::compare);

			for(iter = this->m_allele_files.begin(); iter != this->m_allele_files.end();)
			{
				if (iter == this->m_allele_files.begin())
				{
					memcpy(&kmer[0], (*iter)->getKMer(), kmer_size);
					count_sum = (*iter)->getKMerCount();
				}
				else if (memcmp(kmer.data(), (*iter)->getKMer(), kmer_size) == 0)
				{
					match = true;
					count_sum += (*iter)->getKMerCount();
				}
				else // not the first iter and not matched
				{
					match = false;
					break;
				}
				if ((*iter)->advanceKMerPointer()) // advance file pointer to next line
				{
					++iter; // only increment if haven't reached EOF
				}
				else // if we are at the end of the file, then let's remove it
				{
					this->m_allele_files.erase(iter);
				}
			}
			count_sum_string = std::to_string(count_sum);
			memcpy(&kmer[kmer_size_after_space], count_sum_string.c_str(), count_sum_string.size());
			line_length = kmer_size_after_space + count_sum_string.size();
			kmer[line_length] = '\n';
			std::cout.write(&kmer[0], line_length + 1);
		}
		this->m_allele_files.front()->printRemainingFile(); // print the rest of the file
	}

} // end namespace AlleleCount
