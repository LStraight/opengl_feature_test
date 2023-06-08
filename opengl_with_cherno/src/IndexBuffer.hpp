#pragma once


class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	//data: 数据的大小 count: 计数
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;
};