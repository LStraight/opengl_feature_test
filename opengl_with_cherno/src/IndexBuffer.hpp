#pragma once


class IndexBuffer {
private:
	unsigned int m_RendererID;
	unsigned int m_Count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	//data: ���ݵĴ�С count: ����
	~IndexBuffer();

	unsigned int GetCount() const;
	void Bind() const;
	void Unbind() const;
};