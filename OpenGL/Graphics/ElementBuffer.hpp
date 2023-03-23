#pragma once

namespace Graphics
{
	class ElementBuffer
	{
		unsigned int id = 0;
		int count = 0;

		void Delete() const;
	
	public:
		ElementBuffer(const unsigned* data, int count);
		ElementBuffer(const ElementBuffer& other) = delete;
		ElementBuffer& operator=(const ElementBuffer& other) = delete;
		ElementBuffer(ElementBuffer&& other) noexcept;
		ElementBuffer& operator=(ElementBuffer&& other) noexcept;
		~ElementBuffer();

		void Bind() const;
		void Unbind();

		unsigned int GetCount() const { return count; }
	};

}