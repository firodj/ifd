#pragma once
#include "ImFileDialogIconInfo.hpp"

#include <string>
#include <functional>

namespace ifd
{
////////////////////////////////////////////////////////////////////////////////

class FileInfoLinux: public FileIconInfoBase
{
private:
	struct details;
	details * m_details{};
public:
	FileInfoLinux();
	FileInfoLinux(const std::filesystem::path& path);
	~FileInfoLinux();
	bool HasIcon() override;
	int GetINode() override;
	void *GetIcon(
		std::function<void*(uint8_t*, int, int, char)> createTexture) override;
};

////////////////////////////////////////////////////////////////////////////////
};
