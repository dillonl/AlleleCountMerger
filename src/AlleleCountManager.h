#ifndef ALLELE_COUNT_MANAGER_H
#define ALLELE_COUNT_MANAGER_H

#include "AlleleCountFile.h"

#include <vector>

namespace AlleleCount
{
	/*
	 * This class is a singleton and therefore
	 * should only have one global instance
	 * during the lifetime of this application.
	 */
	class AlleleCountManager
	{
	public:
		static AlleleCountManager* Instance();

		void addAlleleCountFile(const std::string& file_name);
		void run();

	private:
		AlleleCountManager();
		~AlleleCountManager();
		AlleleCountManager(const AlleleCountManager& dfm) = delete;
		AlleleCountManager& operator=(const AlleleCountManager& dfm) = delete;

		std::vector<AlleleCountFile*> m_allele_files;

		static AlleleCountManager* s_instance;
	};
} // end namespace AlleleCount

#endif // ending ifdef ALLELE_COUNT_MANAGER_H
