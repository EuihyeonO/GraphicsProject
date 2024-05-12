#pragma once

class ResourceLoader
{

public:

	ResourceLoader();
	~ResourceLoader();

	ResourceLoader(const ResourceLoader& _Other) = delete;
	ResourceLoader(ResourceLoader&& _Other) noexcept = delete;
	ResourceLoader& operator=(const ResourceLoader& _Other) = delete;
	ResourceLoader& operator=(ResourceLoader&& _Other) noexcept = delete;

protected:

private:

};

