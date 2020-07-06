#ifndef TLG5_READER_H_
#define TLG5_READER_H_
#include "AbstractTlgReader.h"

class Tlg5Reader : public AbstractTlgReader
{
	public:
		virtual const std::string get_magic() const;

	private:
		virtual const Image read_raw_data(std::ifstream &stream) const;
};

#endif
