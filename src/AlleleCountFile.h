#ifndef ALLELE_COUNT_FILE_H
#define ALLELE_COUNT_FILE_H

#include <string>

namespace AlleleCount
{

  class AlleleCountFile
  {

  public:
    AlleleCountFile(const std::string& file_name);
    ~AlleleCountFile();

    /*
     * Sets m_map_ptr, m_file_size, and m_kmer_size
     */
    void init();
    void advanceLine();

    char* getKMer() { return this->m_map_file_pos; }
    unsigned int getKMerSize() {return this->m_kmer_size;}
    size_t getUnreadFileSize() { return this->m_total_file_size - (this->m_map_file_pos - this->m_map_ptr); }

    bool advanceKMerPointer();
    unsigned int getKMerCount();
    void printRemainingFile();

    static bool compare(AlleleCountFile* f1, AlleleCountFile* f2);

  private:
    AlleleCountFile() = delete;                                         // don't use the default constructor
    AlleleCountFile(const AlleleCountFile& allele_count_file) = delete;            // don't use copy constructor
    AlleleCountFile& operator=(const AlleleCountFile& allele_count_file) = delete; // don't use operator equals

    void freeAndReallocateMMap();
    void setKMerSize();

    int m_fd;
    unsigned int m_kmer_size;
    unsigned int m_count_start_pos; // the start position of the int on a line
    char* m_map_file_pos;           // the current read position of the file
    char* m_map_ptr;
    char* m_current_mmap_start_position; // the starting read position, this is used to calculate how much contiguous memory is being consumed by mmap
    size_t m_total_file_size;

    std::string m_file_name;

  };

} // end namespace AlleleCount

#endif // end define ALLELE_COUNT_FILE_H
