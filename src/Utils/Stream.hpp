namespace Utils
{
	class Stream
	{
	private:
		int CriticalSectionState;
		unsigned int BlockSize[Game::MAX_XFILE_COUNT];
		std::vector<Game::XFILE_BLOCK_TYPES> StreamStack;
		std::string Buffer;

	public:
		enum Alignment
		{
			ALIGN_2,
			ALIGN_4,
			ALIGN_8,
			ALIGN_16,
			ALIGN_32,
			ALIGN_64,
			ALIGN_128,
			ALIGN_256,
			ALIGN_512,
			ALIGN_1024,
			ALIGN_2048,
		};

		Stream();
		Stream(size_t size);
		~Stream();

		size_t Length();
		size_t Capacity();

		char* Save(const void * _str, size_t size, size_t count = 1);
		char* Save(Game::XFILE_BLOCK_TYPES stream, const void * _str, size_t size, size_t count);
		char* Save(Game::XFILE_BLOCK_TYPES stream, int value, size_t count);

		char* SaveString(std::string string);
		char* SaveString(const char* string);
		char* SaveString(const char* string, size_t len);
		char* SaveByte(unsigned char byte, size_t count = 1);
		char* SaveNull(size_t count = 1);
		char* SaveMax(size_t count = 1);

		char* SaveText(std::string string);

		void Align(Alignment align);
		bool PushBlock(Game::XFILE_BLOCK_TYPES stream);
		bool PopBlock();
		bool IsValidBlock(Game::XFILE_BLOCK_TYPES stream);
		void IncreaseBlockSize(Game::XFILE_BLOCK_TYPES stream, unsigned int size);
		void IncreaseBlockSize(unsigned int size);
		Game::XFILE_BLOCK_TYPES GetCurrentBlock();
		unsigned int GetBlockSize(Game::XFILE_BLOCK_TYPES stream);

		DWORD GetPackedOffset();

		char* At();
		char* Data();

		void ToBuffer(std::string& outBuffer);
		std::string ToBuffer();

		// Enter/Leave critical sections in which reallocations are not allowed.
		// If buffer reallocation is detected, the operation has to be terminated
		// and more memory has to be allocated next time. This will have to be done
		// by editing the code though.
		void EnterCriticalSection();
		void LeaveCriticalSection();
		bool IsCriticalSection();


		// This represents packed offset in streams:
		// - lowest 28 bits store the value/offset
		// - highest 4 bits store the stream block
		class Offset
		{
		public:
			union
			{
				struct
				{
					uint32_t offset : 28;
					Game::XFILE_BLOCK_TYPES block : 4;
				};
				uint32_t packed;
				void* pointer;
			};

			Offset() : packed(0) {};
			Offset(Game::XFILE_BLOCK_TYPES _block, uint32_t _offset) : offset(_offset), block(_block) {};

			// The game needs it to be incremented
			uint32_t GetPackedOffset() 
			{ 
				return this->packed + 1;
			};

			uint32_t GetUnpackedOffset()
			{
				Offset offset = *this;
				offset.packed--;
				return offset.offset;
			};

			int GetUnpackedBlock()
			{
				Offset offset = *this;
				offset.packed--;
				return offset.block;
			};
		};
	};
}