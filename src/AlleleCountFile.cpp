#include "AlleleCountFile.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>

namespace AlleleCount
{
	const static size_t FILE_CHUNK = 500000;

	AlleleCountFile::AlleleCountFile(const std::string& file_name) :
		m_file_name(file_name),
		m_kmer_size(0),
		m_map_ptr(0),
		m_map_file_pos(0),
		m_count_start_pos(0),
		m_current_mmap_start_position(0)
	{
	}

	AlleleCountFile::~AlleleCountFile()
	{
		munmap(this->m_map_ptr, this->m_total_file_size);
	}

	void AlleleCountFile::init()
	{
		this->m_fd = -1;
		char* result;
		struct stat buf;
		this->m_fd = open(this->m_file_name.c_str(), O_RDONLY);
		if (this->m_fd < 0)
		{
			std::cout << "error while opening file: " << this->m_file_name << std::endl;
			exit(0);
		}

		if (fstat(this->m_fd, &buf) < 0)
		{
			std::cout << "Error while determining file length for file: " << this->m_file_name << std::endl;
			exit(0);
		}

		this->m_total_file_size = (size_t)buf.st_size;

		this->m_map_ptr = (char*)mmap64(NULL, this->m_total_file_size, PROT_READ,MAP_FILE|MAP_PRIVATE,this->m_fd,0);
		this->m_map_file_pos = this->m_map_ptr;
		this->m_current_mmap_start_position = this->m_map_file_pos;

		// set  kmer size
		setKMerSize();

		this->m_count_start_pos = (this->m_kmer_size + 1);
	}

	void AlleleCountFile::setKMerSize()
	{
		this->m_kmer_size = 0;
		// calculate kmer size
		while (this->m_map_ptr[this->m_kmer_size] != '\t')
		{
			++this->m_kmer_size;
		}
	}

	bool AlleleCountFile::advanceKMerPointer()
	{
		while (*this->m_map_file_pos != '\n')
		{
			++this->m_map_file_pos;
		}
		++this->m_map_file_pos; // go one past the new line
		if (this->m_total_file_size <= (this->m_map_file_pos - this->m_map_ptr)) { return false; }
		if ((this->m_map_file_pos - this->m_current_mmap_start_position) > FILE_CHUNK) { freeAndReallocateMMap(); }
		return true;
	}

	unsigned int AlleleCountFile::getKMerCount()
	{
		int val = 0;
		int pos = this->m_count_start_pos;

		while (this->m_map_file_pos[pos] != '\n')
		{
			val = val*10 + (this->m_map_file_pos[pos] - '0');
			++pos;
		}

		return val;
	}

	void AlleleCountFile::freeAndReallocateMMap()
	{
		size_t current_position = (this->m_map_file_pos - this->m_map_ptr);
		munmap(this->m_map_ptr, this->m_total_file_size);
		this->m_map_ptr = (char*)mmap64(NULL, this->m_total_file_size, PROT_READ,MAP_FILE|MAP_PRIVATE,this->m_fd,0);
		this->m_map_file_pos = this->m_map_ptr + current_position;
		this->m_current_mmap_start_position = this->m_map_file_pos;
	}

	void AlleleCountFile::printRemainingFile()
	{
		size_t chunk = FILE_CHUNK;
		size_t unread_file_size;
		while (true)
		{
			unread_file_size = this->getUnreadFileSize();
			chunk = (chunk > unread_file_size) ? unread_file_size : chunk;
			std::cout.write(this->m_map_file_pos, chunk);
			if (chunk < FILE_CHUNK) // done reading the file
			{
				return;
			}
			this->m_map_file_pos += chunk;
			freeAndReallocateMMap();
		}

	}



	bool AlleleCountFile::compare(AlleleCountFile* f1, AlleleCountFile* f2)
	{
		return memcmp(f1->getKMer(), f2->getKMer(), f1->getKMerSize()) < 0;
	}


} // end namespace AlleleCount
